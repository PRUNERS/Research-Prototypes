//===-- ompsan_interface_inl.h ------------------------------------*- C++ -*-===//
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

#define CALLERPC ((uptr)__builtin_return_address(0))

using namespace __ompsan;  // NOLINT

void __ompsan_read1(void *addr, u64 pc1, bool *conflict, u64 *pc2) {
  MemoryRead(CALLERPC, (uptr)addr, kSizeLog1, pc1, conflict, pc2);
}

void __ompsan_read2(void *addr, u64 pc1, bool *conflict, u64 *pc2) {
  MemoryRead(CALLERPC, (uptr)addr, kSizeLog2, pc1, conflict, pc2);
}

void __ompsan_read4(void *addr, u64 pc1, bool *conflict, u64 *pc2) {
  MemoryRead(CALLERPC, (uptr)addr, kSizeLog4, pc1, conflict, pc2);
}

void __ompsan_read8(void *addr, u64 pc1, bool *conflict, u64 *pc2) {
  MemoryRead(CALLERPC, (uptr)addr, kSizeLog8, pc1, conflict, pc2);
}

void __ompsan_write1(void *addr, u64 pc1, bool *conflict, u64 *pc2) {
  MemoryWrite(CALLERPC, (uptr)addr, kSizeLog1, pc1, conflict, pc2);
}

void __ompsan_write2(void *addr, u64 pc1, bool *conflict, u64 *pc2) {
  MemoryWrite(CALLERPC, (uptr)addr, kSizeLog2, pc1, conflict, pc2);
}

void __ompsan_write4(void *addr, u64 pc1, bool *conflict, u64 *pc2) {
  MemoryWrite(CALLERPC, (uptr)addr, kSizeLog4, pc1, conflict, pc2);
}

void __ompsan_write8(void *addr, u64 pc1, bool *conflict, u64 *pc2) {
  MemoryWrite(CALLERPC, (uptr)addr, kSizeLog8, pc1, conflict, pc2);
}

void __ompsan_read1_pc(void *addr, u64 pc1, bool *conflict, u64 *pc2, void *pc) {
  MemoryRead((uptr)pc, (uptr)addr, kSizeLog1, pc1, conflict, pc2);
}

void __ompsan_read2_pc(void *addr, u64 pc1, bool *conflict, u64 *pc2, void *pc) {
  MemoryRead((uptr)pc, (uptr)addr, kSizeLog2, pc1, conflict, pc2);
}

void __ompsan_read4_pc(void *addr, u64 pc1, bool *conflict, u64 *pc2, void *pc) {
  MemoryRead((uptr)pc, (uptr)addr, kSizeLog4, pc1, conflict, pc2);
}

void __ompsan_read8_pc(void *addr, u64 pc1, bool *conflict, u64 *pc2, void *pc) {
  MemoryRead((uptr)pc, (uptr)addr, kSizeLog8, pc1, conflict, pc2);
}

void __ompsan_write1_pc(void *addr, u64 pc1, bool *conflict, u64 *pc2, void *pc) {
  MemoryWrite((uptr)pc, (uptr)addr, kSizeLog1, pc1, conflict, pc2);
}

void __ompsan_write2_pc(void *addr, u64 pc1, bool *conflict, u64 *pc2, void *pc) {
  MemoryWrite((uptr)pc, (uptr)addr, kSizeLog2, pc1, conflict, pc2);
}

void __ompsan_write4_pc(void *addr, u64 pc1, bool *conflict, u64 *pc2, void *pc) {
  MemoryWrite((uptr)pc, (uptr)addr, kSizeLog4, pc1, conflict, pc2);
}

void __ompsan_write8_pc(void *addr, u64 pc1, bool *conflict, u64 *pc2, void *pc) {
  MemoryWrite((uptr)pc, (uptr)addr, kSizeLog8, pc1, conflict, pc2);
}
