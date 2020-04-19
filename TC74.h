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

/*
#pragma warning push
#pragma warning disable 520   //unused functions disbaled warning
#pragma warning pop
*/

#include <stdint.h>

#define TC74_VERSION "1.0.0"
#define TC74_CAPTION     "TC74 " TC74_VERSION


/* 7-bit device address */
typedef enum {
  ADDR_A0    =0x48,     /*0x48*/
  ADDR_A1    =0x49,     /*0x49*/
  ADDR_A2    =0x4A,     /*0x4A*/
  ADDR_A3    =0x4B,     /*0x4B*/
  ADDR_A4    =0x4C,     /*0x4C*/
  ADDR_A5    =0x4D,     /*!< Default Address: 0x4D*/
  ADDR_A6    =0x4E,     /*0x4E*/
  ADDR_A7    =0x4F      /*0x4F*/
}ADDR_SLAVE_7b_t;

typedef enum{
    TC74_RTR  = 0x00, /*!< Register Read Temperature( TEMP)*/
    TC74_RWCR = 0x01   /*!< Read/Write configuration register (CONFIG)*/
}TC74_Reg;


/** @brief Pointer to function for I2C write peripheral uint8_t Address, void *data, uint8_t amount*/
typedef void (*Write_Fcn )(uint8_t, void*, uint8_t);


/** @brief Pointer to function for I2C read peripheral uint8_t Address, void *data, uint8_t amount , el dato leido lo maneja el driver*/
typedef void (*Read_Fcn  )(uint8_t, void*, uint8_t);



typedef struct {
    Write_Fcn I2CWrite;
    Read_Fcn  I2CRead;
    ADDR_SLAVE_7b_t Dev_Addr;
}TC74Data_t;

/*Nota 1:  activa o desactiva sequencia manual start,write,read,ack,stop 
            o solo funciÃ³n write y read que ya realiza la secuencia internamente*/ 

/** */


/**
 * @brief Function for iniatilize driver TC74 with the I2C peripheral functions
 * @note Example :
 *     TC74_Initialize(&TC74Data,ADDR_A5,Wrappers_I2CMaster_Write,Wrappers_I2CMaster_Read);
		
 * @param  obj :       Structure containing all data from the TC74.
 * @param  AddrSlave : address of the slave in 7 bits
 * @param  write_fcn : Pointer to function for write  i2c, this is compatible with different hardware platforms
 * @param  read_fcn :  Pointer to function for read  i2c,this is compatible with different hardware platforms
 */
void TC74_Initialize(TC74Data_t *Obj, ADDR_SLAVE_7b_t AddrSlave ,Write_Fcn write_fcn, Read_Fcn read_fcn);

/**
 * @brief Function put in standby mode the sensor
 * @note Example : TC74_Standby(&TC74Data , 0);
 * @param  obj :    Structure containing all data from the TC74.*/
void TC74_Standby(TC74Data_t *Obj , uint8_t operation);

/**
 * @brief Function read bit data ready 
 * @note Example : TC74_Ready(&TC74Data);
 * @param  obj :    Structure containing all data from the TC74.*/
uint8_t TC74_Ready(TC74Data_t *Obj);

/**
 * @brief Function read the temperature
 * @note Example : int8_t temp = TC74_Temperature(&TC74Data);
 * @param  obj :    Structure containing all data from the TC74.*/
int8_t TC74_Temperature(TC74Data_t *Obj);


