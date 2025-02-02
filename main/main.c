#include <stdio.h>
#include <string.h>

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "gpio_config.h"
#include "uart_config.h"

const char* UART_TAG = "UART";
const char* GPIO_TAG = "GPIO";

typedef struct {
  char command[20];
  char parameter[20];
  int channel;
  int value;
} received_data_t;

void init_received_data(received_data_t* data) {
  data->value = -1;
}

#define PARAM_DIN "DIN"
#define PARAM_DOUT "DOUT"
#define PARAM_DBLINK "BLINK"
#define PARAM_AIN "AIN"
#define PARAM_AOUT "AOUT"
#define PARAM_BAUD_RATE "BAUDRATE"

// predefined constant digital i/o pins
#define NUM_DIO 8
const int dio_pin_map[NUM_DIO] = {GPIO_NUM_2, GPIO_NUM_15, GPIO_NUM_5, GPIO_NUM_4, GPIO_NUM_21, GPIO_NUM_22, GPIO_NUM_23, GPIO_NUM_13};

void dio_operation(uint8_t channel, bool mode /* 1 = input, 0 = output*/) {
  bool found = 0;
  if (mode) {
    for (int i = 0; i < NUM_DIO; i++) {
      if (dio_pin_map[i] == channel) {
        found = 1;
        if (set_di_pin_mode(dio_pin_map[i]) == ESP_OK) {
          ESP_LOGI(GPIO_TAG, "Pin mode setup done as input for GPIO %d!", dio_pin_map[i]);
          uart_write_data("DONE");
        } else {
          ESP_LOGI(GPIO_TAG, "Pin mode setup failed as input for GPIO %d", dio_pin_map[i]);
          uart_write_data("FAIL");
        }
        break;
      }
    }
    if (!found) {
      ESP_LOGE(GPIO_TAG, "Invalid GPIO channel: %d", received_data.channel);
      uart_write_data("INVALID CHANNEL");
    }
  } else {
    for (int i = 0; i < NUM_DIO; i++) {
      if (dio_pin_map[i] == channel) {
        found = 1;
        if (set_do_pin_mode(dio_pin_map[i]) == ESP_OK) {
          ESP_LOGI(GPIO_TAG, "Pin mode setup done as output for GPIO %d!", dio_pin_map[i]);
          uart_write_data("DONE");
        } else {
          ESP_LOGI(GPIO_TAG, "Pin mode setup failed as output for GPIO %d", dio_pin_map[i]);
          uart_write_data("FAIL");
        }
        break;
      }
    }
    if (!found) {
      ESP_LOGE(GPIO_TAG, "Invalid GPIO channel: %d", received_data.channel);
      uart_write_data("INVALID CHANNEL");
    }
  }
}
void app_main(void) {
  if (uart_init() == ESP_OK) {
    ESP_LOGI(UART_TAG, "UART initialization done!");
  } else {
    ESP_LOGE(UART_TAG, "UART initialization failed.");
  }

  char receive_buffer[BUF_SIZE];
  received_data_t received_data;
  init_received_data(&received_data);

  while (1) {
    esp_err_t status = uart_read_data(receive_buffer, BUF_SIZE);
    if (status == ESP_OK) {
      ESP_LOGI(UART_TAG, "Received: %s", receive_buffer);
      int parsed_count = sscanf(receive_buffer, "%s %s %d %d", received_data.command, received_data.parameter, &received_data.channel, &received_data.value);
      if (parsed_count >= 3) {
        if (strcmp(received_data.command, "SET") == 0) {
          if (strcmp(received_data.parameter, PARAM_DIN) == 0) {
            dio_operation(received_data.channel, 1);
          }
        }
      } else if (strcmp(received_data.command, "GET") == 0) {
        // get operation
      } else {
        ESP_LOGE(UART_TAG, "Invalid command received.");
      }
    }
  }
}
