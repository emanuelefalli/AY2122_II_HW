/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
#include "I2C_REG.h"
#define R 0b00000001
#define G 0b00000010
#define B 0b00000100
#define RG 0b00000011
#define RB 0b00000101
#define BG 0b00000110
#define RGB 0b00000111
void Led_colors (uint8 colori);
void PWM_modulation(uint8 led_mod, uint16 ldr_tot_digit, uint16 tmp_tot_digit, uint8 bit_status);
void reset_data(void);
extern uint16 ldr_tot;
extern int16 count_samples;
extern uint32 sum_LDR;
extern uint32 sum_TMP;
extern uint32 average_LDR;
extern uint32 average_TMP;
extern uint8 slaveBuffer[SLAVE_BUFFER_SIZE];
extern uint8_t bit_status;

/* [] END OF FILE */
