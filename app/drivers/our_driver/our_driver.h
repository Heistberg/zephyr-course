#ifndef OUR_DRIVER_H
#define OUR_DRIVER_H

#include <zephyr/device.h>

#ifdef __cplusplus
extern "C" {
#endif

int our_driver_set_custom_value(const struct device *dev, int value);

#ifdef __cplusplus
}
#endif

#endif