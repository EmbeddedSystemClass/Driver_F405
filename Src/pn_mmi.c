/**
  ******************************************************************************
  * File Name          : pn_mmi.c
  * Description        : display the menu items
  ******************************************************************************
  *
  * Author: Tommaso Sabatini
  * Date: 2017-03-01
  ******************************************************************************
  */
#include "pn_mmi.h"
#include "mcp23017.h"
#include "LPS25HB_Driver.h"

const uint8_t CDC_delay = 10;
const uint8_t debounce_delay = 60;

uint16_t led_pc6_timer = 0;
uint16_t led_pc7_timer = 0;
uint16_t led_pc8_timer = 0;
uint16_t led_pc9_timer = 0;
uint16_t timer2 = 0;
uint16_t timer3 = 0;
uint16_t service_timer = 0;

uint32_t io_exp_intcap, io_exp_gpio;

bool timer2_expired = false;
bool timer3_expired = false;
bool service_timer_expired = false;
bool FirstTime = true;
extern bool secondrow;

uint16_t output = 0;

uint8_t mystring1[64]="Premere il pulsante per piu' di 5s";
uint8_t mystring2[64]=" per attivare i menu' di test...\r\n";

uint8_t sel;

uint8_t top_menu_items_row1[64]="\r\n\r\n+-------------------+\r\n";
uint8_t top_menu_items_row2[64]="|  POWER NODE TEST  |\r\n";
uint8_t top_menu_items_row3[64]="+-------------------+\r\n";
uint8_t top_menu_items_row4[64]="| OUTPUTS TEST: [1] |\r\n";
uint8_t top_menu_items_row5[64]="| INPUTS TEST.: [2] |\r\n";
uint8_t top_menu_items_row6[64]="| CAN_1 TEST..: [3] |\r\n";
uint8_t top_menu_items_row7[64]="| CAN_2 TEST..: [4] |\r\n";
uint8_t top_menu_items_row9[64]="  ENTER TEST.....";

uint8_t L10_menu_items_row1[64]="\r\n\r\n+---------------------+\r\n";
uint8_t L10_menu_items_row2[64]="|     OUTPUTS TEST    |\r\n";
uint8_t L10_menu_items_row3[64]="+---------------------+\r\n";
uint8_t L10_menu_items_row4[64]="|   BANK....: [1..5]  |\r\n";
uint8_t L10_menu_items_row5[64]="|   ESC.: MAIN MENU   |\r\n";

uint8_t L11_menu_items_row2[64]="|    BANK 'X' TEST    |\r\n";
uint8_t L11_menu_items_row4[64]="|   OUT....: [1..5]   |\r\n";
uint8_t L11_menu_items_row5[64]="| ESC.: OUT TEST MENU |\r\n";

uint8_t L12_menu_items_row2[64]="| B.'X' SWITCH n TEST |\r\n";
uint8_t L12_menu_items_row4[64]="|   OUT n ON....: 1   |\r\n";
uint8_t L12_menu_items_row5[64]="|   OUT n OFF...: 0   |\r\n";
uint8_t L12_menu_items_row6[64]="| ESC.: BANK 'A' TEST |\r\n";

uint8_t L20_menu_items_row1[64]="\r\n\r\n+-------------------------------------------------------+\r\n";
uint8_t L20_menu_items_row2[64]="|                  DIGITAL INPUTS TEST                  |\r\n";
uint8_t L20_menu_items_row3[64]="|                    ESC.: MAIN MENU                    |\r\n";
uint8_t L20_menu_items_row4[64]="+-------------------------------------------------------+\r\n";
uint8_t L20_menu_items_row5[64]="| 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 |\r\n";
uint8_t L20_menu_items_row6[64]="|                                                       |\r\n";
uint8_t L20_menu_items_row7[64]="|                                                       |\r";

/*! ------------------------------------------------------------------------------------------------------------------
 * Function: write_port(uint8_t port_number, uint8_t value)
 *
 * Activate/Deactivate the selected port
 *
 */
void write_port(uint16_t port_number, uint8_t value)
{
	rx_buffer[0] = 0;
	switch (port_number)
	{
		case 0x3131:							//Bank A Port 1 (Out1)
			if (value == 0x31)
			{
//				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
//				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);
//				HAL_GPIO_WritePin(CLACSON_GPIO_Port, CLACSON_Pin, GPIO_PIN_SET);
			}
			else
			{
//				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
//				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);
//				HAL_GPIO_WritePin(CLACSON_GPIO_Port, CLACSON_Pin, GPIO_PIN_RESET);
			}
			break;
		case 0x3132:							//Bank A Port 2 (Out2)
			if (value == 0x31)
			{
//				HAL_GPIO_WritePin(TERGI_DX_VEL1_GPIO_Port, TERGI_DX_VEL1_Pin, GPIO_PIN_SET);
			}
			else
			{
//				HAL_GPIO_WritePin(TERGI_DX_VEL1_GPIO_Port, TERGI_DX_VEL1_Pin, GPIO_PIN_RESET);
			}
			break;
		case 0x3133:							//Bank A Port 3 (Out3)
			if (value == 0x31)
			{
//				HAL_GPIO_WritePin(TERGI_DX_VEL2_GPIO_Port, TERGI_DX_VEL2_Pin, GPIO_PIN_SET);
			}
			else
			{
//				HAL_GPIO_WritePin(TERGI_DX_VEL2_GPIO_Port, TERGI_DX_VEL2_Pin, GPIO_PIN_RESET);
			}
			break;
		case 0x3134:							//Bank A Port 4 (Out4)
			if (value == 0x31)
			{
//				HAL_GPIO_WritePin(TERGI_SX_VEL1_GPIO_Port, TERGI_SX_VEL1_Pin, GPIO_PIN_SET);
			}
			else
			{
//				HAL_GPIO_WritePin(TERGI_SX_VEL1_GPIO_Port, TERGI_SX_VEL1_Pin, GPIO_PIN_RESET);
			}
			break;
		case 0x3135:							//Bank A Port 5 (Out5)
			if (value == 0x31)
			{
//				HAL_GPIO_WritePin(TERGI_SX_VEL2_GPIO_Port, TERGI_SX_VEL2_Pin, GPIO_PIN_SET);
			}
			else
			{
//				HAL_GPIO_WritePin(TERGI_SX_VEL2_GPIO_Port, TERGI_SX_VEL2_Pin, GPIO_PIN_RESET);
			}
			break;
		case 0x3231:							//Bank B Port 1 (Out6)
			if (value == 0x31)
			{
//				HAL_GPIO_WritePin(LUCI_ROSSE_INTERNE_GPIO_Port, LUCI_ROSSE_INTERNE_Pin, GPIO_PIN_SET);
			}
			else
			{
//				HAL_GPIO_WritePin(LUCI_ROSSE_INTERNE_GPIO_Port, LUCI_ROSSE_INTERNE_Pin, GPIO_PIN_RESET);
			}
			break;
		case 0x3232:							//Bank B Port 2 (Out7)
			if (value == 0x31)
			{
//				HAL_GPIO_WritePin(LUCI_BIANCHE_INTERNE_GPIO_Port, LUCI_BIANCHE_INTERNE_Pin, GPIO_PIN_SET);
			}
			else
			{
//				HAL_GPIO_WritePin(LUCI_BIANCHE_INTERNE_GPIO_Port, LUCI_BIANCHE_INTERNE_Pin, GPIO_PIN_RESET);
			}
			break;
		case 0x3233:							//Bank B Port 3 (Out8)
			if (value == 0x31)
			{
//				HAL_GPIO_WritePin(ON_INVERTER_GPIO_Port, ON_INVERTER_Pin, GPIO_PIN_SET);
			}
			else
			{
//				HAL_GPIO_WritePin(ON_INVERTER_GPIO_Port, ON_INVERTER_Pin, GPIO_PIN_RESET);
			}
			break;
		case 0x3234:							//Bank B Port 4 (Out9)
			if (value == 0x31)
			{
//				HAL_GPIO_WritePin(ON_DC_CONV_GPIO_Port, ON_DC_CONV_Pin, GPIO_PIN_SET);
			}
			else
			{
//				HAL_GPIO_WritePin(ON_DC_CONV_GPIO_Port, ON_DC_CONV_Pin, GPIO_PIN_RESET);
			}
			break;
		case 0x3235:							//Bank B Port 5 (Out10)
			if (value == 0x31)
			{
//				HAL_GPIO_WritePin(ON_ST1_GPIO_Port, ON_ST1_Pin, GPIO_PIN_SET);
			}
			else
			{
//				HAL_GPIO_WritePin(ON_ST1_GPIO_Port, ON_ST1_Pin, GPIO_PIN_RESET);
			}
			break;
		case 0x3331:							//Bank C Port 1 (Out11)
			if (value == 0x31)
			{
//				HAL_GPIO_WritePin(ON_ST2_GPIO_Port, ON_ST2_Pin, GPIO_PIN_SET);
			}
			else
			{
//				HAL_GPIO_WritePin(ON_ST2_GPIO_Port, ON_ST2_Pin, GPIO_PIN_RESET);
			}
			break;
		case 0x3332:							//Bank C Port 2 (Out12)
			if (value == 0x31)
			{
//				HAL_GPIO_WritePin(OFF_ST1_GPIO_Port, OFF_ST1_Pin, GPIO_PIN_SET);
			}
			else
			{
//				HAL_GPIO_WritePin(OFF_ST1_GPIO_Port, OFF_ST1_Pin, GPIO_PIN_RESET);
			}
			break;
		case 0x3333:							//Bank C Port 3 (Out13)
			if (value == 0x31)
			{
//				HAL_GPIO_WritePin(OFF_ST2_GPIO_Port, OFF_ST2_Pin, GPIO_PIN_SET);
			}
			else
			{
//				HAL_GPIO_WritePin(OFF_ST2_GPIO_Port, OFF_ST2_Pin, GPIO_PIN_RESET);
			}
			break;
		case 0x3334:							//Bank C Port 4 (Out14)
			if (value == 0x31)
			{
//				HAL_GPIO_WritePin(SPIA_PULS_4_FRECCE_GPIO_Port, SPIA_PULS_4_FRECCE_Pin, GPIO_PIN_SET);
			}
			else
			{
//				HAL_GPIO_WritePin(SPIA_PULS_4_FRECCE_GPIO_Port, SPIA_PULS_4_FRECCE_Pin, GPIO_PIN_RESET);
			}
			break;
		case 0x3335:							//Bank C Port 5 (Out15)
			if (value == 0x31)
			{
//				HAL_GPIO_WritePin(ILLUM_PULS_INTERR_GPIO_Port, ILLUM_PULS_INTERR_Pin, GPIO_PIN_SET);
			}
			else
			{
//				HAL_GPIO_WritePin(ILLUM_PULS_INTERR_GPIO_Port, ILLUM_PULS_INTERR_Pin, GPIO_PIN_RESET);
			}
			break;
		case 0x3431:							//Bank D Port 1 (Out16)
			if (value == 0x31)
			{
//				HAL_GPIO_WritePin(VENT_CONDIZ_ANT_VEL1_GPIO_Port, VENT_CONDIZ_ANT_VEL1_Pin, GPIO_PIN_SET);
			}
			else
			{
//				HAL_GPIO_WritePin(VENT_CONDIZ_ANT_VEL1_GPIO_Port, VENT_CONDIZ_ANT_VEL1_Pin, GPIO_PIN_RESET);
			}
			break;
		case 0x3432:							//Bank D Port 2 (Out17)
			if (value == 0x31)
			{
//				HAL_GPIO_WritePin(VENT_CONDIZ_ANT_VEL2_GPIO_Port, VENT_CONDIZ_ANT_VEL2_Pin, GPIO_PIN_SET);
			}
			else
			{
//				HAL_GPIO_WritePin(VENT_CONDIZ_ANT_VEL2_GPIO_Port, VENT_CONDIZ_ANT_VEL2_Pin, GPIO_PIN_RESET);
			}
			break;
		case 0x3433:							//Bank D Port 3 (Out18)
			if (value == 0x31)
			{
//				HAL_GPIO_WritePin(VENT_CONDIZ_ANT_VEL3_GPIO_Port, VENT_CONDIZ_ANT_VEL3_Pin, GPIO_PIN_SET);
			}
			else
			{
//				HAL_GPIO_WritePin(VENT_CONDIZ_ANT_VEL3_GPIO_Port, VENT_CONDIZ_ANT_VEL3_Pin, GPIO_PIN_RESET);
			}
			break;
		case 0x3434:							//Bank D Port 4 (Out19)
			if (value == 0x31)
			{
//				HAL_GPIO_WritePin(VENT_CONDIZ_POST_VEL1_GPIO_Port, VENT_CONDIZ_POST_VEL1_Pin, GPIO_PIN_SET);
			}
			else
			{
//				HAL_GPIO_WritePin(VENT_CONDIZ_POST_VEL1_GPIO_Port, VENT_CONDIZ_POST_VEL1_Pin, GPIO_PIN_RESET);
			}
			break;
		case 0x3435:							//Bank D Port 5 (Out20)
			if (value == 0x31)
			{
//				HAL_GPIO_WritePin(VENT_CONDIZ_POST_VEL2_GPIO_Port, VENT_CONDIZ_POST_VEL2_Pin, GPIO_PIN_SET);
			}
			else
			{
//				HAL_GPIO_WritePin(VENT_CONDIZ_POST_VEL2_GPIO_Port, VENT_CONDIZ_POST_VEL2_Pin, GPIO_PIN_RESET);
			}
			break;
		case 0x3531:							//Bank E Port 1 (Out21)
			if (value == 0x31)
			{
//				HAL_GPIO_WritePin(VENT_CONDIZ_POST_VEL3_GPIO_Port, VENT_CONDIZ_POST_VEL3_Pin, GPIO_PIN_SET);
			}
			else
			{
//				HAL_GPIO_WritePin(VENT_CONDIZ_POST_VEL3_GPIO_Port, VENT_CONDIZ_POST_VEL3_Pin, GPIO_PIN_RESET);
			}
			break;
		case 0x3532:							//Bank E Port 2 (Out22)
			if (value == 0x31)
			{
//				HAL_GPIO_WritePin(COMPRESSORE1_GPIO_Port, COMPRESSORE1_Pin, GPIO_PIN_SET);
			}
			else
			{
//				HAL_GPIO_WritePin(COMPRESSORE1_GPIO_Port, COMPRESSORE1_Pin, GPIO_PIN_RESET);
			}
			break;
		case 0x3533:							//Bank E Port 3 (Out23)
			if (value == 0x31)
			{
//				HAL_GPIO_WritePin(COMPRESSORE2_GPIO_Port, COMPRESSORE2_Pin, GPIO_PIN_SET);
			}
			else
			{
//				HAL_GPIO_WritePin(COMPRESSORE2_GPIO_Port, COMPRESSORE2_Pin, GPIO_PIN_RESET);
			}
			break;
		case 0x3534:							//Bank E Port 4 (Out24)
			if (value == 0x31)
			{
//				HAL_GPIO_WritePin(VENTOLE_CONDENS1_GPIO_Port, VENTOLE_CONDENS1_Pin, GPIO_PIN_SET);
			}
			else
			{
//				HAL_GPIO_WritePin(VENTOLE_CONDENS1_GPIO_Port, VENTOLE_CONDENS1_Pin, GPIO_PIN_RESET);
			}
			break;

	}
}

/*! ------------------------------------------------------------------------------------------------------------------
 * Function: copy_received_data()
 *
 * Copy the received data from CDC VDP usb in the user buffer (rx_buffer)
 *
 */
void copy_received_data(uint8_t *receive_data)
{
	memcpy(&rx_buffer[0], &receive_data[0], strlen((const char*)receive_data));
	rx_buffer_full = true;
}

/*! ------------------------------------------------------------------------------------------------------------------
 * Function: top_menu()
 *
 * Display and select the top menu items (TOP MENU)
 *
 */
void top_menu()
{
	rx_buffer[0] = 0;

	HAL_Delay(CDC_delay);
	CDC_Transmit_FS(top_menu_items_row1,strlen((const char*)top_menu_items_row1));
	HAL_Delay(CDC_delay);
	CDC_Transmit_FS(top_menu_items_row2,strlen((const char*)top_menu_items_row2));
	HAL_Delay(CDC_delay);
	CDC_Transmit_FS(top_menu_items_row3,strlen((const char*)top_menu_items_row3));
	HAL_Delay(CDC_delay);
	CDC_Transmit_FS(top_menu_items_row4,strlen((const char*)top_menu_items_row4));
	HAL_Delay(CDC_delay);
	CDC_Transmit_FS(top_menu_items_row5,strlen((const char*)top_menu_items_row5));
	HAL_Delay(CDC_delay);
	CDC_Transmit_FS(top_menu_items_row6,strlen((const char*)top_menu_items_row6));
	HAL_Delay(CDC_delay);
	CDC_Transmit_FS(top_menu_items_row7,strlen((const char*)top_menu_items_row7));
	HAL_Delay(CDC_delay);
	CDC_Transmit_FS(top_menu_items_row3,strlen((const char*)top_menu_items_row3));
	HAL_Delay(CDC_delay);
	CDC_Transmit_FS(top_menu_items_row9,strlen((const char*)top_menu_items_row9));

	while (1)
	{
		sel = rx_buffer[0];
		switch (sel)
		{
			case 0x31:
				L10_menu();
				break;
			case 0x32:
				L20_menu();
				break;
			case 0x33:
				L10_menu();
				break;
			case 0x34:
				L10_menu();
				break;
			case 0x1B:
				top_menu();
				break;
			default:
				// do nothing for undefined led number
				break;
		}
	}

}	// end top_menu

/*! ------------------------------------------------------------------------------------------------------------------
 * Function: L10_menu()
 *
 * Display and select the level 1,0 menu items (OUTPUTS TEST, BANK SELECT)
 *
 */
void L10_menu()
{
	rx_buffer[0] = 0;
	output = 0;

	HAL_Delay(CDC_delay);
	CDC_Transmit_FS(L10_menu_items_row1,strlen((const char*)L10_menu_items_row1));
	HAL_Delay(CDC_delay);
	CDC_Transmit_FS(L10_menu_items_row2,strlen((const char*)L10_menu_items_row2));
	HAL_Delay(CDC_delay);
	CDC_Transmit_FS(L10_menu_items_row3,strlen((const char*)L10_menu_items_row3));
	HAL_Delay(CDC_delay);
	CDC_Transmit_FS(L10_menu_items_row4,strlen((const char*)L10_menu_items_row4));
	HAL_Delay(CDC_delay);
	CDC_Transmit_FS(L10_menu_items_row5,strlen((const char*)L10_menu_items_row5));
	HAL_Delay(CDC_delay);
	CDC_Transmit_FS(L10_menu_items_row3,strlen((const char*)L10_menu_items_row3));
	HAL_Delay(CDC_delay);
	CDC_Transmit_FS(top_menu_items_row9,strlen((const char*)top_menu_items_row9));

	while (1)
	{
		sel = rx_buffer[0];
		output = sel << 8;					//in output bit 4..8 the bank number
		switch (sel)
		{
			case 0x31 ... 0x35:
				L11_menu();
				break;
			case 0x1B:
				top_menu();
				break;
			default:
				// do nothing for undefined led number
				break;
		}
	}

}	// end L10_menu

/*! ------------------------------------------------------------------------------------------------------------------
 * Function: L11_menu()
 *
 * Display and select the level 1,1 menu items (OUTPUT TEST, BANK 'A' OUTPUTS TEST)
 *
 */
void L11_menu()
{
	rx_buffer[0] = 0;

	HAL_Delay(CDC_delay);
	CDC_Transmit_FS(L10_menu_items_row1,strlen((const char*)L10_menu_items_row1));
	HAL_Delay(CDC_delay);
	CDC_Transmit_FS(L11_menu_items_row2,strlen((const char*)L11_menu_items_row2));
	HAL_Delay(CDC_delay);
	CDC_Transmit_FS(L10_menu_items_row3,strlen((const char*)L10_menu_items_row3));
	HAL_Delay(CDC_delay);
	CDC_Transmit_FS(L11_menu_items_row4,strlen((const char*)L11_menu_items_row4));
	HAL_Delay(CDC_delay);
	CDC_Transmit_FS(L11_menu_items_row5,strlen((const char*)L11_menu_items_row5));
	HAL_Delay(CDC_delay);
	CDC_Transmit_FS(L10_menu_items_row3,strlen((const char*)L10_menu_items_row3));
	HAL_Delay(CDC_delay);
	CDC_Transmit_FS(top_menu_items_row9,strlen((const char*)top_menu_items_row9));

	while (1)
	{
		sel = rx_buffer[0];
		output = output & 0xFFFC;		//clear bits 0,1
		output |= sel;					//in output bit 0 the port number
		switch (sel)
		{
			case 0x31 ... 0x35:
				L12_menu();
				break;
			case 0x1B:
				L10_menu();
				break;
			default:
				// do nothing for undefined led number
				break;
		}
	}

}	// end L10_menu

/*! ------------------------------------------------------------------------------------------------------------------
 * Function: L12_menu()
 *
 * Display and select the level 1,2 menu items (BANK 'A' TEST OUTPUT "n")
 *
 */
void L12_menu()
{
	rx_buffer[0] = 0;

	HAL_Delay(CDC_delay);
	CDC_Transmit_FS(L10_menu_items_row1,strlen((const char*)L10_menu_items_row1));
	HAL_Delay(CDC_delay);
	CDC_Transmit_FS(L12_menu_items_row2,strlen((const char*)L12_menu_items_row2));
	HAL_Delay(CDC_delay);
	CDC_Transmit_FS(L10_menu_items_row3,strlen((const char*)L10_menu_items_row3));
	HAL_Delay(CDC_delay);
	CDC_Transmit_FS(L12_menu_items_row4,strlen((const char*)L12_menu_items_row4));
	HAL_Delay(CDC_delay);
	CDC_Transmit_FS(L12_menu_items_row5,strlen((const char*)L12_menu_items_row5));
	HAL_Delay(CDC_delay);
	CDC_Transmit_FS(L12_menu_items_row6,strlen((const char*)L12_menu_items_row6));
	HAL_Delay(CDC_delay);
	CDC_Transmit_FS(L10_menu_items_row3,strlen((const char*)L10_menu_items_row3));
	HAL_Delay(CDC_delay);
	CDC_Transmit_FS(top_menu_items_row9,strlen((const char*)top_menu_items_row9));

	while (1)
	{
		sel = rx_buffer[0];
		switch (sel)
		{
			case 0x30 ... 0x31:
				write_port(output, sel);
				break;
			case 0x1B:
				L11_menu();
				break;
			default:
				// do nothing for undefined led number
				break;
		}
	}

}	// end L10_menu

/*! ------------------------------------------------------------------------------------------------------------------
 * Function: L20_menu()
 *
 * Display and select the level 2,0 menu items (DIGITAL INPUTS TEST)
 *
 */
void L20_menu()
{
	rx_buffer[0] = 0;
	output = 0;

	HAL_Delay(CDC_delay);
	CDC_Transmit_FS(L20_menu_items_row1,strlen((const char*)L20_menu_items_row1));
	HAL_Delay(CDC_delay);
	CDC_Transmit_FS(L20_menu_items_row2,strlen((const char*)L20_menu_items_row2));
	HAL_Delay(CDC_delay);
	CDC_Transmit_FS(L20_menu_items_row3,strlen((const char*)L20_menu_items_row3));
	HAL_Delay(CDC_delay);
	CDC_Transmit_FS(L20_menu_items_row4,strlen((const char*)L20_menu_items_row4));
	HAL_Delay(CDC_delay);
	CDC_Transmit_FS(L20_menu_items_row5,strlen((const char*)L20_menu_items_row5));
	HAL_Delay(CDC_delay);
	CDC_Transmit_FS(L20_menu_items_row6,strlen((const char*)L20_menu_items_row6));

	while (1)
	{
		sel = rx_buffer[0];
		output = sel << 8;					//in output bit 4..8 the bank number
		switch (sel)
		{
			case 0x1B:
				top_menu();
				break;
			default:
				// do nothing for undefined led number
				break;
		}
	}

}	// end L20_menu

/*! ------------------------------------------------------------------------------------------------------------------
 * Function: process_pn_menu_irq(void)
 *
 * PA0 Interrupt Handler; Called when Push Button is pressed (Falling Edge Interrupt, on STM32-H405 Olimex Board)
 * or released (Rising Edge Interrupt)
 * On Falling Edge Interrupt starts timer2; On Rising Edge Interrupt reset Timer2
 *
 * Timer2 Interrupt Handler; Executed when Timer2 expires
 */
void process_pn_menu_irq(void)
{
	if (!HAL_GPIO_ReadPin(BUTTON_GPIO_Port, BUTTON_Pin))
	{
		__HAL_TIM_SetCounter(&htim2, 0);			//Clear Timer2
		HAL_NVIC_ClearPendingIRQ(TIM2_IRQn);
		HAL_TIM_Base_Start_IT(&htim2);				//Start Timer2
		HAL_NVIC_EnableIRQ(TIM2_IRQn);
	}
	else
	{
		HAL_TIM_Base_Stop_IT(&htim2);				//Stop Timer2
		HAL_NVIC_ClearPendingIRQ(TIM2_IRQn);
		HAL_NVIC_DisableIRQ(TIM2_IRQn);
		if (!timer2_expired | FirstTime)
		{
			CDC_Transmit_FS(mystring1,strlen((const char*)mystring1));
			HAL_Delay(CDC_delay);
			CDC_Transmit_FS(mystring2,strlen((const char*)mystring2));
		}
		if (timer2_expired)							//Timer2 Interrupt Handler
		{
			if (!FirstTime) top_menu();
			timer2_expired = false;
			FirstTime = false;
		}
	}
}



/*! ------------------------------------------------------------------------------------------------------------------
 * Function: process_IO_Expander_irq(void)
 *
 * PC2 Interrupt Handler; Called by one of the MC23017 IO Expanders when an input line changes.
 * Active on Falling Edge
 */
void process_IO_Expander_irq(void)
{
	mcp23017_read_registers(MCP23017_MASTER1_BADDR, 0, 22);	//Read all MC23017_1 Registers
	HAL_Delay(i2c_delay);
	mcp23017_read_registers(MCP23017_MASTER2_BADDR, 0, 22);	//Read all MC23017_2 Registers
	HAL_Delay(i2c_delay);
// Combine four 8-bit unsigned ints into one 32-bit unsigned int
	io_exp_intcap = intcap2_b << 24 | intcap2_a << 16 | intcap1_b << 8 | intcap1_a;
	io_exp_gpio = gpio2_b << 24 | gpio2_a << 16 | gpio1_b << 8 | gpio1_a;
// Format Display String
	for (uint8_t i=0; i <= 17; i++)
	{
		if (!TEST_BIT(io_exp_gpio,i))
		{
			L20_menu_items_row7[i*3+3]=0x30;
		}
		else
		{
			L20_menu_items_row7[i*3+3]=0x31;
		}
	}
	HAL_Delay(CDC_delay);
	CDC_Transmit_FS(L20_menu_items_row7,strlen((const char*)L20_menu_items_row7));
}


/*{
	switch (htim->Instance)
	{
	case htim2.Instance:
		HAL_TIM_Base_Stop_IT(&htim2);			//Stop Timer2
		HAL_NVIC_ClearPendingIRQ(TIM2_IRQn);
		HAL_NVIC_DisableIRQ(TIM2_IRQn);
		timer2_expired = true;
		//break;
	case htim3.Instance:
		HAL_TIM_Base_Stop_IT(&htim3);			//Stop Timer3
		HAL_NVIC_ClearPendingIRQ(TIM3_IRQn);
		HAL_NVIC_DisableIRQ(TIM3_IRQn);
		timer3_expired = true;
		process_timer3_irq();
		//break;
	default:
		// do nothing for undefined instance
		break;
	}
}*/

/*! ------------------------------------------------------------------------------------------------------------------
  * Function HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
  *
  * @brief  EXTI line detection callback
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	switch (GPIO_Pin)
	{
	case GPIO_PIN_0:
		HAL_Delay(debounce_delay);			//Debounce delay
		process_pn_menu_irq();
		//break;
	case GPIO_PIN_2:
		HAL_Delay(debounce_delay);			//Debounce delay
		if (!HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2)) process_IO_Expander_irq();
		//break;
	default:
		// do nothing for undefined led number
		break;
	}
}		

/*! ------------------------------------------------------------------------------------------------------------------
 * Function: HAL_SYSTICK_Callback ()
 *
 * Use SysTick timer to blink leds
 *
 */ 
 void HAL_SYSTICK_Callback()
{
	led_pc6_timer++;
	led_pc7_timer++;
	led_pc8_timer++;
	led_pc9_timer++;
	service_timer++;

	 /* We call this handler every 1ms*/
	if (led_pc6_timer == 300)
	{
//		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6);
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_12); 	//Led "STAT" on PC12 on Olimex H405 Board!!
		led_pc6_timer = 0;
	}
/*	if (led_pc7_timer == 450)
	{
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_7);
		led_pc7_timer = 150;
	}
	if (led_pc8_timer == 1000)
	{
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_8);
		led_pc8_timer = 0;
	}
	if (led_pc9_timer == 1000)
	{
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_9);
		led_pc9_timer = 0;
	}*/
	if (service_timer == 1000)
	{
		service_timer_expired = true;
		service_timer = 0;
	}

}

/*! ------------------------------------------------------------------------------------------------------------------
 * Function: led_on (led_t led)
 */
void led_on (led_t led)
{
	switch (led)
	{
	case LED_PC6:
//		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_SET);	//Led "STAT" on PC12 on Olimex H405 Board!!
		break;
/*	case LED_PC7:
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);
		break;
	case LED_PC8:
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
		break;
	case LED_PC9:
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET);
		break;
	case LED_ALL:
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET);
		break; */
	default:
		// do nothing for undefined led number
		break;
	}
}

/*! ------------------------------------------------------------------------------------------------------------------
 * Function: led_off (led_t led)
 */
void led_off (led_t led)
{
	switch (led)
	{
	case LED_PC6:
//		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_RESET);	//Led "STAT" on PC12 on Olimex H405 Board!!
		break;
/*	case LED_PC7:
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);
		break;
	case LED_PC8:
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
		break;
	case LED_PC9:
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
		break;
	case LED_ALL:
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
		break; */
	default:
		// do nothing for undefined led number
		break;
	}
}

/*! ------------------------------------------------------------------------------------------------------------------
 * Function: led_toggle (led_t led)
 */
void led_toggle (led_t led)
{
	switch (led)
	{
	case LED_PC6:
//		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6);
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_12);		//Led "STAT" on PC12 on Olimex H405 Board!!
		break;
/*	case LED_PC7:
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_7);
		break;
	case LED_PC8:
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_8);
		break;
	case LED_PC9:
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_9);
		break;
	case LED_ALL:
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6);
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_7);
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_8);
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_9);
		break; */
	default:
		// do nothing for undefined led number
		break;
	}
}

#pragma GCC optimize ("O3")
/*! ------------------------------------------------------------------------------------------------------------------
 * Function: writetoLCD()
 *
 * Low level abstract function to write data to the LCD display via SPI2 peripheral
 * Takes byte buffer and rs_enable signals
 * or returns -1 if there was an error
 */
void writetoLCD
(
	uint32_t       bodylength,
    uint8_t        rs_enable,
    const uint8_t *bodyBuffer
)
{
	uint8_t row_separator1[64]="\r\n";
	uint8_t row_separator2[64]="\r\n\n";
	const uint8_t CDC_delay = 10;
	const uint8_t UART_delay = 20;

#if (MY_LCD_SUPPORT == 1)
	while (HAL_SPI_GetState(&hspi2) != HAL_SPI_STATE_READY);

	int sleep = 0;

	if(rs_enable)
	{
    	port_LCD_RS_set();
    }
	else
	{
		if(bodylength == 1)
		{
			if(bodyBuffer[0] & 0x3) //if this is command = 1 or 2 - execution time is > 1ms
				sleep = 1 ;
		}
    	port_LCD_RS_clear();
    }

	port_SPIy_clear_chip_select();  //CS low for SW controllable SPI_NSS

    HAL_SPI_Transmit(&hspi2, (uint8_t*)bodyBuffer , bodylength, 10);

    port_LCD_RS_clear();
    port_SPIy_set_chip_select();  //CS high for SW controllable SPI_NSS

    if(sleep) Sleep(2);
#endif

#if (MY_USB_SUPPORT == 1)				//MY_USB_SUPPORT Defined in deca_spi.h
    if(bodylength > 1)
    {
    	CDC_Transmit_FS((uint8_t*)bodyBuffer,strlen((const char*)bodyBuffer));
    	HAL_Delay(CDC_delay);
    	if (secondrow)
    	{
    		CDC_Transmit_FS((uint8_t*)row_separator2,strlen((const char*)row_separator2));
    	}
    	else
    	{
    		CDC_Transmit_FS((uint8_t*)row_separator1,strlen((const char*)row_separator1));
    	}
    	HAL_Delay(CDC_delay);
    }
#endif

#if (MY_UART_SUPPORT == 1)				//MY_UART_SUPPORT Defined in deca_spi.h
    if(bodylength > 1)
    {
    	HAL_UART_Transmit_IT(&huart3, (uint8_t*)bodyBuffer,strlen((const char*)bodyBuffer));
    	HAL_Delay(UART_delay);
    	if (secondrow)
    	{
    		HAL_UART_Transmit_IT(&huart3, (uint8_t*)row_separator2,strlen((const char*)row_separator2));
    	}
    	else
    	{
    		HAL_UART_Transmit_IT(&huart3, (uint8_t*)row_separator1,strlen((const char*)row_separator1));
    	}
    	HAL_Delay(UART_delay);
    }
#endif

} // end writetoLCD()

