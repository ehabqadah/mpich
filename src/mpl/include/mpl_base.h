/* -*- Mode: C; c-basic-offset:4 ; indent-tabs-mode:nil ; -*- */
/*
 *  (C) 2001 by Argonne National Laboratory.
 *      See COPYRIGHT in top-level directory.
 */

#if !defined(MPL_BASE_H_INCLUDED)
#define MPL_BASE_H_INCLUDED

/* this file splits off the base functionality in MPL, which does not
 * depend on any of the exposed features. */

#include "mplconfig.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>

#if defined _mpl_restrict
#define mpl_restrict _mpl_restrict
#else
#define mpl_restrict restrict
#endif /* _mpl_restrict */

#if defined _mpl_const
#define mpl_const _mpl_const
#else
#define mpl_const const
#endif /* _mpl_const */

#if defined _mpl_inline
#define mpl_inline _mpl_inline
#else
#define mpl_inline inline
#endif /* _mpl_inline */

#if defined MPL_HAVE_CTYPE_H
#include <ctype.h>
#endif /* MPL_HAVE_CTYPE_H */

#if defined(MPL_HAVE_INTTYPES_H)
#include <inttypes.h>
#endif /* MPL_HAVE_INTTYPES_H */

#if !defined ATTRIBUTE
#  if defined MPL_HAVE_GCC_ATTRIBUTE
#    define ATTRIBUTE(a_) __attribute__(a_)
#  else /* MPL_HAVE_GCC_ATTRIBUTE */
#    define ATTRIBUTE(a_)
#  endif /* MPL_HAVE_GCC_ATTRIBUTE */
#endif /* ATTRIBUTE */

#define MPL_UNUSED ATTRIBUTE((unused))
#define MPL_STATIC_INLINE_PREFIX ATTRIBUTE((always_inline)) static inline
#define MPL_STATIC_INLINE_SUFFIX ATTRIBUTE((always_inline))

#if MPL_HAVE_VAR_ATTRIBUTE_ALIGNED
#define MPL_ATTR_ALIGNED(x) ATTRIBUTE((aligned(x)))
#else
#define MPL_ATTR_ALIGNED(x)
#endif

/* These likely/unlikely macros provide static branch prediction hints to the
 * compiler, if such hints are available.  Simply wrap the relevant expression in
 * the macro, like this:
 *
 * if (unlikely(ptr == NULL)) {
 *     // ... some unlikely code path ...
 * }
 *
 * They should be used sparingly, especially in upper-level code.  It's easy to
 * incorrectly estimate branching likelihood, while the compiler can often do a
 * decent job if left to its own devices.
 *
 * These macros are not namespaced because the namespacing is cumbersome.
 */
#ifdef MPL_HAVE_BUILTIN_EXPECT
#  define unlikely(x_) __builtin_expect(!!(x_),0)
#  define likely(x_)   __builtin_expect(!!(x_),1)
#else
#  define unlikely(x_) (x_)
#  define likely(x_)   (x_)
#endif

#define MPL_QUOTE(A) MPL_QUOTE2(A)
#define MPL_QUOTE2(A) #A

#define MPL_MAX(a,b)    (((a) > (b)) ? (a) : (b))
#define MPL_MIN(a,b)    (((a) < (b)) ? (a) : (b))

/* Use this macro for each parameter to a function that is not referenced in
   the body of the function */
#ifdef MPL_HAVE_WINDOWS_H
#define MPL_UNREFERENCED_ARG(a) a
#else
#define MPL_UNREFERENCED_ARG(a)
#endif

/* macro for finding the enclosing structure of an element */
#define MPL_container_of(ptr, type, member) (type *)((char *)(ptr) - offsetof(type,member))

/* This macro is used to silence warnings from the Mac OS X linker when
 * an object file "has no symbols".  The unused attribute prevents a
 * warning about the unused dummy variable while the used attribute
 * prevents the compiler from discarding the symbol altogether.  This
 * macro should be used at the top of any file that might not define any
 * other variables or functions (perhaps due to conditional compilation
 * via the preprocessor).  A semicolon is expected after each invocation
 * of this macro. */
#define MPL_SUPPRESS_OSX_HAS_NO_SYMBOLS_WARNING \
    static int MPL_UNIQUE_SYMBOL_NAME(dummy) ATTRIBUTE((unused,used)) = 0

/* we jump through a couple of extra macro hoops to append the line
 * number to the variable name in order to reduce the chance of a name
 * collision with headers that might be included in the translation
 * unit */
#define MPL_UNIQUE_SYMBOL_NAME(prefix_) MPL_UNIQUE_IMPL1_(prefix_##_unique_L,__LINE__)
#define MPL_UNIQUE_IMPL1_(prefix_,line_) MPL_UNIQUE_IMPL2_(prefix_,line_)
#define MPL_UNIQUE_IMPL2_(prefix_,line_) MPL_UNIQUE_IMPL3_(prefix_,line_)
#define MPL_UNIQUE_IMPL3_(prefix_,line_) prefix_##line_

typedef int MPL_bool;

#endif /* !defined(MPL_BASE_H_INCLUDED) */
