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
	printStr("Testing high display\n0x");
	LATB = set_SSD_Hex(LATB,CHECK,true);	// enable high display
	printInt(LATB,(4<<16)+16);
	putChar('\n');
	delay(1000);
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

int main(void)
{
	__init__();
	test_7seg();
	boolean SSD_high = false;
	while(1)
	{
		int i;
		for(i=0; i < 7;i++)
		{
			LATB = set_SSD_Hex(LATB,segment_decode(i+'A'),SSD_high);
			printStr("0x");
			printInt(LATB,(4<<16)+16);
			printStr("; 0b");
			printInt(LATB,(16<<16)+2);
			putChar('\n');
			delay(1000);
		}
	SSD_high ^=1;
	}
	return 0;
}
