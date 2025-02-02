#ifndef GPIO_CONFIG_H
#define GPIO_CONFIG_H

#include <driver/adc.h>
#include <driver/dac.h>

#include "driver/gpio.h"
#include "esp_adc_cal.h"
#include "esp_err.h"
#include "esp_log.h"

typedef struct {
  adc1_channel_t channel;
  adc_bits_width_t width;
  adc_atten_t atten;
} adc_config_t;

esp_err_t set_do_pin_mode(uint8_t pin);
esp_err_t set_di_pin_mode(uint8_t pin);
esp_err_t set_adc_pin_mode(adc_config_t *config);
esp_err_t set_dac_pin_mode(dac_channel_t channel);
esp_err_t write_do_pin(uint8_t pin, uint8_t level);
esp_err_t read_di_pin(uint8_t pin, uint8_t *level);
esp_err_t write_dac(dac_channel_t channel, uint8_t value);
esp_err_t read_adc(adc1_channel_t channel, uint32_t *value);

#endif