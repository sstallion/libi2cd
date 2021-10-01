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

include build-aux/coverage.am
include build-aux/doxygen.am

ACLOCAL_AMFLAGS = -I m4
AM_CFLAGS = -Wall -Wextra -Wno-unused-parameter
AM_CPPFLAGS = -I$(top_srcdir)/include -I$(top_srcdir)/src

EXTRA_DIST = HACKING.md \
	     NEWS.md \
	     README.md \
	     doc/examples/example.c \
	     doc/index.md

include_HEADERS = include/i2cd.h

lib_LTLIBRARIES = libi2cd.la

libi2cd_la_SOURCES = src/i2cd.c src/i2cd-private.h
libi2cd_la_CFLAGS = $(COVERAGE_CFLAGS) $(AM_CFLAGS)
libi2cd_la_LIBADD = $(COVERAGE_LIBS) $(AM_LIBS)
libi2cd_la_LDFLAGS = -version-info $(PACKAGE_VERSION_INFO)

if ENABLE_TESTS
check_LIBRARIES = tests/libmocks.a
TESTS_LIBS = $(check_LIBRARIES) $(CMOCKA_LIBS)

tests_libmocks_a_SOURCES = tests/mocks.c tests/mocks.h

check_PROGRAMS = tests/test-i2cd
TESTS = $(check_PROGRAMS)

tests_test_i2cd_SOURCES = tests/test-i2cd.c
tests_test_i2cd_LDADD = libi2cd.la $(TESTS_LIBS) $(AM_LIBS)
tests_test_i2cd_LDFLAGS = -static \
			  -Wl,--wrap=calloc \
                          -Wl,--wrap=strdup \
                          -Wl,--wrap=free \
                          -Wl,--wrap=open \
                          -Wl,--wrap=close \
                          -Wl,--wrap=ioctl
endif
