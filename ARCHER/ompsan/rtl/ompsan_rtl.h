//===-- ompsan_rtl.h ----------------------------------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file is a part of ThreadSanitizer (TSan), a race detector.
//
// Main internal TSan header file.
//
// Ground rules:
//   - C++ run-time should not be used (static CTORs, RTTI, exceptions, static
//     function-scope locals)
//   - All functions/classes/etc reside in namespace __ompsan, except for those
//     declared in ompsan_interface.h.
//   - Platform-specific files should be used instead of ifdefs (*).
//   - No system headers included in header files (*).
//   - Platform specific headres included only into platform-specific files (*).
//
//  (*) Except when inlining is critical for performance.
//===----------------------------------------------------------------------===//

#ifndef TSAN_RTL_H
#define TSAN_RTL_H

#include "sanitizer_common/sanitizer_allocator.h"
#include "sanitizer_common/sanitizer_allocator_internal.h"
#include "sanitizer_common/sanitizer_asm.h"
#include "sanitizer_common/sanitizer_common.h"
#include "sanitizer_common/sanitizer_deadlock_detector_interface.h"
#include "sanitizer_common/sanitizer_libignore.h"
#include "sanitizer_common/sanitizer_suppressions.h"
#include "sanitizer_common/sanitizer_thread_registry.h"
#include "ompsan_defs.h"
#include "ompsan_platform.h"

#if SANITIZER_WORDSIZE != 64
# error "ThreadSanitizer is supported only on 64-bit platforms"
#endif

namespace __ompsan {

#if !SANITIZER_GO
struct MapUnmapCallback;
#if defined(__mips64) || defined(__aarch64__) || defined(__powerpc__)
static const uptr kAllocatorSpace = 0;
static const uptr kAllocatorSize = SANITIZER_MMAP_RANGE_SIZE;
static const uptr kAllocatorRegionSizeLog = 20;
static const uptr kAllocatorNumRegions =
    kAllocatorSize >> kAllocatorRegionSizeLog;
typedef TwoLevelByteMap<(kAllocatorNumRegions >> 12), 1 << 12,
    MapUnmapCallback> ByteMap;
typedef SizeClassAllocator32<kAllocatorSpace, kAllocatorSize, 0,
    CompactSizeClassMap, kAllocatorRegionSizeLog, ByteMap,
    MapUnmapCallback> PrimaryAllocator;
#else
struct AP64 {  // Allocator64 parameters. Deliberately using a short name.
  static const uptr kSpaceBeg = Mapping::kHeapMemBeg;
  static const uptr kSpaceSize = Mapping::kHeapMemEnd - Mapping::kHeapMemBeg;
  static const uptr kMetadataSize = 0;
  typedef DefaultSizeClassMap SizeClassMap;
  typedef __ompsan::MapUnmapCallback MapUnmapCallback;
  static const uptr kFlags = 0;
};
typedef SizeClassAllocator64<AP64> PrimaryAllocator;
#endif
typedef SizeClassAllocatorLocalCache<PrimaryAllocator> AllocatorCache;
typedef LargeMmapAllocator<MapUnmapCallback> SecondaryAllocator;
typedef CombinedAllocator<PrimaryAllocator, AllocatorCache,
    SecondaryAllocator> Allocator;
Allocator *allocator();
#endif

void OMPsanCheckFailed(const char *file, int line, const char *cond,
                     u64 v1, u64 v2);

const u64 kShadowRodata = (u64)-1;  // .rodata shadow marker

// FastState (from most significant bit):
//   ignore          : 1
//   tid             : kTidBits
//   unused          : -
//   history_size    : 3
//   hash           : kPCBits
class FastState {
 public:
  FastState(u64 tid, u64 hash) {
    x_ = tid << kTidShift;
    x_ |= hash;
    DCHECK_EQ(tid, this->tid());
    DCHECK_EQ(hash, this->pc());
    DCHECK_EQ(GetIgnoreBit(), false);
  }

  explicit FastState(u64 x)
      : x_(x) {
  }

  u64 raw() const {
    return x_;
  }

  u64 tid() const {
    u64 res = (x_ & ~kIgnoreBit) >> kTidShift;
    return res;
  }

  u64 TidWithIgnore() const {
    u64 res = x_ >> kTidShift;
    return res;
  }

  u64 pc() const {
    u64 res = x_ & ((1ull << kPCBits) - 1);
    return res;
  }

  void SetPC(u64 pc) {
    u64 old_pc = pc;
    if(!old_pc)
    	x_ = (x_ & ((1ull << kPCBits) - 1));
  }

  void SetIgnoreBit() { x_ |= kIgnoreBit; }
  void ClearIgnoreBit() { x_ &= ~kIgnoreBit; }
  bool GetIgnoreBit() const { return (s64)x_ < 0; }

  void SetHistorySize(int hs) {
    CHECK_GE(hs, 0);
    CHECK_LE(hs, 7);
    x_ = (x_ & ~(kHistoryMask << kHistoryShift)) | (u64(hs) << kHistoryShift);
  }

  ALWAYS_INLINE
  int GetHistorySize() const {
    return (int)((x_ >> kHistoryShift) & kHistoryMask);
  }

  void ClearHistorySize() {
    SetHistorySize(0);
  }

 private:
  friend class Shadow;
  static const int kTidShift = 64 - kTidBits - 1;
  static const u64 kIgnoreBit = 1ull << 63;
  static const u64 kFreedBit = 1ull << 63;
  static const u64 kHistoryShift = kPCBits;
  static const u64 kHistoryMask = 7;
  u64 x_;
};

// Shadow (from most significant bit):
//   freed           : 1
//   tid             : kTidBits
//   is_atomic       : 1
//   is_read         : 1
//   size_log        : 2
//   addr0           : 3
//   hash           : kPCBits
class Shadow : public FastState {
 public:
  explicit Shadow(u64 x)
      : FastState(x) {
  }

  explicit Shadow(const FastState &s)
      : FastState(s.x_) {
    ClearHistorySize();
  }

  void SetAddr0AndSizeLog(u64 addr0, unsigned kAccessSizeLog) {
    DCHECK_EQ((x_ >> kPCBits) & 31, 0);
    DCHECK_LE(addr0, 7);
    DCHECK_LE(kAccessSizeLog, 3);
    x_ |= ((kAccessSizeLog << 3) | addr0) << kPCBits;
    DCHECK_EQ(kAccessSizeLog, size_log());
    DCHECK_EQ(addr0, this->addr0());
  }

  void SetWrite(unsigned kAccessIsWrite) {
    DCHECK_EQ(x_ & kReadBit, 0);
    if (!kAccessIsWrite)
      x_ |= kReadBit;
    DCHECK_EQ(kAccessIsWrite, IsWrite());
  }

  void SetAtomic(bool kIsAtomic) {
    DCHECK(!IsAtomic());
    if (kIsAtomic)
      x_ |= kAtomicBit;
    DCHECK_EQ(IsAtomic(), kIsAtomic);
  }

  bool IsAtomic() const {
    return x_ & kAtomicBit;
  }

  bool IsZero() const {
    return x_ == 0;
  }

  static inline bool TidsAreEqual(const Shadow s1, const Shadow s2) {
    u64 shifted_xor = (s1.x_ ^ s2.x_) >> kTidShift;
    DCHECK_EQ(shifted_xor == 0, s1.TidWithIgnore() == s2.TidWithIgnore());
    return shifted_xor == 0;
  }

  static ALWAYS_INLINE
  bool Addr0AndSizeAreEqual(const Shadow s1, const Shadow s2) {
    u64 masked_xor = ((s1.x_ ^ s2.x_) >> kPCBits) & 31;
    return masked_xor == 0;
  }

  static ALWAYS_INLINE bool TwoRangesIntersect(Shadow s1, Shadow s2,
      unsigned kS2AccessSize) {
    bool res = false;
    u64 diff = s1.addr0() - s2.addr0();
    if ((s64)diff < 0) {  // s1.addr0 < s2.addr0  // NOLINT
      // if (s1.addr0() + size1) > s2.addr0()) return true;
      if (s1.size() > -diff)
        res = true;
    } else {
      // if (s2.addr0() + kS2AccessSize > s1.addr0()) return true;
      if (kS2AccessSize > diff)
        res = true;
    }
    DCHECK_EQ(res, TwoRangesIntersectSlow(s1, s2));
    DCHECK_EQ(res, TwoRangesIntersectSlow(s2, s1));
    return res;
  }

  u64 ALWAYS_INLINE addr0() const { return (x_ >> kPCBits) & 7; }
  u64 ALWAYS_INLINE size() const { return 1ull << size_log(); }
  bool ALWAYS_INLINE IsWrite() const { return !IsRead(); }
  bool ALWAYS_INLINE IsRead() const { return x_ & kReadBit; }

  // The idea behind the freed bit is as follows.
  // When the memory is freed (or otherwise unaccessible) we write to the shadow
  // values with tid/hash related to the free and the freed bit set.
  // During memory accesses processing the freed bit is considered
  // as msb of tid. So any access races with shadow with freed bit set
  // (it is as if write from a thread with which we never synchronized before).
  // This allows us to detect accesses to freed memory w/o additional
  // overheads in memory access processing and at the same time restore
  // tid/hash of free.
  void MarkAsFreed() {
     x_ |= kFreedBit;
  }

  bool IsFreed() const {
    return x_ & kFreedBit;
  }

  bool GetFreedAndReset() {
    bool res = x_ & kFreedBit;
    x_ &= ~kFreedBit;
    return res;
  }

  bool ALWAYS_INLINE IsBothReadsOrAtomic(bool kIsWrite, bool kIsAtomic) const {
    bool v = x_ & ((u64(kIsWrite ^ 1) << kReadShift)
        | (u64(kIsAtomic) << kAtomicShift));
    DCHECK_EQ(v, (!IsWrite() && !kIsWrite) || (IsAtomic() && kIsAtomic));
    return v;
  }

  bool ALWAYS_INLINE IsRWNotWeaker(bool kIsWrite, bool kIsAtomic) const {
    bool v = ((x_ >> kReadShift) & 3)
        <= u64((kIsWrite ^ 1) | (kIsAtomic << 1));
    DCHECK_EQ(v, (IsAtomic() < kIsAtomic) ||
        (IsAtomic() == kIsAtomic && !IsWrite() <= !kIsWrite));
    return v;
  }

  bool ALWAYS_INLINE IsRWWeakerOrEqual(bool kIsWrite, bool kIsAtomic) const {
    bool v = ((x_ >> kReadShift) & 3)
        >= u64((kIsWrite ^ 1) | (kIsAtomic << 1));
    DCHECK_EQ(v, (IsAtomic() > kIsAtomic) ||
        (IsAtomic() == kIsAtomic && !IsWrite() >= !kIsWrite));
    return v;
  }

 private:
  static const u64 kReadShift   = 5 + kPCBits;
  static const u64 kReadBit     = 1ull << kReadShift;
  static const u64 kAtomicShift = 6 + kPCBits;
  static const u64 kAtomicBit   = 1ull << kAtomicShift;

  u64 size_log() const { return (x_ >> (3 + kPCBits)) & 3; }

  static bool TwoRangesIntersectSlow(const Shadow s1, const Shadow s2) {
    if (s1.addr0() == s2.addr0()) return true;
    if (s1.addr0() < s2.addr0() && s1.addr0() + s1.size() > s2.addr0())
      return true;
    if (s2.addr0() < s1.addr0() && s2.addr0() + s2.size() > s1.addr0())
      return true;
    return false;
  }
};

void MapShadow(uptr addr, uptr size);
void MapThreadTrace(uptr addr, uptr size, const char *name);
void DontNeedShadowFor(uptr addr, uptr size);
void InitializeShadowMemory();

#if defined(TSAN_DEBUG_OUTPUT) && TSAN_DEBUG_OUTPUT >= 1
# define DPrintf Printf
#else
# define DPrintf(...)
#endif

#if defined(TSAN_DEBUG_OUTPUT) && TSAN_DEBUG_OUTPUT >= 2
# define DPrintf2 Printf
#else
# define DPrintf2(...)
#endif

void Initialize();
int Finalize();

void MemoryAccess(uptr pc, uptr addr,
    int kAccessSizeLog, bool kAccessIsWrite, bool kIsAtomic,
	u64 pc1, bool *conflict, u64 *pc2);
void UnalignedMemoryAccess(uptr pc, uptr addr,
    int size, bool kAccessIsWrite, bool kIsAtomic,
	u64 pc1, bool *conflict, u64 *pc2);

const int kSizeLog1 = 0;
const int kSizeLog2 = 1;
const int kSizeLog4 = 2;
const int kSizeLog8 = 3;

void ALWAYS_INLINE MemoryRead(uptr pc,
                                     uptr addr, int kAccessSizeLog,
									 u64 pc1, bool *conflict, u64 *pc2) {
  MemoryAccess(pc, addr, kAccessSizeLog, false, false, pc1, conflict, pc2);
}

void ALWAYS_INLINE MemoryWrite(uptr pc,
                                      uptr addr, int kAccessSizeLog,
									  u64 pc1, bool *conflict, u64 *pc2) {
  MemoryAccess(pc, addr, kAccessSizeLog, true, false, pc1, conflict, pc2);
}

void ALWAYS_INLINE MemoryReadAtomic(uptr pc,
                                           uptr addr, int kAccessSizeLog,
										   u64 pc1, bool *conflict, u64 *pc2) {
  MemoryAccess(pc, addr, kAccessSizeLog, false, true, pc1, conflict, pc2);
}

void ALWAYS_INLINE MemoryWriteAtomic(uptr pc,
                                            uptr addr, int kAccessSizeLog,
											u64 pc1, bool *conflict, u64 *pc2) {
  MemoryAccess(pc, addr, kAccessSizeLog, true, true, pc1, conflict, pc2);
}

#if !SANITIZER_GO
uptr ALWAYS_INLINE HeapEnd() {
  return HeapMemEnd() + PrimaryAllocator::AdditionalSize();
}
#endif

}  // namespace __ompsan

#endif  // TSAN_RTL_H
