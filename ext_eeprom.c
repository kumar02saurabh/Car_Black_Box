
#include "ext_eeprom.h"
#include "i2c.h"
#include <xc.h>

/* 
 * DS1307 Slave address
 * D0  -  Write Mode
 * D1  -  Read Mode
 */


void write_ext_eeprom(unsigned char address1,  unsigned char data)
{
	i2c_start();
	i2c_write(SLAVE_WRITE_E);
	i2c_write(address1);
	i2c_write(data);
	i2c_stop();
    
    for(unsigned int i=0;i<=3000;i++);
}

unsigned char read_ext_eeprom(unsigned char address1)
{
	unsigned char data;

	i2c_start();
	i2c_write(SLAVE_WRITE_E);
	i2c_write(address1);
	i2c_rep_start();
	i2c_write(SLAVE_READ_E);
	data = i2c_read();
	i2c_stop();

	return data;
}