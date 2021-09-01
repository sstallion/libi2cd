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

#ifndef I2CD_H
#define I2CD_H

#include <stddef.h>
#include <stdint.h>
#include <linux/i2c.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup main Main API
 *
 * @brief Main data structures and functions.
 *
 * <tt>\#include <i2cd.h></tt>
 *
 * @{
 */

/**
 * @struct i2cd
 *
 * @brief Handle to an I2C character device.
 */
struct i2cd;

/**
 * @brief Open the I2C character device specified by @p path.
 *
 * @param path Path to the I2C character device to open.
 *
 * @return Pointer to an I2C character device handle, or @c NULL on error with
 * @c errno set appropriately.
 */
struct i2cd *i2cd_open(const char *path);

/**
 * @brief Open the I2C character device specified by @p name.
 *
 * @param name Name of the I2C character device to open.
 *
 * @return Pointer to an I2C character device handle, or @c NULL on error with
 * @c errno set appropriately.
 *
 * This function is equivalent to calling:
 * @code
 * char path[PATH_MAX];
 * snprintf(path, sizeof(path), "/dev/%s", name);
 * i2cd_open(path);
 * @endcode
 */
struct i2cd *i2cd_open_by_name(const char *name);

/**
 * @brief Open the I2C character device specified by @p num.
 *
 * @param num Number of the I2C character device to open.
 *
 * @return Pointer to an I2C character device handle, or @c NULL on error with
 * @c errno set appropriately.
 *
 * This function is equivalent to calling:
 * @code
 * char path[PATH_MAX];
 * snprintf(path, sizeof(path), "/dev/i2c-%u", num);
 * i2cd_open(path);
 * @endcode
 */
struct i2cd *i2cd_open_by_number(unsigned int num);

/**
 * @brief Close an I2C character device handle and free associated memory.
 *
 * @param dev Pointer to an I2C character device handle.
 *
 * Once closed, @p dev is no longer valid for use.
 */
void i2cd_close(struct i2cd *dev);

/**
 * @brief Get the path used to open the I2C character device handle.
 *
 * @param dev Pointer to an I2C character device handle.
 *
 * @return The path used to open the I2C character device handle.
 */
const char *i2cd_get_path(struct i2cd *dev);

/**
 * @brief Set the number of times a slave address should be polled when
 * not acknowledging.
 *
 * @param dev     Pointer to an I2C character device handle.
 * @param retries Number of times to retry.
 *
 * @return 0 on success, or -1 on error with @c errno set appropriately.
 *
 * This function corresponds to the @c I2C_RETRIES @c ioctl() request.
 */
int i2cd_set_retries(struct i2cd *dev, unsigned long retries);

/**
 * @brief Set the timeout in units of 10ms.
 *
 * @param dev     Pointer to an I2C character device handle.
 * @param timeout Timeout in units of 10ms.
 *
 * @return 0 on success, or -1 on error with @c errno set appropriately.
 *
 * This function corresponds to the @c I2C_TIMEOUT @c ioctl() request.
 */
int i2cd_set_timeout(struct i2cd *dev, unsigned long timeout);

/**
 * @brief Get the adapter functionality mask.
 *
 * @param dev   Pointer to an I2C character device handle.
 * @param funcs Pointer to a buffer to receive a mask.
 *
 * @return 0 on success, or -1 on error with @c errno set appropriately.
 *
 * Adapter functionality can be determined by comparing the returned mask to
 * values defined by the @c I2C_FUNC_* macros in @c linux/i2c.h.
 *
 * This function corresponds to the @c I2C_FUNCS @c ioctl() request.
 */
int i2cd_get_functionality(struct i2cd *dev, unsigned long *funcs);

/**
 * @brief Transfer one or more low-level messages terminated with a single
 * STOP condition.
 *
 * @param dev   Pointer to an I2C character device handle.
 * @param msgs  Array of messages to transfer.
 * @param nmsgs Number of messages to transfer.
 *
 * @return Number of messages transferred on success, or -1 on error with @c
 * errno set appropriately.
 *
 * This function corresponds to the @c I2C_RDWR @c ioctl() request.
 */
int i2cd_transfer(struct i2cd *dev, struct i2c_msg msgs[], size_t nmsgs);

/**
 * @brief Read bytes from a slave device.
 *
 * @param dev  Pointer to an I2C character device handle.
 * @param addr I2C slave address.
 * @param buf  Pointer to a buffer to receive bytes.
 * @param len  Number of bytes to read.
 *
 * @return Number of messages transferred on success, or -1 on error with @c
 * errno set appropriately.
 */
int i2cd_read(struct i2cd *dev, uint16_t addr, void *buf, size_t len);

/**
 * @brief Write bytes to a slave device.
 *
 * @param dev  Pointer to an I2C character device handle.
 * @param addr I2C slave address.
 * @param buf  Pointer to a buffer to send bytes.
 * @param len  Number of bytes to send.
 *
 * @return Number of messages transferred on success, or -1 on error with @c
 * errno set appropriately.
 */
int i2cd_write(struct i2cd *dev, uint16_t addr, const void *buf, size_t len);

/**
 * @brief Write and read bytes from a slave device using a repeated START
 * condition.
 *
 * @param dev       Pointer to an I2C character device handle.
 * @param addr      I2C slave address.
 * @param write_buf Pointer to a buffer to send bytes.
 * @param write_len Number of bytes to send.
 * @param read_buf  Pointer to a buffer to receive bytes.
 * @param read_len  Number of bytes to receive.
 *
 * @return Number of messages transferred on success, or -1 on error with @c
 * errno set appropriately.
 */
int i2cd_write_read(struct i2cd *dev, uint16_t addr,
		const void *write_buf, size_t write_len,
		void *read_buf, size_t read_len);

/**
 * @defgroup register Register Access
 *
 * @brief Register access functions.
 *
 * These functions simplify interacting with slave devices that require a
 * repeated START condition to separate writing a register address and reading
 * back content. Register addresses are transmitted in host byte order; if a
 * slave device requires different byte ordering, @p reg should be converted
 * before being passed as an argument. See @c endian(3) for more details.
 *
 * @{
 */

/**
 * @brief Read bytes from an 8-bit slave register.
 *
 * @param dev  Pointer to an I2C character device handle.
 * @param addr I2C slave address.
 * @param reg  I2C slave register.
 * @param buf  Pointer to a buffer to receive bytes.
 * @param len  Number of bytes to read.
 *
 * @return Number of messages transferred on success, or -1 on error with @c
 * errno set appropriately.
 *
 * This function is equivalent to calling:
 * @code
 * i2cd_write_read(dev, addr, &reg, sizeof(reg), buf, len);
 * @endcode
 */
static inline int i2cd_register_read(struct i2cd *dev, uint16_t addr,
		uint8_t reg, void *buf, size_t len)
{
	return i2cd_write_read(dev, addr, &reg, sizeof(reg), buf, len);
}

/**
 * @brief Read bytes from a 16-bit slave register.
 *
 * @param dev  Pointer to an I2C character device handle.
 * @param addr I2C slave address.
 * @param reg  I2C slave register.
 * @param buf  Pointer to a buffer to receive bytes.
 * @param len  Number of bytes to read.
 *
 * @return Number of messages transferred on success, or -1 on error with @c
 * errno set appropriately.
 *
 * This function is equivalent to calling:
 * @code
 * i2cd_write_read(dev, addr, &reg, sizeof(reg), buf, len);
 * @endcode
 */
static inline int i2cd_register_read16(struct i2cd *dev, uint16_t addr,
		uint16_t reg, void *buf, size_t len)
{
	return i2cd_write_read(dev, addr, &reg, sizeof(reg), buf, len);
}

/** @} */
/** @} */

#ifdef __cplusplus
}
#endif

#endif /* I2CD_H */
