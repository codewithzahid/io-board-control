#ifndef GPIO_CONFIG_H
#define GPIO_CONFIG_H

#include <driver/adc.h>
#include <driver/dac.h>

#include "driver/gpio.h"
#include "esp_adc_cal.h"
#include "esp_err.h"
#include "esp_log.h"

typedef enum dio_pins {
  dio_pin0 = GPIO_NUM_2,   // GPIO2 (DIO PIN 1)
  dio_pin1 = GPIO_NUM_15,  // GPIO15 (DIO PIN 2)
  dio_pin2 = GPIO_NUM_5,   // GPIO5 (DIO PIN 3)
  dio_pin3 = GPIO_NUM_4,   // GPIO4 (DIO PIN 4)
  dio_pin4 = GPIO_NUM_21,  // GPIO21 (DIO PIN 5)
  dio_pin5 = GPIO_NUM_22,  // GPIO22 (DIO PIN 6)
  dio_pin6 = GPIO_NUM_23,  // GPIO23 (DIO PIN 7)
  dio_pin7 = GPIO_NUM_13   // GPIO13 (DIO PIN 8)
} dio_pins_t;

typedef enum ai_pins {
  ai_pin0 = ADC1_CHANNEL_0,  // GPIO36 (ADC1 Channel 0)
  ai_pin1 = ADC1_CHANNEL_3   // GPIO39 (ADC1 Channel 3)
} ai_pins_t;

typedef enum ao_pins {
  ao_pin0 = DAC_CHANNEL_1,  // GPIO25 (DAC1)
  ao_pin1 = DAC_CHANNEL_2   // GPIO26 (DAC2)
} ao_pins_t;

typedef struct {
  adc1_channel_t channel;
  adc_bits_width_t width;
  adc_atten_t atten;
} adc_config_t;

esp_err_t set_do_pin_mode(uint8_t pin);
esp_err_t set_di_pin_mode(uint8_t pin);
esp_err_t set_adc_pin_mode(adc_config_t *config);
esp_err_t set_dac_pin_mode(dac_channel_t channel);
esp_err_t write_dac(dac_channel_t channel, uint8_t value);
esp_err_t read_adc(adc1_channel_t channel, uint32_t *value);

#endif