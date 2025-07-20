/*
 * Author       : Saurabh kumar yadav
 * Date         : 20/04/25
 * Project Name : Car Black Box
 */


#include "black_box.h"

State_t state;

unsigned char time[9]; //to store the time from RTC
unsigned char clock_reg[3];

/*function to get time from RTC*/
static void get_time(void)
{
	clock_reg[0] = read_ds1307(HOUR_ADDR);
	clock_reg[1] = read_ds1307(MIN_ADDR);
	clock_reg[2] = read_ds1307(SEC_ADDR);

	if (clock_reg[0] & 0x40)
	{
		time[0] = '0' + ((clock_reg[0] >> 4) & 0x01);
		time[1] = '0' + (clock_reg[0] & 0x0F);
	}
	else
	{
		time[0] = '0' + ((clock_reg[0] >> 4) & 0x03);
		time[1] = '0' + (clock_reg[0] & 0x0F);
	}
	time[2] = ':';
	time[3] = '0' + ((clock_reg[1] >> 4) & 0x0F);
	time[4] = '0' + (clock_reg[1] & 0x0F);
	time[5] = ':';
	time[6] = '0' + ((clock_reg[2] >> 4) & 0x0F);
	time[7] = '0' + (clock_reg[2] & 0x0F);
	time[8] = '\0';
}

/*function to initialize the all needed components*/
void init_config()
{
    init_clcd();
    init_matrix_keypad();
    init_adc();
    init_uart();
    init_i2c();
    init_ds1307();
   state = e_dashboard;
}


void main(void) 
{
    init_config();// to initialize the required components
    while(1)
    {
        get_time();//to get the real time from RTC
        
        switch (state)  //to access the features
        {
            case e_dashboard:
                // Display dashboard
                view_dashboard();
                break;
            
            case e_main_menu:
                // Display dashboard
                display_main_menu();
                break;
            
            case e_view_log:
                // Display dashboard
                view_log();
                break;
                 
            case e_download_log:
                //display Download log 
                download_log();
                break;
                
            case e_clear_log:
                //clearing the logs
                clear_log();
                break;
                
                      
            case e_set_time:
                //presetting the time
                set_time();
                break;
                
        }
        
    }
    
}
