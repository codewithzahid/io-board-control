#ifndef UART_CONFIG_H
#define UART_CONFIG_H

#include <stdio.h>
#include <string.h>

#include "driver/gpio.h"
#include "driver/uart.h"
#include "esp_err.h"
#include "esp_log.h"

#define UART_PORT_NUM UART_NUM_0
#define UART_TX_PIN GPIO_NUM_1
#define UART_RX_PIN GPIO_NUM_3
#define BUF_SIZE 1024
int baud_rate = 115200

esp_err_t uart_init();
esp_err_t uart_write_data(const char *tx_buffer);
esp_err_t uart_read_data(char *rx_buffer, size_t buf_size);
void parse_command(char *cmd);

#endif