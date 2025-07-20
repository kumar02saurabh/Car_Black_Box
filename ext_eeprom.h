#ifndef Ext_eeprom_H
#define Ext_eeprom_H

#define SLAVE_READ_E		0xA1
#define SLAVE_WRITE_E		0xA0

void write_ext_eeprom(unsigned char address1,  unsigned char data);
unsigned char read_ext_eeprom(unsigned char address1);

#endif