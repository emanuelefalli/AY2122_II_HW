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
#include "I2C_REG.h"
#include "RGB_PWM.h"


 void Led_colors (uint8 colori){
    switch (colori) {
        case R:
        PWM_RED_Wakeup();
        PWM_BLUE_Sleep();
        PWM_GREEN_Sleep();
        case B:
        PWM_RED_Sleep();
        PWM_BLUE_Wakeup();
        PWM_GREEN_Sleep();
        case G:
        PWM_RED_Sleep();
        PWM_BLUE_Sleep();
        PWM_GREEN_Wakeup();
        case RB:
        PWM_RED_Wakeup();
        PWM_BLUE_Wakeup();
        PWM_GREEN_Sleep();
        case RG:
        PWM_RED_Wakeup();
        PWM_BLUE_Sleep();
        PWM_GREEN_Wakeup();
        case BG: 
        PWM_RED_Sleep();
        PWM_BLUE_Wakeup();
        PWM_GREEN_Wakeup();
        case RGB:
        PWM_RED_Wakeup();
        PWM_BLUE_Wakeup();
        PWM_GREEN_Wakeup();
    }
}

void PWM_modulation (uint8 led_mod, uint16 ldr_tot_digit, uint16 tmp_tot_digit, uint8 bit_status) {
    switch (led_mod) {
    case LED_MOD_LDR:
            if (bit_status == SLAVE_LDR_ON_CTRL_REG1 || bit_status == SLAVE_BOTH_ON_CTRL_REG1){
                if(ldr_tot_digit > MID_RANGE_LDR){
                    PWM_RED_WriteCompare(0);
                    PWM_BLUE_WriteCompare(0);
                    PWM_GREEN_WriteCompare(0);
                }
                else {
                    PWM_RED_WriteCompare(65535);
                    PWM_BLUE_WriteCompare(65535);
                    PWM_GREEN_WriteCompare(65535);
                }
            }
            else if (bit_status == SLAVE_TMP_ON_CTRL_REG1)
            UART_PutString("Il bit di led modality non corrisponde al segnale campionato!\r\n");
            
    break;
    case LED_MOD_TMP:
        if(bit_status == SLAVE_TMP_ON_CTRL_REG1 || bit_status == SLAVE_BOTH_ON_CTRL_REG1) {
            PWM_RED_WriteCompare(tmp_tot_digit-BASE_LINE_TMP);
            PWM_BLUE_WriteCompare(tmp_tot_digit-BASE_LINE_TMP);
            PWM_GREEN_WriteCompare(tmp_tot_digit-BASE_LINE_TMP);
        }
        else if (bit_status == SLAVE_LDR_ON_CTRL_REG1)
        UART_PutString("Il bit di led modality non corrisponde al segnale campionato!\r\n");
    break;
    }
}


void reset_data(void) {
    slaveBuffer[MSB_LDR]= 0x00;                                
    slaveBuffer[LSB_LDR]= 0x00;                                
    slaveBuffer[MSB_TMP]= 0x00;                                
    slaveBuffer[LSB_TMP]= 0x00;
    sum_TMP=0;
    sum_LDR=0;
    count_samples=0;
    average_TMP=0;
    average_LDR=0;
    ldr_tot =0;
}
/* [] END OF FILE */
