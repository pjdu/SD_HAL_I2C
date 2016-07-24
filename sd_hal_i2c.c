/*
 *  sd_hal_i2c.c
 *
 *  Created on: Mar 5, 2016
 *      Author: Sina Darvishi
 */

#include "sd_hal_i2c.h"
#include "stm32f0xx_hal_def.h"

/**
 * @brief  This Function check I2Cx peripheral's Error that would be useful
 * @param  *I2Cx: Pointer to I2Cx peripheral to be used in communication
 * @retval One of @ref SD_I2C_Result enumeration
 */
SD_I2C_Result SD_I2C_CheckError(I2C_HandleTypeDef* I2Cx)
{
			/* Check error */

			if (HAL_I2C_GetError(I2Cx) == HAL_I2C_ERROR_BERR)
			{
				return SD_I2C_Result_BERR;
			}
			else if (HAL_I2C_GetError(I2Cx) == HAL_I2C_ERROR_ARLO)
			{
				return SD_I2C_Result_ARLO;
			}

			else if (HAL_I2C_GetError(I2Cx) == HAL_I2C_ERROR_AF)
			{
				return SD_I2C_Result_AF;
			}

			else if (HAL_I2C_GetError(I2Cx) == HAL_I2C_ERROR_OVR)
			{
				return SD_I2C_Result_OVR;
			}

			else if (HAL_I2C_GetError(I2Cx) == HAL_I2C_ERROR_DMA)
			{
				return SD_I2C_Result_DMA;
			}

			else if (HAL_I2C_GetError(I2Cx) == HAL_I2C_ERROR_TIMEOUT)
			{
				return SD_I2C_Result_TIMEOUT;
			}

			else if (HAL_I2C_GetError(I2Cx) == HAL_I2C_ERROR_SIZE)
			{
				return SD_I2C_Result_SIZE;
			}

		/* Return error */
		return SD_I2C_Result_Error;

}

/**
 * @brief  Checks if device is connected to I2C and ready to use
 * @param  *I2Cx: Pointer to I2Cx peripheral to be used in communication
 * @param  device_address: 7-bit, left aligned device address used for communication
 * @retval One of @ref SD_I2C_Result enumeration
 */

SD_I2C_Result SD_I2C_IsDeviceConnected(I2C_HandleTypeDef* I2Cx, uint8_t device_address) {

	/* Check if device is ready for communication */
	if (HAL_I2C_IsDeviceReady(I2Cx, device_address, 2, 5) != HAL_OK)
	{
		return SD_I2C_CheckError(I2Cx);
	}
	/* Return OK */
	return SD_I2C_Result_Ok;
}
/**
 *  write a single bit in an 8-bit device register.
 * @param device_address I2C slave device address
 * @param register_address Register regAddr to write to
 * @param bitNum Bit position to write (0-7)
 * @param value New bit value to write
 * @retval One of @ref SD_I2C_Result enumeration
 */

SD_I2C_Result SD_I2C_WriteBit(I2C_HandleTypeDef* I2Cx,uint8_t device_address
		, uint8_t register_address, uint8_t bitNum, uint8_t data)
{

//	uint8_t b;
//	if(SD_I2C_Read(I2Cx,device_address,register_address,&b)!= SD_I2C_Result_Ok)
//	{
//		return SD_I2C_CheckError(I2Cx);
//	}
//
//	b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
//
//	if(SD_I2C_Write(I2Cx,device_address,register_address,b)!= SD_I2C_Result_Ok)
//	{
//		return SD_I2C_CheckError(I2Cx);
//	}

	uint8_t b;
	SD_I2C_ReadByte(I2Cx,device_address, register_address, &b);
	b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
	SD_I2C_WriteByte(I2Cx,device_address, register_address, b);

	/* Return OK */
	return SD_I2C_Result_Ok;
}
/** write a single bit in a 16-bit device register.
 * @param device_address I2C slave device address
 * @param register_address Register regAddr to write to
 * @param bitNum Bit position to write (0-15)
 * @param value New bit value to write
 * @return One of @ref SD_I2C_Result enumeration
 */
SD_I2C_Result SD_I2C_WriteBitW(I2C_HandleTypeDef* I2Cx,uint8_t device_address, uint8_t register_address, uint8_t bitNum,uint16_t data)
{
	 uint16_t w;
	 SD_I2C_ReadWord(I2Cx,device_address, register_address, &w);
	 w = (data != 0) ? (w | (1 << bitNum)) : (w & ~(1 << bitNum));
	 SD_I2C_WriteWord(I2Cx,device_address, register_address, w);
	/* Return OK */
	return SD_I2C_Result_Ok;
}
/** Write multiple bits in an 8-bit device register.
 * @param device_address I2C slave device address
 * @param register_address Register regAddr to write to
 * @param bitStart First bit position to write (0-7)
 * @param length Number of bits to write (not more than 8)
 * @param data Right-aligned value to write
 * @return One of @ref SD_I2C_Result enumeration
 */
SD_I2C_Result SD_I2C_WriteBits(I2C_HandleTypeDef* I2Cx,uint8_t device_address, uint16_t register_address, uint8_t bitStart, uint8_t length,uint8_t data)
{
	//      010 value to write
	// 76543210 bit numbers
	//    xxx   args: bitStart=4, length=3
	// 00011100 mask byte
	// 10101111 original value (sample)
	// 10100011 original & ~mask
	// 10101011 masked | value
	uint8_t b;
	if (SD_I2C_ReadByte(I2Cx,device_address, register_address, &b) != SD_I2C_Result_Ok)
    {
		return SD_I2C_CheckError(I2Cx);
	}
	else
	{
		uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
		data <<= (bitStart - length + 1); // shift data into correct position
		data &= mask; // zero all non-important bits in data
		b &= ~(mask); // zero all important bits in existing byte
		b |= data; // combine data with existing byte
		if (SD_I2C_WriteByte(I2Cx,device_address, register_address, b) != SD_I2C_Result_Ok)
		{
			return SD_I2C_CheckError(I2Cx);
		}
	}

	/* Return OK */
	return SD_I2C_Result_Ok;
}

/** Write multiple bits in a 16-bit device register.
 * @param device_address I2C slave device address
 * @param register_address Register regAddr to write to
 * @param bitStart First bit position to write (0-15)
 * @param length Number of bits to write (not more than 16)
 * @param data Right-aligned value to write
 * @return One of @ref SD_I2C_Result enumeration
 */

SD_I2C_Result SD_I2C_WriteBitsW(I2C_HandleTypeDef* I2Cx,uint8_t device_address, uint16_t register_address, uint8_t bitStart, uint8_t length,uint16_t data)
{
    //              010 value to write
    // fedcba9876543210 bit numbers
    //    xxx           args: bitStart=12, length=3
    // 0001110000000000 mask word
    // 1010111110010110 original value (sample)
    // 1010001110010110 original & ~mask
    // 1010101110010110 masked | value
    uint16_t w;
    if (SD_I2C_ReadWord(I2Cx,device_address, register_address, &w) != SD_I2C_Result_Ok)
    {
    	return SD_I2C_CheckError(I2Cx);
    }
    else
    {
        uint16_t mask = ((1 << length) - 1) << (bitStart - length + 1);
        data <<= (bitStart - length + 1); // shift data into correct position
        data &= mask; // zero all non-important bits in data
        w &= ~(mask); // zero all important bits in existing word
        w |= data; // combine data with existing word
        SD_I2C_WriteWord(I2Cx,device_address, register_address, w);
    }
	/* Return OK */
	return SD_I2C_Result_Ok;
}

/**
 * @brief  Writes single byte to device
 * @param  *I2Cx: Pointer to I2Cx peripheral to be used in communication
 * @param  device_address: 7-bit, left aligned device address used for communication
 * @param  register_address: Register address where you want to write data
 * @param  data: Data to be written to device
 * @retval Member of @ref SD_I2C_Result enumeration
 */
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

/**
 * @brief  Writes multiple data to device
 * @param  *I2Cx: Pointer to I2Cx peripheral to be used in communication
 * @param  device_address: 7-bit, left aligned device address used for communication
 * @param  register_address: Register address where data will be written
 * @param  *data: Data to be written to device.
 * @param  count: Number of elements to write starting at register register_address
 * @retval One of @ref SD_I2C_Result enumeration
 */
SD_I2C_Result SD_I2C_WriteSome(I2C_HandleTypeDef* I2Cx, uint8_t device_address, uint16_t register_address, uint16_t count, uint8_t* data)
{

	/* transmit via I2C */
	if (HAL_I2C_Mem_Write(I2Cx, device_address, register_address, register_address > 0xFF ? I2C_MEMADD_SIZE_16BIT : I2C_MEMADD_SIZE_8BIT, data, count, 1000) != HAL_OK)
		return SD_I2C_CheckError(I2Cx);

	/* Return OK */
	return SD_I2C_Result_Ok;
}

/** Write single byte from an 8-bit device register.
 * @param device_address I2C slave device address
 * @param register_address Register regAddr to read from
 * @param data Container for byte value read from device
 * @return One of @ref SD_I2C_Result enumeration
 */

SD_I2C_Result SD_I2C_WriteByte(I2C_HandleTypeDef* I2Cx, uint8_t device_address, uint8_t register_address, uint8_t data)
{
	SD_I2C_WriteBytes(I2Cx,device_address, register_address, 1, &data);
	/* Return OK */
	return SD_I2C_Result_Ok;
}
/** Write multiple bytes to an 8-bit device register.
 * @param device_address I2C slave device address
 * @param register_address First register address to write to
 * @param length Number of bytes to write
 * @param data Buffer to copy new data from
 * @return One of @ref SD_I2C_Result enumeration
 */

SD_I2C_Result SD_I2C_WriteBytes(I2C_HandleTypeDef* I2Cx, uint8_t device_address, uint8_t register_address
		, uint8_t length, uint8_t *data)
{
    // Creating dynamic array to store regAddr + data in one buffer
    uint8_t * dynBuffer;
    dynBuffer = (uint8_t *) malloc(sizeof(uint8_t) * (length+1));
    dynBuffer[0] = register_address;

    // copy array
    memcpy(dynBuffer+1, data, sizeof(uint8_t) * length);

    if( HAL_I2C_Master_Transmit(I2Cx, device_address << 1, dynBuffer, length + 1, 1000)!=HAL_OK)
    {
    	return SD_I2C_CheckError(I2Cx);
    }
    free(dynBuffer);

	return SD_I2C_Result_Ok;
}
/** Write single word to a 16-bit device register.
 * @param device_address I2C slave device address
 * @param register_address Register address to write to
 * @param data New word value to write
 * @return One of @ref SD_I2C_Result enumeration
 */
SD_I2C_Result SD_I2C_WriteWord(I2C_HandleTypeDef* I2Cx,uint8_t device_address, uint8_t register_address,uint16_t data)
{
	SD_I2C_WriteWords(I2Cx,device_address,register_address,1,&data);
	/* Return OK */
	return SD_I2C_Result_Ok;
}
/**
 * @brief  Writes single byte to device without specifying register address, can be used for command write
 * @param  *I2Cx: Pointer to I2Cx peripheral to be used in communication
 * @param  device_address: 7-bit, left aligned device address used for communication
 * @param  data: Data to be written to device
 * @retval One of @ref SD_I2C_Result enumeration
 */

SD_I2C_Result SD_I2C_WriteWithNoRegisterAddress(I2C_HandleTypeDef* I2Cx, uint8_t device_address, uint8_t data)
{

	/* transmit via I2C */
	if (HAL_I2C_Master_Transmit(I2Cx, (uint16_t)device_address, &data, 1, 1000) != HAL_OK)
		return SD_I2C_CheckError(I2Cx);

	/* Return OK */
	return SD_I2C_Result_Ok;
}

/**
 * @brief  Writes multiple data to device without register address
 * @param  *I2Cx: Pointer to I2Cx peripheral to be used in communication
 * @param  device_address: 7-bit, left aligned device address used for communication
 * @param  *data: Pointer to data array to be written to device. Array length is the same as number of elements you want to write
 * @param  count: Number of elements to write
 * @retval One of @ref SD_I2C_Result enumeration
 */

SD_I2C_Result SD_I2C_WriteMultiWithNoRegisterAddress(I2C_HandleTypeDef* I2Cx, uint8_t device_address, uint8_t* data, uint16_t count)
{

	/* Try to transmit via I2C */
	if (HAL_I2C_Master_Transmit(I2Cx, (uint16_t)device_address, data, count, 1000) != HAL_OK)
		return SD_I2C_CheckError(I2Cx);

	/* Return OK */
	return SD_I2C_Result_Ok;
}

/**
 * @brief  Writes single byte in a 16-bit length register address
 * @param  *I2Cx: Pointer to I2Cx peripheral to be used in communication
 * @param  device_address: 7-bit, left aligned device address used for communication
 * @param  register_address: Register address where data will be written
 * @param  data: Data byte to write
 * @retval One of @ref SD_I2C_Result enumeration
 */

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
/** Read a single bit from an 8-bit device register.
 * @param device_address I2C slave device address
 * @param register_addr Register regAddr to read from
 * @param bitNum Bit position to read (0-7)
 * @param data Container for single bit value
 * @return One of @ref SD_I2C_Result enumeration
 */
SD_I2C_Result SD_I2C_ReadBit(I2C_HandleTypeDef* I2Cx,uint8_t device_address, uint8_t register_address, uint8_t bitNum, uint8_t *data)
{
	uint8_t b;
	if(SD_I2C_ReadByte(I2Cx,device_address, register_address, &b)!=SD_I2C_Result_Ok)
	{
		return SD_I2C_CheckError(I2Cx);
	}
	*data = b & (1 << bitNum);
	/* Return OK */
	return SD_I2C_Result_Ok;
}
/** Read a single bit from a 16-bit device register.
 * @param device_address I2C slave device address
 * @param register_address Register regAddr to read from
 * @param bitNum Bit position to read (0-15)
 * @param data Container for single bit value
 * @return One of @ref SD_I2C_Result enumeration
 */
SD_I2C_Result SD_I2C_ReadBitW(I2C_HandleTypeDef* I2Cx,uint8_t device_address, uint8_t register_address, uint8_t bitNum,uint16_t *data)
{
	uint16_t b;
	SD_I2C_ReadWord(I2Cx,device_address, register_address, &b);
	*data = b & (1 << bitNum);
	/* Return OK */
	return SD_I2C_Result_Ok;
}
/** Read multiple bits from an 8-bit device register.
 * @param device_address I2C slave device address
 * @param register_addr Register regAddr to read from
 * @param bitStart First bit position to read (0-7)
 * @param length Number of bits to read (not more than 8)
 * @param data Container for right-aligned value (i.e. '101' read from any bitStart position will equal 0x05)
 * @return One of @ref SD_I2C_Result enumeration
 */
SD_I2C_Result SD_I2C_ReadBits(I2C_HandleTypeDef* I2Cx,uint8_t device_address, uint16_t register_address, uint8_t bitStart, uint8_t length,uint8_t *data)
{
	// 01101001 read byte
	// 76543210 bit numbers
	//    xxx   args: bitStart=4, length=3
	//    010   masked
	//   -> 010 shifted
	uint8_t b;
	if(SD_I2C_ReadByte(I2Cx,device_address,register_address,&b)!= SD_I2C_Result_Ok)
	{
		return SD_I2C_Result_Error;
	}

		uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
		b &= mask;
		b >>= (bitStart - length + 1);
		*data = b;

		/* Return OK */
		return SD_I2C_Result_Ok;
}
/** Read multiple bits from a 16-bit device register.
 * @param device_address I2C slave device address
 * @param register_address Register regAddr to read from
 * @param bitStart First bit position to read (0-15)
 * @param length Number of bits to read (not more than 16)
 * @param data Container for right-aligned value (i.e. '101' read from any bitStart position will equal 0x05)
 * @return One of @ref SD_I2C_Result enumeration
 */
SD_I2C_Result SD_I2C_ReadBitsW(I2C_HandleTypeDef* I2Cx,uint8_t device_address, uint16_t register_address, uint8_t bitStart, uint8_t length,uint16_t *data)
{
    // 1101011001101001 read byte
    // fedcba9876543210 bit numbers
    //    xxx           args: bitStart=12, length=3
    //    010           masked
    //           -> 010 shifted
    uint16_t w;
    if (SD_I2C_ReadWord(I2Cx,device_address, register_address, &w) != SD_I2C_Result_Ok)
    {
    	return SD_I2C_CheckError(I2Cx);
    }
    else
    {
        uint16_t mask = ((1 << length) - 1) << (bitStart - length + 1);
        w &= mask;
        w >>= (bitStart - length + 1);
        *data = w;
    }
	return SD_I2C_Result_Ok;
}
/**
 * @brief  Reads single byte from device
 * @param  *I2Cx: Pointer to I2Cx peripheral to be used in communication
 * @param  device_address: 7-bit, left aligned device address used for communication
 * @param  register_address: Register address from where read will be done
 * @param  *data: Pointer to variable where data will be stored from read operation
 * @retval One of @ref SD_I2C_Result enumeration
 */

SD_I2C_Result SD_I2C_Read(I2C_HandleTypeDef* I2Cx, uint8_t device_address, uint8_t register_address, uint8_t* data)
{

	/* Send address */
	if (HAL_I2C_Master_Transmit(I2Cx, (uint16_t)device_address, &register_address, 1, 1000) != HAL_OK)
	{
		return SD_I2C_CheckError(I2Cx);
	}

	/* Receive multiple byte */
	if (HAL_I2C_Master_Receive(I2Cx, (uint16_t)device_address, data, 1, 1000) != HAL_OK)
	{
		return SD_I2C_CheckError(I2Cx);
	}

	/* Return OK */
	return SD_I2C_Result_Ok;
}

/**
 * @brief  Reads multiple bytes from device
 * @param  *I2Cx: Pointer to I2Cx peripheral to be used in communication
 * @param  device_address: 7-bit, left aligned device address used for communication
 * @param  register_address: Register address from where read operation will start
 * @param  *data: Pointer to variable where data will be stored from read operation
 * @param  count: Number of elements to read from device
 * @retval One of @ref SD_I2C_Result enumeration
 */

SD_I2C_Result SD_I2C_ReadSome(I2C_HandleTypeDef* I2Cx, uint8_t device_address
		, uint8_t register_address, uint16_t count, uint8_t* data)
{
	if (HAL_I2C_Mem_Read(I2Cx, (uint16_t)device_address
			, register_address,register_address > 0xFF ? I2C_MEMADD_SIZE_16BIT : I2C_MEMADD_SIZE_8BIT
					,data, count, 1000) != HAL_OK)
		return SD_I2C_CheckError(I2Cx);

	/* Return OK */
	return SD_I2C_Result_Ok;
}
/** Read single byte from an 8-bit device register.
 * @param device_address I2C slave device address
 * @param register_address Register regAddr to read from
 * @param data Container for byte value read from device
 * @return One of @ref SD_I2C_Result enumeration
 */

SD_I2C_Result SD_I2C_ReadByte(I2C_HandleTypeDef* I2Cx, uint8_t device_address, uint8_t register_address, uint8_t *data)
{
	SD_I2C_ReadBytes(I2Cx,device_address, register_address, 1, data);
	/* Return OK */
	return SD_I2C_Result_Ok;
}



/** Read multiple bytes from an 8-bit device register.
 * @param device_address I2C slave device address
 * @param register_address First register regAddr to read from
 * @param count Number of bytes to read
 * @param data Buffer to store read data in
 * @return One of @ref SD_I2C_Result enumeration
 */

SD_I2C_Result SD_I2C_ReadBytes(I2C_HandleTypeDef* I2Cx, uint8_t device_address
		, uint8_t register_address, uint8_t count, uint8_t *data)
{
		if(HAL_I2C_Master_Transmit(I2Cx, device_address << 1, &register_address, 1 , 1000) != HAL_OK )
		{
			return SD_I2C_CheckError(I2Cx);
		}
		if (HAL_I2C_Master_Receive(I2Cx, device_address << 1, data, count , 1000 ) != HAL_OK)
		{
			return SD_I2C_CheckError(I2Cx);
		}

	return SD_I2C_Result_Ok;
}
/** Write single word to a 16-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr Register address to write to
 * @param data New word value to write
 * @return One of @ref SD_I2C_Result enumeration
 */
SD_I2C_Result SD_I2C_ReadWord(I2C_HandleTypeDef* I2Cx,uint8_t device_address, uint8_t register_address,uint16_t *data)
{
	SD_I2C_ReadWords(I2Cx,device_address,register_address,1,data);
	/* Return OK */
	return SD_I2C_Result_Ok;
}
/** Read multiple words from a 16-bit device register.
 * @param device_address I2C slave device address
 * @param register_address First register regAddr to read from
 * @param length Number of words to read
 * @param data Buffer to store read data in
 * @return One of @ref SD_I2C_Result enumeration
 */
SD_I2C_Result SD_I2C_ReadWords(I2C_HandleTypeDef* I2Cx,uint8_t device_address, uint8_t register_address
		, uint8_t length,uint16_t *data)
{
//	uint8_t count;
//	uint8_t AWord[2];
//
//	for (count = 0; count < length; count++) {
//		SD_I2C_ReadSome(I2Cx,device_address,register_address,1,&AWord[0]);
//		SD_I2C_ReadSome(I2Cx,device_address,register_address,1,&AWord[1]);
//
//		uint16_t DataBits = (( AWord[0]<<8 )| AWord[1] );
//		*data++ = DataBits;
//		}
//	/* Return OK */
	if(HAL_I2C_Master_Transmit(I2Cx, device_address << 1, &register_address, 1 , 1000)!=HAL_OK)
	{
		return SD_I2C_CheckError(I2Cx);
	}
	if (HAL_I2C_Master_Receive(I2Cx, device_address << 1, (uint8_t *)data, length*2 , 1000) != HAL_OK)
	{
		return SD_I2C_CheckError(I2Cx);
	}
	return SD_I2C_Result_Ok;
}
/**
 * @brief  Reads I2C data without specifying register address
 * @note   This can be used if your sensors just sends data, without any registers
 * @param  *I2Cx: Pointer to I2Cx peripheral to be used in communication
 * @param  device_address: 7-bit, left aligned device address used for communication
 * @param  *data: Pointer to variable where data will be stored from read operation
 * @retval One of @ref SD_I2C_Result enumeration
 */
SD_I2C_Result SD_I2C_ReadWithNoRegisterAddress(I2C_HandleTypeDef* I2Cx, uint8_t device_address, uint8_t* data)
{

	/* Receive single byte without specifying  */
	if (HAL_I2C_Master_Receive(I2Cx, (uint16_t)device_address, data, 1, 1000) != HAL_OK)
		return SD_I2C_CheckError(I2Cx);

	/* Return OK */
	return SD_I2C_Result_Ok;
}
/**
 * @brief  Reads multiple bytes from device without specifying register address
 * @param  *I2Cx: Pointer to I2Cx peripheral to be used in communication
 * @param  device_address: 7-bit, left aligned device address used for communication
 * @param  *data: Pointer to variable where data will be stored from read operation
 * @param  count: Number of elements to read from device
 * @retval One of @ref SD_I2C_Result enumeration
 */
SD_I2C_Result SD_I2C_ReadSomeWithNoRegisterAddress(I2C_HandleTypeDef* I2Cx, uint8_t device_address, uint8_t* data, uint16_t count)
{

	/* Receive multi bytes without specifying  */
	if (HAL_I2C_Master_Receive(I2Cx, (uint16_t)device_address, data, count, 1000) != HAL_OK)
		return SD_I2C_CheckError(I2Cx);

	/* Return OK */
	return SD_I2C_Result_Ok;
}
/**
 * @brief  Reads single byte from device with 16-bit register address
 * @param  *I2Cx: Pointer to I2Cx peripheral to be used in communication
 * @param  device_address: 7-bit, left aligned device address used for communication
 * @param  register_address: Register address from where read will be done
 * @param  *data: Pointer to variable where data will be stored from read operation
 * @retval One of @ref SD_I2C_Result enumeration
 */
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
