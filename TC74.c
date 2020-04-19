#ifndef TC74_H
#define	TC74_H

/*!
 * *******************************************************************************
 * @file TC74.c
 * @brief Driver for digital temperature sensor
 * @version 1.0.0
 * @date April 17, 2020
 * @author Esp.Ing.Julian Bustamante N
 * @note  +-2°C(max) Accuracy from +25°C to +85°C
 *        +-3°C(max) Accuracy from +0°C to +125°C
 *        +-2°C(max) Accuracy from -40°C to +125°C
 * @see   http://ww1.microchip.com/downloads/en/devicedoc/21462d.pdf
 *********************************************************************************/



#include "TC74.h"
#include <stdlib.h>


static uint8_t TC74_Write(TC74Data_t *Obj, uint8_t TC74_Register, uint16_t data, uint8_t amount);

static uint8_t TC74_Read(TC74Data_t *Obj, uint8_t TC74_Register, uint8_t amount);

/*@brief Function for iniatilize driver TC74 with the I2C peripheral functions*/
void TC74_Initialize(TC74Data_t *Obj, ADDR_SLAVE_7b_t AddrSlave ,Write_Fcn write_fcn, Read_Fcn read_fcn){   
    /*Asigno nombre de la funciÃ³n al puntero a funciÃ³n del objeto*/
    Obj->I2CWrite = write_fcn;
    Obj->I2CRead = read_fcn;
    Obj->Dev_Addr = AddrSlave;  /*_xxxxxxx 7 bits*/

}

/* @brief Function for Write to sensor */
static uint8_t TC74_Write(TC74Data_t *Obj, uint8_t TC74_Register, uint16_t data, uint8_t amount){
	uint8_t Retvalue = 1;
    uint8_t buffer[2]; /*envio el registro a escribir con el dato*/
	buffer[0] = TC74_Register;
	buffer[1] = data;
	if(Obj->I2CWrite != NULL){
        Obj->I2CWrite( Obj->Dev_Addr, buffer, (amount+1)); /*registro + dato = amount + 1*/
    }
    else Retvalue = 0; /*no write*/
    return Retvalue;
}

/* @brief Function for Read the sensor */
static uint8_t TC74_Read(TC74Data_t *Obj, uint8_t TC74_Register, uint8_t amount){
	uint8_t buffer[2];
	buffer[0] = TC74_Register;
	if(Obj->I2CRead != NULL){ 
        Obj->I2CRead( Obj->Dev_Addr, buffer, amount); /*guarda en el mismo registro que lee*/
    }
	return buffer[0];
}

/* @brief Function put in standby mode the sensor*/
void TC74_Standby(TC74Data_t *Obj , uint8_t operation){
    uint8_t RCWR;
    operation = operation ?0x80:0x00;
    RCWR = TC74_Read(Obj,TC74_RWCR,1);
    TC74_Write(Obj,TC74_RWCR,operation ,1); /*bit 7 -> 1= standby, 0 = normal*/
}

/*  @brief Function read bit data ready */
uint8_t TC74_Ready(TC74Data_t *Obj){
    uint8_t Ready=0;
    Ready = TC74_Read(Obj,TC74_RWCR,1); /*bit 6 1=data ready, 0= no data  ready*/
    /**DATA_RDY bit RESET at power-up and SHDN enable.*/
    return ( (Ready & 0x40)?1:0 ); /*masking bit 6 only*/
}

/* @brief Function read the temperature*/
int8_t TC74_Temperature(TC74Data_t *Obj){
    uint8_t Digital_Temperature = 0;
    int8_t Temperature = 0;
    if(TC74_Ready(Obj)) {/*if data is ready*/
        Digital_Temperature = TC74_Read(Obj,TC74_RTR,1);
    }
    /*2's complement binary*/
    if(Digital_Temperature > 127){
        Temperature = ((~Digital_Temperature) |0x01)*(-1) ;  /*10111111 -> not(191) = 0x40 +1  =  0x41 = 65 = -65 in 2's complement*/
    }else Temperature = (int8_t)Digital_Temperature;
        
    return Temperature;
}


#endif  /*TC74_H*/