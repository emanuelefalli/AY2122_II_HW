/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================*/
#include "project.h"
#include "UART.h"
#include "InterruptRoutines.h"
#include "I2C_REG.h"
#include "Logging.h"



uint8_t average_sample;
uint8_t bit_status;
uint8_t LED_modality;
uint8_t blue;
uint8_t green;
uint8_t red;
uint8_t tot_LDR;
int16 count_samples;
uint32 sum_LDR=0;
uint32 sum_TMP=0;
uint32 average_LDR=0;
uint32 average_TMP=0;
uint16_t ldr_tot;
uint16_t tmp_tot;
uint16_t tmp_V;
uint16_t tmp_C;
uint16_t ldr_V;


uint16 color_int=0;


char message[50] = {'\0'};

volatile uint8 flagData=0;

//Define slaveBuffer of the EZI2C
uint8 slaveBuffer[SLAVE_BUFFER_SIZE];


int main(void)
{
                            
    CyGlobalIntEnable; /* Enable global interrupts. */
    Timer_Start();
    Logging_Start();
    ADC_DelSig_Start();
    
    isr_ADC_StartEx(ISR_ADC);
    AMux_ADC_Start();
    EZI2C_Start();
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    
    UART_Start();
    PWM_RED_Start();
    PWM_BLUE_Start();
    PWM_GREEN_Start();
    Clock_PWM_Start();
    
    UART_PutString("Inizio Campionamento!\r\n");
    
  
    // Start the ADC conversion
    ADC_DelSig_StartConvert();

    slaveBuffer[WHO_AM_I] = I2C_WHO_AM_I_REG_VALUE;         // Set who am i register value
    slaveBuffer[CTRL_REG1] = SLAVE_MODE_OFF_CTRL_REG1;  //set control reg 1 with all bits = 0 
    EZI2C_SetBuffer1(SLAVE_BUFFER_SIZE, 1 ,slaveBuffer);
    
    
    for(;;)
    {
        //reading of the registers
        average_sample = (slaveBuffer[CTRL_REG1] & 0x18) >> 3; // I check the number of samples to extract
        average_sample += 1;
        // I write the period register according to the number of average samples to obtain 50Hz transmission rate
        Timer_WritePeriod((0.02/average_sample)*5000);  

        bit_status = slaveBuffer[CTRL_REG1] & 0x03; //I check the bit status configuration

        LED_modality = (slaveBuffer[CTRL_REG1] & 0x04)>>2; //I check the LedMod of the CTRL REG 1

        blue = (slaveBuffer[CTRL_REG1] & 0b00100000)>>5;
        green = (slaveBuffer[CTRL_REG1] & 0b01000000)>>6;
        red = (slaveBuffer[CTRL_REG1] & 0b10000000)>>7;
        
        switch (bit_status){
            
            case SLAVE_MODE_OFF_CTRL_REG1:
                PWM_RED_Sleep();
                PWM_BLUE_Sleep();
                PWM_GREEN_Sleep();
                slaveBuffer[MSB_LDR]= 0x00;                                
                slaveBuffer[LSB_LDR]= 0x00;                                
                slaveBuffer[MSB_TMP]= 0x00;                                
                slaveBuffer[LSB_TMP]= 0x00; 
                flagData=0;

            break;
                
            case SLAVE_LDR_ON_CTRL_REG1:
                UART_PutString("LDR ON!\r\n");
                if(flagData==1)
                {                                       
                    sum_LDR+=value_digit_LDR;
                    count_samples++;
                    flagData=0;
                
                    if (count_samples==average_sample)
                    {
                        average_LDR=sum_LDR/average_sample;
                    
                        slaveBuffer[MSB_LDR]= average_LDR >> 8;
                        slaveBuffer[LSB_LDR]= average_LDR & 0xFF;
                        slaveBuffer[MSB_TMP]= 0x00;
                        slaveBuffer[LSB_TMP]= 0x00;
                        ldr_tot= (slaveBuffer[LSB_LDR] | slaveBuffer[MSB_LDR] << 8 );

                        //uint16 ldr_V = ADC_DelSig_CountsTo_mVolts(average_LDR);

                        sprintf(message, "LDR Output: %d\r\n", ldr_tot );
                        UART_PutString(message);                                                                                                                                                                                                        
                        
                        sum_LDR=0; 
                        average_LDR=0;
                        //count_samples = 0;
                        PWM_RED_Start();
                        PWM_GREEN_Start();
                        PWM_BLUE_Start();
                    }
                }
                break;
 
                
            case SLAVE_TMP_ON_CTRL_REG1:
                UART_PutString("TMP ON!\r\n");
                if(flagData==1)
                {
                    sum_TMP+=value_digit_TMP;
                    count_samples++;
                    flagData=0;
                
                    if (count_samples==average_sample)
                    {
                        average_TMP=sum_TMP/average_sample;
                    
                        slaveBuffer[MSB_LDR]=average_LDR & 0x00;
                        slaveBuffer[LSB_LDR]=average_LDR & 0x00;
                        slaveBuffer[MSB_TMP]=average_TMP >>8;
                        slaveBuffer[LSB_TMP]=average_TMP & 0xFF;
                        tmp_tot= (slaveBuffer[LSB_TMP] | slaveBuffer[MSB_TMP] << 8 );
                        tmp_V = ADC_DelSig_CountsTo_mVolts(tmp_tot);
                        tmp_C = (tmp_V-TMP_OFF)/TMP_M;
                        sprintf(message, "Temp Output: %d\r\n", (tmp_tot - BASE_LINE_TMP)*15);                     
                        UART_PutString(message);
                        
                        sum_TMP=0;
                        average_TMP=0;
                        //count_samples = 0;
                        PWM_RED_Start();
                        PWM_GREEN_Start();
                        PWM_BLUE_Start();
                    }
                }
                break;
            
                case SLAVE_BOTH_ON_CTRL_REG1:
                UART_PutString("LDR and TMP ON!\r\n");
                if (flagData==1)
                {
                    sum_LDR+=value_digit_LDR;
                    sum_TMP+=value_digit_TMP;
                    count_samples++; 
                    flagData=0;
                
                    if (count_samples==average_sample)
                    {
                        average_LDR=sum_LDR/average_sample;
                        average_TMP=sum_TMP/average_sample;
                         
                        slaveBuffer[MSB_LDR]=average_LDR >> 8;
                        slaveBuffer[LSB_LDR]=average_LDR & 0xFF;
                        slaveBuffer[MSB_TMP]=average_TMP >>8;
                        slaveBuffer[LSB_TMP]=average_TMP & 0xFF;                         
                        
                        ldr_tot= (slaveBuffer[LSB_LDR] | slaveBuffer[MSB_LDR] << 8 );
                        ldr_V = ADC_DelSig_CountsTo_mVolts(ldr_tot);
                        sprintf(message, "LDR Output: %d\r\n", ldr_tot );
                        UART_PutString(message);
                    
                        tmp_tot= (slaveBuffer[LSB_TMP] | slaveBuffer[MSB_TMP] << 8 );
                        tmp_V = ADC_DelSig_CountsTo_mVolts(tmp_tot);
                        tmp_C = (tmp_V-TMP_OFF)/TMP_M;
                        sprintf(message, "Temp Output: %dC\r\n",(tmp_tot - BASE_LINE_TMP)*15); //We use int16 since the sensor accuracy is +/- 1°C
                        UART_PutString(message);    
                    
                        average_LDR=0;
                        average_TMP=0;
                        sum_LDR=0;
                        sum_TMP=0;    
                        //count_samples = 0;
                        PWM_RED_Start();
                        PWM_GREEN_Start();
                        PWM_BLUE_Start();
                    }
                }
                break;
                
            }
        
            if (count_samples == average_sample) {
                if (LED_modality==LED_MOD_LDR)
                {
                    if (bit_status == SLAVE_TMP_ON_CTRL_REG1) {
                        PWM_RED_Sleep();
                        PWM_GREEN_Sleep();
                        PWM_BLUE_Sleep();
                    }
                    if(ldr_tot > MID_RANGE_LDR) color_int=0;
                    else color_int = 65535;
                }
            
                else if (LED_modality==LED_MOD_TMP)
                {
                    if (bit_status == SLAVE_LDR_ON_CTRL_REG1) {
                        PWM_RED_Sleep();
                        PWM_GREEN_Sleep();
                        PWM_BLUE_Sleep();
                    }
                    color_int = (tmp_tot - BASE_LINE_TMP)*15;
                    if (color_int > 65535) color_int = 65535;
                    else if (color_int < 0) color_int = 0;
                }
                
                
                if(red) {
                    PWM_RED_WriteCompare(color_int);
                    color_int = 0;
                }
                else PWM_RED_WriteCompare(0);
                
                if (green) {
                    PWM_GREEN_WriteCompare(color_int);
                    color_int = 0;
                }
                else PWM_GREEN_WriteCompare(0);
                
                if(blue) { 
                    PWM_BLUE_WriteCompare(color_int);
                    color_int = 0;
                }
                else PWM_BLUE_WriteCompare(0);
                
                count_samples = 0;
            }
            
           /* if(red) {
                PWM_RED_WriteCompare(color_int);
                color_int = 0;
            }
            else PWM_RED_WriteCompare(0);
            
            if (green) {
                PWM_GREEN_WriteCompare(color_int);
                color_int = 0;
            }
            else PWM_GREEN_WriteCompare(0);
            
            if(blue) { 
                PWM_BLUE_WriteCompare(color_int);
                color_int = 0;
            }
            else PWM_BLUE_WriteCompare(0); */
        
    }
}
                
            
         
        
        
            
            
        
        
        
    
    
 
    


/* [] END OF FILE */
