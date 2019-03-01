# -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([1.0])
AC_INIT(test, 1.0, lwchao89@outlook.com)
AC_CONFIG_SRCDIR([])
AM_INIT_AUTOMAKE

# Checks for programs.
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MMAP
AC_CHECK_FUNCS([memset select strcasecmp])

if test "$prefix" != "NONE"
then
    eval cfgpath=$sysconfdir
else
    cfgpath=/
fi

AC_SUBST(cfgpath)
AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_OUTPUT
