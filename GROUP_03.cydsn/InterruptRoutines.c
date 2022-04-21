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


#include "InterruptRoutines.h"
#include "project.h"
#define LDR_CHANNEL 0
#define TMP_CHANNEL 1

extern volatile uint8_t flagData;


CY_ISR_PROTO(Custom_ISR_ADC){
    
    Timer_ReadStatusRegister();
    
    flagData = 1;  //Set the flag active to confirm the reading from the ADC
    
    AMux_ADC_FastSelect(LDR_CHANNEL);           // Select the ldr sensor channel
    value_digit_LDR = ADC_DelSig_Read32();  // Read the value of the ldr sensor
    
    // The values are kept inside the valid range
    if (value_digit_LDR < 0)     value_digit_LDR = 0;
    if (value_digit_LDR > 65535) value_digit_LDR = 65535;
   

    AMux_ADC_FastSelect(TMP_CHANNEL);           // Select the temperature sensor channel
    value_digit_TMP = ADC_DelSig_Read32();  // Read the value of the temperature sensor
    
    // The values are kept inside the valid range
    if (value_digit_TMP < 0)     value_digit_TMP = 0;
    if (value_digit_TMP > 65535) value_digit_TMP = 65535; 
    
}


/*
int32 value_digit;
int32 value;
uint8 ch_receveid;
uint8 SendBytesFlag=0;
int16 count_samples = 0;
uint16 sum=0;
uint16 value_digit_ldr;
uint16 value_digit_tmp;

CY_ISR(Custom_ISR_ADC)
{
    Timer_ReadStatusRegister();
    value_digit = ADC_DelSig_Read32(); 
    if (value_digit < 0) value_digit=0;
    if (value_digit > 65535) value_digit=65535;
    sum += value_digit; //integration of the samples
    count_samples++;
    
    if(count_samples==5) // after 100ms (20ms for one single sample)
        value_digit_ldr = sum/5;
        DataBuffer[1] = value_digit_ldr >> 8;
        DataBuffer[2] = value_digit_ldr & 0xFF;
               
    if(count_samples==10) // after 100ms (20ms for one single sample)
        value_digit_tmp = sum/5;
        DataBuffer[1] = value_digit_tmp >> 8;
        DataBuffer[2] = value_digit_tmp & 0xFF;
        PacketReadyFlag=1; // all data are ready to be sent
    
}

 
    
#define LDR_CHANNEL 0
#define TMP_CHANNEL 1

extern volatile uint8_t ReadValue;


CY_ISR_PROTO(Custom_ISR_ADC){
    
    Timer_ReadStatusRegister();
    
    ReadValue = 1;  //Set the flag active to confirm the reading from the ADC
    
    AMux_FastSelect(LDR_CHANNEL);           // Select the ldr sensor channel
    value_digit_LDR = ADC_DelSig_Read32();  // Read the value of the ldr sensor
    
    // The values are kept inside the valid range
    if (value_digit_LDR < 0)     value_digit_LDR = 0;
    if (value_digit_LDR > 65535) value_digit_LDR = 65535;
   

    AMux_FastSelect(TMP_CHANNEL);           // Select the temperature sensor channel
    value_digit_TMP = ADC_DelSig_Read32();  // Read the value of the temperature sensor
    
    // The values are kept inside the valid range
    if (value_digit_TMP < 0)     value_digit_TMP = 0;
    if (value_digit_TMP > 65535) value_digit_TMP = 65535; 
    
}




*\
