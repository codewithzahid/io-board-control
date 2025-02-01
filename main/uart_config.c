#include "uart_config.h"

esp_err_t uart_init() {
  uart_config_t uart_conf = {
      .baud_rate = UART_BAUD_RATE,
      .data_bits = UART_DATA_8_BITS,
      .parity = UART_PARITY_DISABLE,
      .stop_bits = UART_STOP_BITS_1,
      .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
      .source_clk = UART_SCLK_DEFAULT};

  esp_err_t ret = uart_driver_install(UART_PORT_NUM, BUF_SIZE * 2, 0, 0, NULL, 0);
  if (ret != ESP_OK) {
    return ret;
  }

  ret = uart_param_config(UART_PORT_NUM, &uart_conf);
  if (ret != ESP_OK) {
    return ret;
  }
  ret = uart_set_pin(UART_PORT_NUM, UART_TX_PIN, UART_RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
  if (ret != ESP_OK) {
    return ret;
  }
  return ESP_OK;
}

// Function to write data to UART
esp_err_t uart_write_data(const char *tx_buffer) {  // Changed parameter type to pointer
  if (tx_buffer == NULL) {                          // Added NULL check
    return ESP_ERR_INVALID_ARG;
  }

  size_t len = strlen(tx_buffer);
  if (len > 0) {
    esp_err_t ret = uart_write_bytes(UART_PORT_NUM, tx_buffer, len);
    return ret;
  } else {
    return ESP_ERR_INVALID_ARG;
  }
}

// Function to read data from UART
esp_err_t uart_read_data(char *rx_buffer, size_t buf_size) {  // Added buffer size parameter
  if (rx_buffer == NULL || buf_size == 0) {
    return ESP_ERR_INVALID_ARG;
  }

  int data_len = uart_read_bytes(UART_PORT_NUM, (uint8_t *)rx_buffer, buf_size - 1, pdMS_TO_TICKS(100));
  if (data_len > 0) {
    rx_buffer[data_len] = '\0';  // Null terminate the string
    return ESP_OK;
  }
  return ESP_ERR_TIMEOUT;  // Return timeout if no data received
}

/*
void parse_command(char* cmd) {
    char command[20];
    char parameter[20];
    int channel;
    int value;

    // Parse command like "SET AOUT 1 512"
    if (sscanf(cmd, "%s %s %d %d", command, parameter, &channel, &value) == 4) {
        if (strcmp(command, "SET") == 0 && strcmp(parameter, "AOUT") == 0) {
            printf("Parsed Command:\n");
            printf("Action: %s\n", command);
            printf("Parameter: %s\n", parameter);
            printf("Channel: %d\n", channel);
            printf("Value: %d\n", value);

            // Here you can add your logic to handle the analog output
            // For example: set_analog_output(channel, value);
        }
    } else {
        printf("Invalid command format\n");
    }
}
*/