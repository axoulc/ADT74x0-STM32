/**
 ***************************************************************************************
 * @file    ADT74x0.c
 * @brief   This file provides code to use temperature sensors Analog ADT7410 and ADT7420
 * @author	 Axel Chabot
 ***************************************************************************************
 */

#include "ADT74x0.h"

/**
 * Init function
 * @param adt74x0 Struct pointer
 * @param addr I2C address of the IC depend of the config a0, a1
 */
void ADT74x0_Init(ADT74X0 *adt74x0, uint8_t addr) {
	adt74x0->fault_queue = ADT74X0_1_FAULT;
	adt74x0->ct_polarity = ADT74X0_CT_ACTIVE_LOW;
	adt74x0->int_polarity = ADT74X0_INT_ACTIVE_LOW;
	adt74x0->int_ct_mode = ADT74X0_INTERRUPT_MODE;
	adt74x0->operation_mode = ADT74X0_CONTINOUS_MODE;
	adt74x0->resolution = ADT74X0_13BITS;
	adt74x0->address = (uint16_t) (addr << 1);
}

/**
 * Reset function
 * @param adt74x0 Struct pointer
 */
void ADT74x0_Reset(ADT74X0 *adt74x0) {
	uint8_t addr = ADT74X0_ADDR_RESET;
	HAL_I2C_Master_Transmit(adt74x0->adti2c, adt74x0->address, &addr, 1,
			ADT74X0_TIMEOUT_I2C);
	HAL_Delay(1);
	adt74x0->fault_queue = ADT74X0_1_FAULT;
	adt74x0->ct_polarity = ADT74X0_CT_ACTIVE_LOW;
	adt74x0->int_polarity = ADT74X0_INT_ACTIVE_LOW;
	adt74x0->int_ct_mode = ADT74X0_INTERRUPT_MODE;
	adt74x0->operation_mode = ADT74X0_CONTINOUS_MODE;
	adt74x0->resolution = ADT74X0_13BITS;
}

/**
 * Conversion state
 * @param adt74x0 Struct pointer
 * @return true when temperature conversion is done, false otherwise
 */
uint8_t ADT74x0_Ready(ADT74X0 *adt74x0) {
	uint8_t rdy = 0;
	rdy = (ADT74x0_GetRegisterValue(adt74x0, ADT74X0_ADDR_STATUS) & 0x80);
	return (!rdy);
}

/**
 * Identification number of the IC
 * @param adt74x0 Struct pointer
 * @return ID number
 */
uint8_t ADT74x0_ID(ADT74X0 *adt74x0) {
	return ADT74x0_GetRegisterValue(adt74x0, ADT74X0_ADDR_ID);
}

/**
 * Get register value from an address
 * @param adt74x0 Struct pointer
 * @param regAddress Register address
 * @return Return the value asked
 */
uint8_t ADT74x0_GetRegisterValue(ADT74X0 *adt74x0, uint8_t regAddress) {
	uint8_t receivedData = 0x00;
	HAL_I2C_Master_Transmit(adt74x0->adti2c, adt74x0->address, &regAddress, 1,
			ADT74X0_TIMEOUT_I2C);
	HAL_I2C_Master_Receive(adt74x0->adti2c, adt74x0->address, &receivedData, 1,
			ADT74X0_TIMEOUT_I2C);
	return receivedData;
}

/**
 * Set register value from an address
 * @param adt74x0 Struct pointer
 * @param regAddress Register address
 * @param regValue Value to write into the register
 */
void ADT74x0_SetRegisterValue(ADT74X0 *adt74x0, uint8_t regAddress,
		uint8_t regValue) {
	uint8_t data[2] = { 0x00, 0x00 };
	data[0] = regAddress;
	data[1] = regValue;
	HAL_I2C_Master_Transmit(adt74x0->adti2c, adt74x0->address, data, 2,
			ADT74X0_TIMEOUT_I2C);
}

/**
 * Change configuration of fault queue in case of noisy temperature environment.
 * @param adt74x0 Struct pointer
 * @param new New fault queue value
 */
void ADT74x0_SetFaultQueue(ADT74X0 *adt74x0, ADT74X0_Fault_Queue new) {
	uint8_t reg;
	reg = ADT74x0_GetRegisterValue(adt74x0, ADT74X0_ADDR_CONF);
	reg &= ~ADT74X0_FAULT_QUEUE(0xFF);
	reg |= ADT74X0_FAULT_QUEUE((uint8_t ) new);
	ADT74x0_SetRegisterValue(adt74x0, ADT74X0_ADDR_CONF, reg);
	adt74x0->fault_queue = new;
}

/**
 * Critical Overtemperature Indicator polarity
 * @param adt74x0 Struct pointer
 * @param new New polarity
 */
void ADT74x0_SetCTPolarity(ADT74X0 *adt74x0, ADT74X0_CT_Polarity new) {
	uint8_t reg;
	reg = ADT74x0_GetRegisterValue(adt74x0, ADT74X0_ADDR_CONF);
	reg &= ~ADT74X0_CT_POLARITY(0xFF);
	reg |= ADT74X0_CT_POLARITY((uint8_t ) new);
	ADT74x0_SetRegisterValue(adt74x0, ADT74X0_ADDR_CONF, reg);
	adt74x0->ct_polarity = new;
}

/**
 * Overtemperature and Undertemperature Indicator polarity
 * @param adt74x0 Struct pointer
 * @param new New polarity
 */
void ADT74x0_SetINTPolarity(ADT74X0 *adt74x0, ADT74X0_INT_Polarity new) {
	uint8_t reg;
	reg = ADT74x0_GetRegisterValue(adt74x0, ADT74X0_ADDR_CONF);
	reg &= ~ADT74X0_INT_POLARITY(0xFF);
	reg |= ADT74X0_INT_POLARITY((uint8_t ) new);
	ADT74x0_SetRegisterValue(adt74x0, ADT74X0_ADDR_CONF, reg);
	adt74x0->int_polarity = new;
}

/**
 * Set the functioning mode for CT and INT pins(comparator or interruption)
 * @param adt74x0 Struct pointer
 * @param new New mode
 */
void ADT74x0_SetINTCTMode(ADT74X0 *adt74x0, ADT74X0_INT_CT_Mode new) {
	uint8_t reg;
	reg = ADT74x0_GetRegisterValue(adt74x0, ADT74X0_ADDR_CONF);
	reg &= ~ADT74X0_INT_CT_MODE(0xFF);
	reg |= ADT74X0_INT_CT_MODE((uint8_t ) new);
	ADT74x0_SetRegisterValue(adt74x0, ADT74X0_ADDR_CONF, reg);
	adt74x0->int_ct_mode = new;
}

/**
 * Set Set the functioning mode for temperature conversion
 * @param adt74x0 Struct pointer
 * @param new New mode
 */
void ADT74x0_SetOperationMode(ADT74X0 *adt74x0, ADT74X0_Operation_Mode new) {
	uint8_t reg;
	reg = ADT74x0_GetRegisterValue(adt74x0, ADT74X0_ADDR_CONF);
	reg &= ~ADT74X0_OPERATION_MODE(0xFF);
	reg |= ADT74X0_OPERATION_MODE((uint8_t ) new);
	ADT74x0_SetRegisterValue(adt74x0, ADT74X0_ADDR_CONF, reg);
	adt74x0->operation_mode = new;
}

/**
 * Switch between 13bits and 16bits resolution
 * @param adt74x0 Struct pointer
 * @param new New resolution
 */
void ADT74x0_SetResolution(ADT74X0 *adt74x0, ADT74X0_Resolution new) {
	uint8_t reg;
	reg = ADT74x0_GetRegisterValue(adt74x0, ADT74X0_ADDR_CONF);
	reg &= ~ADT74X0_RESOLUTION(0xFF);
	reg |= ADT74X0_RESOLUTION((uint8_t ) new);
	ADT74x0_SetRegisterValue(adt74x0, ADT74X0_ADDR_CONF, reg);
	adt74x0->resolution = new;
}

/**
 * Set the high temperature setpoint
 * @param adt74x0 Struct pointer
 * @param newTemp New high temperature
 */
void ADT74x0_SetTHigh(ADT74X0 *adt74x0, uint16_t newTemp) {
	uint8_t data[3];
	data[0] = ADT74X0_ADDR_THIGH_MSB;
	data[1] = newTemp >> 8;
	data[2] = newTemp & 0xFF;
	HAL_I2C_Master_Transmit(adt74x0->adti2c, adt74x0->address, data, 3,
			ADT74X0_TIMEOUT_I2C);
}

/**
 * Set the low temperature setpoint
 * @param adt74x0 Struct pointer
 * @param newTemp New low temperature
 */
void ADT74x0_SetTLow(ADT74X0 *adt74x0, uint16_t newTemp) {
	uint8_t data[3];
	data[0] = ADT74X0_ADDR_TLOW_MSB;
	data[1] = newTemp >> 8;
	data[2] = newTemp & 0xFF;
	HAL_I2C_Master_Transmit(adt74x0->adti2c, adt74x0->address, data, 3,
			ADT74X0_TIMEOUT_I2C);
}

/**
 * Set the critical temperature setpoint
 * @param adt74x0 Struct pointer
 * @param newTemp New critical temperature
 */
void ADT74x0_SetTCrit(ADT74X0 *adt74x0, uint16_t newTemp) {
	uint8_t data[3];
	data[0] = ADT74X0_ADDR_TCRIT_MSB;
	data[1] = newTemp >> 8;
	data[2] = newTemp & 0xFF;
	HAL_I2C_Master_Transmit(adt74x0->adti2c, adt74x0->address, data, 3,
			ADT74X0_TIMEOUT_I2C);
}

/**
 * Set the hysteresis temperature setpoint
 * @param adt74x0 Struct pointer
 * @param newTemp New hysteresis temperature
 */
void ADT74x0_SetTHyst(ADT74X0 *adt74x0, uint8_t newTemp) {
	if (newTemp <= 15) {
		uint8_t hyst = newTemp & 0xF;
		ADT74x0_SetRegisterValue(adt74x0, ADT74X0_ADDR_THYST, hyst);
	}
}

/**
 * Start single conversion of temperature
 * @param adt74x0 Struct pointer
 * @return HAL_OK when everything good, HAL_TIMEOUT otherwise
 */
HAL_StatusTypeDef ADT74x0_ReadTemp(ADT74X0 *adt74x0) {
	ADT74x0_SetOperationMode(adt74x0, ADT74X0_ONE_SHOT_MODE);

	uint32_t startTime = HAL_GetTick();
	while (!ADT74x0_Ready(adt74x0)) {
		if (HAL_GetTick()
				- startTime> (uint32_t) 1.5 * ADT74X0_SINGLE_CONV_TIME) {
			return HAL_TIMEOUT;
		}
	}
	uint8_t addr = ADT74X0_ADDR_TEMP_MSB;
	uint8_t raw[2] = { 0x00, 0x00 };
	uint16_t raw_read = 0x0000;
	HAL_I2C_Master_Transmit(adt74x0->adti2c, adt74x0->address, &addr, 1,
			ADT74X0_TIMEOUT_I2C);
	HAL_I2C_Master_Receive(adt74x0->adti2c, adt74x0->address, raw, 2,
			ADT74X0_TIMEOUT_I2C);
	raw_read += (raw[0] << 8);
	raw_read += (raw[1] << 0);
	adt74x0->raw_data = raw_read;
	ADT74x0_RawToTemp(adt74x0);

	return HAL_OK;
}

/**
 * Convert raw temperature to decimal temperature
 * @param adt74x0 Struct pointer
 */
void ADT74x0_RawToTemp(ADT74X0 *adt74x0) {
	if (adt74x0->resolution == ADT74X0_16BITS) {
		if ((adt74x0->raw_data & 0x8000) == 0x8000) {
			adt74x0->deg_data = ((float) (adt74x0->raw_data - 65536)) / 128.0;
		} else {
			adt74x0->deg_data = ((float) (adt74x0->raw_data)) / 128.0;
		}
	} else {
		if ((adt74x0->raw_data & 0x8000) == 0x8000) {
			adt74x0->deg_data = ((float) (adt74x0->raw_data - 8192)) / 16.0;
		} else {
			adt74x0->deg_data = ((float) (adt74x0->raw_data)) / 16.0;
		}
	}
}
