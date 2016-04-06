#include <detpic32.h>
#define CALIBRATION_VALUE 4998

typedef enum{false,true} boolean;
typedef unsigned int uint;
typedef unsigned char uchar;
enum SSD_segments{
        b = 0x01,
        a = 0x02,
        f = 0x04,
        g = 0x08,
        e = 0x10,
        d = 0x20,
        c = 0x40,
        dp= 0x80
};

void delay(uint n_intervals)
{
        volatile uint i;

        for(; n_intervals != 0; n_intervals--)
                for(i = CALIBRATION_VALUE; i != 0; i--)
                ;
}


void _init_(void)
{
	TRISB = TRISB & 0xFC00;
	LATB = (LATB & 0xFC00);
}

void send2SSD(uchar value)
{
	LATB = ((LATB & 0xFC00) | 0x100 ) | value;
}

int main(void)
{
	static uchar segments[]={f,a,b,g,e,d,c,g};
	_init_();
	int j = 0;
	while(true)
	{
		send2SSD(segments[j]);
		delay(250);
		if(++j == 8)
			j = 0;
	}

	return 0;
}
