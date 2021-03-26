/**
 ***************************************************************************************
 * @file    ADT74x0.h
 * @brief   This header provides code to use temperature sensors Analog ADT7410 and ADT7420
 * @author	 Axel Chabot
 ***************************************************************************************
 */

#ifndef INC_ADT74X0_H_
#define INC_ADT74X0_H_

#include "main.h"

#define ADT74X0_ADDR_TEMP_MSB	0x00
#define ADT74X0_ADDR_TEMP_LSB	0x01
#define ADT74X0_ADDR_STATUS		0x02
#define ADT74X0_ADDR_CONF		0x03
#define ADT74X0_ADDR_THIGH_MSB	0x04
#define ADT74X0_ADDR_THIGH_LSB	0x05
#define ADT74X0_ADDR_TLOW_MSB	0x06
#define ADT74X0_ADDR_TLOW_LSB	0x07
#define ADT74X0_ADDR_TCRIT_MSB	0x08
#define ADT74X0_ADDR_TCRIT_LSB	0x09
#define ADT74X0_ADDR_THYST		0x0A
#define ADT74X0_ADDR_ID			0x0B
#define ADT74X0_ADDR_RESET		0x2F

#define ADT74X0_FAULT_QUEUE(x)		((x) & 0x3)
#define ADT74X0_CT_POLARITY(x)		(((x) & 0x1) << 2)
#define ADT74X0_INT_POLARITY(x)		(((x) & 0x1) << 3)
#define ADT74X0_INT_CT_MODE(x)		(((x) & 0x1) << 4)
#define ADT74X0_OPERATION_MODE(x)	(((x) & 0x3) << 5)
#define ADT74X0_RESOLUTION(x)		(((x) & 0x1) << 7)

#define ADT74X0_TIMEOUT_I2C			1000
#define ADT74X0_SINGLE_CONV_TIME	240

/**
 * Enum fault queue
 */
typedef enum {
	ADT74X0_1_FAULT = 0,/**< ADT74X0_1_FAULT */
	ADT74X0_2_FAULT, /**< ADT74X0_2_FAULT */
	ADT74X0_3_FAULT, /**< ADT74X0_3_FAULT */
	ADT74X0_4_FAULT /**< ADT74X0_4_FAULT */
} ADT74X0_Fault_Queue;

/**
 * Enum critical Overtemperature Indicator polarity
 */
typedef enum {
	ADT74X0_CT_ACTIVE_LOW = 0,/**< ADT74X0_CT_ACTIVE_LOW */
	ADT74X0_CT_ACTIVE_HIGH /**< ADT74X0_CT_ACTIVE_HIGH */
} ADT74X0_CT_Polarity;

/**
 * Enum Overtemperature and Undertemperature Indicator polarity
 */
typedef enum {
	ADT74X0_INT_ACTIVE_LOW = 0,/**< ADT74X0_INT_ACTIVE_LOW */
	ADT74X0_INT_ACTIVE_HIGH /**< ADT74X0_INT_ACTIVE_HIGH */
} ADT74X0_INT_Polarity;

/**
 * Enum functioning mode for CT and INT pins
 */
typedef enum {
	ADT74X0_INTERRUPT_MODE = 0,/**< ADT74X0_INTERRUPT_MODE */
	ADT74X0_COMPARATOR_MODE /**< ADT74X0_COMPARATOR_MODE */
} ADT74X0_INT_CT_Mode;

/**
 * Enum conversion mode
 */
typedef enum {
	ADT74X0_CONTINOUS_MODE = 0,/**< ADT74X0_CONTINOUS_MODE */
	ADT74X0_ONE_SHOT_MODE, /**< ADT74X0_ONE_SHOT_MODE */
	ADT74X0_1SPS_MODE, /**< ADT74X0_1SPS_MODE */
	ADT74X0_SHUTDOWN_MODE /**< ADT74X0_SHUTDOWN_MODE */
} ADT74X0_Operation_Mode;

/**
 * Enum resolution
 */
typedef enum {
	ADT74X0_13BITS = 0,/**< ADT74X0_13BITS */
	ADT74X0_16BITS /**< ADT74X0_16BITS */
} ADT74X0_Resolution;

/**
 * Struct to easily use configuration and values from the IC
 */
typedef struct {
	I2C_HandleTypeDef *adti2c; /**< I2C handler */
	uint16_t address; /**< I2C address */

	ADT74X0_Fault_Queue fault_queue; /**< Fault queue enum */
	ADT74X0_CT_Polarity ct_polarity; /**< CT polarity enum */
	ADT74X0_INT_Polarity int_polarity; /**< INT polarity enum */
	ADT74X0_INT_CT_Mode int_ct_mode; /**< Interrupt or comparator enum */
	ADT74X0_Operation_Mode operation_mode;/**< Conversion mode enum */
	ADT74X0_Resolution resolution; /**< Resolution enum */

	uint16_t raw_data; /**< Raw temperature data */
	float deg_data; /**< Decimal temperature data */

} ADT74X0;

void ADT74x0_Init(ADT74X0 *adt74x0, uint8_t addr);
void ADT74x0_Reset(ADT74X0 *adt74x0);
uint8_t ADT74x0_Ready(ADT74X0 *adt74x0);
uint8_t ADT74x0_ID(ADT74X0 *adt74x0);
uint8_t ADT74x0_GetRegisterValue(ADT74X0 *adt74x0, uint8_t regAddress);
void ADT74x0_SetRegisterValue(ADT74X0 *adt74x0, uint8_t regAddress,
		uint8_t regValue);
void ADT74x0_SetFaultQueue(ADT74X0 *adt74x0, ADT74X0_Fault_Queue new);
void ADT74x0_SetCTPolarity(ADT74X0 *adt74x0, ADT74X0_CT_Polarity new);
void ADT74x0_SetINTPolarity(ADT74X0 *adt74x0, ADT74X0_INT_Polarity new);
void ADT74x0_SetINTCTMode(ADT74X0 *adt74x0, ADT74X0_INT_CT_Mode new);
void ADT74x0_SetOperationMode(ADT74X0 *adt74x0, ADT74X0_Operation_Mode new);
void ADT74x0_SetResolution(ADT74X0 *adt74x0, ADT74X0_Resolution new);
void ADT74x0_SetTHigh(ADT74X0 *adt74x0, uint16_t newTemp);
void ADT74x0_SetTLow(ADT74X0 *adt74x0, uint16_t newTemp);
void ADT74x0_SetTCrit(ADT74X0 *adt74x0, uint16_t newTemp);
void ADT74x0_SetTHyst(ADT74X0 *adt74x0, uint8_t newTemp);
HAL_StatusTypeDef ADT74x0_ReadTemp(ADT74X0 *adt74x0);
void ADT74x0_RawToTemp(ADT74X0 *adt74x0);

#endif /* INC_ADT74X0_H_ */
