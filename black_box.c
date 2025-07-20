#include "black_box.h"

unsigned char events[8][3]={"ON","GN","GR","G1","G2","G3","G4","C "};
extern unsigned char time[9];
unsigned char gear=0;
unsigned char address=0x00;
unsigned short speed;
unsigned char event_count=0;
unsigned char switches;
signed char disp_cnt;
unsigned char view_address=0x00;
unsigned char view[10][15];
signed char view_var=0;
unsigned char set_flag=0; 
unsigned char crash_flag=0;
unsigned char gear_flag=0;


/*function for view board*/
void view_dashboard(void)
{
  clcd_print("TIME     EVT SPD",LINE1(0));//prompting the functions which we are going to display
  for(unsigned char i=0;i<8;i++) //displaying the time
   {
     clcd_putch(time[i],LINE2(i));
   }
  clcd_putch(' ',LINE2(8));
  speed=read_adc()/10.24;//reading the speed using ADC
  clcd_print("  ",LINE2(11));
  clcd_putch(speed/10+48,LINE2(13));//displaying the speed
  clcd_putch(speed%10+48,LINE2(14));
  clcd_print("  ",LINE2(15));
  clcd_putch(' ',LINE2(8));
  switches=read_switches(STATE_CHANGE);//reading the gears by using MKP
   

    if(switches==MK_SW1)
      {
        if(crash_flag==1) //after the crash reset to GN
        {
            gear=1;
        }
      else if(gear==0)//to skip ON condition
        {
              gear=1;   
        }
      else if(gear>=6) //not to go above maximum gears
      {
          gear=6;
      }
      else //increasing the gears
      {
          gear++;
          event_store();
      } 
  }
  else if(switches==MK_SW2) 
  {   
      if(crash_flag==1)//after the crash reset to GN
      {
        gear= 1;
      } 
      
      if(gear>1)
      {
        gear--; 
        event_store();
        if(gear<1)
        {
            gear_flag=1;
        }
      }
      else if(gear<1) //not to go behind minimum gears
      {   
          if(gear_flag==0)
          {
              gear=0;
          }
          else
          {
            gear=1;  
          }
      } 
      
        
        
      
  }
  else if(switches==MK_SW3)//for crash condition
  {
      gear=7;
      crash_flag=1;
      event_store();
  } 
  
   clcd_putch(events[gear][0],LINE2(9));
   clcd_putch(events[gear][1],LINE2(10)); 
   
   if(switches==MK_SW11)//for menu option
   {
       state=e_main_menu;
   }
}

/*function to store the events*/
void event_store(void)
{
    event_count++;
    if(address==120) //for checking maximum 10 events
    {
      address=0x00; //resetting to initial value
      
    }
    
    if(event_count==11)
    {
        event_count=10;//set max condition count
    }
    
    for(unsigned char i=0;i<8;i++)//writing the time in external EEPROM
    { 
        write_ext_eeprom(address++,time[i]);
    }
   write_ext_eeprom(address++,events[gear][0]);//writing the current condition (event)
   write_ext_eeprom(address++,events[gear][1]);
   
   write_ext_eeprom(address++,speed/10+48);//writing the speed
   write_ext_eeprom(address++,speed%10+48);
   
}

/*function to display main menu*/
void display_main_menu()
{  
   switches=read_switches(STATE_CHANGE);//to select main menu features
    if(switches==MK_SW1) //to scroll down
    {
        disp_cnt++;
    }
    else if(switches==MK_SW2)//to scroll up 
    {
        disp_cnt--;
    }
    else if(switches==MK_SW12) //to return to dashboard
    {
        state=e_dashboard;
    }
    
   /*edge conditions to maintain the disp_cnt variable*/
    if(disp_cnt<0) 
    {
        disp_cnt=0;
    }
    else if(disp_cnt>3)
    {
        disp_cnt=3;
    }
    
  /*for view log feature*/
    if(disp_cnt==0)
    {
      clcd_print("-> View Log     ",LINE1(0));
      clcd_print("   download Log ",LINE2(0));  
      if(switches==MK_SW11) //to select OK
      {
          state=e_view_log;
      }
    }
   /*for display the log*/
    else if(disp_cnt==1)
    {
        clcd_print("   View Log     ",LINE1(0));
        clcd_print("-> download Log ",LINE2(0));
       if(switches==MK_SW11) //to select OK
      {
          state=e_download_log;
      }
    }
   
   /*for clear the log*/
    else if(disp_cnt==2)
    {
        clcd_print("   download Log ",LINE1(0));
        clcd_print("-> clear log    ",LINE2(0)); 
      if(switches==MK_SW11)  //to select OK
      {
          state=e_clear_log;
      }
    }
   /*for set time feature*/
    else if(disp_cnt==3)
    {
        clcd_print("   clear log    ",LINE1(0));
        clcd_print("-> Set time     ",LINE2(0));   
      if(switches==MK_SW11) //to select OK
      {
          state=e_set_time;
      }
    }  
}

/*function for view log*/
void view_log()
{ 
    if(event_count==0)  //if no events happened
    {
      clcd_print("No Events Found",LINE1(0)); 
      clcd_print("               ",LINE2(0));
      __delay_ms(600);
      state=e_main_menu;      
    }
    else
    {
      event_reader(); //storing the events in one array
      clcd_print("No Time    EV SP",LINE1(0));
      
      /*displaying the events*/
         switches=read_switches(STATE_CHANGE);
         
         if(switches==MK_SW1)//scroll down the events
         {
             view_var++;
         }
         else if(switches==MK_SW2)//scroll up the events
         {
             view_var--;
         }
         else if(switches==MK_SW12)//to exit to main menu
         {
           state=e_main_menu;  
         }
         
         if(view_var<0)// not go behind zero
         {
             view_var=0;
         }
         else if(view_var>=event_count) // not go above event count
         {
             view_var=event_count-1;
         }
            clcd_putch(view_var+48,LINE2(0));
            clcd_putch(' ',LINE2(1));
            clcd_print(view[view_var],LINE2(2));            
    }
    
}

/*function for clear log*/
void clear_log()
{
    clcd_print("Clearing....   ",LINE1(0));
    clcd_print("               ",LINE2(0));
    __delay_ms(600);
    event_count=0;//resetting event count
    address=0x00;//resetting the EEPROM 
    view_var=0;//viewing the events
   state=e_main_menu; //return to main menu
}

/*storing events into one array*/
void event_reader(void)
{
    for(unsigned char i=0;i<event_count;i++)
         {
           for(unsigned char j=0;j<15;j++)
              {
                 if(j==8 || j==11)
                   {
                     view[i][j]=' ';
                   }
                  else if(j==14)
                  {
                    view[i][j]='\0';  
                  }
                 else
                  {
                    view[i][j]=read_ext_eeprom(view_address++);
                  } 
             }
         }
    view_address=0x00;
}


/*function to downloading the logs*/
void download_log()
{
    if(event_count==0) //if no events happened
    {
        clcd_print("NO Events Found ",LINE1(0));
        clcd_print("                ",LINE2(0));
         __delay_ms(700);
         state=e_main_menu; 
    }
    else 
    {
    clcd_print("Downloading.... ",LINE1(0));
    clcd_print("                ",LINE2(0));
    __delay_ms(500);
    event_reader();
    
    
    /*displaying the events in xterm using uart protocol*/
    for(int i = 0; i < event_count; i++) 
    {
                if(i == 9) //for displaying the serial number of event
                {
                    uart_putch(49);
                    uart_putch(48);
                }
                else 
                {
                    uart_putch(i+1+48);
                }
                uart_puts(". ");

                uart_puts(view[i]);//displaying the events
                uart_puts("\n\r");
   }
    
    clcd_print("  Downloaded    ",LINE1(0));
    clcd_print("  Successfully  ",LINE2(0));
    __delay_ms(500);
    state=e_main_menu; 
    
    }
 
}

/*function to set time*/
void set_time()
{
    
     unsigned char set_time_flag=0,b_flag=1,flag=0;
     /*set_time_flag for controlling while loop*/
     /*b_flag for blinking the particular field*/
     /*flag for displaying the other fields*/
     
     unsigned int i=0;//for delay
     
     unsigned int hr=((time[0]-48) * 10) + (time[1]-48);//storing the hour
     unsigned int min=((time[3]-48) * 10) + (time[4]-48);//storing the min
     unsigned int sec=((time[6]-48) * 10) + (time[7]-48);//storing the second
     
     
     while(set_time_flag!=1)//loop to control the set time feature
       {
        clcd_print("    HH:MM:SS    ",LINE1(0));
        clcd_print("    ",LINE2(0));
    
        if(flag==0)
          {
            if(i<250) //for normal displaying hour
              {
                clcd_putch((hr/10)+48,LINE2(4));
                clcd_putch((hr%10)+48,LINE2(5));
              }
            else if(i<500 && b_flag==1)//to blink the hour field
              {
              clcd_putch(0xff,LINE2(4));
              clcd_putch(0xff,LINE2(5));
              }
        }
      if(flag==0)
        {
           if(i<250) //for normal displaying minutes
            {
             clcd_putch((min/10)+48,LINE2(7));
             clcd_putch((min%10)+48,LINE2(8));
            }
           else if(i<500 && b_flag==2) //to blink the minute field
           {
             clcd_putch(0xff,LINE2(7));
             clcd_putch(0xff,LINE2(8));
           }  
        }
     if(flag==0) 
       {
         if(i<250) //for normal displaying seconds
          {
           clcd_putch((sec/10)+48,LINE2(10));
           clcd_putch((sec%10)+48,LINE2(11));
           }
         else if(i<500 && b_flag==3) //to blink the seconds field
           {
           clcd_putch(0xff,LINE2(10));
           clcd_putch(0xff,LINE2(11));
           }  
       }
    clcd_putch(':',LINE2(6));
    
    clcd_putch(':',LINE2(9));
    
    clcd_print("     ",LINE2(12));
    
    switches=read_switches(STATE_CHANGE); //to set time using MKP
    
    if(switches==MK_SW1)//to set time
    {
       if(set_flag==0)  
       {
           hr++;
       }
       else if(set_flag==1)
       {
           min++;
       }
       else if(set_flag==2)
       {
           sec++;
       }
       
       if(hr>23) //behind 24 hr format
       {
           hr=0;
       }
       if(min>59) //behind 60 minutes
       {
           min=0;
       }
       if(sec>59) //behind 60 seconds
       {
           sec=0;
       }
    }
    else if(switches==MK_SW2)//to change the field
    {
      set_flag++;
     
      b_flag++;
      
      if(set_flag>2) //edge cases
      {
          set_flag=0;
      }
      if(b_flag>3)
      {
          b_flag=1;
      }
    }
    else if(switches==MK_SW11) //save and exit option
     {
        /*/presetting value in BCD's*/
        unsigned char hour  = BCD_conversion(hr);
        unsigned char minute= BCD_conversion(min);
        unsigned char second= BCD_conversion(sec);
        
        /*writing presetting values in BCD form into respective address of RTC*/
        write_ds1307(HOUR_ADDR,hour);
        write_ds1307(MIN_ADDR,minute);
        write_ds1307(SEC_ADDR,second);
        
        set_time_flag=1;
        state=e_dashboard;
     }
    
    else if(switches==MK_SW12)//exit option
      {
        set_time_flag=1;
        state=e_dashboard;
      }
    if(i==500)
    {
        i=0;
    }
    i++;
  }
     
}

/*function to convert binary into BCD form to update the time*/
unsigned char BCD_conversion(unsigned char input)
{
    unsigned char output=(unsigned char)((input/10)) <<4; //getting the tens place and updating higher nibble
    unsigned char get=(unsigned char)(input%10) ;//getting ones place and updating into lower nibbles
    
    return output | get; //returning the BCD form
}
