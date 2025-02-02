while (1) {
    if (parsed_count >= 3) {
        if (strcmp(received_data.command, "SET") == 0) {
            else if (strcmp(received_data.parameter, PARAM_DBLINK) == 0) {
                // add function here
            }
            else if (strcmp(received_data.parameter, PARAM_AIN) == 0) {
                if (adc_channels[0] == received_data.channel) {
                    esp_err_t ret =
                        if (ret != ESP_OK) {
                        ESP_LOGE(GPIO_TAG, "ADC initialization failed");
                        uart_write_data("FAIL");
                    }
                    else {
                        uart_write_data("DONE");
                    }
                } else if (adc_channels[1] == received_data.channel) {
                    adc_config_t adc_conf = {
                        .channel = adc_channels[1],
                        .width = ADC_WIDTH_BIT_12,
                        .atten = ADC_ATTEN_DB_11};

                    esp_err_t ret = set_adc_pin_mode(&adc_conf);
                    if (ret != ESP_OK) {
                        ESP_LOGE(GPIO_TAG, "ADC initialization failed");
                        uart_write_data("FAIL");
                    } else {
                        ESP_LOGE(GPIO_TAG, "ADC initialization successful");
                        uart_write_data("DONE");
                    }
                }
            }
            else if (strcmp(received_data.parameter, PARAM_AOUT) == 0) {
                if (dac_channels[0] == received_data.channel) {
                    esp_err_t ret = set_dac_pin_mode(dac_channels[0]);
                    if (ret == ESP_OK) {
                        ESP_LOGE(GPIO_TAG, "DAC initialization successful");
                        uart_write_data("DONE");
                    } else {
                        ESP_LOGE(GPIO_TAG, "DAC initialization failed");
                        uart_write_data("FAIL");
                    }
                }
            }
            else if (strcmp(received_data.parameter, PARAM_BAUD_RATE) == 0) {
                baud_rate = received_data.channel;
                uart_write_data("DONE");
            }
        } else if (strcmp(received_data.command, "GET") == 0) {
            if (strcmp(received_data.parameter, PARAM_AIN) == 0) {
            }
        } else {
            ESP_LOGE(UART_TAG, "Invalid command received.");
        }
    }
}
}



for (int i = 0; i < NUM_DIO; i++) {
            if (dio_pin_map[i] == channel) {
                found = 1;
                if (write_do_pin(channel, &status) == ESP_OK) {
                    uart_write_data("DONE");
                } else {
                    uart_write_data("FAIL");
                }
                ESP_LOGI(GPIO_TAG, "GPIO %d status %d!", dio_pin_map[i], status);
                break;
            }
        }
        if (!found) {
            ESP_LOGE(GPIO_TAG, "Invalid GPIO channel: %d", received_data.channel);
            uart_write_data("INVALID CHANNEL");
        }