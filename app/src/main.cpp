#include <zephyr/drivers/sensor.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <our_driver.h>

/* The devicetree node identifier for the "app-led" alias. */

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

const struct device* driver = DEVICE_DT_GET(DT_NODELABEL(our_driver0));

int main(void)
{
    struct sensor_value val;
    int iter = 0;
    bool led_state = true;
    
    while (1) {
        auto ret2 = sensor_sample_fetch(driver);
        k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);
        
        auto ret1 = sensor_channel_get(driver, SENSOR_CHAN_AMBIENT_TEMP, &val);
        k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);

        our_driver_set_custom_value(driver, iter++);
    }
    return 0;
}
