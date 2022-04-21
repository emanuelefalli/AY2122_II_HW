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

#ifndef __I2C_REG_H
    #define __I2C_REG_H

    /**
    *   \brief 7-bit I2C address of the slave device.
    */
    #define I2C_DEVICE_ADDRESS 0x08

    /**
    *   \brief Address of the WHO AM I register return value
    */
    #define I2C_WHO_AM_I_REG_VALUE 0xBC
    
    // Who i am register address
    #define I2C_WHO_I_AM_REG_ADDR 0x01
   
    
    //brief Address of the Control register 1
    
    #define I2C_CTRL_REG1 0x00
    
    
   

    // Brief Address of the ADC output LSB register of the first channel
    
 //   #define CH0_ADC_L 0x04

    // Brief Address of the ADC output MSB register of the first channel
    
 //   #define CH0_ADC_H 0x03
    
    // Brief Address of the ADC output LSB register of the second channel
    
 //   #define CH1_ADC_L 0x06

    // Brief Address of the ADC output MSB register of the second channel
    
 //   #define CH1_ADC_H 0x05
    
    //brief 8 bit to set no sampling (status bit 1 = 0, status bit 0 = 0)
    
    #define SLAVE_MODE_OFF_CTRL_REG1 0x00
    
    /**
    *   \brief 8 bit to set the LDR sampling on (status bit 1 = 0, status bit 0 = 1)
    **/
    #define SLAVE_LDR_ON_CTRL_REG1 0x01
    
    /**
    *   \brief 8 bit to set the TMP sampling on (status bit 1 = 1, status bit 0 = 0)
    **/
    #define SLAVE_TMP_ON_CTRL_REG1 0x02
    
    /**
    *   \brief 8 bit to sampling both LDR and TMP (status bit 1 = 1, status bit 0 = 1)
    **/
    #define SLAVE_BOTH_ON_CTRL_REG1 0x03
    
    //LED MODALITY
       
    #define LED_MOD_TMP 0x04
    
    #define LED_MOD_LDR 0x0
    
    
    
#endif


/* [] END OF FILE */
