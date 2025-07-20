#ifndef BLACK_BOX_H
#define BLACK_BOX_H


#include <xc.h>
#include "matrix_keypad.h"
#include "ext_eeprom.h"
#include "clcd.h"
#include "adc.h"
#include "i2c.h"
#include "ds1307.h"
#include "uart.h"

/* Enum for maintaining the app state */
typedef enum {
    e_dashboard, e_main_menu, e_view_log, e_set_time, e_download_log, e_clear_log
} State_t;

#define _XTAL_FREQ 20000000

extern State_t state; // App state

//Function declarations

//Dashboard function declaration
void view_dashboard(void);

//Storing events function declaration
void event_store(void);

//main menu function declaration
void display_main_menu(void);

//View log function declaration
void view_log(void);

//Reading events function declaration
void event_reader(void);

//Set time function declaration
void set_time(void);

//Download log function _decleration
void download_log(void);

//Clear log function declaration
void clear_log(void);

static void get_time(void);

unsigned char BCD_conversion(unsigned char input);

#endif