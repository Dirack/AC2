#include <detpic32.h>
#include <ptrodrigues.h>
#define refresh_rate_SSD 10 // 200ms => 5Hz 100ms => 10Hz
#define refresh_counter_rate 200 // 1000ms => 1Hz
void __init__()
{
	TRISB &= 0xFC00; 			// Set RB0 to RB9 as outputs
}
void test_7seg()
{
	printStr("Testing low display\n0x");	 // enable low display
	printInt(LATB,(4<<16)+16);
	putChar('\n');
	int i;
	for(i = 1000; i!=0; )
	{
		LATB = set_SSD_Hex(LATB,CHECK,false);
		i -= CALIBRATION_SSD;
		delay(CALIBRATION_SSD);
		LATB ^= 0x200;
		i -=(10-CALIBRATION_SSD);
		delay(10-CALIBRATION_SSD);
		LATB ^= 0x200;
	}
	printStr("Testing high display\n0x");
	LATB = set_SSD_Hex(LATB,CHECK,true);	// enable high display
	printInt(LATB,(4<<16)+16);
	putChar('\n');
	for(i = 1000; i!=0; )
	{
		LATB = set_SSD_Hex(LATB,CHECK,true);
		i -= (10-CALIBRATION_SSD);
		delay(10-CALIBRATION_SSD);
		LATB ^= 0x100;
		i -= CALIBRATION_SSD;
		delay(CALIBRATION_SSD);
		LATB ^= 0x100;
	}
	LATB &= 0xFC00;				// turn off both displays
}

uchar hex_char_decode(uchar value)
{
	static uchar char_array[] = {
	ZERO,ONE,TWO,THREE,FOUR,FIVE,SIX,SEVEN,EIGHT,NINE,
	A,B,C,D,E,F,DP,0x00};
	// arguments = a/A<=character<=f/F or character='.'
	// decode_array = {0,1,2,3,4,5,6,7,8,9,A,B,C,D,E,F,.,OTHERS}	
	return char_array[value];
}

void send2display(uchar value)
{	
	static boolean SSD_high = false;
	if(SSD_high^0x1)
	{
		LATB = set_SSD_Hex(LATB,hex_char_decode(value & 0xF),SSD_high);
	}
	else
	{
		LATB = set_SSD_Hex(LATB,hex_char_decode(value >> 4),SSD_high);
	}

	SSD_high ^= 0x1;
}

uchar counter_8bits(uchar value)
{
	
	return (++value);
}

int main(void)
{
	__init__();
	test_7seg();
	int i;
	uchar value = 0;
	while(true)
	{
		i=0;
		do{
			delay(refresh_rate_SSD);
			send2display(value);
		}while(++i < (refresh_counter_rate/refresh_rate_SSD));
		value = counter_8bits(value);
	}
	return 0;
}
