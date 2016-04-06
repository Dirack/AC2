#include <detpic32.h>
#include <ptrodrigues.h>

void __init__()
{
	TRISB &= 0xFC00; 			// Set RB0 to RB9 as outputs
	TRISE = (TRISE & 0xFFFF) | 0x00F0;
}
void test_7seg()
{
	printStr("Testing low display\n0x");
	LATB = set_SSD_Hex(LATB,CHECK,false);	 // enable low display
	printInt(LATB,(4<<16)+16);
	putChar('\n');
	delay(1000);
	printStr("Testing high display\n0x");
	LATB = set_SSD_Hex(LATB,CHECK,true);	// enable high display
	printInt(LATB,(4<<16)+16);
	putChar('\n');
	delay(1000);
	LATB &= 0xFC00;				// turn off both displays
}
uchar dip_char_decode(uchar value)
{
	static uchar char_array[] = {
	ZERO,ONE,TWO,THREE,FOUR,FIVE,SIX,SEVEN,EIGHT,NINE,
	A,B,C,D,E,F,DP,0x00};
	// arguments = a/A<=character<=f/F or character='.'
	// decode_array = {0,1,2,3,4,5,6,7,8,9,A,B,C,D,E,F,.,OTHERS}	
	return char_array[value];
}

int main(void)
{
	__init__();
	test_7seg();
	boolean SSD_high = false;
	while(1)
	{
		LATB = set_SSD_Hex(LATB,dip_char_decode((PORTE & 0xF0)>>4),SSD_high);
		if((LATB & 0x3F) == F)
		{
			SSD_high ^= 1;
			while((PORTE & 0xF0)>>4 == 0xF);
		}
	}
	return 0;
}
