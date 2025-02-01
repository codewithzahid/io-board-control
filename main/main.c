#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "gpio_config.h"
#include "uart_config.h"

void app_main(void) {
  uart_init();
  set_di_pin_mode(dio_pin0);
}
