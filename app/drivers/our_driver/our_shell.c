#include <zephyr/shell/shell.h>
#include <zephyr/drivers/sensor.h>
#include <our_driver.h>

const struct device* shell_driver = DEVICE_DT_GET(DT_NODELABEL(our_driver0));


static int cmd_fetch(const struct shell *sh, size_t argc, char **argv)
{
    sensor_sample_fetch(shell_driver);
    return 0;
}

static int cmd_read(const struct shell *sh, size_t argc, char **argv)
{
    struct sensor_value val;
    int err;
    enum sensor_channel channel = (enum sensor_channel)shell_strtoul(argv[1], 10, &err);

    int r_value = sensor_channel_get(shell_driver, channel, &val);

    if(r_value == 0){
        shell_print(sh, "Sensor %d read: %d", channel, val.val1);
    }
    return 0;
}

static int cmd_info(const struct shell *sh, size_t argc, char **argv)
{
    bool isInitialized = shell_driver->state->initialized;
    if(isInitialized){
        shell_print(sh, "This device is %s, and is initialized", shell_driver->name);
    } else {
        shell_print(sh, "This device is %s, and is NOT initialized", shell_driver->name);
    }
    return 0;
}

static int cmd_set(const struct shell *sh, size_t argc, char **argv)
{
    int err;
    int value = (int)shell_strtol(argv[1], 10, &err);

    int r_value = our_driver_set_custom_value(shell_driver, value);

    if(value > 1000){
        shell_error(sh, "Input argument too large. Maximum 1000, current %d", value);
        return -1;
    }

    if(r_value == 0){
        shell_print(sh, "Sensor set: %d", value);
    }
    return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(
    sub_sensor,
    SHELL_CMD_ARG(fetch, NULL, "fetch sensor data", cmd_fetch, 2, 0),
    SHELL_CMD_ARG(read, NULL, "read sensor data", cmd_read, 2, 0),
    SHELL_CMD_ARG(info, NULL, "info sensor data", cmd_info, 1, 0),
    SHELL_CMD_ARG(set, NULL, "set sensor data", cmd_set, 2, 0),
    SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(sensor, &sub_sensor, "Sensor commands", NULL);