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

#include <assert.h>
#include <fcntl.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

struct i2cd *i2cd_open(const char *path)
{
	struct i2cd *dev;

	assert(path != NULL);

	dev = malloc(sizeof(*dev));
	if (dev == NULL)
		return NULL;

	memset(dev, 0, sizeof(*dev));

	dev->path = strdup(path);
	if (dev->path == NULL) {
		free(dev);
		return NULL;
	}

	dev->fd = open(dev->path, O_RDWR);
	if (dev->fd < 0) {
		free(dev->path);
		free(dev);
		return NULL;
	}

	return dev;
}

struct i2cd *i2cd_open_by_name(const char *name)
{
	char path[PATH_MAX];

	assert(name != NULL);

	snprintf(path, sizeof(path), "/dev/%s", name);
	return i2cd_open(path);
}

struct i2cd *i2cd_open_by_number(unsigned int num)
{
	char path[PATH_MAX];

	snprintf(path, sizeof(path), "/dev/i2c-%u", num);
	return i2cd_open(path);
}

void i2cd_close(struct i2cd *dev)
{
	assert(dev != NULL);

	close(dev->fd);

	free(dev->path);
	free(dev);
}

const char *i2cd_get_path(struct i2cd *dev)
{
	assert(dev != NULL);

	return dev->path;
}

int i2cd_set_retries(struct i2cd *dev, unsigned long retries)
{
	assert(dev != NULL);

	return ioctl(dev->fd, I2C_RETRIES, retries);
}

int i2cd_set_timeout(struct i2cd *dev, unsigned long timeout)
{
	assert(dev != NULL);

	return ioctl(dev->fd, I2C_TIMEOUT, timeout);
}

int i2cd_get_functionality(struct i2cd *dev, unsigned long *funcs)
{
	assert(dev != NULL);
	assert(funcs != NULL);

	return ioctl(dev->fd, I2C_FUNCS, funcs);
}

int i2cd_transfer(struct i2cd *dev, struct i2c_msg msgs[], size_t nmsgs)
{
	struct i2c_rdwr_ioctl_data msgset = {msgs, nmsgs};

	assert(dev != NULL);
	assert(msgs != NULL);
	assert(nmsgs <= I2C_RDWR_IOCTL_MAX_MSGS);

	return ioctl(dev->fd, I2C_RDWR, &msgset);
}

int i2cd_read(struct i2cd *dev, uint16_t addr, void *buf, size_t len)
{
	struct i2c_msg msgs[] = {
		{
			.addr	= addr,
			.flags	= I2C_M_RD,
			.len	= len,
			.buf	= buf
		}
	};

	assert(buf != NULL);
	assert(len <= UINT16_MAX);

	return i2cd_transfer(dev, msgs, ARRAY_SIZE(msgs));
}

int i2cd_write(struct i2cd *dev, uint16_t addr, const void *buf, size_t len)
{
	struct i2c_msg msgs[] = {
		{
			.addr	= addr,
			.flags	= 0,
			.len	= len,
			.buf	= (void *)buf
		}
	};

	assert(buf != NULL);
	assert(len <= UINT16_MAX);

	return i2cd_transfer(dev, msgs, ARRAY_SIZE(msgs));
}

int i2cd_write_read(struct i2cd *dev, uint16_t addr,
		const void *write_buf, size_t write_len,
		void *read_buf, size_t read_len)
{
	struct i2c_msg msgs[] = {
		{
			.addr	= addr,
			.flags	= 0,
			.len	= write_len,
			.buf	= (void *)write_buf
		},
		{
			.addr	= addr,
			.flags	= I2C_M_RD,
			.len	= read_len,
			.buf	= read_buf
		}
	};

	assert(write_buf != NULL);
	assert(write_len <= UINT16_MAX);
	assert(read_buf != NULL);
	assert(read_len <= UINT16_MAX);

	return i2cd_transfer(dev, msgs, ARRAY_SIZE(msgs));
}
