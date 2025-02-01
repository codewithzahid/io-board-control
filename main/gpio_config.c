#include "gpio_config.h"

#define DEFAULT_VREF 1100  // Use default 1.1V reference voltage
#define ADC_SAMPLES 64     // Number of samples for averaging

static esp_adc_cal_characteristics_t *adc_chars = NULL;

esp_err_t set_do_pin_mode(uint8_t pin) {
  gpio_config_t io_conf = {
      .pin_bit_mask = (1ULL << pin),
      .mode = GPIO_MODE_OUTPUT,
      .pull_up_en = GPIO_PULLUP_DISABLE,
      .pull_down_en = GPIO_PULLDOWN_DISABLE,
      .intr_type = GPIO_INTR_DISABLE};

  esp_err_t status = gpio_config(&io_conf);
  return status;
}

esp_err_t set_di_pin_mode(uint8_t pin) {
  gpio_config_t io_config = {
      .pin_bit_mask = (1ULL << pin),
      .mode = GPIO_MODE_OUTPUT,
      .pull_up_en = GPIO_PULLUP_DISABLE,
      .pull_down_en = GPIO_PULLDOWN_DISABLE,
      .intr_type = GPIO_INTR_DISABLE};

  esp_err_t status = gpio_config(&io_config);
  return status;
}

// Write to digital output pin
esp_err_t write_do_pin(uint8_t pin, uint8_t level) {
  if (pin >= GPIO_NUM_MAX || level > 1) {
    return ESP_ERR_INVALID_ARG;
  }

  return gpio_set_level(pin, level);
}

// Read from digital input pin
esp_err_t read_di_pin(uint8_t pin, uint8_t *level) {
  if (pin >= GPIO_NUM_MAX || level == NULL) {
    return ESP_ERR_INVALID_ARG;
  }

  *level = gpio_get_level(pin);
  return ESP_OK;
}

esp_err_t set_adc_pin_mode(adc_config_t *config) {
  if (config == NULL) {
    return ESP_ERR_INVALID_ARG;
  }

  esp_err_t ret = adc1_config_width(config->width);
  if (ret != ESP_OK) {
    return ret;
  }

  ret = adc1_config_channel_atten(config->channel, config->atten);
  if (ret != ESP_OK) {
    return ret;
  }

  if (adc_chars == NULL) {
    return ESP_ERR_INVALID_STATE;
  }
  return ESP_OK;
}

esp_err_t set_dac_pin_mode(dac_channel_t channel) {
  if (channel != DAC_CHANNEL_1 || channel != DAC_CHANNEL_2) {
    return ESP_ERR_INVALID_ARG;
  }
  esp_err_t ret = dac_output_enable(channel);
  return ret;
}

esp_err_t write_dac(dac_channel_t channel, uint8_t value) {
  if (channel != DAC_CHANNEL_1 || channel != DAC_CHANNEL_2) {
    return ESP_ERR_INVALID_ARG;
  }
  esp_err_t ret = dac_output_voltage(channel, value);
  return ret;
}

esp_err_t read_adc(adc1_channel_t channel, uint32_t *value) {
  if (value == NULL) {
    return ESP_ERR_INVALID_ARG;
  }

  uint32_t adc_reading = 0;
  for (int8_t i = 0; i < ADC_SAMPLES; i++) {
    adc_reading += adc1_get_raw(channel);
  }
  *value = adc_reading / ADC_SAMPLES;
  return ESP_OK;
}