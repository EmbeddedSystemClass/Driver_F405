/*  MCP23017 library for STM32CubeMX
    Copyright (C) 2009 David Pye    <davidmpye@gmail.com
    Modified by Tommaso Sabatini <algol6555.gmail.com> for use in the STM32CubeMX platform

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/ 

// User include starts here
#include "mcp23017.h"
// User include ends here

uint8_t receive_data[22];
uint8_t send_data;

/*! ------------------------------------------------------------------------------------------------------------------
 *  \struct     MCP23017
 *  \brief      Used to define a MCP23017 device on the I2C bus
 *  \details    MCP23017 type is used to hold the device address and the initialization data to 
 *              be written to the device
 */
uint8_t master_1[22] = {
							MASTER1_IODIRA,	 
							MASTER1_IODIRB, 	
							MASTER1_IPOLA, 	
							MASTER1_IPOLB, 	
							MASTER1_GPINTENA,
							MASTER1_GPINTENB,
							MASTER1_DEFVALA, 
							MASTER1_DEFVALB, 
							MASTER1_INTCONA, 
							MASTER1_INTCONB, 
							MASTER1_IOCONA,
							MASTER1_IOCONB,
							MASTER1_GPPUA, 	
							MASTER1_GPPUB, 	
							MASTER1_INTFA, 	
							MASTER1_INTFB, 	
							MASTER1_INTCAPA, 
							MASTER1_INTCAPB, 
							MASTER1_GPIOA, 	
							MASTER1_GPIOB, 	
							MASTER1_OLATA, 	
							MASTER1_OLATB 	
						};
uint8_t master_2[22] = {
							MASTER2_IODIRA,	 
							MASTER2_IODIRB, 	
							MASTER2_IPOLA, 	
							MASTER2_IPOLB, 	
							MASTER2_GPINTENA,
							MASTER2_GPINTENB,
							MASTER2_DEFVALA, 
							MASTER2_DEFVALB, 
							MASTER2_INTCONA, 
							MASTER2_INTCONB, 
							MASTER2_IOCONA,
							MASTER2_IOCONB,
							MASTER2_GPPUA, 	
							MASTER2_GPPUB, 	
							MASTER2_INTFA, 	
							MASTER2_INTFB, 	
							MASTER2_INTCAPA, 
							MASTER2_INTCAPB, 
							MASTER2_GPIOA, 	
							MASTER2_GPIOB, 	
							MASTER2_OLATA, 	
							MASTER2_OLATB 	
						};

/*! ------------------------------------------------------------------------------------------------------------------
 *  \fn			uint8_t BCD2DEC(uint8_t data)			
 *  \brief      Convert Binary Coded Decimal (BCD) to Decimal 
 *  
 */
uint8_t BCD2DEC(uint8_t data)
{
	return ((data >> 4)*10 + (data & 0x0F));
}

/*! ------------------------------------------------------------------------------------------------------------------
 *  \fn			uint8_t DEC2BCD(uint8_t data)			
 *  \brief      Convert Decimal to Binary Coded Decimal (BCD)
 *  
 */
uint8_t DEC2BCD(uint8_t data)
{
	 return ((data/10) << 4 | (data %10)); 
} 

/*! ------------------------------------------------------------------------------------------------------------------
 *  \fn         void mcp23017_init(uint8_t *RegValues, uint16_t address)
 *  \brief      Initialize the MCP23017 device.
 *  \details    Setup the device to use on the I2C bus.
 *  \param      address The uint16_t address should just be the hardware address
 *              component setup at the circuit level, i.e. at A2-A0 (e.g.
 *              0x4000 if all are GND.)
 */
 void mcp23017_init(uint8_t *RegValues, uint8_t base_address)
{
	HAL_I2C_Mem_Write_DMA(&hi2c1,base_address,0,I2C_MEMADD_SIZE_8BIT,&RegValues[0],22);
}

/*! ------------------------------------------------------------------------------------------------------------------
 *  \fn         	mcp23017_read_register(uint16_t base_address, uint8_t reg)
 *  \brief      	Reads "size" number of registers starting from base_address | reg
 *  \param      	base_address: the base address to the MCP23017 structure.
 *  \param      	reg: The register to be read as uint8_t.  
 *  \receive_data   The method returns the contents of the specified register in receive_data as uint8_t
 */
void mcp23017_read_registers(uint8_t base_address, uint8_t reg, uint8_t size)
{
	ioext_a = false;
	HAL_I2C_Mem_Read_DMA(&hi2c1,base_address,reg,I2C_MEMADD_SIZE_8BIT,&receive_data[0],size);
	if (base_address == MCP23017_MASTER1_BADDR)
	{
		ioext_a = true;
	}
}

/*! ------------------------------------------------------------------------------------------------------------------
 *  \fn         	mcp23017_write_register(uint16_t base_address, uint8_t reg, uint8_t size)
 *  \brief      	Writes "size" number of registers starting from base_address | reg
 *  \param      	base_address: the base address to the MCP23017 structure.
 *  \param      	reg: The register to be write as uint8_t.  
 *  \send_data   	The method write the contents of send_data the specified register in receive_data as uint8_t
 */
void mcp23017_write_registers(uint8_t base_address, uint8_t reg, uint8_t size)
{
	ioext_a = false;
	HAL_I2C_Mem_Write_IT(&hi2c1,base_address,reg,I2C_MEMADD_SIZE_8BIT,&send_data,size);
	if (base_address == MCP23017_MASTER1_BADDR)
	{
		ioext_a = true;
	}
}

/*! ------------------------------------------------------------------------------------------------------------------
 *  \fn			HAL_I2C_MemRxCpltCallback(I2C_HandleTypedef *hi2c)			
 *  \brief      I2C Rx Interrupt Service Routine
 *  
 */
void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	if (hi2c->Instance == hi2c1.Instance)
	{
		if (ioext_a)
		{
			intcap1_a = receive_data[0x10];
			intcap1_b = receive_data[0x11];
			gpio1_a = receive_data[0x12];
			gpio1_b = receive_data[0x13];
		}
		else
		{
			intcap2_a = receive_data[0x10];
			intcap2_b = receive_data[0x11];
			gpio2_a = receive_data[0x12];
			gpio2_b = receive_data[0x13];
		}
	}	
}

/*! ------------------------------------------------------------------------------------------------------------------
 *  \fn			MX_MCP23017_Init()			
 *  \brief      Initialize the MCP23017 IO Expanders
 *  
 */
void MX_MCP23017_Init()
{
	mcp23017_init(master_1, MCP23017_MASTER1_BADDR);
	HAL_Delay(i2c_delay);
	mcp23017_init(master_2, MCP23017_MASTER2_BADDR);
	HAL_Delay(i2c_delay);
}
