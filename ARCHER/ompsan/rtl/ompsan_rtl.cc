//===-- ompsan_rtl.cc -------------------------------------------------------===//
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
// Main file (entry points) for the TSan run-time.
//===----------------------------------------------------------------------===//

#include "sanitizer_common/sanitizer_atomic.h"
#include "sanitizer_common/sanitizer_common.h"
#include "sanitizer_common/sanitizer_libc.h"
#include "sanitizer_common/sanitizer_stackdepot.h"
#include "sanitizer_common/sanitizer_placement_new.h"
#include "sanitizer_common/sanitizer_symbolizer.h"
#include "ompsan_defs.h"
#include "ompsan_platform.h"
#include "ompsan_rtl.h"

#ifdef __SSE3__
// <emmintrin.h> transitively includes <stdlib.h>,
// and it's prohibited to include std headers into ompsan runtime.
// So we do this dirty trick.
#define _MM_MALLOC_H_INCLUDED
#define __MM_MALLOC_H
#include <emmintrin.h>
typedef __m128i m128;
#endif

volatile int __ompsan_resumed = 0;

extern "C" void __ompsan_resume() {
  __ompsan_resumed = 1;
}

extern thread_local int tid;

namespace __ompsan {

// Can be overriden by a front-end.
#ifdef TSAN_EXTERNAL_HOOKS
bool OnFinalize(bool failed);
void OnInitialize();
#else
SANITIZER_WEAK_CXX_DEFAULT_IMPL
bool OnFinalize(bool failed) {
  return failed;
}
SANITIZER_WEAK_CXX_DEFAULT_IMPL
void OnInitialize() {}
#endif

void DontNeedShadowFor(uptr addr, uptr size) {
  uptr shadow_beg = MemToShadow(addr);
  uptr shadow_end = MemToShadow(addr + size);
  ReleaseMemoryToOS(shadow_beg, shadow_end - shadow_beg);
}

void MapShadow(uptr addr, uptr size) {
  // Global data is not 64K aligned, but there are no adjacent mappings,
  // so we can get away with unaligned mapping.
  // CHECK_EQ(addr, addr & ~((64 << 10) - 1));  // windows wants 64K alignment
  const uptr kPageSize = GetPageSizeCached();
  uptr shadow_begin = RoundDownTo((uptr)MemToShadow(addr), kPageSize);
  uptr shadow_end = RoundUpTo((uptr)MemToShadow(addr + size), kPageSize);
  MmapFixedNoReserve(shadow_begin, shadow_end - shadow_begin, "shadow");

  // Meta shadow is 2:1, so tread carefully.
  static bool data_mapped = false;
  static uptr mapped_meta_end = 0;
  uptr meta_begin = (uptr)MemToMeta(addr);
  uptr meta_end = (uptr)MemToMeta(addr + size);
  meta_begin = RoundDownTo(meta_begin, 64 << 10);
  meta_end = RoundUpTo(meta_end, 64 << 10);
  if (!data_mapped) {
    // First call maps data+bss.
    data_mapped = true;
    MmapFixedNoReserve(meta_begin, meta_end - meta_begin, "meta shadow");
  } else {
    // Mapping continous heap.
    // Windows wants 64K alignment.
    meta_begin = RoundDownTo(meta_begin, 64 << 10);
    meta_end = RoundUpTo(meta_end, 64 << 10);
    if (meta_end <= mapped_meta_end)
      return;
    if (meta_begin < mapped_meta_end)
      meta_begin = mapped_meta_end;
    MmapFixedNoReserve(meta_begin, meta_end - meta_begin, "meta shadow");
    mapped_meta_end = meta_end;
  }
  VPrintf(2, "mapped meta shadow for (%p-%p) at (%p-%p)\n",
      addr, addr+size, meta_begin, meta_end);
}

void MapThreadTrace(uptr addr, uptr size, const char *name) {
  DPrintf("#0: Mapping trace at %p-%p(0x%zx)\n", addr, addr + size, size);
  CHECK_GE(addr, TraceMemBeg());
  CHECK_LE(addr + size, TraceMemEnd());
  CHECK_EQ(addr, addr & ~((64 << 10) - 1));  // windows wants 64K alignment
  uptr addr1 = (uptr)MmapFixedNoReserve(addr, size, name);
  if (addr1 != addr) {
    Printf("FATAL: ThreadSanitizer can not mmap thread trace (%p/%p->%p)\n",
        addr, size, addr1);
    Die();
  }
}

static void CheckShadowMapping() {
  uptr beg, end;
  for (int i = 0; GetUserRegion(i, &beg, &end); i++) {
    // Skip cases for empty regions (heap definition for architectures that
    // do not use 64-bit allocator).
    if (beg == end)
      continue;
    VPrintf(3, "checking shadow region %p-%p\n", beg, end);
    uptr prev = 0;
    for (uptr p0 = beg; p0 <= end; p0 += (end - beg) / 4) {
      for (int x = -(int)kShadowCell; x <= (int)kShadowCell; x += kShadowCell) {
        const uptr p = RoundDown(p0 + x, kShadowCell);
        if (p < beg || p >= end)
          continue;
        const uptr s = MemToShadow(p);
        const uptr m = (uptr)MemToMeta(p);
        VPrintf(3, "  checking pointer %p: shadow=%p meta=%p\n", p, s, m);
        CHECK(IsAppMem(p));
        CHECK(IsShadowMem(s));
        CHECK_EQ(p, ShadowToMem(s));
        CHECK(IsMetaMem(m));
        if (prev) {
          // Ensure that shadow and meta mappings are linear within a single
          // user range. Lots of code that processes memory ranges assumes it.
          const uptr prev_s = MemToShadow(prev);
          const uptr prev_m = (uptr)MemToMeta(prev);
          CHECK_EQ(s - prev_s, (p - prev) * kShadowMultiplier);
          CHECK_EQ((m - prev_m) / kMetaShadowSize,
                   (p - prev) / kMetaShadowCell);
        }
        prev = p;
      }
    }
  }
}

void Initialize() {
  // Thread safe because done before all threads exist.
  static bool is_initialized = false;
  if (is_initialized)
    return;
  is_initialized = true;
  SanitizerToolName = "OpenMPSanitizer";

  CacheBinaryName();
  AvoidCVE_2016_2143();
  InitializePlatformEarly();

#if !SANITIZER_GO
  // Re-exec ourselves if we need to set additional env or command line args.
  MaybeReexec();
#endif

  CheckShadowMapping();
  InitializePlatform();

#if !SANITIZER_GO
  InitializeShadowMemory();
#endif

  // OnInitialize();
}

ALWAYS_INLINE
Shadow LoadShadow(u64 *p) {
  u64 raw = atomic_load((atomic_uint64_t*)p, memory_order_relaxed);
  return Shadow(raw);
}

ALWAYS_INLINE
void StoreShadow(u64 *sp, u64 s) {
  atomic_store((atomic_uint64_t*)sp, s, memory_order_relaxed);
}

ALWAYS_INLINE
void StoreIfNotYetStored(u64 *sp, u64 *s) {
  StoreShadow(sp, *s);
  *s = 0;
}

void UnalignedMemoryAccess(uptr pc, uptr addr,
    int size, bool kAccessIsWrite, bool kIsAtomic,
	u64 pc1, bool *conflict, u64 *pc2) {
  while (size) {
    int size1 = 1;
    int kAccessSizeLog = kSizeLog1;
    if (size >= 8 && (addr & ~7) == ((addr + 7) & ~7)) {
      size1 = 8;
      kAccessSizeLog = kSizeLog8;
    } else if (size >= 4 && (addr & ~7) == ((addr + 3) & ~7)) {
      size1 = 4;
      kAccessSizeLog = kSizeLog4;
    } else if (size >= 2 && (addr & ~7) == ((addr + 1) & ~7)) {
      size1 = 2;
      kAccessSizeLog = kSizeLog2;
    }
    MemoryAccess(pc, addr, kAccessSizeLog, kAccessIsWrite, kIsAtomic, pc1, conflict, pc2);
    addr += size1;
    size -= size1;
  }
}

ALWAYS_INLINE USED
void MemoryAccess(uptr pc, uptr addr,
    int kAccessSizeLog, bool kAccessIsWrite, bool kIsAtomic,
	u64 pc1, bool *conflict, u64 *pc2) {
  u64 *shadow_mem = (u64*)MemToShadow(addr);
  DPrintf2("#%d: MemoryAccess: @%p %p size=%d"
      " is_write=%d shadow_mem=%p {%x}\n",
      (int)tid, (void*)pc, (void*)addr,
      (int)(1 << kAccessSizeLog), kAccessIsWrite, shadow_mem,
      (uptr)shadow_mem[0]);
#if SANITIZER_DEBUG
  if (!IsAppMem(addr)) {
    Printf("Access to non app mem %zx\n", addr);
    DCHECK(IsAppMem(addr));
  }
  if (!IsShadowMem((uptr)shadow_mem)) {
    Printf("Bad shadow addr %p (%zx)\n", shadow_mem, addr);
    DCHECK(IsShadowMem((uptr)shadow_mem));
  }
#endif

  Shadow cur(FastState(tid, pc1));
  cur.SetAddr0AndSizeLog(addr & 7, kAccessSizeLog);
  cur.SetWrite(kAccessIsWrite);
  cur.SetAtomic(kIsAtomic);

  // This potentially can live in an MMX/SSE scratch register.
  // The required intrinsics are:
  // __m128i _mm_move_epi64(__m128i*);
  // _mm_storel_epi64(u64*, __m128i);
  u64 store_word = cur.raw();

  Shadow old(0);

#include "ompsan_update_shadow_word_inl.h"
  return;
  RACE:
   *conflict = true;
   *pc2 = old.pc();
   return;
}

#if SANITIZER_DEBUG
void build_consistency_debug() {}
#else
void build_consistency_release() {}
#endif

#if TSAN_COLLECT_STATS
void build_consistency_stats() {}
#else
void build_consistency_nostats() {}
#endif

}  // namespace __ompsan

#if !SANITIZER_GO
// Must be included in this file to make sure everything is inlined.
#include "ompsan_interface_inl.h"
#endif
