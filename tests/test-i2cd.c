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

#include "i2cd-private.h"

#include <fcntl.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <cmocka.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

#include "mocks.h"

int check_i2c_msg(const LargestIntegralType value,
		const LargestIntegralType check_value)
{
	struct i2c_msg *msg_value = (struct i2c_msg *)(uintptr_t)value;
	struct i2c_msg *msg_check = (struct i2c_msg *)(uintptr_t)check_value;

	return (msg_value->addr == msg_check->addr) &&
		(msg_value->flags == msg_check->flags) &&
		(msg_value->len == msg_check->len) &&
		(memcmp(msg_value->buf, msg_check->buf, msg_check->len) == 0);
}

int setup(void **state)
{
	mocks_enabled = true;
	return 0;
}

int teardown(void **state)
{
	mocks_enabled = false;
	return 0;
}

void test_i2cd_open(void **state)
{
	const char *mock_path = "/dev/i2c-0";
	int mock_fd = 42;
	struct i2cd mock_dev, *dev;

	expect_value(mock_calloc, nmemb, 1);
	expect_value(mock_calloc, size, sizeof(mock_dev));
	will_return(mock_calloc, &mock_dev);

	expect_string(mock_strdup, s, mock_path);
	will_return(mock_strdup, mock_path);

	expect_string(mock_open, pathname, mock_path);
	expect_value(mock_open, flags, O_RDWR);
	will_return(mock_open, mock_fd);

	/* Check behavior when function succeeds */
	dev = i2cd_open(mock_path);

	assert_non_null(dev);
	assert_string_equal(dev->path, mock_path);
	assert_int_equal(dev->fd, mock_fd);
}

void test_i2cd_open_by_name(void **state)
{
	const char *mock_path = "/dev/i2c-0";
	int mock_fd = 42;
	struct i2cd mock_dev, *dev;

	expect_value(mock_calloc, nmemb, 1);
	expect_value(mock_calloc, size, sizeof(mock_dev));
	will_return(mock_calloc, &mock_dev);

	expect_string(mock_strdup, s, mock_path);
	will_return(mock_strdup, mock_path);

	expect_string(mock_open, pathname, mock_path);
	expect_value(mock_open, flags, O_RDWR);
	will_return(mock_open, mock_fd);

	/* Check behavior when function succeeds */
	dev = i2cd_open_by_name("i2c-0");

	assert_non_null(dev);
	assert_string_equal(dev->path, mock_path);
	assert_int_equal(dev->fd, mock_fd);
}

void test_i2cd_open_by_number(void **state)
{
	const char *mock_path = "/dev/i2c-0";
	int mock_fd = 42;
	struct i2cd mock_dev, *dev;

	expect_value(mock_calloc, nmemb, 1);
	expect_value(mock_calloc, size, sizeof(mock_dev));
	will_return(mock_calloc, &mock_dev);

	expect_string(mock_strdup, s, mock_path);
	will_return(mock_strdup, mock_path);

	expect_string(mock_open, pathname, mock_path);
	expect_value(mock_open, flags, O_RDWR);
	will_return(mock_open, mock_fd);

	/* Check behavior when function succeeds */
	dev = i2cd_open_by_number(0);

	assert_non_null(dev);
	assert_string_equal(dev->path, mock_path);
	assert_int_equal(dev->fd, mock_fd);
}

void test_i2cd_open_fail_calloc(void **state)
{
	const char *mock_path = "/dev/i2c-0";
	struct i2cd *dev;

	expect_any(mock_calloc, nmemb);
	expect_any(mock_calloc, size);
	will_return(mock_calloc, NULL);

	/* Check behavior when calloc fails */
	dev = i2cd_open(mock_path);

	assert_null(dev);
}

void test_i2cd_open_fail_strdup(void **state)
{
	const char *mock_path = "/dev/i2c-0";
	struct i2cd mock_dev, *dev;

	expect_any(mock_calloc, nmemb);
	expect_any(mock_calloc, size);
	will_return(mock_calloc, &mock_dev);

	expect_any(mock_strdup, s);
	will_return(mock_strdup, NULL);

	expect_value(mock_free, ptr, &mock_dev);

	/* Check behavior when strdup fails */
	dev = i2cd_open(mock_path);

	assert_null(dev);
}

void test_i2cd_open_fail_open(void **state)
{
	const char *mock_path = "/dev/i2c-0";
	struct i2cd mock_dev, *dev;

	expect_any(mock_calloc, nmemb);
	expect_any(mock_calloc, size);
	will_return(mock_calloc, &mock_dev);

	expect_any(mock_strdup, s);
	will_return(mock_strdup, mock_path);

	expect_any(mock_open, pathname);
	expect_any(mock_open, flags);
	will_return(mock_open, -1);

	expect_value(mock_free, ptr, mock_path);
	expect_value(mock_free, ptr, &mock_dev);

	/* Check behavior when open fails */
	dev = i2cd_open(mock_path);

	assert_null(dev);
}

void test_i2cd_close(void **state)
{
	struct i2cd mock_dev = {"/dev/i2c-0", 42};

	expect_value(mock_close, fd, mock_dev.fd);
	will_return(mock_close, 0);

	expect_value(mock_free, ptr, mock_dev.path);
	expect_value(mock_free, ptr, &mock_dev);

	/* Check behavior when function succeeds */
	i2cd_close(&mock_dev);
}

void test_i2cd_set_retries(void **state)
{
	struct i2cd mock_dev = {"/dev/i2c-0", 42};
	unsigned long mock_retries = 3;
	int rc;

	expect_value(mock_ioctl, fd, mock_dev.fd);
	expect_value(mock_ioctl, request, I2C_RETRIES);
	expect_value(mock_ioctl, retries, mock_retries);
	will_return(mock_ioctl, 0);

	/* Check behavior when function succeeds */
	rc = i2cd_set_retries(&mock_dev, mock_retries);

	assert_return_code(rc, 0);
}

void test_i2cd_set_timeout(void **state)
{
	struct i2cd mock_dev = {"/dev/i2c-0", 42};
	unsigned long mock_timeout = 10;
	int rc;

	expect_value(mock_ioctl, fd, mock_dev.fd);
	expect_value(mock_ioctl, request, I2C_TIMEOUT);
	expect_value(mock_ioctl, timeout, mock_timeout);
	will_return(mock_ioctl, 0);

	/* Check behavior when function succeeds */
	rc = i2cd_set_timeout(&mock_dev, mock_timeout);

	assert_return_code(rc, 0);
}

void test_i2cd_get_functionality(void **state)
{
	struct i2cd mock_dev = {"/dev/i2c-0", 42};
	unsigned long mock_funcs;
	int rc;

	expect_value(mock_ioctl, fd, mock_dev.fd);
	expect_value(mock_ioctl, request, I2C_FUNCS);
	expect_value(mock_ioctl, funcs, &mock_funcs);
	will_return(mock_ioctl, 0);

	/* Check behavior when function succeeds */
	rc = i2cd_get_functionality(&mock_dev, &mock_funcs);

	assert_return_code(rc, 0);
}

void test_i2cd_read(void **state)
{
	struct i2cd mock_dev = {"/dev/i2c-0", 42};
	uint16_t mock_addr = 0x20;
	uint8_t mock_buf[8];
	struct i2c_msg expect_msg = {
		.addr	= mock_addr,
		.flags	= I2C_M_RD,
		.len	= sizeof(mock_buf),
		.buf	= mock_buf
	};
	int rc;

	expect_value(mock_ioctl, fd, mock_dev.fd);
	expect_value(mock_ioctl, request, I2C_RDWR);
	expect_check(mock_ioctl, msg, check_i2c_msg, &expect_msg);
	will_return(mock_ioctl, 0);

	/* Check behavior when function succeeds */
	rc = i2cd_read(&mock_dev, mock_addr, mock_buf, sizeof(mock_buf));

	assert_return_code(rc, 0);
}

void test_i2cd_write(void **state)
{
	struct i2cd mock_dev = {"/dev/i2c-0", 42};
	uint16_t mock_addr = 0x20;
	uint8_t mock_buf[8];
	struct i2c_msg expect_msg = {
		.addr	= mock_addr,
		.flags	= 0,
		.len	= sizeof(mock_buf),
		.buf	= mock_buf
	};
	int rc;

	expect_value(mock_ioctl, fd, mock_dev.fd);
	expect_value(mock_ioctl, request, I2C_RDWR);
	expect_check(mock_ioctl, msg, check_i2c_msg, &expect_msg);
	will_return(mock_ioctl, 0);

	/* Check behavior when function succeeds */
	rc = i2cd_write(&mock_dev, mock_addr, mock_buf, sizeof(mock_buf));

	assert_return_code(rc, 0);
}

void test_i2cd_write_read(void **state)
{
	struct i2cd mock_dev = {"/dev/i2c-0", 42};
	uint16_t mock_addr = 0x20;
	uint8_t mock_write_buf[2], mock_read_buf[8];
	struct i2c_msg expect_msgs[] = {
		{
			.addr	= mock_addr,
			.flags	= 0,
			.len	= sizeof(mock_write_buf),
			.buf	= mock_write_buf
		},
		{
			.addr	= mock_addr,
			.flags	= I2C_M_RD,
			.len	= sizeof(mock_read_buf),
			.buf	= mock_read_buf
		}
	};
	int rc;

	expect_value(mock_ioctl, fd, mock_dev.fd);
	expect_value(mock_ioctl, request, I2C_RDWR);
	expect_check(mock_ioctl, msg, check_i2c_msg, &expect_msgs[0]);
	expect_check(mock_ioctl, msg, check_i2c_msg, &expect_msgs[1]);
	will_return(mock_ioctl, 0);

	/* Check behavior when function succeeds */
	rc = i2cd_write_read(&mock_dev, mock_addr,
		mock_write_buf, sizeof(mock_write_buf),
		mock_read_buf, sizeof(mock_read_buf));

	assert_return_code(rc, 0);
}

int main(void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_i2cd_open),
		cmocka_unit_test(test_i2cd_open_by_name),
		cmocka_unit_test(test_i2cd_open_by_number),
		cmocka_unit_test(test_i2cd_open_fail_calloc),
		cmocka_unit_test(test_i2cd_open_fail_strdup),
		cmocka_unit_test(test_i2cd_open_fail_open),
		cmocka_unit_test(test_i2cd_close),
		cmocka_unit_test(test_i2cd_set_retries),
		cmocka_unit_test(test_i2cd_set_timeout),
		cmocka_unit_test(test_i2cd_get_functionality),
		cmocka_unit_test(test_i2cd_read),
		cmocka_unit_test(test_i2cd_write),
		cmocka_unit_test(test_i2cd_write_read),
	};

	return cmocka_run_group_tests(tests, setup, teardown);
}
