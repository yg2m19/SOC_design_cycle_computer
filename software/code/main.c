#include <stdint.h>
#include <stdbool.h>

#define AHB_SW_BASE        0x40000000
#define AHB_OUT_BASE       0x50000000

// The locations in the devices can then be accessed as:
//    SW_REGS[0]   count of TRIP
//    SW_REGS[1]   count of MODE
//    SW_REGS[2]   count of fork
//    SW_REGS[3]   count of total time
//    SW_REGS[4]   count of time per cycle of crank
//    SW_REGS[5]   count of time per cycle of fork
//    SW_REGS[6]   status
//    OUT_REGS[0]  data output
//    OUT_REGS[1]  status

volatile uint32_t* SW_REGS = (volatile uint32_t*) AHB_SW_BASE;
volatile uint32_t* OUT_REGS = (volatile uint32_t*) AHB_OUT_BASE;

#include <stdint.h>

void write_out(uint32_t value) {
OUT_REGS[1]=1;
OUT_REGS[0]=value;
}

uint32_t read_out(void){
return OUT_REGS[0];
}

uint32_t read_switches(int addr){
return SW_REGS[addr];
}
////////////////////////////////////////////////////////////////////////////
////delay approximate 1 second(frequcncy is 12.8 kHz)
////////////////////////////////////////////////////////////////////////////
void delay_1s(void)
{
int i,j;
for(i=20;i>0;i--){
for(j=35;j>0;j--);
}
}
///////////////////////////////////////////////////////////////////////////
////Function of distance
///////////////////////////////////////////////////////////////////////////
int distance(int count_fork,int circumference){
int value;
int result;
value = count_fork*circumference*10/1000000;
result=value+1073741824;                      //high 3 digits are 010
return(result);
}
///////////////////////////////////////////////////////////////////////////
////Function of timer
///////////////////////////////////////////////////////////////////////////
int timer(int count_of_total_time){
int value;
int result;
int time_h;
int time_min;
time_h=count_of_total_time/(3600*12800);
time_min=(count_of_total_time-time_h*3600*12800)/(60*12800);
value =time_h*100+time_min;
result=value+1610612736;                      //high 3 digits are 011
return(result);
}
///////////////////////////////////////////////////////////////////////////
////Function of rpm
///////////////////////////////////////////////////////////////////////////
int rpm(int switch_count_of_time_per_cycle_of_crank){
int value;
int result;
if(60*12800/switch_count_of_time_per_cycle_of_crank==0){
return(2147483648);
}
else{
value=60*12800/switch_count_of_time_per_cycle_of_crank;
result=value+2147483648;                   //high 3 digits are 100
return(result);
}
}
///////////////////////////////////////////////////////////////////////////
////Fuction of current_speed
///////////////////////////////////////////////////////////////////////////
int current_speed_function(int switch_count_of_time_per_cycle_of_fork, int circumference){
int speed_int, speed_fraction, cycle_per_hour;
int value, result;
if(switch_count_of_time_per_cycle_of_fork==0){
                     speed_int=0;
        	     speed_fraction=0;
		     }
else{
                     cycle_per_hour=460800000/switch_count_of_time_per_cycle_of_fork;
                     speed_int=(circumference*cycle_per_hour/1000000)/10;
                     speed_fraction=((circumference*cycle_per_hour)-speed_int*10000000)/1000000;
}
value=speed_int*10+speed_fraction;
result= value+2147483648+536870912;         //high 3 digits are 101
return (result);
}		     
///////////////////////////////////////////////////////////////////////////
//// Main Function for(i/o)
///////////////////////////////////////////////////////////////////////////

int main(void){
int button_trip, button_mode;
int state;
int priority;
int switch_count_of_fork, circumference, switch_count_of_total_time, switch_count_of_time_per_cycle_of_crank, switch_count_of_time_per_cycle_of_fork, max_speed, current_speed;
int bit;
int digit;
int press_mode, press_trip;
int press_two_button;
int bit_0_digit, bit_1_digit, bit_2_digit, bit_3_digit;
int speed_of_current;
state=0;
priority=0;
digit=0;
bit=0;
press_trip=0;
press_mode=0;
press_two_button=0;
button_trip=0;
button_mode=0;
switch_count_of_fork=0;
switch_count_of_total_time=0;
switch_count_of_time_per_cycle_of_crank=0;
switch_count_of_time_per_cycle_of_fork=0;
circumference=2136;
max_speed=0;
current_speed=0;
bit_0_digit=6;
bit_1_digit=3;
bit_2_digit=1;
bit_3_digit=2;
speed_of_current=0;
while(1){
    button_trip = read_switches(0);
    button_mode = read_switches(1);
       switch_count_of_time_per_cycle_of_fork=read_switches(5);
       current_speed=current_speed_function(switch_count_of_time_per_cycle_of_fork, circumference);
       speed_of_current=current_speed-2147483648-536870912;
       if((speed_of_current) > (max_speed)){
       max_speed=speed_of_current;
       }
if (((button_mode>320)&&(button_trip>320))){
press_two_button=1;
}
if ((button_mode==0)&&(button_trip==0)&&(press_two_button==1)){
priority = 1;
bit = 3;
press_two_button=0;
press_mode=0;
press_trip=0;
write_out(0); 
delay_1s();             //display "size" for 1s and high 3 digits are 000
write_out(536870912+2136);             //display "2136" high 3 digits are 001
}
if(priority){
        if((button_trip>=320)&&(button_mode==0)){
	press_trip=1;
	}
	if((button_trip==0)&&(press_trip==1)&&(button_mode==0)){
	         if(digit<9){
	          digit = digit+1;
	         }
	         else{
	          digit = 0;
	         }
		 press_trip=0;
		 }		 
        if((button_mode>=320)&&(button_trip==0)){
	press_mode=1;
	}
	if((button_mode==0)&&(press_mode==1)&&(button_trip==0)){
	         if(bit>0){
		 bit = bit-1;
		 digit=0;
		 press_mode=0;
		 }
		 else{
		 circumference=bit_3_digit*1000+bit_2_digit*100+bit_1_digit*10+bit_0_digit;
		 priority = 0 ;
		 state = 0;
		 press_mode=0;
		 write_out(bit_3_digit*1000+bit_2_digit*100+bit_1_digit*10+bit_0_digit+536870912);
		 }
		 }
     
	switch(bit){
	case 0:
		switch(digit){
		case 0: 
			bit_0_digit=0;
			write_out(bit_3_digit*1000+bit_2_digit*100+bit_1_digit*10+bit_0_digit+536870912);
			break;
		case 1:
			bit_0_digit=1;
			write_out(bit_3_digit*1000+bit_2_digit*100+bit_1_digit*10+bit_0_digit+536870912);
			break;
		case 2:
			bit_0_digit=2;
			write_out(bit_3_digit*1000+bit_2_digit*100+bit_1_digit*10+bit_0_digit+536870912);
			break;
		case 3:
			bit_0_digit=3;
			write_out(bit_3_digit*1000+bit_2_digit*100+bit_1_digit*10+bit_0_digit+536870912);
			break;
		case 4:
			bit_0_digit=4;
			write_out(bit_3_digit*1000+bit_2_digit*100+bit_1_digit*10+bit_0_digit+536870912);
			break;
		case 5:
			bit_0_digit=5;
			write_out(bit_3_digit*1000+bit_2_digit*100+bit_1_digit*10+bit_0_digit+536870912);
			break;
		case 6:
			bit_0_digit=6;
			write_out(bit_3_digit*1000+bit_2_digit*100+bit_1_digit*10+bit_0_digit+536870912);
			break;
		case 7:
			bit_0_digit=7;
			write_out(bit_3_digit*1000+bit_2_digit*100+bit_1_digit*10+bit_0_digit+536870912);
			break;
		case 8:
			bit_0_digit=8;
			write_out(bit_3_digit*1000+bit_2_digit*100+bit_1_digit*10+bit_0_digit+536870912);
			break;
		case 9:
			bit_0_digit=9;
			write_out(bit_3_digit*1000+bit_2_digit*100+bit_1_digit*10+bit_0_digit+536870912);
			break;
		}
		break;
	case 1:
		switch(digit){
		case 0: 
			bit_1_digit=0;
			write_out(bit_3_digit*1000+bit_2_digit*100+bit_1_digit*10+bit_0_digit+536870912);
			break;
		case 1:
			bit_1_digit=1;
			write_out(bit_3_digit*1000+bit_2_digit*100+bit_1_digit*10+bit_0_digit+536870912);
			break;
		case 2:
			bit_1_digit=2;
			write_out(bit_3_digit*1000+bit_2_digit*100+bit_1_digit*10+bit_0_digit+536870912);
			break;
		case 3:
			bit_1_digit=3;
			write_out(bit_3_digit*1000+bit_2_digit*100+bit_1_digit*10+bit_0_digit+536870912);
			break;
		case 4:
			bit_1_digit=4;
			write_out(bit_3_digit*1000+bit_2_digit*100+bit_1_digit*10+bit_0_digit+536870912);
			break;
		case 5:
			bit_1_digit=5;
			write_out(bit_3_digit*1000+bit_2_digit*100+bit_1_digit*10+bit_0_digit+536870912);
			break;
		case 6:
			bit_1_digit=6;
			write_out(bit_3_digit*1000+bit_2_digit*100+bit_1_digit*10+bit_0_digit+536870912);
			break;
		case 7:
			bit_1_digit=7;
			write_out(bit_3_digit*1000+bit_2_digit*100+bit_1_digit*10+bit_0_digit+536870912);
			break;
		case 8:
			bit_1_digit=8;
			write_out(bit_3_digit*1000+bit_2_digit*100+bit_1_digit*10+bit_0_digit+536870912);
			break;
		case 9:
			bit_1_digit=9;
			write_out(bit_3_digit*1000+bit_2_digit*100+bit_1_digit*10+bit_0_digit+536870912);
			break;
		}
		break;
	case 2:
		switch(digit)
		case 0: {
			bit_2_digit=0;
			write_out(bit_3_digit*1000+bit_2_digit*100+bit_1_digit*10+bit_0_digit+536870912);
			break;
		case 1:
			bit_2_digit=1;
			write_out(bit_3_digit*1000+bit_2_digit*100+bit_1_digit*10+bit_0_digit+536870912);
			break;
		case 2:
			bit_2_digit=2;
			write_out(bit_3_digit*1000+bit_2_digit*100+bit_1_digit*10+bit_0_digit+536870912);
			break;
		case 3:
			bit_2_digit=3;
			write_out(bit_3_digit*1000+bit_2_digit*100+bit_1_digit*10+bit_0_digit+536870912);
			break;
		case 4:
			bit_2_digit=4;
			write_out(bit_3_digit*1000+bit_2_digit*100+bit_1_digit*10+bit_0_digit+536870912);
			break;
		case 5:
			bit_2_digit=5;
			write_out(bit_3_digit*1000+bit_2_digit*100+bit_1_digit*10+bit_0_digit+536870912);
			break;
		case 6:
			bit_2_digit=6;
			write_out(bit_3_digit*1000+bit_2_digit*100+bit_1_digit*10+bit_0_digit+536870912);
			break;
		case 7:
			bit_2_digit=7;
			write_out(bit_3_digit*1000+bit_2_digit*100+bit_1_digit*10+bit_0_digit+536870912);
			break;
		case 8:
			bit_2_digit=8;
			write_out(bit_3_digit*1000+bit_2_digit*100+bit_1_digit*10+bit_0_digit+536870912);
			break;
		case 9:
			bit_2_digit=9;
			write_out(bit_3_digit*1000+bit_2_digit*100+bit_1_digit*10+bit_0_digit+536870912);
			break;
		}
		break;
	case 3:
		switch(digit){
		case 0: 
			bit_3_digit=0;
			write_out(bit_3_digit*1000+bit_2_digit*100+bit_1_digit*10+bit_0_digit+536870912);
			break;
		case 1:
			bit_3_digit=1;
			write_out(bit_3_digit*1000+bit_2_digit*100+bit_1_digit*10+bit_0_digit+536870912);
			break;
		case 2:
			bit_3_digit=2;
			write_out(bit_3_digit*1000+bit_2_digit*100+bit_1_digit*10+bit_0_digit+536870912);
			break;
		case 3:
			bit_3_digit=3;
			write_out(bit_3_digit*1000+bit_2_digit*100+bit_1_digit*10+bit_0_digit+536870912);
			break;
		case 4:
			bit_3_digit=4;
			write_out(bit_3_digit*1000+bit_2_digit*100+bit_1_digit*10+bit_0_digit+536870912);
			break;
		case 5:
			bit_3_digit=5;
			write_out(bit_3_digit*1000+bit_2_digit*100+bit_1_digit*10+bit_0_digit+536870912);
			break;
		case 6:
			bit_3_digit=6;
			write_out(bit_3_digit*1000+bit_2_digit*100+bit_1_digit*10+bit_0_digit+536870912);
			break;
		case 7:
			bit_3_digit=7;
			write_out(bit_3_digit*1000+bit_2_digit*100+bit_1_digit*10+bit_0_digit+536870912);
			break;
		case 8:
			bit_3_digit=8;
			write_out(bit_3_digit*1000+bit_2_digit*100+bit_1_digit*10+bit_0_digit+536870912);
			break;
		case 9:
			bit_3_digit=9;
			write_out(bit_3_digit*1000+bit_2_digit*100+bit_1_digit*10+bit_0_digit+536870912);
			break;
		}
	        break;
		}
}
else {
       if((button_mode>=320)&&(button_trip==0)){
         press_mode=1;
       }
       if((button_mode==0)&&(press_mode==1)&&(button_trip==0)){
       if(state<5){
         state=state+1;
	 press_mode=0;
	 }
	 else{
	 state=0;
	 press_mode=0;
	 }
	 }
       switch (state){
       case 0: 
       switch_count_of_fork=read_switches(2);
       write_out(distance(switch_count_of_fork, circumference));
       break;
       case 1: 
       switch_count_of_total_time=read_switches(3);
       write_out(timer(switch_count_of_total_time));
       break;
       case 2: 
       switch_count_of_time_per_cycle_of_crank=read_switches(4);       
       write_out(rpm(switch_count_of_time_per_cycle_of_crank));
       break; 
       case 3:
       write_out(current_speed);
       break;
       case 4:
       write_out(max_speed+3221225472);       //high 3 digits are 110
       break;
       case 5:
       switch_count_of_fork=read_switches(2);
       switch_count_of_total_time=read_switches(3);
       write_out((switch_count_of_fork*circumference*10/1000000)/(switch_count_of_total_time/(12800*3600))+3758096384);       //high 3 digits are 111
       break;
       }
       }
}
}
       
       
