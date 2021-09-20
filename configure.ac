# SPDX-License-Identifier: LGPL-2.1-or-later
# Copyright (C) 2021 Steven Stallion <sstallion@gmail.com>
#
# This library is free software; you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published
# by the Free Software Foundation; either version 2.1 of the License, or
# (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
# the GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this library; if not, see <http://www.gnu.org/licenses/>.

AC_PREREQ([2.69])

AC_INIT([libi2cd], [1.0],
        [https://github.com/sstallion/libi2cd/issues])

AC_SUBST([PACKAGE_DESCRIPTION], ["Linux kernel I2C character device library"])
AC_SUBST([PACKAGE_LIBS], [-li2cd])

# Update package version information with care; see
# <https://www.gnu.org/software/libtool/manual/html_node/Updating-version-info.html>.
AC_SUBST([PACKAGE_VERSION_INFO], [0:0:0])

AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIRS([m4])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_SRCDIR([src/i2cd.c])

AM_INIT_AUTOMAKE([foreign subdir-objects 1.12])
AM_SILENT_RULES([yes])

# Silence warning: ar: 'u' modifier ignored since 'D' is the default
AC_SUBST(ARFLAGS, [cr])

AM_PROG_AR
AC_PROG_AWK
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_SED

LT_INIT

ENABLE_TESTS
ENABLE_COVERAGE
ENABLE_DOXYGEN

TESTS_LIB_CMOCKA([TAP])
TESTS_TAP_DRIVER

AC_CHECK_HEADER([sys/ioctl.h], [],
                [AC_MSG_ERROR([cannot find header file sys/ioctl.h])])

AC_CHECK_HEADER([linux/i2c.h], [],
                [AC_MSG_ERROR([cannot find header file linux/i2c.h])])

AC_CHECK_HEADER([linux/i2c-dev.h], [],
                [AC_MSG_ERROR([cannot find header file linux/i2c-dev.h])])

AC_CHECK_FUNC([ioctl], [],
              [AC_MSG_ERROR([cannot find ioctl system call])])

AC_CONFIG_FILES([Makefile libi2cd.pc])

AC_OUTPUT
