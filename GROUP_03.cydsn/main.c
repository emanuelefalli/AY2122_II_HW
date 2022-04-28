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
#include "RGB_PWM.h"
#include "InterruptRoutines.h"
#include "I2C_REG.h"
#include "Logging.h"

//Define structure of slave buffer


uint8 average_sample=0;
uint8 curr_avg_sample;
uint8 bit_status;
uint8 LED_modality;
uint8 colors;
uint16 ldr_tot;
uint16 tmp_tot;
int16 count_samples;
uint32 sum_LDR=0;
uint32 sum_TMP=0;
uint32 average_LDR=0;
uint32 average_TMP=0;


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
        curr_avg_sample = (slaveBuffer[CTRL_REG1] & 0x18) >> 3; // I check the number of samples to extract
        if (curr_avg_sample != average_sample) {
            average_sample = curr_avg_sample;
            average_sample=average_sample+1;
        }
        // I write the period register according to the number of average samples to obtain 50Hz transmission rate
        Timer_WritePeriod((0.02/average_sample)*5000);  
        

        bit_status = slaveBuffer[CTRL_REG1] & 0x03; //I check the bit status configration

        LED_modality = (slaveBuffer[CTRL_REG1] & 0x04)>>2; //I check the LedMod of the CTRL REG 1

        colors = (slaveBuffer[CTRL_REG1] & 0xE0) >> 5;
        
        
        switch (bit_status){
            
            case SLAVE_MODE_OFF_CTRL_REG1:
                Pin_RED_Write(0);
                Pin_GREEN_Write(0);
                Pin_BLUE_Write(0);
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
                    
                        slaveBuffer[MSB_LDR]=average_LDR >> 8;
                        slaveBuffer[LSB_LDR]=average_LDR & 0xFF;
                        slaveBuffer[MSB_TMP]= 0x00;
                        slaveBuffer[LSB_TMP]= 0x00;
                        ldr_tot= (slaveBuffer[LSB_LDR] | slaveBuffer[MSB_LDR] << 8 );

                        //uint16 ldr_V = ADC_DelSig_CountsTo_mVolts(average_LDR);

                        sprintf(message, "LDR Output: %d\r\n", ldr_tot );
                        UART_PutString(message);
                        Led_colors(colors);
                        if (LED_modality == LED_MOD_LDR)
                        PWM_modulation(LED_MOD_LDR, ldr_tot, tmp_tot, SLAVE_LDR_ON_CTRL_REG1);
                        //reset_data();
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
                        //int16 tmp_V = ADC_DelSig_CountsTo_mVolts(tmp_tot);
                        //int16 tmp_C = (tmp_V-TMP_OFF)/TMP_M;
                        sprintf(message, "Temp Output: %dC\r\n", tmp_tot);                     
                        UART_PutString(message);
                     
                        Led_colors(colors);
                        if (LED_modality == LED_MOD_TMP)
                        PWM_modulation(LED_MOD_TMP, ldr_tot, tmp_tot, SLAVE_TMP_ON_CTRL_REG1);
                        //reset_data();
                    }
                }
                
                
            break;
                
                
            case SLAVE_BOTH_ON_CTRL_REG1:
                UART_PutString("Both ON!\r\n");
                
                if (flagData==1) {
                    sum_LDR+=value_digit_LDR;
                    sum_TMP+=value_digit_TMP;
                    count_samples++; 
                    flagData=0;
                
                    if (count_samples==average_sample) {
                        average_LDR=sum_LDR/average_sample;
                        average_TMP=sum_TMP/average_sample;
                    
                        slaveBuffer[MSB_LDR]=average_LDR >> 8;
                        slaveBuffer[LSB_LDR]=average_LDR & 0xFF;
                        slaveBuffer[MSB_TMP]=average_TMP >>8;
                        slaveBuffer[LSB_TMP]=average_TMP & 0xFF;
                    
                           
                        ldr_tot= (slaveBuffer[LSB_LDR] | slaveBuffer[MSB_LDR] << 8 );
                        //int16 ldr_V = ADC_DelSig_CountsTo_mVolts(ldr_tot);
                        sprintf(message, "LDR Output: %d\r\n", ldr_tot );
                        UART_PutString(message);
                        tmp_tot= (slaveBuffer[LSB_TMP] | slaveBuffer[MSB_TMP] << 8 );
                        //int16 tmp_V = ADC_DelSig_CountsTo_mVolts(tmp_tot);
                        //int16 tmp_C = (tmp_V-TMP_OFF)/TMP_M;
                        sprintf(message, "Temp Output: %dC\r\n", tmp_tot); //We use int16 since the sensor accuracy is +/- 1°C
                        UART_PutString(message);
                        
                        Led_colors(colors);
                        if (LED_modality == LED_MOD_LDR)
                        PWM_modulation(LED_MOD_LDR, ldr_tot, tmp_tot, SLAVE_LDR_ON_CTRL_REG1);
                        else if (LED_modality == LED_MOD_TMP)
                        PWM_modulation(LED_MOD_TMP, ldr_tot, tmp_tot, SLAVE_TMP_ON_CTRL_REG1);
                        //reset_data();
                    }
                }
            break;
                
        }
        
    }
}
                
            
         
        
        
            
            
        
        
        
    
    
 
    


/* [] END OF FILE */
