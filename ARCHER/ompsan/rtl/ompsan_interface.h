//===-- ompsan_interface.h ----------------------------------------*- C++ -*-===//
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
// The functions declared in this header will be inserted by the instrumentation
// module.
// This header can be included by the instrumented program or by TSan tests.
//===----------------------------------------------------------------------===//
#ifndef TSAN_INTERFACE_H
#define TSAN_INTERFACE_H

#include <sanitizer_common/sanitizer_internal_defs.h>
using __sanitizer::uptr;
using __sanitizer::u64;

// This header should NOT include any other headers.
// All functions in this header are extern "C" and start with __ompsan_.

#ifdef __cplusplus
extern "C" {
#endif

#if !SANITIZER_GO

// This function should be called at the very beginning of the process,
// before any instrumented code is executed and before any call to malloc.
SANITIZER_INTERFACE_ATTRIBUTE void __ompsan_init();

SANITIZER_INTERFACE_ATTRIBUTE void __ompsan_read1(void *addr, u64 pc1, bool *conflict, u64 *pc2);
SANITIZER_INTERFACE_ATTRIBUTE void __ompsan_read2(void *addr, u64 pc1, bool *conflict, u64 *pc2);
SANITIZER_INTERFACE_ATTRIBUTE void __ompsan_read4(void *addr, u64 pc1, bool *conflict, u64 *pc2);
SANITIZER_INTERFACE_ATTRIBUTE void __ompsan_read8(void *addr, u64 pc1, bool *conflict, u64 *pc2);
SANITIZER_INTERFACE_ATTRIBUTE void __ompsan_read16(void *addr, u64 pc1, bool *conflict, u64 *pc2);

SANITIZER_INTERFACE_ATTRIBUTE void __ompsan_write1(void *addr, u64 pc1, bool *conflict, u64 *pc2);
SANITIZER_INTERFACE_ATTRIBUTE void __ompsan_write2(void *addr, u64 pc1, bool *conflict, u64 *pc2);
SANITIZER_INTERFACE_ATTRIBUTE void __ompsan_write4(void *addr, u64 pc1, bool *conflict, u64 *pc2);
SANITIZER_INTERFACE_ATTRIBUTE void __ompsan_write8(void *addr, u64 pc1, bool *conflict, u64 *pc2);
SANITIZER_INTERFACE_ATTRIBUTE void __ompsan_write16(void *addr, u64 pc1, bool *conflict, u64 *pc2);

SANITIZER_INTERFACE_ATTRIBUTE void __ompsan_unaligned_read2(const void *addr, u64 pc1, bool *conflict, u64 *pc2);
SANITIZER_INTERFACE_ATTRIBUTE void __ompsan_unaligned_read4(const void *addr, u64 pc1, bool *conflict, u64 *pc2);
SANITIZER_INTERFACE_ATTRIBUTE void __ompsan_unaligned_read8(const void *addr, u64 pc1, bool *conflict, u64 *pc2);
SANITIZER_INTERFACE_ATTRIBUTE void __ompsan_unaligned_read16(const void *addr, u64 pc1, bool *conflict, u64 *pc2);

SANITIZER_INTERFACE_ATTRIBUTE void __ompsan_unaligned_write2(void *addr, u64 pc1, bool *conflict, u64 *pc2);
SANITIZER_INTERFACE_ATTRIBUTE void __ompsan_unaligned_write4(void *addr, u64 pc1, bool *conflict, u64 *pc2);
SANITIZER_INTERFACE_ATTRIBUTE void __ompsan_unaligned_write8(void *addr, u64 pc1, bool *conflict, u64 *pc2);
SANITIZER_INTERFACE_ATTRIBUTE void __ompsan_unaligned_write16(void *addr, u64 pc1, bool *conflict, u64 *pc2);

SANITIZER_INTERFACE_ATTRIBUTE void __ompsan_read1_pc(void *addr, u64 pc1, bool *conflict, u64 *pc2, void *pc);
SANITIZER_INTERFACE_ATTRIBUTE void __ompsan_read2_pc(void *addr, u64 pc1, bool *conflict, u64 *pc2, void *pc);
SANITIZER_INTERFACE_ATTRIBUTE void __ompsan_read4_pc(void *addr, u64 pc1, bool *conflict, u64 *pc2, void *pc);
SANITIZER_INTERFACE_ATTRIBUTE void __ompsan_read8_pc(void *addr, u64 pc1, bool *conflict, u64 *pc2, void *pc);
SANITIZER_INTERFACE_ATTRIBUTE void __ompsan_read16_pc(void *addr, u64 pc1, bool *conflict, u64 *pc2, void *pc);

SANITIZER_INTERFACE_ATTRIBUTE void __ompsan_write1_pc(void *addr, u64 pc1, bool *conflict, u64 *pc2, void *pc);
SANITIZER_INTERFACE_ATTRIBUTE void __ompsan_write2_pc(void *addr, u64 pc1, bool *conflict, u64 *pc2, void *pc);
SANITIZER_INTERFACE_ATTRIBUTE void __ompsan_write4_pc(void *addr, u64 pc1, bool *conflict, u64 *pc2, void *pc);
SANITIZER_INTERFACE_ATTRIBUTE void __ompsan_write8_pc(void *addr, u64 pc1, bool *conflict, u64 *pc2, void *pc);
SANITIZER_INTERFACE_ATTRIBUTE void __ompsan_write16_pc(void *addr, u64 pc1, bool *conflict, u64 *pc2, void *pc);

#endif  // SANITIZER_GO

#ifdef __cplusplus
}  // extern "C"
#endif

namespace __ompsan {

// These should match declarations from public ompsan_interface_atomic.h header.
typedef unsigned char      a8;
typedef unsigned short     a16;  // NOLINT
typedef unsigned int       a32;
typedef unsigned long long a64;  // NOLINT
#if !SANITIZER_GO && (defined(__SIZEOF_INT128__) \
    || (__clang_major__ * 100 + __clang_minor__ >= 302)) && !defined(__mips64)
__extension__ typedef __int128 a128;
# define __TSAN_HAS_INT128 1
#else
# define __TSAN_HAS_INT128 0
#endif

// Part of ABI, do not change.
// http://llvm.org/viewvc/llvm-project/libcxx/trunk/include/atomic?view=markup
typedef enum {
  mo_relaxed,
  mo_consume,
  mo_acquire,
  mo_release,
  mo_acq_rel,
  mo_seq_cst
} morder;

extern "C" {
SANITIZER_INTERFACE_ATTRIBUTE
a8 __ompsan_atomic8_load(const volatile a8 *a, morder mo);
SANITIZER_INTERFACE_ATTRIBUTE
a16 __ompsan_atomic16_load(const volatile a16 *a, morder mo);
SANITIZER_INTERFACE_ATTRIBUTE
a32 __ompsan_atomic32_load(const volatile a32 *a, morder mo);
SANITIZER_INTERFACE_ATTRIBUTE
a64 __ompsan_atomic64_load(const volatile a64 *a, morder mo);
#if __TSAN_HAS_INT128
SANITIZER_INTERFACE_ATTRIBUTE
a128 __ompsan_atomic128_load(const volatile a128 *a, morder mo);
#endif

SANITIZER_INTERFACE_ATTRIBUTE
void __ompsan_atomic8_store(volatile a8 *a, a8 v, morder mo);
SANITIZER_INTERFACE_ATTRIBUTE
void __ompsan_atomic16_store(volatile a16 *a, a16 v, morder mo);
SANITIZER_INTERFACE_ATTRIBUTE
void __ompsan_atomic32_store(volatile a32 *a, a32 v, morder mo);
SANITIZER_INTERFACE_ATTRIBUTE
void __ompsan_atomic64_store(volatile a64 *a, a64 v, morder mo);
#if __TSAN_HAS_INT128
SANITIZER_INTERFACE_ATTRIBUTE
void __ompsan_atomic128_store(volatile a128 *a, a128 v, morder mo);
#endif

SANITIZER_INTERFACE_ATTRIBUTE
a8 __ompsan_atomic8_exchange(volatile a8 *a, a8 v, morder mo);
SANITIZER_INTERFACE_ATTRIBUTE
a16 __ompsan_atomic16_exchange(volatile a16 *a, a16 v, morder mo);
SANITIZER_INTERFACE_ATTRIBUTE
a32 __ompsan_atomic32_exchange(volatile a32 *a, a32 v, morder mo);
SANITIZER_INTERFACE_ATTRIBUTE
a64 __ompsan_atomic64_exchange(volatile a64 *a, a64 v, morder mo);
#if __TSAN_HAS_INT128
SANITIZER_INTERFACE_ATTRIBUTE
a128 __ompsan_atomic128_exchange(volatile a128 *a, a128 v, morder mo);
#endif

SANITIZER_INTERFACE_ATTRIBUTE
a8 __ompsan_atomic8_fetch_add(volatile a8 *a, a8 v, morder mo);
SANITIZER_INTERFACE_ATTRIBUTE
a16 __ompsan_atomic16_fetch_add(volatile a16 *a, a16 v, morder mo);
SANITIZER_INTERFACE_ATTRIBUTE
a32 __ompsan_atomic32_fetch_add(volatile a32 *a, a32 v, morder mo);
SANITIZER_INTERFACE_ATTRIBUTE
a64 __ompsan_atomic64_fetch_add(volatile a64 *a, a64 v, morder mo);
#if __TSAN_HAS_INT128
SANITIZER_INTERFACE_ATTRIBUTE
a128 __ompsan_atomic128_fetch_add(volatile a128 *a, a128 v, morder mo);
#endif

SANITIZER_INTERFACE_ATTRIBUTE
a8 __ompsan_atomic8_fetch_sub(volatile a8 *a, a8 v, morder mo);
SANITIZER_INTERFACE_ATTRIBUTE
a16 __ompsan_atomic16_fetch_sub(volatile a16 *a, a16 v, morder mo);
SANITIZER_INTERFACE_ATTRIBUTE
a32 __ompsan_atomic32_fetch_sub(volatile a32 *a, a32 v, morder mo);
SANITIZER_INTERFACE_ATTRIBUTE
a64 __ompsan_atomic64_fetch_sub(volatile a64 *a, a64 v, morder mo);
#if __TSAN_HAS_INT128
SANITIZER_INTERFACE_ATTRIBUTE
a128 __ompsan_atomic128_fetch_sub(volatile a128 *a, a128 v, morder mo);
#endif

SANITIZER_INTERFACE_ATTRIBUTE
a8 __ompsan_atomic8_fetch_and(volatile a8 *a, a8 v, morder mo);
SANITIZER_INTERFACE_ATTRIBUTE
a16 __ompsan_atomic16_fetch_and(volatile a16 *a, a16 v, morder mo);
SANITIZER_INTERFACE_ATTRIBUTE
a32 __ompsan_atomic32_fetch_and(volatile a32 *a, a32 v, morder mo);
SANITIZER_INTERFACE_ATTRIBUTE
a64 __ompsan_atomic64_fetch_and(volatile a64 *a, a64 v, morder mo);
#if __TSAN_HAS_INT128
SANITIZER_INTERFACE_ATTRIBUTE
a128 __ompsan_atomic128_fetch_and(volatile a128 *a, a128 v, morder mo);
#endif

SANITIZER_INTERFACE_ATTRIBUTE
a8 __ompsan_atomic8_fetch_or(volatile a8 *a, a8 v, morder mo);
SANITIZER_INTERFACE_ATTRIBUTE
a16 __ompsan_atomic16_fetch_or(volatile a16 *a, a16 v, morder mo);
SANITIZER_INTERFACE_ATTRIBUTE
a32 __ompsan_atomic32_fetch_or(volatile a32 *a, a32 v, morder mo);
SANITIZER_INTERFACE_ATTRIBUTE
a64 __ompsan_atomic64_fetch_or(volatile a64 *a, a64 v, morder mo);
#if __TSAN_HAS_INT128
SANITIZER_INTERFACE_ATTRIBUTE
a128 __ompsan_atomic128_fetch_or(volatile a128 *a, a128 v, morder mo);
#endif

SANITIZER_INTERFACE_ATTRIBUTE
a8 __ompsan_atomic8_fetch_xor(volatile a8 *a, a8 v, morder mo);
SANITIZER_INTERFACE_ATTRIBUTE
a16 __ompsan_atomic16_fetch_xor(volatile a16 *a, a16 v, morder mo);
SANITIZER_INTERFACE_ATTRIBUTE
a32 __ompsan_atomic32_fetch_xor(volatile a32 *a, a32 v, morder mo);
SANITIZER_INTERFACE_ATTRIBUTE
a64 __ompsan_atomic64_fetch_xor(volatile a64 *a, a64 v, morder mo);
#if __TSAN_HAS_INT128
SANITIZER_INTERFACE_ATTRIBUTE
a128 __ompsan_atomic128_fetch_xor(volatile a128 *a, a128 v, morder mo);
#endif

SANITIZER_INTERFACE_ATTRIBUTE
a8 __ompsan_atomic8_fetch_nand(volatile a8 *a, a8 v, morder mo);
SANITIZER_INTERFACE_ATTRIBUTE
a16 __ompsan_atomic16_fetch_nand(volatile a16 *a, a16 v, morder mo);
SANITIZER_INTERFACE_ATTRIBUTE
a32 __ompsan_atomic32_fetch_nand(volatile a32 *a, a32 v, morder mo);
SANITIZER_INTERFACE_ATTRIBUTE
a64 __ompsan_atomic64_fetch_nand(volatile a64 *a, a64 v, morder mo);
#if __TSAN_HAS_INT128
SANITIZER_INTERFACE_ATTRIBUTE
a128 __ompsan_atomic128_fetch_nand(volatile a128 *a, a128 v, morder mo);
#endif

SANITIZER_INTERFACE_ATTRIBUTE
int __ompsan_atomic8_compare_exchange_strong(volatile a8 *a, a8 *c, a8 v,
                                           morder mo, morder fmo);
SANITIZER_INTERFACE_ATTRIBUTE
int __ompsan_atomic16_compare_exchange_strong(volatile a16 *a, a16 *c, a16 v,
                                            morder mo, morder fmo);
SANITIZER_INTERFACE_ATTRIBUTE
int __ompsan_atomic32_compare_exchange_strong(volatile a32 *a, a32 *c, a32 v,
                                            morder mo, morder fmo);
SANITIZER_INTERFACE_ATTRIBUTE
int __ompsan_atomic64_compare_exchange_strong(volatile a64 *a, a64 *c, a64 v,
                                            morder mo, morder fmo);
#if __TSAN_HAS_INT128
SANITIZER_INTERFACE_ATTRIBUTE
int __ompsan_atomic128_compare_exchange_strong(volatile a128 *a, a128 *c, a128 v,
                                             morder mo, morder fmo);
#endif

SANITIZER_INTERFACE_ATTRIBUTE
int __ompsan_atomic8_compare_exchange_weak(volatile a8 *a, a8 *c, a8 v, morder mo,
                                         morder fmo);
SANITIZER_INTERFACE_ATTRIBUTE
int __ompsan_atomic16_compare_exchange_weak(volatile a16 *a, a16 *c, a16 v,
                                          morder mo, morder fmo);
SANITIZER_INTERFACE_ATTRIBUTE
int __ompsan_atomic32_compare_exchange_weak(volatile a32 *a, a32 *c, a32 v,
                                          morder mo, morder fmo);
SANITIZER_INTERFACE_ATTRIBUTE
int __ompsan_atomic64_compare_exchange_weak(volatile a64 *a, a64 *c, a64 v,
                                          morder mo, morder fmo);
#if __TSAN_HAS_INT128
SANITIZER_INTERFACE_ATTRIBUTE
int __ompsan_atomic128_compare_exchange_weak(volatile a128 *a, a128 *c, a128 v,
                                           morder mo, morder fmo);
#endif

SANITIZER_INTERFACE_ATTRIBUTE
a8 __ompsan_atomic8_compare_exchange_val(volatile a8 *a, a8 c, a8 v, morder mo,
                                       morder fmo);
SANITIZER_INTERFACE_ATTRIBUTE
a16 __ompsan_atomic16_compare_exchange_val(volatile a16 *a, a16 c, a16 v,
                                         morder mo, morder fmo);
SANITIZER_INTERFACE_ATTRIBUTE
a32 __ompsan_atomic32_compare_exchange_val(volatile a32 *a, a32 c, a32 v,
                                         morder mo, morder fmo);
SANITIZER_INTERFACE_ATTRIBUTE
a64 __ompsan_atomic64_compare_exchange_val(volatile a64 *a, a64 c, a64 v,
                                         morder mo, morder fmo);
#if __TSAN_HAS_INT128
SANITIZER_INTERFACE_ATTRIBUTE
a128 __ompsan_atomic128_compare_exchange_val(volatile a128 *a, a128 c, a128 v,
                                           morder mo, morder fmo);
#endif

SANITIZER_INTERFACE_ATTRIBUTE
void __ompsan_atomic_thread_fence(morder mo);
SANITIZER_INTERFACE_ATTRIBUTE
void __ompsan_atomic_signal_fence(morder mo);

SANITIZER_INTERFACE_ATTRIBUTE
void __ompsan_go_atomic32_load(uptr cpc, uptr pc, u8 *a);
SANITIZER_INTERFACE_ATTRIBUTE
void __ompsan_go_atomic64_load(uptr cpc, uptr pc, u8 *a);
SANITIZER_INTERFACE_ATTRIBUTE
void __ompsan_go_atomic32_store(uptr cpc, uptr pc, u8 *a);
SANITIZER_INTERFACE_ATTRIBUTE
void __ompsan_go_atomic64_store(uptr cpc, uptr pc, u8 *a);
SANITIZER_INTERFACE_ATTRIBUTE
void __ompsan_go_atomic32_fetch_add(uptr cpc, uptr pc, u8 *a);
SANITIZER_INTERFACE_ATTRIBUTE
void __ompsan_go_atomic64_fetch_add(uptr cpc, uptr pc, u8 *a);
SANITIZER_INTERFACE_ATTRIBUTE
void __ompsan_go_atomic32_exchange(uptr cpc, uptr pc, u8 *a);
SANITIZER_INTERFACE_ATTRIBUTE
void __ompsan_go_atomic64_exchange(uptr cpc, uptr pc, u8 *a);
SANITIZER_INTERFACE_ATTRIBUTE
void __ompsan_go_atomic32_compare_exchange(uptr cpc, uptr pc, u8 *a);
SANITIZER_INTERFACE_ATTRIBUTE
void __ompsan_go_atomic64_compare_exchange(uptr cpc, uptr pc, u8 *a);
}  // extern "C"

}  // namespace __ompsan

#endif  // TSAN_INTERFACE_H
