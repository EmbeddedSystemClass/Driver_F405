/**
  ******************************************************************************
  * @file        MotionFX_Manager.c
  * @author      MEMS Application Team
  * @version     V2.1.0
  * @date        01-November-2017
  * @brief       This file includes sensor fusion interface functions
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "MotionFX_Manager.h"

/** @addtogroup MOTION_FX_Applications
  * @{
  */

/** @addtogroup DATALOG_FUSION
  * @{
  */

/** @addtogroup FX_Driver FX_Driver
  * @{
  */

/* Extern variables ----------------------------------------------------------*/

/* Private defines -----------------------------------------------------------*/
#define SAMPLETODISCARD                 10
#define GBIAS_ACC_TH_SC_6X              (2.0f*0.000765f)
#define GBIAS_GYRO_TH_SC_6X             (6.0f*0.002f)
#define GBIAS_MAG_TH_SC_6X              (2.0f*0.001500f)
#define GBIAS_ACC_TH_SC_9X              (2.0f*0.000765f)
#define GBIAS_GYRO_TH_SC_9X             (2.0f*0.002f)
#define GBIAS_MAG_TH_SC_9X              (2.0f*0.001500f)

#define DECIMATION                      1


  


/* Private variables ---------------------------------------------------------*/
MFX_knobs_t iKnobs;
MFX_knobs_t *ipKnobs = &iKnobs;

volatile int sampleToDiscard = SAMPLETODISCARD;
int discardedCount = 0;

/* Public Functions ----------------------------------------------------------*/

/**
  * @brief  Initialises MotionFX algorithm
  * @param  handle handle to gyroscope sensor
  * @retval none
  */
void MotionFX_manager_init(int mode)
{
  uint8_t instance;

  MotionFX_initialize();

  MotionFX_getKnobs(ipKnobs);

  ipKnobs->gbias_acc_th_sc_6X = GBIAS_ACC_TH_SC_6X;
  ipKnobs->gbias_gyro_th_sc_6X = GBIAS_GYRO_TH_SC_6X;
  ipKnobs->gbias_mag_th_sc_6X = GBIAS_MAG_TH_SC_6X;

  ipKnobs->gbias_acc_th_sc_9X = GBIAS_ACC_TH_SC_9X;
  ipKnobs->gbias_gyro_th_sc_9X = GBIAS_GYRO_TH_SC_9X;
  ipKnobs->gbias_mag_th_sc_9X = GBIAS_MAG_TH_SC_9X;


   //case LSM6DS3_G_0
      ipKnobs->acc_orientation[0] = 'n';
      ipKnobs->acc_orientation[1] = 'e';
      ipKnobs->acc_orientation[2] = 'u';

      ipKnobs->gyro_orientation[0] = 'n';
      ipKnobs->gyro_orientation[1] = 'e';
      ipKnobs->gyro_orientation[2] = 'u';



  ipKnobs->output_type = MFX_ENGINE_OUTPUT_NED;
  ipKnobs->LMode = 1;
  ipKnobs->modx = DECIMATION;
  
  ipKnobs->ATime=1.5;
  ipKnobs->FrTime=9.0;



  MotionFX_setKnobs(ipKnobs);


  MotionFX_manager_stop_6X();
  MotionFX_manager_stop_9X();



  MotionFX_setGbias(&ipKnobs->gbias_gyro_th_sc_6X);

  if(mode == 0)
  {
	  MotionFX_manager_start_6X();

  }
  else {


	  MotionFX_MagCal_init(10,1);
	  MotionFX_manager_start_9X();
  }


}


/**
  * @brief  Run sensor fusion algorithm
  * @param  None
  * @retval None
  */
void MotionFX_manager_run(MFX_input_t *data_in, MFX_output_t *data_out, float delta_time)
{
  static uint8_t mod;
    
  if(discardedCount == sampleToDiscard)
  {
    MotionFX_propagate(data_out, data_in, delta_time);

    if (mod++ == ( DECIMATION - 1))
    {
      MotionFX_update(data_out, data_in, delta_time, NULL);
      mod = 0;
    }   
  }
  else
  {
    discardedCount++;
  }
}


/**
 * @brief  Start 6 axes MotionFX engine
 * @param  None
 * @retval None
 */
void MotionFX_manager_start_6X(void)
{
  MotionFX_enable_6X(MFX_ENGINE_ENABLE);
}


/**
 * @brief  Stop 6 axes MotionFX engine
 * @param  None
 * @retval None
 */
void MotionFX_manager_stop_6X(void)
{
  MotionFX_enable_6X(MFX_ENGINE_DISABLE);
}


/**
 * @brief  Start 9 axes MotionFX engine
 * @param  None
 * @retval None
 */
void MotionFX_manager_start_9X(void)
{
  MotionFX_enable_9X(MFX_ENGINE_ENABLE);
}


/**
 * @brief  Stop 9 axes MotionFX engine
 * @param  None
 * @retval None
 */
void MotionFX_manager_stop_9X(void)
{
  MotionFX_enable_9X(MFX_ENGINE_DISABLE);
}


/**
  * @brief  Get the library version
  * @param  version  library version string (must be array of 35 char)
  * @param  lengh  Library version string length
  * @retval none
  */
void MotionFX_manager_get_version(char *version, int *length)
{
  *length = MotionFX_GetLibVersion(version);
}


/**
  * @brief  Run magnetometer calibration algorithm
  * @param  None
  * @retval None
  */
void MotionFX_manager_MagCal_run(MFX_MagCal_input_t *data_in, MFX_MagCal_output_t *data_out)
{
   MotionFX_MagCal_run(data_in);
   MotionFX_MagCal_getParams(data_out);
}


/**
 * @brief  Start magnetometer calibration
 * @param  None
 * @retval None
 */
void MotionFX_manager_MagCal_start(int sampletime)
{
  MotionFX_MagCal_init(sampletime, 1);
}


/**
 * @brief  Stop magnetometer calibration
 * @param  None
 * @retval None
 */
void MotionFX_manager_MagCal_stop(int sampletime)
{
  MotionFX_MagCal_init(sampletime, 0);
}


/**
 * @brief  Load calibration parameter from memory
 * @param  dataSize length ot the data
 * @param  data pointer to the data
 * @retval (1) fail, (0) success
 */
char MotionFX_LoadMagCalFromNVM(unsigned short int dataSize, unsigned int *data)
{
#if (defined (MOTION_FX_STORE_CALIB_FLASH))
  RecallCalibrationFromMemory(dataSize / 4, (uint32_t*) data);
  return 0;
#else
  return 1;
#endif
}


/**
 * @brief  Save calibration parameter to memory
 * @param  dataSize length ot the data
 * @param  data pointer to the data
 * @retval (1) fail, (0) success
 */
char MotionFX_SaveMagCalInNVM(unsigned short int dataSize, unsigned int *data)
{
#if (defined (MOTION_FX_STORE_CALIB_FLASH))
  SaveCalibrationToMemory(dataSize / 4, (uint32_t*) data);
  return 0;
#else
  return 1;
#endif
}


/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
