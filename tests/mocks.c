/* SPDX-License-Identifier: LGPL-2.1-or-later */
/*
 * Copyright (C) 2021 Steven Stallion <sstallion@gmail.com>
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
 * the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#include "mocks.h"

#include <setjmp.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <sys/types.h>
#include <cmocka.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

bool mocks_enabled;

void *mock_malloc(size_t size)
{
	check_expected(size);

	return mock_type(void *);
}

void *__wrap_malloc(size_t size)
{
	extern void *__real_malloc(size_t size);

	if (mocks_enabled)
		return mock_malloc(size);

	return __real_malloc(size);
}

char *mock_strdup(const char *s)
{
	check_expected_ptr(s);

	return mock_type(char *);
}

char *__wrap_strdup(const char *s)
{
	extern char *__real_strdup(const char *s);

	if (mocks_enabled)
		return mock_strdup(s);

	return __real_strdup(s);
}

void mock_free(void *ptr)
{
	check_expected_ptr(ptr);
}

void __wrap_free(void *ptr)
{
	extern void __real_free(void *ptr);

	if (mocks_enabled)
		mock_free(ptr);
	else
		__real_free(ptr);
}

int mock_open(const char *pathname, int flags)
{
	check_expected_ptr(pathname);
	check_expected(flags);

	return mock_type(int);
}

int __wrap_open(const char *pathname, int flags, mode_t mode)
{
	extern int __real_open(const char *pathname, int flags, mode_t mode);

	if (mocks_enabled)
		return mock_open(pathname, flags);

	return __real_open(pathname, flags, mode);
}

int mock_close(int fd)
{
	check_expected(fd);

	return mock_type(int);
}

int __wrap_close(int fd)
{
	extern int __real_close(int fd);

	if (mocks_enabled)
		return mock_close(fd);

	return __real_close(fd);
}

int mock_ioctl(int fd, unsigned long request, ...)
{
	va_list ap;

	check_expected(fd);
	check_expected(request);

	va_start(ap, request);
	switch (request) {
	case I2C_RETRIES: {
		unsigned long retries;

		retries = va_arg(ap, unsigned long);
		check_expected(retries);
		break;
	}

	case I2C_TIMEOUT: {
		unsigned long timeout;

		timeout = va_arg(ap, unsigned long);
		check_expected(timeout);
		break;
	}

	case I2C_FUNCS: {
		unsigned long *funcs;

		funcs = va_arg(ap, unsigned long *);
		check_expected_ptr(funcs);
		break;
	}

	case I2C_RDWR: {
		struct i2c_rdwr_ioctl_data *msgset;
		struct i2c_msg *msg;
		size_t i;

		msgset = va_arg(ap, struct i2c_rdwr_ioctl_data *);
		for (i = 0; i < msgset->nmsgs; i++) {
			msg = &msgset->msgs[i];
			check_expected_ptr(msg);
		}
		break;
	}
	}
	va_end(ap);

	return mock_type(int);
}

int __wrap_ioctl(int fd, unsigned long request, ...)
{
	extern int __real_ioctl(int fd, unsigned long request, ...);
	va_list ap;
	void *argp;

	va_start(ap, request);
	argp = va_arg(ap, void *);
	va_end(ap);

	if (mocks_enabled)
		return mock_ioctl(fd, request, argp);

	return __real_ioctl(fd, request, argp);
}
