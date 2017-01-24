//===-- ompsan_interface.cc -------------------------------------------------===//
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
//===----------------------------------------------------------------------===//

#include "ompsan_interface.h"
#include "ompsan_rtl.h"
#include "sanitizer_common/sanitizer_internal_defs.h"

#define CALLERPC ((uptr)__builtin_return_address(0))

using namespace __ompsan;  // NOLINT

typedef u16 uint16_t;
typedef u32 uint32_t;
typedef u64 uint64_t;

void __ompsan_init() {
  Initialize();
}

void __ompsan_read16(void *addr, u64 pc1, bool *conflict, u64 *pc2) {
  MemoryRead(CALLERPC, (uptr)addr, kSizeLog8, pc1, conflict, pc2);
  MemoryRead(CALLERPC, (uptr)addr + 8, kSizeLog8, pc1, conflict, pc2);
}

void __ompsan_write16(void *addr, u64 pc1, bool *conflict, u64 *pc2) {
  MemoryWrite(CALLERPC, (uptr)addr, kSizeLog8, pc1, conflict, pc2);
  MemoryWrite(CALLERPC, (uptr)addr + 8, kSizeLog8, pc1, conflict, pc2);
}

void __ompsan_read16_pc(void *addr, u64 pc1, bool *conflict, u64 *pc2, void *pc) {
  MemoryRead((uptr)pc, (uptr)addr, kSizeLog8, pc1, conflict, pc2);
  MemoryRead((uptr)pc, (uptr)addr + 8, kSizeLog8, pc1, conflict, pc2);
}

void __ompsan_write16_pc(void *addr, u64 pc1, bool *conflict, u64 *pc2, void *pc) {
  MemoryWrite((uptr)pc, (uptr)addr, kSizeLog8, pc1, conflict, pc2);
  MemoryWrite((uptr)pc, (uptr)addr + 8, kSizeLog8, pc1, conflict, pc2);
}

// __ompsan_unaligned_read/write calls are emitted by compiler.

void __ompsan_unaligned_read2(const void *addr, u64 pc1, bool *conflict, u64 *pc2) {
  UnalignedMemoryAccess(CALLERPC, (uptr)addr, 2, false, false, pc1, conflict, pc2);
}

void __ompsan_unaligned_read4(const void *addr, u64 pc1, bool *conflict, u64 *pc2) {
  UnalignedMemoryAccess(CALLERPC, (uptr)addr, 4, false, false, pc1, conflict, pc2);
}

void __ompsan_unaligned_read8(const void *addr, u64 pc1, bool *conflict, u64 *pc2) {
  UnalignedMemoryAccess(CALLERPC, (uptr)addr, 8, false, false, pc1, conflict, pc2);
}

void __ompsan_unaligned_read16(const void *addr, u64 pc1, bool *conflict, u64 *pc2) {
  UnalignedMemoryAccess(CALLERPC, (uptr)addr, 16, false, false, pc1, conflict, pc2);
}

void __ompsan_unaligned_write2(void *addr, u64 pc1, bool *conflict, u64 *pc2) {
  UnalignedMemoryAccess(CALLERPC, (uptr)addr, 2, true, false, pc1, conflict, pc2);
}

void __ompsan_unaligned_write4(void *addr, u64 pc1, bool *conflict, u64 *pc2) {
  UnalignedMemoryAccess(CALLERPC, (uptr)addr, 4, true, false, pc1, conflict, pc2);
}

void __ompsan_unaligned_write8(void *addr, u64 pc1, bool *conflict, u64 *pc2) {
  UnalignedMemoryAccess(CALLERPC, (uptr)addr, 8, true, false, pc1, conflict, pc2);
}

void __ompsan_unaligned_write16(void *addr, u64 pc1, bool *conflict, u64 *pc2) {
  UnalignedMemoryAccess(CALLERPC, (uptr)addr, 16, true, false, pc1, conflict, pc2);
}

// __sanitizer_unaligned_load/store are for user instrumentation.

extern "C" {
SANITIZER_INTERFACE_ATTRIBUTE
u16 __sanitizer_unaligned_load16(const uu16 *addr, u64 pc1, bool *conflict, u64 *pc2) {
  __ompsan_unaligned_read2(addr, pc1, conflict, pc2);
  return *addr;
}

SANITIZER_INTERFACE_ATTRIBUTE
u32 __sanitizer_unaligned_load32(const uu32 *addr, u64 pc1, bool *conflict, u64 *pc2) {
  __ompsan_unaligned_read4(addr, pc1, conflict, pc2);
  return *addr;
}

SANITIZER_INTERFACE_ATTRIBUTE
u64 __sanitizer_unaligned_load64(const uu64 *addr, u64 pc1, bool *conflict, u64 *pc2) {
  __ompsan_unaligned_read8(addr, pc1, conflict, pc2);
  return *addr;
}

SANITIZER_INTERFACE_ATTRIBUTE
void __sanitizer_unaligned_store16(uu16 *addr, u64 pc1, bool *conflict, u64 *pc2, u16 v) {
  __ompsan_unaligned_write2(addr, pc1, conflict, pc2);
  *addr = v;
}

SANITIZER_INTERFACE_ATTRIBUTE
void __sanitizer_unaligned_store32(uu32 *addr, u64 pc1, bool *conflict, u64 *pc2, u32 v) {
  __ompsan_unaligned_write4(addr, pc1, conflict, pc2);
  *addr = v;
}

SANITIZER_INTERFACE_ATTRIBUTE
void __sanitizer_unaligned_store64(uu64 *addr, u64 pc1, bool *conflict, u64 *pc2, u64 v) {
  __ompsan_unaligned_write8(addr, pc1, conflict, pc2);
  *addr = v;
}
}  // extern "C"
