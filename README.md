# libi2cd

libi2cd provides a simple and straightforward API for accessing I2C devices from
userspace. It relies on the [i2c-dev][1] Linux kernel module and is intended to
complement existing tools and libraries, such as those provided by
[i2c-tools][2]. The design for libi2cd was inspired by [libgpiod][3]; it
provides both high- and low-level access to the underlying ioctl requests and
has no dependencies apart from the standard C library.

libi2cd documentation is hosted on [GitHub Pages][4]. A [code coverage report][5]
is also available.

## Installation

> **Note**: These instructions assume you are installing from a source
> distribution and have a working Linux installation (most distributions are
> supported) with GCC and GNU Autotools installed. If installing from source
> control, see [HACKING.md].

[cmocka][6] is required for building tests and should be installed using the
system package manager (eg. `libcmocka-dev` on Debian-based distributions via
`apt-get`).

If you wish to build without tests, the `--disable-tests` option may be passed
to `configure`:

    $ ./configure --disable-tests

To build and install libi2cd, issue the following:

    $ ./configure
    $ make all
    $ make check
    $ make install

By default, `make install` will install files in `/usr/local`, which may require
superuser privileges.

## Hacking

Pull requests are welcome! See [HACKING.md] for more details.

## License

libi2cd is distributed under the terms of the GNU Lesser General Public License
as published by the Free Software Foundation; either version 2.1 of the License,
or any later version. See [COPYING] for more details.

[1]: https://www.kernel.org/doc/Documentation/i2c/dev-interface
[2]: https://git.kernel.org/pub/scm/utils/i2c-tools/i2c-tools.git/
[3]: https://git.kernel.org/pub/scm/libs/libgpiod/libgpiod.git/
[4]: https://sstallion.github.io/libi2cd/
[5]: https://sstallion.github.io/libi2cd/coverage/
[6]: https://cmocka.org/

[COPYING]: COPYING
[HACKING.md]: HACKING.md
