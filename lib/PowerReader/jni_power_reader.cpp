#include "jni_power_reader.h"


PowerReader::PowerReader() {
}


void PowerReader::setup() {
	esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_2_5, ADC_WIDTH_BIT_12, 0, &adc_cal);
	adc1_config_channel_atten(VBAT_ADC_CHANNEL, ADC_ATTEN_DB_2_5);
	pinMode(VBUS_SENSE, INPUT);
}


void PowerReader::updatePowerStatus() {
	powerStatus.batteryVoltage = getBatteryVoltage();
	powerStatus.usbPowerPresent = digitalRead(VBUS_SENSE);

	if (!powerStatus.usbPowerPresent) {
		if (powerStatus.batteryVoltage < 3.1) {
			esp_deep_sleep_start();
		} 
	}
}


float PowerReader::getBatteryVoltage() {
	uint32_t raw = adc1_get_raw(VBAT_ADC_CHANNEL);
	uint32_t millivolts = esp_adc_cal_raw_to_voltage(raw, &adc_cal);
	const uint32_t upper_divider = 442;
	const uint32_t lower_divider = 160;
	return (float)(upper_divider + lower_divider) / lower_divider / 1000 * millivolts;
}