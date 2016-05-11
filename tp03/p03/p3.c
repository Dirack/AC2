#include <detpic32.h>
#include <ptrodrigues.h>

void __init__()
{
	TRISB &= 0xFC00; 			// Set RB0 to RB9 as outputs
}
void test_7seg()
{
	printStr("Testing low display\n0x");
	LATB = set_SSD_Hex(LATB,CHECK,false);	 // enable low display
	printInt(LATB,(4<<16)+16);
	putChar('\n');
	delay(1000);
	//while(readCoreTimer() < (FREQ/2));	// period = 1s
	//resetCoreTimer();
	printStr("Testing high display\n0x");
	LATB = set_SSD_Hex(LATB,CHECK,true);	// enable high display
	printInt(LATB,(4<<16)+16);
	putChar('\n');
	delay(1000);
	//while(readCoreTimer() < (FREQ/2)); 	// period = 1s
	//resetCoreTimer();
	LATB &= 0xFC00;				// turn off both displays
}

uchar segment_decode(uchar character)
{
	static uchar segment_array[] = {a,b,c,d,e,f,g,dp,0x00};
	// arguments = a/A<=character<=g/G or character='.'
	// segment_array = {a,b,c,d,e,f,g,.,OTHERS}	
	if(character >= 'a' && character <= 'g' )
		character -= 'a'-'A';
	
	if(character >= 'A' && character <= 'G')
		return segment_array[character-65];
	else if(character == '.')
		return segment_array[7];
	else
		return segment_array[8];
}

uchar char_decode(uchar character)
{
	static uchar char_array[] = {
	ZERO,ONE,TWO,THREE,FOUR,FIVE,SIX,SEVEN,EIGHT,NINE,
	A,B,C,D,E,F,DP,0x00};
	// arguments = a/A<=character<=f/F or character='.'
	// decode_array = {0,1,2,3,4,5,6,7,8,9,A,B,C,D,E,F,.,OTHERS}	
	if(character >= 'a' && character <= 'g' )
		character -= 'a'-'A';
	
	if(character >= 'A' && character <= 'G')
		return char_array[character-65+10];
	else if(character >= '0' && character <= '9')
		return char_array[character-48];
	else if(character == '.')
		return char_array[16];
	else
		return char_array[17];
}

int main(void)
{
	__init__();
	test_7seg();
	boolean SSD_high = false;
	while(1)
	{			
		LATB = set_SSD_Hex(LATB,segment_decode(getChar()),SSD_high);
		if((LATB & 0xFF) == 0x00)
		{
			SSD_high ^= 0x1;
			LATB = set_SSD_Hex(LATB,segment_decode(0),SSD_high);
		}
		printStr("0x");
		printInt(LATB,(4<<16)+16);
		printStr("; 0b");
		printInt(LATB,(16<<16)+2);
		putChar('\n');
	}
	return 0;
}


