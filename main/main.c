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

void dio_operation(uint8_t channel, int mode) {
    bool found = 0;
    if (mode == 1) {
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
            ESP_LOGE(GPIO_TAG, "Invalid GPIO channel: %d", channel);
            uart_write_data("INVALID CHANNEL");
        }
    } else if (mode == 2) {
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
            ESP_LOGE(GPIO_TAG, "Invalid GPIO channel: %d", channel);
            uart_write_data("INVALID CHANNEL");
        }
    } else if (mode == 3) {
        for (int i = 0; i < NUM_DIO; i++) {
            if (dio_pin_map[i] == channel) {
                found = 1;
                int status;
                if (read_dio_pin(channel, &status) == ESP_OK) {
                    uart_write_data(status);
                } else {
                    uart_write_data("FAIL");
                }
                ESP_LOGI(GPIO_TAG, "GPIO %d status %d!", dio_pin_map[i], status);
                break;
            }
        }
        if (!found) {
            ESP_LOGE(GPIO_TAG, "Invalid GPIO channel: %d", channel);
            uart_write_data("INVALID CHANNEL");
        }
    }
}

int blink_frequency[8];
bool blink_channel[8];

void handle_uart_task(void* pvParameters);
void handle_blink_task(void* pvParameters);

void app_main(void) {
    // set adc channels as pin mode
    adc_config_t adc_conf1 = {
        .channel = ADC1_CHANNEL_0,
        .width = ADC_WIDTH_BIT_12,
        .atten = ADC_ATTEN_DB_11};
    set_adc_pin_mode(&adc_conf1);

    adc_config_t adc_conf2 = {
        .channel = ADC1_CHANNEL_3,
        .width = ADC_WIDTH_BIT_12,
        .atten = ADC_ATTEN_DB_11};
    set_adc_pin_mode(&adc_conf2);

    // set dac channel as pin mode
    set_dac_pin_mode(DAC_CHANNEL_1);
    set_dac_pin_mode(DAC_CHANNEL_2);

    // create tasks
    xTaskCreate(handle_uart_task, "handle_uart_task", 2048, NULL, 1, NULL);
    xTaskCreate(handle_blink_task, "handle_blink_task", 2048, NULL, 2, NULL);
}

void handle_blink_task(void* pvParameters) {
    while (1) {
        for (int8_t 0; i <= NUM_DIO; i++) {
            if (blink_channel[i]) {
                write_dio_pin(dio_pin_map[i], 1);
                vTaskDelay(pdMS_TO_TICKS(blink_frequency[i] / 2));
                write_dio_pin(dio_pin_map[i], 0);
                vTaskDelay(pdMS_TO_TICKS(blink_frequency[i] / 2));
            }
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void handle_uart_task(void* pvParameters) {
    if (uart_init() == ESP_OK) {
        ESP_LOGI(UART_TAG, "UART initialization done!");
    } else {
        ESP_LOGE(UART_TAG, "UART initialization failed.");
    }

    char receive_buffer[BUF_SIZE];
    received_data_t received_data;

    while (1) {
        init_received_data(&received_data);
        esp_err_t status = uart_read_data(receive_buffer, BUF_SIZE);
        if (status == ESP_OK) {
            ESP_LOGI(UART_TAG, "Received: %s", receive_buffer);
            int parsed_count = sscanf(receive_buffer, "%s %s %d %d", received_data.command, received_data.parameter, &received_data.channel, &received_data.value);
            if (parsed_count >= 3 && parsed_count < 4) {
                if (strcmp(received_data.command, "SET") == 0) {
                    if (strcmp(received_data.parameter, PARAM_DIN) == 0) {
                        dio_operation(received_data.channel, 1);
                    } else if (strcmp(received_data.parameter, PARAM_DOUT) == 0) {
                        dio_operation(received_data.channel, 2);
                    } else if (strcmp(received_data.parameter, PARAM_BAUD_RATE) == 0) {
                        baud_rate = received_data.channel;
                    }
                } else if (strcmp(received_data.command, "GET") == 0) {
                    if (strcmp(received_data.parameter, PARAM_DIN) == 0 || strcmp(received_data.parameter, PARAM_DOUT) == 0) {
                        dio_operation(received_data.channel, 3);
                    } else if (strcmp(received_data.parameter, PARAM_AIN) == 0) {
                        if (ADC1_CHANNEL_0 == received_data.channel) {
                            int readings;
                            read_adc(ADC1_CHANNEL_0, &readings);
                            uart_write_data(readings);
                        } else if (ADC1_CHANNEL_3 == received_data.channel) {
                            int readings;
                            read_adc(ADC1_CHANNEL_3, &readings);
                            uart_write_data(readings);
                        } else {
                            uart_write_data("INVALID CHANNEL");
                        }
                    }
                }
            } else if (parse_count >= 4) {
                bool found = 0;
                if (strcmp(received_data.command, "SET") == 0) {
                    if (strcmp(received_data.parameter, PARAM_DOUT) == 0) {
                        for (int i = 0; i < NUM_DIO; i++) {
                            if (dio_pin_map[i] == received_data.channel) {
                                found = 1;
                                blink_channel[i] = false;
                                if (write_dio_pin(received_data.channel, &received_data.value) == ESP_OK) {
                                    uart_write_data("DONE");
                                } else {
                                    uart_write_data("FAIL");
                                }
                                ESP_LOGI(GPIO_TAG, "GPIO %d status %d!", dio_pin_map[i], received_data.value);
                                break;
                            }
                        }
                        if (!found) {
                            ESP_LOGE(GPIO_TAG, "Invalid GPIO channel: %d", received_data.channel);
                            uart_write_data("INVALID CHANNEL");
                        }
                    } else if (strcmp(received_data.parameter, PARAM_DBLINK) == 0) {
                        for (int i = 0; i < NUM_DIO; i++) {
                            if (dio_pin_map[i] == received_data.channel) {
                                found = 1;
                                blink_channel[i] = true;
                                blink_frequency[i] = received_data.value;
                                uart_write_data("DONE");
                                break;
                            }
                        }
                        if (!found) {
                            ESP_LOGE(GPIO_TAG, "Invalid GPIO channel: %d", received_data.channel);
                            uart_write_data("INVALID CHANNEL");
                        }
                    } else if (strcmp(received_data.parameter, PARAM_AOUT) == 0) {
                        if (DAC_CHANNEL_1 == received_data.channel) {
                            write_dac(DAC_CHANNEL_1, received_data.value);
                            uart_write_data("DONE");
                        } else if (DAC_CHANNEL_2 == received_data.channel) {
                            write_dac(DAC_CHANNEL_2, received_data.value);
                            uart_write_data("DONE");
                        } else {
                            uart_write_data("FAIL");
                        }
                    }
                }
            }
        }
    }
}