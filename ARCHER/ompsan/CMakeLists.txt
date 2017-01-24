# Build for the ThreadSanitizer runtime support library.

include_directories(..)

set(OMPSAN_CFLAGS ${SANITIZER_COMMON_CFLAGS})
 # SANITIZER_COMMON_CFLAGS contains -fPIC, but it's performance-critical for
 # TSan runtime to be built with -fPIE to reduce the number of register spills.
append_list_if(COMPILER_RT_HAS_FPIE_FLAG -fPIE OMPSAN_CFLAGS)
append_rtti_flag(OFF OMPSAN_CFLAGS)

if(COMPILER_RT_OMPSAN_DEBUG_OUTPUT)
  # Add extra debug information to TSan runtime. This configuration is rarely
  # used, but we need to support it so that debug output will not bitrot.
  list(APPEND OMPSAN_CFLAGS -DOMPSAN_COLLECT_STATS=1
                          -DOMPSAN_DEBUG_OUTPUT=2)
endif()

set(OMPSAN_RTL_CFLAGS ${OMPSAN_CFLAGS})
append_list_if(COMPILER_RT_HAS_MSSE3_FLAG -msse3 OMPSAN_RTL_CFLAGS)
append_list_if(SANITIZER_LIMIT_FRAME_SIZE -Wframe-larger-than=530
               OMPSAN_RTL_CFLAGS)
append_list_if(COMPILER_RT_HAS_WGLOBAL_CONSTRUCTORS_FLAG -Wglobal-constructors
               OMPSAN_RTL_CFLAGS)

set(OMPSAN_SOURCES
  rtl/ompsan_interface.cc
  rtl/ompsan_interface_atomic.cc
  rtl/ompsan_preinit.cc
  rtl/ompsan_rtl.cc
  )

if(UNIX)
  # Assume Linux
  list(APPEND OMPSAN_SOURCES
    rtl/ompsan_platform_linux.cc
    rtl/ompsan_platform_posix.cc)
endif()

set(OMPSAN_HEADERS
  rtl/ompsan_defs.h
  rtl/ompsan_interface.h
  rtl/ompsan_interface_inl.h
  rtl/ompsan_platform.h
  rtl/ompsan_rtl.h
  rtl/ompsan_update_shadow_word_inl.h)

set(OMPSAN_RUNTIME_LIBRARIES)
add_compiler_rt_component(ompsan)

foreach(arch ${OMPSAN_SUPPORTED_ARCH})
    add_compiler_rt_runtime(clang_rt.ompsan
      STATIC
      ARCHS ${arch}
      SOURCES ${OMPSAN_SOURCES} ${OMPSAN_ASM_SOURCES}
              $<TARGET_OBJECTS:RTInterception.${arch}>
              $<TARGET_OBJECTS:RTSanitizerCommon.${arch}>
              $<TARGET_OBJECTS:RTSanitizerCommonLibc.${arch}>
              $<TARGET_OBJECTS:RTUbsan.${arch}>
      CFLAGS ${OMPSAN_RTL_CFLAGS})
    add_compiler_rt_runtime(clang_rt.ompsan_cxx
      STATIC
      ARCHS ${arch}
      SOURCES ${OMPSAN_CXX_SOURCES}
              $<TARGET_OBJECTS:RTUbsan_cxx.${arch}>
      CFLAGS ${OMPSAN_RTL_CFLAGS})
    list(APPEND OMPSAN_RUNTIME_LIBRARIES clang_rt.ompsan-${arch}
                                       clang_rt.ompsan_cxx-${arch})
    add_sanitizer_rt_symbols(clang_rt.ompsan
      ARCHS ${arch}
      EXTRA rtl/ompsan.syms.extra)
    add_sanitizer_rt_symbols(clang_rt.ompsan_cxx
      ARCHS ${arch}
      EXTRA rtl/ompsan.syms.extra)
    add_dependencies(ompsan clang_rt.ompsan-${arch}
                          clang_rt.ompsan_cxx-${arch}
                          clang_rt.ompsan-${arch}-symbols
                          clang_rt.ompsan_cxx-${arch}-symbols)
endforeach()

# Make sure that non-platform-specific files don't include any system headers.
# FreeBSD does not install a number of Clang-provided headers for the compiler
# in the base system due to incompatibilities between FreeBSD's and Clang's
# versions. As a workaround do not use --sysroot=. on FreeBSD until this is
# addressed.
if(COMPILER_RT_HAS_SYSROOT_FLAG AND NOT CMAKE_SYSTEM_NAME MATCHES "FreeBSD")
  file(GLOB _ompsan_generic_sources rtl/ompsan*)
  file(GLOB _ompsan_platform_sources rtl/ompsan*posix* rtl/ompsan*mac*
                                   rtl/ompsan*linux*)
  list(REMOVE_ITEM _ompsan_generic_sources ${_ompsan_platform_sources})
  set_source_files_properties(${_ompsan_generic_sources}
    PROPERTIES COMPILE_FLAGS "--sysroot=.")
endif()

# Build libcxx instrumented with TSan.
if(COMPILER_RT_HAS_LIBCXX_SOURCES AND
   COMPILER_RT_TEST_COMPILER_ID STREQUAL "Clang")
  set(libcxx_ompsan_deps)
  foreach(arch ${OMPSAN_SUPPORTED_ARCH})
    get_target_flags_for_arch(${arch} TARGET_CFLAGS)
    set(LIBCXX_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/libcxx_ompsan_${arch})
    add_custom_libcxx(libcxx_ompsan_${arch} ${LIBCXX_PREFIX}
      DEPS ${OMPSAN_RUNTIME_LIBRARIES}
      CFLAGS ${TARGET_CFLAGS} -fsanitize=thread)
    list(APPEND libcxx_ompsan_deps libcxx_ompsan_${arch})
  endforeach()

  add_custom_target(libcxx_ompsan DEPENDS ${libcxx_ompsan_deps})
endif()
