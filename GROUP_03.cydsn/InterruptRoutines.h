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


#ifndef __INTERRUPT_ROUTINES_H
    #define __INTERRUPT_ROUTINES_H
    
    #include "cytypes.h"
    #include "stdio.h"
    
    //#define BYTES_TO_SEND 2
    #define TRANSMIT_BUFFER_SIZE 16
    
    
    CY_ISR_PROTO (Custom_ISR_ADC);
    CY_ISR_PROTO (Custom_ISR_RX);
    
    int32 value_digit_TMP;      //value in digit of TMP sensor from ADC
    int32 value_digit_LDR;      //value in digit of LDR sensor from ADC
    
    char DataBuffer[TRANSMIT_BUFFER_SIZE];
    volatile uint8 PacketReadyFlag;
#endif
/* [] END OF FILE */
