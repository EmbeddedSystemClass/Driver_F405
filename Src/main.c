
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2018 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"
#include "can.h"
#include "dma.h"
#include "i2c.h"
#include "rtc.h"
#include "tim.h"
#include "usb_device.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "usbd_cdc_if.h"
#include "mcp23017.h"
#include "Drivers/LSM9DS1-Driver/LSM9DS1_Driver.h"
#include "MotionFX_Manager.h"
#include "motion_fx.h"
#include <math.h>
#include <sys/time.h>
#include <inttypes.h>
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
#define SAMPLE_FREQ                     ((uint8_t)100)  /* [Hz] */
#define SAMPLE_PERIOD                   ((uint8_t)10)   /* [ms] */
#define MOTIONFX_ENGINE_DELTATIME       0.01f

#define LCD_BUFF_LEN (100)

typedef enum {
	HIGH_COMPUTATION_MODE = 1,
	LOW_COMPUTATION_MODE = 0,
	TEST_COMPUTATION_MODE= 2,
	TEST_SPEED_MODE = 3
}COMPUTATION_MODE;

typedef enum{
	MODE_9DOF = 1,
	MODE_6DOF = 0,
}DOF;

typedef enum{
	USB_SPEED_TEST = 1,
	I2C_SPEED_TEST =2
}SPEED;

#define ComputationMode					TEST_COMPUTATION_MODE
#define DOF_MODE						MODE_9DOF
#define TEST_MODE						USB_SPEED_TEST


#define HighComputationDecimation		1


HAL_StatusTypeDef mystatus = HAL_OK;


bool secondrow = false;
bool lcl_mems_data_rdy  = false;
bool display_mems_data  = false;
bool send_lcl_mems_data  = false;
bool rmt_mems_data_rdy  = false;
int64_t MEMS_LclData;
int64_t MEMS_RmtData;						//R: Remote

uint8_t dataseq[LCD_BUFF_LEN];
uint8_t dataseq1[LCD_BUFF_LEN];
uint8_t command = 0x0;
uint8_t toggle = 0x0;

MFX_input_t data_in;
MFX_output_t data_out;


int  tempComputation=1;
char data[128];
char timer[32];
int flag=0;
int count=1;
int skipped=0;
int id=1;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
//extern void initialise_monitor_handles(void);

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_RTC_Init();
  MX_USB_DEVICE_Init();
  MX_CAN1_Init();
  MX_CAN2_Init();
  MX_I2C1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
  HAL_NVIC_ClearPendingIRQ(EXTI0_IRQn);
  HAL_NVIC_ClearPendingIRQ(EXTI1_IRQn);
  HAL_NVIC_ClearPendingIRQ(EXTI2_IRQn);
  HAL_NVIC_ClearPendingIRQ(EXTI3_IRQn);
  HAL_NVIC_ClearPendingIRQ(EXTI4_IRQn);
  HAL_NVIC_ClearPendingIRQ(I2C1_EV_IRQn);
  HAL_NVIC_ClearPendingIRQ(TIM3_IRQn);

  HAL_GPIO_WritePin(PC_11_DISC_GPIO_Port, PC_11_DISC_Pin, GPIO_PIN_RESET);	//Pull-up Enable on USBDP (Only for Olimex STM32-H405 Board)


  __CRC_CLK_ENABLE();

  LSM9DS1_State_Connection testConnection= LSM9DS1_ERROR;
  testConnection=LSM9DS1_IsConnected();
  LSM9DS1_XLG_TurnOff();

  if(DOF_MODE==MODE_9DOF){
	  LSM9DS1_XLG_READ start= LSM9DS1_XLGM_Start(119,40,14,2000,4);
	  MotionFX_manager_init(MODE_9DOF);
  }
  else{
	  LSM9DS1_XLG_READ start= LSM9DS1_XLG_Start(119,245,2);
	  MotionFX_manager_init(MODE_6DOF);
  }





  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */



  	 //start when button is pressed
	 while (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)){};


	 if(ComputationMode == TEST_COMPUTATION_MODE){
		 // Use test computation mode
		 TestComputationMode();
	 }
	 else if (ComputationMode == HIGH_COMPUTATION_MODE){
		// Use in High computation mode
		 HighComputationMode();
	 }
	 else if (ComputationMode == LOW_COMPUTATION_MODE){

		 LowComputationMode();
	 }

	 else if(ComputationMode == TEST_SPEED_MODE){

		 SpeedTestMode();
	 }



  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */




  /* USER CODE END 3 */

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV4;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}



/* USER CODE BEGIN 4 */

void handler(){

	flag=1;
}

void HighComputationMode(){

	 HAL_TIM_Base_Start_IT(&htim3);
	 		while(1)
	 			{
	 			  if(flag==1)
	 			  	{

	 				  HAL_NVIC_DisableIRQ(TIM3_IRQn);

	 				  //prepare packet
	 				  if(DOF_MODE == MODE_6DOF)
	 				  	{
	 					  LSM9DS1_Read_XLG(&data_in,1);
	 					  MotionFX_manager_run(&data_in,&data_out,MFX_DELTATIME);
	 					  snprintf(data, 25, "%3.1f;%3.1f;%3.1f\n",
	 										  data_out.rotation_6X[0],data_out.rotation_6X[1],data_out.rotation_6X[2]);
	 				  	}
	 				  	else
	 				  	{
	 				  		LSM9DS1_Read_XLGM(&data_in,1);
	 				  		MotionFX_manager_run(&data_in,&data_out,MFX_DELTATIME);
	 				  		snprintf(data, 25, "%3.1f;%3.1f;%3.1f\n",
	 				  			 			 	data_out.rotation_9X[0],data_out.rotation_9X[1],data_out.rotation_9X[2]);
	 				  	}

	 			  	  //transmit data
	 			  	  CDC_Transmit_FS((uint8_t *)data,strlen(data));
	 			  	  flag=0;
	 			  	  HAL_NVIC_EnableIRQ(TIM3_IRQn);
	 			  	  }

	 			}


}



void LowComputationMode(){

	HAL_TIM_Base_Start_IT(&htim3);
			while(1)
				{
				  if(flag==1)
				  	{

					  HAL_NVIC_DisableIRQ(TIM3_IRQn);

					  if(DOF_MODE == MODE_6DOF)
					  	{

						  LSM9DS1_Read_XLG(&data_in,1);
						  snprintf(data, 64, "%.3f;%.3f;%.3f|%3.1f;%3.1f;%3.1f\n",
											   data_in.acc[0], data_in.acc[1], data_in.acc[2],
											   data_in.gyro[0],data_in.gyro[1],data_in.gyro[2]);
					  	}
					  	else
					  	{
					  		 LSM9DS1_Read_XLGM(&data_in,1);
					  		 snprintf(data, 64, "%.3f;%.3f;%.3f|%3.1f;%3.1f;%3.1f|%3.1f;%3.1f;%3.1f\n",
					  							data_in.acc[0], data_in.acc[1], data_in.acc[2],
					  							data_in.gyro[0],data_in.gyro[1],data_in.gyro[2],
												data_in.mag[0],data_in.mag[1],data_in.mag[2]);
					  	}

				  	  //transmit data
				  	  CDC_Transmit_FS((uint8_t *)data,strlen(data));
				  	  flag=0;
				  	  HAL_NVIC_EnableIRQ(TIM3_IRQn);
				  	  }

				}


}



void TestComputationMode(){

	HAL_TIM_Base_Start_IT(&htim3);
		while(1)
			{
			  if(flag==1)
			  	{

				  HAL_NVIC_DisableIRQ(TIM3_IRQn);

				  //prepare packet
				  if(DOF_MODE == MODE_6DOF)
				  	{
					  LSM9DS1_Read_XLG(&data_in,1);
					  MotionFX_manager_run(&data_in,&data_out,MFX_DELTATIME);
					  snprintf(data, 128, "%.3f;%.3f;%.3f|%3.1f;%3.1f;%3.1f|%3.1f;%3.1f;%3.1f\n",
										   data_in.acc[0], data_in.acc[1], data_in.acc[2],
										   data_in.gyro[0],data_in.gyro[1],data_in.gyro[2],
										   data_out.rotation_6X[0],data_out.rotation_6X[1],data_out.rotation_6X[2]);
				  	}
				  	else
				  	{
				  		LSM9DS1_Read_XLGM(&data_in,1);
				  		MotionFX_manager_run(&data_in,&data_out,MFX_DELTATIME);
				  		snprintf(data, 128, "%.3f;%.3f;%.3f|%3.1f;%3.1f;%3.1f|%3.1f;%3.1f;%3.1f|%3.1f;%3.1f;%3.1f\n",
				  							data_in.acc[0], data_in.acc[1], data_in.acc[2],
				  							data_in.gyro[0],data_in.gyro[1],data_in.gyro[2],
											data_in.mag[0],data_in.mag[1],data_in.mag[2],
				  							data_out.rotation_9X[0],data_out.rotation_9X[1],data_out.rotation_9X[2]);
				  	}

			  	  //transmit data
			  	  CDC_Transmit_FS((uint8_t *)data,strlen(data));
			  	  flag=0;
			  	  HAL_NVIC_EnableIRQ(TIM3_IRQn);
			  	  }

			}

}



void SpeedTestMode(){

	//Test I2C speed
	 uint32_t t0,t1;
	 t1=0;
	 id=0;
	 int lost=0;
	 uint8_t result;
	 if(TEST_MODE == USB_SPEED_TEST){
		 t0=HAL_GetTick();
		 	 while(1){
		 		     if(id<1000){

		 		    	     if(!I2C_ReadData(LSM9DS1_I2C_BADD,LSM9DS1_WHO_AM_I,&result,1)){
		 		    	    	 if( result == LSM9DS1_WHO_AM_I_VALUE)
		 		    	    		 				  id++;
		 		    	     }
		 		 			  else
		 		 			  {
		 		 				  lost++;
		 		 			  }


		 		 			  if(id==1000)
		 		 			  {
		 		 				  t1=HAL_GetTick()-t0;
		 		 				  HAL_Delay(2);
		 		 				  snprintf(data, 10, "%Lu",t1);
		 		 				  CDC_Transmit_FS((uint8_t *)data,strlen(data));
		 		 			  }
		 		 		  }
		 	 }
	 }
	 else{
		 //Test usb CDC speed
		 t0=HAL_GetTick();
		 while(1){
			 snprintf(data, 10, "%d|%d\n",id,lost);
			 if(id<1000){
				if(CDC_Transmit_FS((uint8_t *)data,strlen(data))== USBD_OK){
					id++;
				}
				else{
					 lost++;
				}
				if(id==1000){
					t1=HAL_GetTick()-t0;
					HAL_Delay(2);
					snprintf(data, 10, "%Lu\n",t1);
					CDC_Transmit_FS((uint8_t *)data,strlen(data));
						  }
					  }
				  }


	 }


}
/*! ------------------------------------------------------------------------------------------------------------------
 * Function: process_timer3_irq(void)
 *
 * Timer3 Interrupt Handler; Executed when Timer3 expires
 */
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
