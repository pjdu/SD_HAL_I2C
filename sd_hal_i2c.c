/*
 * sd_hal_i2c.h
 *
 *  Created on: Mar 5, 2016
 *      Author: Sina Darvishi
 */

#ifndef DRIVERS_MYLIB_SD_HAL_I2C_H_
#define DRIVERS_MYLIB_SD_HAL_I2C_H_


/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif


#include "stm32f0xx_hal.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

 /**
 * @}
 */

/**
 * @defgroup SD_I2C_Typedefs
 * @brief    Library Typedefs
 * @{
 */


/**
 * @brief  I2C result enumeration
 */
typedef enum {
	SD_I2C_Result_Ok       = 0x00,     /*!< Everything OK */
	SD_I2C_Result_Error    = 0x01,     /*!< An error has occurred */
	SD_I2C_Result_Busy     = 0x02,	   /*!< I2C is Busy */
	SD_I2C_Result_BERR     = 0x03,     /*!< BERR error */
	SD_I2C_Result_ARLO     = 0x04,     /*!< ARLO error */
	SD_I2C_Result_AF       = 0x05,     /*!< ACKF error */
	SD_I2C_Result_OVR      = 0x06,     /*!< OVR error */
	SD_I2C_Result_DMA      = 0x07,     /*!< DMA error */
	SD_I2C_Result_TIMEOUT  = 0x08,     /*!< Timeout error */
	SD_I2C_Result_SIZE     = 0x09,     /*!< Size Management error */
} SD_I2C_Result;

/**
 * @}
 */

/**
 * @defgroup SD_I2C_Functions
 * @brief    Library Functions
 * @{
 */


/**
 * @brief  Checks if device is connected to I2C and ready to use
 * @param  *I2Cx: Pointer to I2Cx peripheral to be used in communication
 * @param  device_address: 7-bit, left aligned device address used for communication
 * @retval One of @ref SD_I2C_Result enumeration
 */

SD_I2C_Result SD_I2C_CheckError(I2C_HandleTypeDef* I2Cx);
SD_I2C_Result SD_I2C_IsDeviceConnected(I2C_HandleTypeDef* I2Cx, uint8_t address);
SD_I2C_Result SD_I2C_WriteBit(I2C_HandleTypeDef* I2Cx,uint8_t device_address, uint8_t register_address, uint8_t bitNum, uint8_t data);
SD_I2C_Result SD_I2C_WriteBitW(I2C_HandleTypeDef* I2Cx,uint8_t device_address, uint8_t register_address, uint8_t bitNum,uint16_t data);
SD_I2C_Result SD_I2C_WriteBits(I2C_HandleTypeDef* I2Cx,uint8_t device_address, uint16_t register_address, uint8_t bitStart, uint8_t length,uint8_t data);
SD_I2C_Result SD_I2C_WriteBitsW(I2C_HandleTypeDef* I2Cx,uint8_t device_address, uint16_t register_address, uint8_t bitStart, uint8_t length,uint16_t data);
SD_I2C_Result SD_I2C_Write(I2C_HandleTypeDef* I2Cx, uint8_t device_address, uint8_t register_address, uint8_t data);
SD_I2C_Result SD_I2C_WriteSome(I2C_HandleTypeDef* I2Cx, uint8_t device_address, uint16_t register_address, uint16_t count, uint8_t *data);
SD_I2C_Result SD_I2C_WriteByte(I2C_HandleTypeDef* I2Cx, uint8_t device_address, uint8_t register_address, uint8_t data);
SD_I2C_Result SD_I2C_WriteBytes(I2C_HandleTypeDef* I2Cx, uint8_t device_address, uint8_t register_address, uint8_t count, uint8_t *data);
SD_I2C_Result SD_I2C_WriteWord(I2C_HandleTypeDef* I2Cx,uint8_t device_address, uint8_t register_address,uint16_t data);
SD_I2C_Result SD_I2C_WriteWords(I2C_HandleTypeDef* I2Cx,uint8_t device_address, uint8_t register_address, uint8_t length,uint16_t *data);
SD_I2C_Result SD_I2C_WriteWithNoRegisterAddress(I2C_HandleTypeDef* I2Cx, uint8_t device_address, uint8_t data);
SD_I2C_Result SD_I2C_WriteMultiWithNoRegisterAddress(I2C_HandleTypeDef* I2Cx, uint8_t device_address, uint8_t* data, uint16_t count);
SD_I2C_Result SD_I2C_WriteWith16BitRegisterAddress(I2C_HandleTypeDef* I2Cx, uint8_t device_address, uint16_t register_address, uint8_t data);
SD_I2C_Result SD_I2C_ReadBit(I2C_HandleTypeDef* I2Cx,uint8_t device_address, uint8_t register_address, uint8_t bitNum, uint8_t *data);
SD_I2C_Result SD_I2C_ReadBitW(I2C_HandleTypeDef* I2Cx,uint8_t device_address, uint8_t register_address, uint8_t bitNum,uint16_t *data);
SD_I2C_Result SD_I2C_ReadBits(I2C_HandleTypeDef* I2Cx,uint8_t device_address, uint16_t register_address, uint8_t bitStart, uint8_t length,uint8_t *data);
SD_I2C_Result SD_I2C_ReadBitsW(I2C_HandleTypeDef* I2Cx,uint8_t device_address, uint16_t register_address, uint8_t bitStart, uint8_t length,uint16_t *data);
SD_I2C_Result SD_I2C_Read(I2C_HandleTypeDef* I2Cx, uint8_t device_address, uint8_t register_address, uint8_t *data);
SD_I2C_Result SD_I2C_ReadSome(I2C_HandleTypeDef* I2Cx, uint8_t device_address, uint8_t register_address, uint16_t count, uint8_t *data);
SD_I2C_Result SD_I2C_ReadByte(I2C_HandleTypeDef* I2Cx, uint8_t device_address, uint8_t register_address, uint8_t *data);
SD_I2C_Result SD_I2C_ReadBytes(I2C_HandleTypeDef* I2Cx, uint8_t device_address, uint8_t register_address, uint8_t count, uint8_t *data);
SD_I2C_Result SD_I2C_ReadWord(I2C_HandleTypeDef* I2Cx,uint8_t device_address, uint8_t register_address,uint16_t *data);
SD_I2C_Result SD_I2C_ReadWords(I2C_HandleTypeDef* I2Cx,uint8_t device_address, uint8_t register_address, uint8_t length,uint16_t *data);
SD_I2C_Result SD_I2C_ReadWithNoRegisterAddress(I2C_HandleTypeDef* I2Cx, uint8_t device_address, uint8_t* data);
SD_I2C_Result SD_I2C_ReadSomeWithNoRegisterAddress(I2C_HandleTypeDef* I2Cx, uint8_t device_address, uint8_t* data, uint16_t count);
SD_I2C_Result SD_I2C_ReadWith16BitRegisterAddress(I2C_HandleTypeDef* I2Cx, uint8_t device_address, uint16_t register_address, uint8_t* data);
/**
 * @}
 */

/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif /* DRIVERS_MYLIB_SD_HAL_I2C_H_ */
