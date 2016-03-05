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
SD_I2C_Result SD_I2C_IsDeviceConnected(I2C_HandleTypeDef* I2Cx, uint8_t address);

/**
 * @brief  Check which error has occurred
 * @param  *I2Cx: Pointer to I2Cx peripheral to be used in communication
 * @retval One of @ref SD_I2C_Result enumeration
 */
SD_I2C_Result SD_I2C_CheckError(I2C_HandleTypeDef* I2Cx);

/**
 * @brief  Writes single byte to device
 * @param  *I2Cx: Pointer to I2Cx peripheral to be used in communication
 * @param  device_address: 7-bit, left aligned device address used for communication
 * @param  register_address: Register address where you want to write data
 * @param  data: Data to be written to device
 * @retval Member of @ref SD_I2C_Result enumeration
 */
SD_I2C_Result SD_I2C_Write(I2C_HandleTypeDef* I2Cx, uint8_t device_address, uint8_t register_address, uint8_t data);

/**
 * @brief  Writes multiple data to device
 * @param  *I2Cx: Pointer to I2Cx peripheral to be used in communication
 * @param  device_address: 7-bit, left aligned device address used for communication
 * @param  register_address: Register address where data will be written
 * @param  *data: Data to be written to device.
 * @param  count: Number of elements to write starting at register register_address
 * @retval One of @ref SD_I2C_Result enumeration
 */
SD_I2C_Result SD_I2C_WriteSome(I2C_HandleTypeDef* I2Cx, uint8_t device_address, uint16_t register_address, uint8_t *data, uint16_t count);

/**
 * @brief  Writes single byte to device without specifying register address, can be used for command write
 * @param  *I2Cx: Pointer to I2Cx peripheral to be used in communication
 * @param  device_address: 7-bit, left aligned device address used for communication
 * @param  data: Data to be written to device
 * @retval One of @ref SD_I2C_Result enumeration
 */
SD_I2C_Result SD_I2C_WriteWithNoRegisterAddress(I2C_HandleTypeDef* I2Cx, uint8_t device_address, uint8_t data);

/**
 * @brief  Writes multiple data to device without register address
 * @param  *I2Cx: Pointer to I2Cx peripheral to be used in communication
 * @param  device_address: 7-bit, left aligned device address used for communication
 * @param  *data: Pointer to data array to be written to device. Array length is the same as number of elements you want to write
 * @param  count: Number of elements to write
 * @retval One of @ref SD_I2C_Result enumeration
 */
SD_I2C_Result SD_I2C_WriteMultiWithNoRegisterAddress(I2C_HandleTypeDef* I2Cx, uint8_t device_address, uint8_t* data, uint16_t count);

/**
 * @brief  Writes single byte in a 16-bit length register address
 * @param  *I2Cx: Pointer to I2Cx peripheral to be used in communication
 * @param  device_address: 7-bit, left aligned device address used for communication
 * @param  register_address: Register address where data will be written
 * @param  data: Data byte to write
 * @retval One of @ref SD_I2C_Result enumeration
 */
SD_I2C_Result SD_I2C_WriteWith16BitRegisterAddress(I2C_HandleTypeDef* I2Cx, uint8_t device_address, uint16_t register_address, uint8_t data);

/**
 * @brief  Reads single byte from device
 * @param  *I2Cx: Pointer to I2Cx peripheral to be used in communication
 * @param  device_address: 7-bit, left aligned device address used for communication
 * @param  register_address: Register address from where read will be done
 * @param  *data: Pointer to variable where data will be stored from read operation
 * @retval One of @ref SD_I2C_Result enumeration
 */
SD_I2C_Result SD_I2C_Read(I2C_HandleTypeDef* I2Cx, uint8_t device_address, uint8_t register_address, uint8_t* data);

/**
 * @brief  Reads multiple bytes from device
 * @param  *I2Cx: Pointer to I2Cx peripheral to be used in communication
 * @param  device_address: 7-bit, left aligned device address used for communication
 * @param  register_address: Register address from where read operation will start
 * @param  *data: Pointer to variable where data will be stored from read operation
 * @param  count: Number of elements to read from device
 * @retval One of @ref SD_I2C_Result enumeration
 */
SD_I2C_Result SD_I2C_ReadSome(I2C_HandleTypeDef* I2Cx, uint8_t device_address, uint8_t register_address, uint8_t* data, uint16_t count);

/**
 * @brief  Reads I2C data without specifying register address
 * @note   This can be used if your sensors just sends data, without any registers
 * @param  *I2Cx: Pointer to I2Cx peripheral to be used in communication
 * @param  device_address: 7-bit, left aligned device address used for communication
 * @param  *data: Pointer to variable where data will be stored from read operation
 * @retval One of @ref SD_I2C_Result enumeration
 */
SD_I2C_Result SD_I2C_ReadWithNoRegisterAddress(I2C_HandleTypeDef* I2Cx, uint8_t device_address, uint8_t* data);

/**
 * @brief  Reads multiple bytes from device without specifying register address
 * @param  *I2Cx: Pointer to I2Cx peripheral to be used in communication
 * @param  device_address: 7-bit, left aligned device address used for communication
 * @param  *data: Pointer to variable where data will be stored from read operation
 * @param  count: Number of elements to read from device
 * @retval One of @ref SD_I2C_Result enumeration
 */
SD_I2C_Result SD_I2C_ReadSomeWithNoRegisterAddress(I2C_HandleTypeDef* I2Cx, uint8_t device_address, uint8_t* data, uint16_t count);

/**
 * @brief  Reads single byte from device with 16-bit register address
 * @param  *I2Cx: Pointer to I2Cx peripheral to be used in communication
 * @param  device_address: 7-bit, left aligned device address used for communication
 * @param  register_address: Register address from where read will be done
 * @param  *data: Pointer to variable where data will be stored from read operation
 * @retval One of @ref SD_I2C_Result enumeration
 */
SD_I2C_Result SD_I2C_ReadWith16BitRegisterAddress(I2C_HandleTypeDef* I2Cx, uint8_t device_address, uint16_t register_address, uint8_t* data);


/**
 * @}
 */

/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif /* DRIVERS_MYLIB_SD_HAL_I2C_H_ */
