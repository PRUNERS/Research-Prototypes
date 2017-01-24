//===-- ompsan_interface_atomic.cc ------------------------------------------===//
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

// ThreadSanitizer atomic operations are based on C++11/C1x standards.
// For background see C++11 standard.  A slightly older, publicly
// available draft of the standard (not entirely up-to-date, but close enough
// for casual browsing) is available here:
// http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2011/n3242.pdf
// The following page contains more background information:
// http://www.hpl.hp.com/personal/Hans_Boehm/c++mm/

#include "ompsan_interface.h"
#include "ompsan_rtl.h"

using namespace __ompsan;  // NOLINT

extern "C" {
SANITIZER_INTERFACE_ATTRIBUTE
a8 __ompsan_atomic8_load(const volatile a8 *a, morder mo) {

}

SANITIZER_INTERFACE_ATTRIBUTE
a16 __ompsan_atomic16_load(const volatile a16 *a, morder mo) {

}

SANITIZER_INTERFACE_ATTRIBUTE
a32 __ompsan_atomic32_load(const volatile a32 *a, morder mo) {

}

SANITIZER_INTERFACE_ATTRIBUTE
a64 __ompsan_atomic64_load(const volatile a64 *a, morder mo) {

}

#if __TSAN_HAS_INT128
SANITIZER_INTERFACE_ATTRIBUTE
a128 __ompsan_atomic128_load(const volatile a128 *a, morder mo) {

}
#endif

SANITIZER_INTERFACE_ATTRIBUTE
void __ompsan_atomic8_store(volatile a8 *a, a8 v, morder mo) {

}

SANITIZER_INTERFACE_ATTRIBUTE
void __ompsan_atomic16_store(volatile a16 *a, a16 v, morder mo) {

}

SANITIZER_INTERFACE_ATTRIBUTE
void __ompsan_atomic32_store(volatile a32 *a, a32 v, morder mo) {

}

SANITIZER_INTERFACE_ATTRIBUTE
void __ompsan_atomic64_store(volatile a64 *a, a64 v, morder mo) {

}

#if __TSAN_HAS_INT128
SANITIZER_INTERFACE_ATTRIBUTE
void __ompsan_atomic128_store(volatile a128 *a, a128 v, morder mo) {

}
#endif

SANITIZER_INTERFACE_ATTRIBUTE
a8 __ompsan_atomic8_exchange(volatile a8 *a, a8 v, morder mo) {

}

SANITIZER_INTERFACE_ATTRIBUTE
a16 __ompsan_atomic16_exchange(volatile a16 *a, a16 v, morder mo) {

}

SANITIZER_INTERFACE_ATTRIBUTE
a32 __ompsan_atomic32_exchange(volatile a32 *a, a32 v, morder mo) {

}

SANITIZER_INTERFACE_ATTRIBUTE
a64 __ompsan_atomic64_exchange(volatile a64 *a, a64 v, morder mo) {

}

#if __TSAN_HAS_INT128
SANITIZER_INTERFACE_ATTRIBUTE
a128 __ompsan_atomic128_exchange(volatile a128 *a, a128 v, morder mo) {

}
#endif

SANITIZER_INTERFACE_ATTRIBUTE
a8 __ompsan_atomic8_fetch_add(volatile a8 *a, a8 v, morder mo) {

}

SANITIZER_INTERFACE_ATTRIBUTE
a16 __ompsan_atomic16_fetch_add(volatile a16 *a, a16 v, morder mo) {

}

SANITIZER_INTERFACE_ATTRIBUTE
a32 __ompsan_atomic32_fetch_add(volatile a32 *a, a32 v, morder mo) {

}

SANITIZER_INTERFACE_ATTRIBUTE
a64 __ompsan_atomic64_fetch_add(volatile a64 *a, a64 v, morder mo) {

}

#if __TSAN_HAS_INT128
SANITIZER_INTERFACE_ATTRIBUTE
a128 __ompsan_atomic128_fetch_add(volatile a128 *a, a128 v, morder mo) {

}
#endif

SANITIZER_INTERFACE_ATTRIBUTE
a8 __ompsan_atomic8_fetch_sub(volatile a8 *a, a8 v, morder mo) {

}

SANITIZER_INTERFACE_ATTRIBUTE
a16 __ompsan_atomic16_fetch_sub(volatile a16 *a, a16 v, morder mo) {

}

SANITIZER_INTERFACE_ATTRIBUTE
a32 __ompsan_atomic32_fetch_sub(volatile a32 *a, a32 v, morder mo) {

}

SANITIZER_INTERFACE_ATTRIBUTE
a64 __ompsan_atomic64_fetch_sub(volatile a64 *a, a64 v, morder mo) {

}

#if __TSAN_HAS_INT128
SANITIZER_INTERFACE_ATTRIBUTE
a128 __ompsan_atomic128_fetch_sub(volatile a128 *a, a128 v, morder mo) {

}
#endif

SANITIZER_INTERFACE_ATTRIBUTE
a8 __ompsan_atomic8_fetch_and(volatile a8 *a, a8 v, morder mo) {

}

SANITIZER_INTERFACE_ATTRIBUTE
a16 __ompsan_atomic16_fetch_and(volatile a16 *a, a16 v, morder mo) {

}

SANITIZER_INTERFACE_ATTRIBUTE
a32 __ompsan_atomic32_fetch_and(volatile a32 *a, a32 v, morder mo) {

}

SANITIZER_INTERFACE_ATTRIBUTE
a64 __ompsan_atomic64_fetch_and(volatile a64 *a, a64 v, morder mo) {

}

#if __TSAN_HAS_INT128
SANITIZER_INTERFACE_ATTRIBUTE
a128 __ompsan_atomic128_fetch_and(volatile a128 *a, a128 v, morder mo) {

}
#endif

SANITIZER_INTERFACE_ATTRIBUTE
a8 __ompsan_atomic8_fetch_or(volatile a8 *a, a8 v, morder mo) {

}

SANITIZER_INTERFACE_ATTRIBUTE
a16 __ompsan_atomic16_fetch_or(volatile a16 *a, a16 v, morder mo) {

}

SANITIZER_INTERFACE_ATTRIBUTE
a32 __ompsan_atomic32_fetch_or(volatile a32 *a, a32 v, morder mo) {

}

SANITIZER_INTERFACE_ATTRIBUTE
a64 __ompsan_atomic64_fetch_or(volatile a64 *a, a64 v, morder mo) {

}

#if __TSAN_HAS_INT128
SANITIZER_INTERFACE_ATTRIBUTE
a128 __ompsan_atomic128_fetch_or(volatile a128 *a, a128 v, morder mo) {

}
#endif

SANITIZER_INTERFACE_ATTRIBUTE
a8 __ompsan_atomic8_fetch_xor(volatile a8 *a, a8 v, morder mo) {

}

SANITIZER_INTERFACE_ATTRIBUTE
a16 __ompsan_atomic16_fetch_xor(volatile a16 *a, a16 v, morder mo) {

}

SANITIZER_INTERFACE_ATTRIBUTE
a32 __ompsan_atomic32_fetch_xor(volatile a32 *a, a32 v, morder mo) {

}

SANITIZER_INTERFACE_ATTRIBUTE
a64 __ompsan_atomic64_fetch_xor(volatile a64 *a, a64 v, morder mo) {

}

#if __TSAN_HAS_INT128
SANITIZER_INTERFACE_ATTRIBUTE
a128 __ompsan_atomic128_fetch_xor(volatile a128 *a, a128 v, morder mo) {

}
#endif

SANITIZER_INTERFACE_ATTRIBUTE
a8 __ompsan_atomic8_fetch_nand(volatile a8 *a, a8 v, morder mo) {

}

SANITIZER_INTERFACE_ATTRIBUTE
a16 __ompsan_atomic16_fetch_nand(volatile a16 *a, a16 v, morder mo) {

}

SANITIZER_INTERFACE_ATTRIBUTE
a32 __ompsan_atomic32_fetch_nand(volatile a32 *a, a32 v, morder mo) {

}

SANITIZER_INTERFACE_ATTRIBUTE
a64 __ompsan_atomic64_fetch_nand(volatile a64 *a, a64 v, morder mo) {

}

#if __TSAN_HAS_INT128
SANITIZER_INTERFACE_ATTRIBUTE
a128 __ompsan_atomic128_fetch_nand(volatile a128 *a, a128 v, morder mo) {

}
#endif

SANITIZER_INTERFACE_ATTRIBUTE
int __ompsan_atomic8_compare_exchange_strong(volatile a8 *a, a8 *c, a8 v,
    morder mo, morder fmo) {

}

SANITIZER_INTERFACE_ATTRIBUTE
int __ompsan_atomic16_compare_exchange_strong(volatile a16 *a, a16 *c, a16 v,
    morder mo, morder fmo) {

}

SANITIZER_INTERFACE_ATTRIBUTE
int __ompsan_atomic32_compare_exchange_strong(volatile a32 *a, a32 *c, a32 v,
    morder mo, morder fmo) {

}

SANITIZER_INTERFACE_ATTRIBUTE
int __ompsan_atomic64_compare_exchange_strong(volatile a64 *a, a64 *c, a64 v,
    morder mo, morder fmo) {

}

#if __TSAN_HAS_INT128
SANITIZER_INTERFACE_ATTRIBUTE
int __ompsan_atomic128_compare_exchange_strong(volatile a128 *a, a128 *c, a128 v,
    morder mo, morder fmo) {

}
#endif

SANITIZER_INTERFACE_ATTRIBUTE
int __ompsan_atomic8_compare_exchange_weak(volatile a8 *a, a8 *c, a8 v,
    morder mo, morder fmo) {

}

SANITIZER_INTERFACE_ATTRIBUTE
int __ompsan_atomic16_compare_exchange_weak(volatile a16 *a, a16 *c, a16 v,
    morder mo, morder fmo) {

}

SANITIZER_INTERFACE_ATTRIBUTE
int __ompsan_atomic32_compare_exchange_weak(volatile a32 *a, a32 *c, a32 v,
    morder mo, morder fmo) {

}

SANITIZER_INTERFACE_ATTRIBUTE
int __ompsan_atomic64_compare_exchange_weak(volatile a64 *a, a64 *c, a64 v,
    morder mo, morder fmo) {

}

#if __TSAN_HAS_INT128
SANITIZER_INTERFACE_ATTRIBUTE
int __ompsan_atomic128_compare_exchange_weak(volatile a128 *a, a128 *c, a128 v,
    morder mo, morder fmo) {

}
#endif

SANITIZER_INTERFACE_ATTRIBUTE
a8 __ompsan_atomic8_compare_exchange_val(volatile a8 *a, a8 c, a8 v,
    morder mo, morder fmo) {

}

SANITIZER_INTERFACE_ATTRIBUTE
a16 __ompsan_atomic16_compare_exchange_val(volatile a16 *a, a16 c, a16 v,
    morder mo, morder fmo) {

}

SANITIZER_INTERFACE_ATTRIBUTE
a32 __ompsan_atomic32_compare_exchange_val(volatile a32 *a, a32 c, a32 v,
    morder mo, morder fmo) {

}

SANITIZER_INTERFACE_ATTRIBUTE
a64 __ompsan_atomic64_compare_exchange_val(volatile a64 *a, a64 c, a64 v,
    morder mo, morder fmo) {

}

#if __TSAN_HAS_INT128
SANITIZER_INTERFACE_ATTRIBUTE
a128 __ompsan_atomic128_compare_exchange_val(volatile a128 *a, a128 c, a128 v,
    morder mo, morder fmo) {

}
#endif

SANITIZER_INTERFACE_ATTRIBUTE
void __ompsan_atomic_thread_fence(morder mo) {

}

SANITIZER_INTERFACE_ATTRIBUTE
void __ompsan_atomic_signal_fence(morder mo) {

}

}  // extern "C"

