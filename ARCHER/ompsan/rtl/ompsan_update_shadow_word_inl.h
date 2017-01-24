//===-- tsan_update_shadow_word_inl.h ---------------------------*- C++ -*-===//
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
// Body of the hottest inner loop.
// If we wrap this body into a function, compilers (both gcc and clang)
// produce sligtly less efficient code.
//===----------------------------------------------------------------------===//
do {
  const unsigned kAccessSize = 1 << kAccessSizeLog;
  u64 *sp = &shadow_mem[0];
  old = LoadShadow(sp);
  if (old.IsZero()) {
      StoreIfNotYetStored(sp, &store_word);
    // The above StoreIfNotYetStored could be done unconditionally
    // and it even shows 4% gain on synthetic benchmarks (r4307).
    break;
  }
  // is the memory access equal to the previous?
  if (Shadow::Addr0AndSizeAreEqual(cur, old)) {
    // same thread?
    if (Shadow::TidsAreEqual(old, cur)) {
      if (old.IsRWWeakerOrEqual(kAccessIsWrite, kIsAtomic))
        StoreIfNotYetStored(sp, &store_word);
      break;
    }
    if (old.IsBothReadsOrAtomic(kAccessIsWrite, kIsAtomic))
      break;
    goto RACE;
  }
  // Do the memory access intersect?
  if (Shadow::TwoRangesIntersect(old, cur, kAccessSize)) {
    if (Shadow::TidsAreEqual(old, cur)) {
      break;
    }
    if (old.IsBothReadsOrAtomic(kAccessIsWrite, kIsAtomic))
      break;
    goto RACE;
  }
  // The accesses do not intersect.
  break;
} while (0);
