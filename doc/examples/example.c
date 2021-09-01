#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <i2cd.h>

int main(void)
{
	struct i2cd *dev;
	uint8_t buf[8];
	size_t i;

	/* Open I2C character device */
	dev = i2cd_open("/dev/i2c-0");
	if (dev == NULL) {
		perror(NULL);
		return EXIT_FAILURE;
	}

	/* Read bytes from slave address 0x20 */
	if (i2cd_read(dev, 0x20, buf, sizeof(buf)) < 0) {
		perror(NULL);
		i2cd_close(dev);
		return EXIT_FAILURE;
	}

	for (i = 0; i < sizeof(buf); i++)
		printf("[%zu] 0x%02x\n", i, buf[i]);

	/* Close character device handle */
	i2cd_close(dev);

	return EXIT_SUCCESS;
}
