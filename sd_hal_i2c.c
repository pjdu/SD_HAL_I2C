/*
 *  sd_hal_i2c.c
 *
 *  Created on: Mar 5, 2016
 *      Author: Sina Darvishi
 */

#include "sd_hal_i2c.h"

SD_I2C_Result SD_I2C_CheckError(I2C_HandleTypeDef* I2Cx)
{
			/* Check error */

			if      (HAL_I2C_GetError(I2Cx) != HAL_I2C_ERROR_BERR)
				return SD_I2C_Result_BERR;

			else if (HAL_I2C_GetError(I2Cx) != HAL_I2C_ERROR_ARLO)
				return SD_I2C_Result_ARLO;

			else if (HAL_I2C_GetError(I2Cx) != HAL_I2C_ERROR_AF)
				return SD_I2C_Result_AF;

			else if (HAL_I2C_GetError(I2Cx) != HAL_I2C_ERROR_OVR)
				return SD_I2C_Result_OVR;

			else if (HAL_I2C_GetError(I2Cx) != HAL_I2C_ERROR_DMA)
				return SD_I2C_Result_DMA;

			else if (HAL_I2C_GetError(I2Cx) != HAL_I2C_ERROR_TIMEOUT)
				return SD_I2C_Result_TIMEOUT;

			else if (HAL_I2C_GetError(I2Cx) != HAL_I2C_ERROR_SIZE)
				return SD_I2C_Result_SIZE;

		/* Return error */
		return SD_I2C_Result_Error;

}


SD_I2C_Result SD_I2C_IsDeviceConnected(I2C_HandleTypeDef* I2Cx, uint8_t device_address) {

	/* Check if device is ready for communication */
	if (HAL_I2C_IsDeviceReady(I2Cx, device_address, 2, 5) != HAL_OK)
		return SD_I2C_CheckError(I2Cx);

	/* Return OK */
	return SD_I2C_Result_Ok;
}

SD_I2C_Result SD_I2C_Write(I2C_HandleTypeDef* I2Cx, uint8_t device_address, uint8_t register_address, uint8_t data)
{
	uint8_t d[2];

	/* Format array to send */
	d[0] = register_address;
	d[1] = data;

	/* Try to transmit via I2C */
	if (HAL_I2C_Master_Transmit(I2Cx, (uint16_t)device_address, (uint8_t *)d, 2, 1000) != HAL_OK)
		return SD_I2C_CheckError(I2Cx);

	/* Return OK */
	return SD_I2C_Result_Ok;
}

SD_I2C_Result SD_I2C_WriteSome(I2C_HandleTypeDef* I2Cx, uint8_t device_address, uint16_t register_address, uint8_t* data, uint16_t count)
{

	/* transmit via I2C */
	if (HAL_I2C_Mem_Write(I2Cx, device_address, register_address, register_address > 0xFF ? I2C_MEMADD_SIZE_16BIT : I2C_MEMADD_SIZE_8BIT, data, count, 1000) != HAL_OK)
		return SD_I2C_CheckError(I2Cx);

	/* Return OK */
	return SD_I2C_Result_Ok;
}

SD_I2C_Result SD_I2C_WriteWithNoRegisterAddress(I2C_HandleTypeDef* I2Cx, uint8_t device_address, uint8_t data)
{

	/* transmit via I2C */
	if (HAL_I2C_Master_Transmit(I2Cx, (uint16_t)device_address, &data, 1, 1000) != HAL_OK)
		return SD_I2C_CheckError(I2Cx);

	/* Return OK */
	return SD_I2C_Result_Ok;
}

SD_I2C_Result SD_I2C_WriteSomeWithNoRegisterAddress(I2C_HandleTypeDef* I2Cx, uint8_t device_address, uint8_t* data, uint16_t count)
{

	/* Try to transmit via I2C */
	if (HAL_I2C_Master_Transmit(I2Cx, (uint16_t)device_address, data, count, 1000) != HAL_OK)
		return SD_I2C_CheckError(I2Cx);

	/* Return OK */
	return SD_I2C_Result_Ok;
}

SD_I2C_Result SD_I2C_WriteWith16BitRegisterAddress(I2C_HandleTypeDef* I2Cx, uint8_t device_address, uint16_t register_address, uint8_t data)
{
	uint8_t d[3];

	/* Format array to send */
	d[0] = (register_address >> 8) & 0xFF; /* High byte */
	d[1] = (register_address) & 0xFF;      /* Low byte */
	d[2] = data;                           /* Data byte */

	/* Try to transmit via I2C */
	if (HAL_I2C_Master_Transmit(I2Cx, (uint16_t)device_address, (uint8_t *)d, 3, 1000) != HAL_OK)
		return SD_I2C_CheckError(I2Cx);

	/* Return OK */
	return SD_I2C_Result_Ok;
}

SD_I2C_Result SD_I2C_Read(I2C_HandleTypeDef* I2Cx, uint8_t device_address, uint8_t register_address, uint8_t* data)
{

	/* Send address */
	if (HAL_I2C_Master_Transmit(I2Cx, (uint16_t)device_address, &register_address, 1, 1000) != HAL_OK)
		return SD_I2C_CheckError(I2Cx);

	/* Receive multiple byte */
	if (HAL_I2C_Master_Receive(I2Cx, device_address, data, 1, 1000) != HAL_OK)
		return SD_I2C_CheckError(I2Cx);

	/* Return OK */
	return SD_I2C_Result_Ok;
}

SD_I2C_Result SD_I2C_ReadSome(I2C_HandleTypeDef* I2Cx, uint8_t device_address, uint8_t register_address, uint8_t* data, uint16_t count)
{

	/* Send register address */
	if (HAL_I2C_Master_Transmit(I2Cx, (uint16_t)device_address, &register_address, 1, 1000) != HAL_OK)
		return SD_I2C_CheckError(I2Cx);

	/* Receive multiple byte */
	if (HAL_I2C_Master_Receive(I2Cx, device_address, data, count, 1000) != HAL_OK)
		return SD_I2C_CheckError(I2Cx);

	/* Return OK */
	return SD_I2C_Result_Ok;
}

SD_I2C_Result SD_I2C_ReadWithNoRegisterAddress(I2C_HandleTypeDef* I2Cx, uint8_t device_address, uint8_t* data)
{

	/* Receive single byte without specifying  */
	if (HAL_I2C_Master_Receive(I2Cx, (uint16_t)device_address, data, 1, 1000) != HAL_OK)
		return SD_I2C_CheckError(I2Cx);

	/* Return OK */
	return SD_I2C_Result_Ok;
}

SD_I2C_Result SD_I2C_ReadSomeWithNoRegisterAddress(I2C_HandleTypeDef* I2Cx, uint8_t device_address, uint8_t* data, uint16_t count)
{

	/* Receive multi bytes without specifying  */
	if (HAL_I2C_Master_Receive(I2Cx, (uint16_t)device_address, data, count, 1000) != HAL_OK)
		return SD_I2C_CheckError(I2Cx);

	/* Return OK */
	return SD_I2C_Result_Ok;
}

SD_I2C_Result SD_I2C_ReadWith16BitRegisterAddress(I2C_HandleTypeDef* I2Cx, uint8_t device_address, uint16_t register_address, uint8_t* data) {
	uint8_t adr[2];

	/* Format I2C address */
	adr[0] = (register_address >> 8) & 0xFF; /* High byte */
	adr[1] = (register_address) & 0xFF;      /* Low byte */

	/* Send address */
	if (HAL_I2C_Master_Transmit(I2Cx, (uint16_t)device_address, adr, 2, 1000) != HAL_OK)
		return SD_I2C_CheckError(I2Cx);

	/* Receive multiple byte */
	if (HAL_I2C_Master_Receive(I2Cx, device_address, data, 1, 1000) != HAL_OK)
		return SD_I2C_CheckError(I2Cx);


	/* Return OK */
	return SD_I2C_Result_Ok;
}
