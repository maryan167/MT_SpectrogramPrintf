/******************************************************************************
* File Name:   main.c
*
* Description: This is the source code for the PSoC 6 MCU Hello World Example
*              for ModusToolbox.
*
* Related Document: See Readme.md
*
*******************************************************************************
* (c) 2019-2020, Cypress Semiconductor Corporation. All rights reserved.
*******************************************************************************
* This software, including source code, documentation and related materials
* ("Software"), is owned by Cypress Semiconductor Corporation or one of its
* subsidiaries ("Cypress") and is protected by and subject to worldwide patent
* protection (United States and foreign), United States copyright laws and
* international treaty provisions. Therefore, you may use this Software only
* as provided in the license agreement accompanying the software package from
* which you obtained this Software ("EULA").
*
* If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
* non-transferable license to copy, modify, and compile the Software source
* code solely for use in connection with Cypress's integrated circuit products.
* Any reproduction, modification, translation, compilation, or representation
* of this Software except as specified above is prohibited without the express
* written permission of Cypress.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
* reserves the right to make changes to the Software without notice. Cypress
* does not assume any liability arising out of the application or use of the
* Software or any product or circuit described in the Software. Cypress does
* not authorize its products for use in any products where a malfunction or
* failure of the Cypress product may reasonably be expected to result in
* significant property damage, injury or death ("High Risk Product"). By
* including Cypress's product in a High Risk Product, the manufacturer of such
* system or application assumes all risk of such use and in doing so agrees to
* indemnify Cypress against all liability.
*******************************************************************************/

#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include "cy_retarget_io.h"

//#include <stdlib.h>
//#include <stdio.h>

#include "led.h"
#include "audio.h"
#include "fft.h"
#include "error.h"


void init(int16_t* data);
static inline void print_array(int8_t active_button, int16_t *data, uint16_t frame_num, uint16_t frame_size);
static inline void print_array_(int8_t active_button, int16_t *data, uint16_t frame_num, uint16_t frame_size);
/*******************************************************************************
* Function Name: main
***************************************
* Summary:
* 	This is the main function for CM4 CPU. It does...
*    	1. Initialize the hardware and configure the audio codec.
*
*    	Do Forever loop:
*    		2. Check state of CapSense buttons.
*				If some button is active call touch_control().
*    		3. Check the user button.
*    			If it pressed, audio recording start within 2 sec.
*    		4. If UART output is enable, than audio data and fft data are output
*    			throw UART.
*    		5. The audio is played after this.
*
*    	Advice:
*    		To output recorded sound use next code:
*    							print_array(active_button, recorded_data[0], 0, 0);
*
*    		To output "magnitude" from 	fft_q15(...)
*    									fft_q15_test_1khz_fft(...)
*				use next code:
*								fft_q15(recorded_data[0], recorded_data[1], frame_num, frame_size);
*								print_array(-1, recorded_data[1], frame_num, frame_size);
*
*			To output sound from	fft_q15_sound
*									fft_q15_heming_sound
*									fft_q15_test_1khz_sound
*				use next code:
*								fft_q15_sound(recorded_data[0], recorded_data[1], frame_num, frame_size);
*								print_array(active_button, recorded_data[], frame_num, frame_size);
*
*								If you want to listen sound after FFT -> IFFT set in function
*									parameter "*data_output" pointer to the "record_data[0]".
*
* Parameters:
*
* Return:
*  int
*
*******************************************************************************/
int main(void)
{
    uint8_t active_button = BUTTON1; //BUTTON0 - no, BUTTON1 - yes, BUTTON3 - inshe
	uint16_t counter = 0;

	// FFT
	uint16_t frame_size = 128;
	uint16_t frame_num = BUFFER_SIZE/frame_size;

	// Array containing the recorded data (stereo)
	// 	must be two-dimensional.
	int16_t recorded_data[NUM_CHANNELS][BUFFER_SIZE];
	//int16_t out_data[BUFFER_SIZE] = {0};
	//for (size_t i = 0; i < 2 * BUFFER_SIZE; i++) out_data[i] = 0;

	init(recorded_data[0]);

    /* \x1b[2J\x1b[;H - ANSI ESC sequence for clear screen */
    printf("\x1b[2J\x1b[;H");

    change_led_duty_cycle(active_button, led[active_button].brightness_passive);
	for(;;){
		/* Check if the User_Button is pressed */
		if (cyhal_gpio_read(CYBSP_USER_BTN) == CYBSP_BTN_PRESSED){
			// If the active_button is not equal to 0 or 1 then recording will start
			//		with the active "Yes" led.
			if (0u != active_button &&
				1u != active_button &&
				3u != active_button &&
				4u != active_button){
					change_led_duty_cycle(BUTTON0, 100);
					change_led_duty_cycle(BUTTON1, led[1].brightness_passive);
					change_led_duty_cycle(BUTTON2, 100);
					change_led_duty_cycle(BUTTON3, 100);
					active_button = 1;
			}
			// To avoid button sound recording.
			Cy_SysLib_Delay(750);

			// Start record
			record_audio(active_button);

			// UART output.
			if (uart_enable()){
				turn_led_on_off(active_button, 0);
				change_led_duty_cycle(BUTTON2, led[BUTTON2].brightness_passive);

				print_array_(active_button, recorded_data[0], frame_num, frame_size);

				//for (size_t i = 0; i < BUFFER_SIZE; i++) {
				//	printf("%d ", recorded_data[0][i]);
				//}
				//printf("\r\n");

				//print_array(active_button, recorded_data[0], frame_num, frame_size);
				//printf("\r\n");
				//print_array(active_button, recorded_data[1], frame_num, frame_size);
				//printf("\r\n");
				//fft_q15(recorded_data[0], recorded_data[1], frame_num, frame_size);
				//print_array(active_button, recorded_data[1], frame_num, frame_size);
				//fft_q15_out(recorded_data[0], out_data, frame_num, frame_size);
				//for (size_t i = 0; i < BUFFER_SIZE; i++) {
				//	if (i == 16000) printf("\r\n");
				//	printf("%d ", out_data[i]);
				//}
				//printf("\r\n");
				//print_array_(active_button, out_data, frame_num, frame_size);

				//fft_q15(recorded_data[0], recorded_data[1], frame_num, frame_size);
				//printf("\r\n");
				//print_array(active_button, recorded_data[1], frame_num, frame_size);

				//printf("\r\n");
				change_led_duty_cycle(BUTTON2, led[BUTTON2].brightness_active);
				turn_led_on_off(active_button, 1);
			}
			play_record();
		}
	}
}

static inline void print_array_(int8_t active_button, int16_t *data, uint16_t frame_num, uint16_t frame_size){
  static uint16_t counter = 0;
  counter++;

  if (0 == active_button)
    printf("W %d Ni ", counter);
  else if (1 == active_button)
    printf("W %d Tak ", counter);
  else if (3 == active_button)
    printf("W %d Inshe ", counter);

  printf("%d %d ", frame_num, frame_size);

  printf("%d", data[0]);
  for (size_t i = 1; i < BUFFER_SIZE; ++i) {
	  printf(" %d", data[i]);
  }

  printf("\r\n");
}

/******************************************************************************
 * 	Function name: init
 **************************************
 *	Summary:
 *		Initialize all peripherals
 *
 *	Parameters:
 *		*data      -  array with data that will be print;
 */
void init(int16_t* data){
	// To control the initialize process.
		cy_status status;
	    cy_rslt_t result;

		// Initialize the device and board peripherals
	    result = cybsp_init() ;
	    if (result != CY_RSLT_SUCCESS){
	    	/* Halt the CPU if the device and board peripherals initialization failed */
	    	halt();
	    }


		// Enable global interrupts
	    __enable_irq();

		/* Initialize retarget-io to use the debug UART port
		     * CYBSP_SPI_MISO -> P12_1 -> TX
		     * CYBSP_SPI_MOSI -> P12_0 -> RX */
		result = cy_retarget_io_init(CYBSP_SPI_MISO, CYBSP_SPI_MOSI, CY_RETARGET_IO_BAUDRATE);
		if (result != CY_RSLT_SUCCESS){
				/* Halt the CPU if the Button initialization failed */
				halt_with_error("\tMain -> main() ->"
								"\n\r\t\t\t-> UART initialization failed");
			}


	    // Initialize the TCPWM for PWM
	    initialize_led();


		// Initialize the User Button
	    result = cyhal_gpio_init((cyhal_gpio_t) CYBSP_USER_BTN, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_PULLUP, CYBSP_BTN_OFF);
	    if (result != CY_RSLT_SUCCESS){
			/* Halt the CPU if the Button initialization failed */
			halt_with_error("\tMain -> main() ->"
							"\n\r\t\t\t-> Button initialization failed");
		}
	    cyhal_gpio_enable_event((cyhal_gpio_t) CYBSP_USER_BTN, CYHAL_GPIO_IRQ_BOTH, CYHAL_ISR_PRIORITY_DEFAULT, true);

	    // Initialize peripheral components for audio
	    initialize_audio(data);
}

/******************************************************************************
 * 	Function Name: UART
 **************************************
 * 	Summary:
 *   	Function to simplify output.
 *   	Output format:
 *     		1 0 0 3 3 3 0 0 0 0 0 0 0 0 0 0 5 6 1... -> 1 0 2 3 3 3 0 10 5 6 1...
 *
 *     		1           -> 1
 *     		0 0         -> 0 2
 *     		3 3 3         -> 3 3 3
 *     		0 0 0 0 0 0 0 0 0 0 -> 0 10
 *     		5 6 1        -> 5 6 1
 *
 * 	 	P.S.
 * 	 		In this version this function return only spectrogram label.
 * 	 			It is to simplify the reverse conversion on the PC.
 *
 *
 * 	Parameters:
 *   	active_button  -  number to choose letter and type of output:
 *                 				-1  - Spectrogram
 *                               0  - No
 *                               1  - Yes
 *                               2 - Maybe
 *   	*data      -  array with data that will be print;
 *  	frame_num    -  number FFT frames;
 *  	frame_size    -  number of sound bits for FFT.
 *
 */


static inline void print_array(int8_t active_button, int16_t *data, uint16_t frame_num, uint16_t frame_size){
  static uint16_t counter = 0;
  uint32_t size = frame_num * frame_size;

  if (0 == active_button)
    printf("S %d Ni ", counter);
  else if (1 == active_button)
    printf("S %d Tak ", counter);

  counter++;

  printf("%d %d ", frame_num, frame_size/2);

  uint32_t pos = frame_size/2;
  uint32_t t_pos;
  uint8_t prev_zero = 0;  // Bool
  uint16_t zero_count = 0;
  for (int i=0; i<frame_num; i++){
    t_pos = pos;
    for (int j=frame_size/2; j<frame_size; j++){
      if (0 == data[t_pos]){
        zero_count++;
        prev_zero = 1;
      }
      else{
        if (1 == prev_zero){
          printf("%d ", 0);
          printf("%u ", zero_count);
          zero_count = 0;
          prev_zero = 0;
        }
        printf("%d ", data[t_pos]);
      }
      t_pos++;
    }
    pos += frame_size;
  }

  if (1 == prev_zero){
    printf("%d ", 0);
    printf("%u", zero_count);
  }
  printf("\r\n");
}
/* [] END OF FILE */
