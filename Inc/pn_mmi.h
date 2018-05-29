/**
  ******************************************************************************
  * File Name          : display_menu.h
  * Description        : display the menu items
  ******************************************************************************
  *
  * Author: Tommaso Sabatini
  * Date: 2017-03-01
  ******************************************************************************
  */
#ifndef _PN_MMI_H_
#define _PN_MMI_H_

#define MY_LCD_SUPPORT				(0)
#define MY_USB_SUPPORT				(1)
#define MY_UART_SUPPORT				(0)

#include "stm32f4xx_hal.h"
#include "usbd_cdc_if.h"
#include "gpio.h"

uint8_t rx_buffer[64];
bool rx_buffer_full;

/*! ------------------------------------------------------------------------------------------------------------------
 * Function: write_port(uint16_t port_number, uint8_t value)
 *
 * Activate/Deactivate the selected port
 *
 */
void write_port(uint16_t port_number, uint8_t value);

/*! ------------------------------------------------------------------------------------------------------------------
 * Function: copy_received_data(uint8_t *receive_data);
 *
 * Copy the received data from CDC VDP usb in the user buffer (rx_buffer)
 *
 */
void copy_received_data(uint8_t *receive_data);

/*! ------------------------------------------------------------------------------------------------------------------
 * Function: top_menu()
 *
 * Display and select the top menu items (TOP MENU)
 *
 */
void top_menu();

/*! ------------------------------------------------------------------------------------------------------------------
 * Function: L10_menu()
 *
 * Display and select the level 1,0 menu items  (OUTPUTS TEST)
 *
 */
void L10_menu();

/*! ------------------------------------------------------------------------------------------------------------------
 * Function: L11_menu()
 *
 * Display and select the level 1,1 menu items (OUTPUTS TEST, BANK SELECT)
 *
 */
void L11_menu();

/*! ------------------------------------------------------------------------------------------------------------------
 * Function: L12_menu()
 *
 * Display and select the level 1,2 menu items  (OUTPUT TEST, BANK 'A' OUTPUTS TEST)
 *
 */
void L12_menu();

/*! ------------------------------------------------------------------------------------------------------------------
 * Function: L20_menu()
 *
 * Display and select the level 2,0 menu items (DIGITAL INPUTS TEST)
 *
 */
void L20_menu();

/*! ------------------------------------------------------------------------------------------------------------------
 * Function: process_pn_menu_irq(void)
 *
 * PA0 Interrupt Handler; Called when Push Button is pressed (Falling Edge Interrupt, on STM32-H405 Olimex Board)
 * or released (Rising Edge Interrupt)
 * On Falling Edge Interrupt starts timer2; On Rising Edge Interrupt reset Timer2
 *
 * Timer2 Interrupt Handler; Executed when Timer2 expires
 */
void process_pn_menu_irq(void);

/*! ------------------------------------------------------------------------------------------------------------------
 * Function: process_timer3_irq(void)
 *
 * Timer3 Interrupt Handler; Executed when Timer3 expires
 */
void process_timer3_irq(void);

/*! ------------------------------------------------------------------------------------------------------------------
 * Function: process_IO_Expander_irq(void)
 *
 * PC2 Interrupt Handler; Called by one of the MC23017 IO Expanders when an input line changes.
 * Active on Falling Edge
 */
void process_IO_Expander_irq(void);

/*! ------------------------------------------------------------------------------------------------------------------
 * Function: HAL_TIM_PeriodElapsedCallback
 *
 * Timer 2 Interrupt Handler; Called when Push Button is pressed for more then 5s.
 * Display Menu Entries when Button is pressed for more then 5s
 *
 * Timer 3 Interrupt Handler; Called when Timer3 counter expires (3 second period).
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

/*! ------------------------------------------------------------------------------------------------------------------
  * Function HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
  *
  * @brief  EXTI line detection callback
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

/*! ------------------------------------------------------------------------------------------------------------------
 * Function: HAL_SYSTICK_Callback ()
 *
 * Use SysTick timer to blink leds
 *
 */ 
 void HAL_SYSTICK_Callback();
 
/*! ------------------------------------------------------------------------------------------------------------------
 * Function: led_on (led_t led)
 */
void led_on (led_t led);

/*! ------------------------------------------------------------------------------------------------------------------
 * Function: led_off (led_t led)
 */
void led_off (led_t led);

/*! ------------------------------------------------------------------------------------------------------------------
 * Function: led_toggle (led_t led)
 */
void led_toggle (led_t led);

/*! ------------------------------------------------------------------------------------------------------------------
 * Function: blink_led(led_t led)
 *
 * Use SysTick timer to blink leds
 *
 */

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
);

#endif //_PN_MMI_H_
 
