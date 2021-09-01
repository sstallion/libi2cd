# libi2cd Documentation                                                 {#index}

libi2cd provides a simple and straightforward API for accessing I2C devices from
userspace. It relies on the [i2c-dev][1] Linux kernel module and is intended to
complement existing tools and libraries, such as those provided by
[i2c-tools][2]. The design for libi2cd was inspired by [libgpiod][3]; it
provides both high- and low-level access to the underlying ioctl requests and
has no dependencies apart from the standard C library.

libi2cd development is hosted on [GitHub][4]. A [code coverage report][5] is
also available.

## Getting Started

libi2cd's main data structures and functions are documented in the
[Main API](@ref main) module. Most uses of the API are straightforward: an I2C
character device handle is first created by calling i2cd_open(), which is then
passed to supporting functions followed by a call to i2cd_close() to close the
handle and free associated memory. A set of register access functions are also
available, which are documented in the [Register Access](@ref register) module.

The following example demonstrates reading bytes from a fictitious slave device
located at address `0x20`:

@include example.c

For more advanced uses, the i2cd_get_functionality() and i2cd_transfer()
functions may be called to get the adapter functionality mask and to transfer
one or more low-level messages, respectively.

Care should be taken if the character device handle is shared between threads as
libi2cd is not inherently thread-safe. Calls using the same handle should be
restricted to a single thread or synchronized using a mutual exclusion
mechanism.

## License

libi2cd is distributed under the terms of the GNU Lesser General Public License
as published by the Free Software Foundation; either version 2.1 of the License,
or any later version. See [COPYING] for more details.

[1]: https://www.kernel.org/doc/Documentation/i2c/dev-interface
[2]: https://git.kernel.org/pub/scm/utils/i2c-tools/i2c-tools.git/
[3]: https://git.kernel.org/pub/scm/libs/libgpiod/libgpiod.git/
[4]: https://github.com/sstallion/libi2cd
[5]: https://sstallion.github.io/libi2cd/coverage/

[COPYING]: https://github.com/sstallion/libi2cd/blob/master/COPYING
