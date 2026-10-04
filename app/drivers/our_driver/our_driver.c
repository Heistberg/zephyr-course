#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/gpio.h>

#define DT_DRV_COMPAT our_driver
#define LED_NODE DT_ALIAS(led0)

LOG_MODULE_REGISTER(our_driver, LOG_LEVEL_INF);

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

typedef struct{
    int custom_value;
}my_sensor_data_t;

static my_sensor_data_t my_sensor_data;

int our_driver_set_custom_value(const struct device *dev, int value)
{
    my_sensor_data_t *data = dev->data;

    data->custom_value = value;
    LOG_INF("New custom value: %d", value);

    return 0;
}

static int channel_get_my_impl(const struct device *dev,
				    enum sensor_channel chan,
				    struct sensor_value *val)
{
    gpio_pin_set_dt(&led, 0);
    LOG_INF("LED state: OFF");
    return 0;
}

static int channel_fetch_my_impl(const struct device *dev,
    enum sensor_channel chan)
{
    gpio_pin_set_dt(&led, 1);
    LOG_INF("LED state: ON");
    return 0;
}


static DEVICE_API(sensor, api_iomico_lecture) = {
    .channel_get = channel_get_my_impl,
    .sample_fetch = channel_fetch_my_impl,
};

static int init(const struct device* dev)
{
    bool ret_b;
    int ret_i;

    ret_b = gpio_is_ready_dt(&led);
    if (!ret_b) return -1;

    ret_i = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
    if (ret_i < 0) return -2;
    
    LOG_INF("Device initialized");
    return 0;
}

DEVICE_DT_INST_DEFINE(0, init, NULL, &my_sensor_data, NULL, POST_KERNEL, 80, &api_iomico_lecture);