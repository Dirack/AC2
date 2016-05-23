#include <detpic32.h>
#define	adc_samples 1
#define T1_FREQ 4
#define T1_PS 3
#define T2_FREQ 165
#define T2_PS 1
#define uart1_baud 115200
#define MASK_adc_hist 0x3
#define DisableADC() IEC0bits.T1IE = 0
#define EnableADC() IEC0bits.T1IE = 1

typedef enum{false, true}boolean;
typedef unsigned int uint;
typedef unsigned char uchar;
typedef struct {
	uint	data[4];
	uint	pointer; 
	uint 	adc_max;
	uint	adc_value;
}buffer;

enum _SSD_segments{
	b = 0x01,
	a = 0x02,
	f = 0x04,
	g = 0x08,
	e = 0x10,
	d = 0x20,
	c = 0x40,
	dp= 0x80
};

enum _SSD_hex{
	A = a|b|c|e|f|g,
	B = c|d|e|f|g,
	C = a|d|e|f,
	D = b|c|d|e|g,
	E = a|d|e|f|g,
	F = a|e|f|g,
	DP= dp,
	ZERO = C|b|c,
	ONE  = b|c,
	TWO  = a|b|d|e|g,
	THREE= ONE|a|d|g,
	FOUR = ONE|f|g,
	FIVE = a|c|d|f|g,
	SIX  = B|a,
	SEVEN= ONE|a,
	EIGHT= ZERO|g,
	NINE = FIVE|b,
	CHECK= EIGHT|dp
};

volatile char state = 0;
volatile buffer adc_hist;

void init_port(void)
{
	TRISE &= 0xFFF0;
	TRISB &= 0xFC00;
}
void init_adc(n_samples)
{
	AD1PCFGbits.PCFG14 = 0;
	AD1CON1bits.SSRC = 7;
	AD1CON1bits.CLRASAM = 1;
	AD1CON3bits.SAMC = 16;
	AD1CON2bits.SMPI = 0;
	AD1CHSbits.CH0SA = 14;
	AD1CON1bits.ON = 1;
}
void init_timers(void)
{
	static uint TIMERA_PR[] = {1, 8, 64, 256};
	static uint TIMERB_PR[] = {1, 2, 4, 8, 16, 32, 64, 256};
	//Timer 1
	T1CONbits.TCKPS = T1_PS;
	PR1 = (PBCLK/(TIMERA_PR[T1_PS])/T1_FREQ)-1;
	TMR1 = 0;
	T1CONbits.TON = 1;
	
	//Timer 2
	T2CONbits.TCKPS = T2_PS;
	PR2 = (PBCLK/(TIMERB_PR[T2_PS])/T2_FREQ)-1;
	TMR2 = 0;
	T2CONbits.TON = 1;
}
void init_UART1(void)
{
	U1BRG = ((PBCLK + 8 * uart1_baud) / (16 * uart1_baud)) - 1;
	U1MODEbits.BRGH = 0;
	U1MODEbits.PDSEL = 0;
	U1MODEbits.STSEL = 0;
	U1STAbits.URXEN = 1;
	U1STAbits.UTXEN = 1;
	U1MODEbits.ON = 1;
}
void init_interrupts(void)
{
	//ADC
	IPC6bits.AD1IP = 6;
	IEC1bits.AD1IE = 1;
	IFS1bits.AD1IF = 0;
	
	//TIMER1
	IPC1bits.T1IP = 5;
	IEC0bits.T1IE = 1;
	IFS0bits.T1IF = 0;
	
	//TIMER2
	IPC2bits.T2IP = 4;
	IEC0bits.T2IE = 1;
	IFS0bits.T2IF = 0;
	
	//UART1
	
	U1STAbits.URXISEL = 0;
	IEC0bits.U1RXIE = 1;
	IFS0bits.U1RXIF = 0;
	IEC0bits.U1RXIE = 1;
	IFS0bits.U1RXIF = 0;
	IPC6bits.U1IP = 3;
	
}
uchar tobcd(uint value)
{
	return (((value/10)<<4) + value%10);
}
void send2displays(uint value)
{
	static uchar char_array[] = {ZERO,ONE,TWO,THREE,FOUR,FIVE,SIX,SEVEN,EIGHT,NINE,A,B,C,D,E,F,DP,0x00};
	
	static boolean high_ds = false;
	if(high_ds)
		LATB = (LATB & 0xFC00) | 0x100 | char_array[value >> 4];
	else
		LATB = (LATB & 0xFC00) | 0x200 | char_array[value & 0xF];
	high_ds ^= 1;
}
void putc(char byte2send)
{
	while(U1STAbits.UTXBF);
	U1TXREG = byte2send;
}
void puts(char *str)
{
	while(*str != '\0')
	{
		putc(*str++);
	}
}
char getc(void)
{
	if(U1STAbits.OERR)
		U1STAbits.OERR = 0;
	while(!U1STAbits.URXDA);
	return U1RXREG;
}
void adc_buffer_reset(void)
{
	adc_hist.pointer = 0;
	adc_hist.data[0] = 0;
	adc_hist.data[1] = 0;
	adc_hist.data[2] = 0;
	adc_hist.data[3] = 0;
	adc_hist.adc_max = 0;
	adc_hist.adc_value = 0;
}
int main(void)
{
	adc_hist.pointer = 0;
	init_port();
	init_adc(adc_samples);
	init_timers();
	init_UART1();
	init_interrupts();
	EnableInterrupts();
	
	while(true)
	{
		state = (PORTE >> 4) & 0x3;
	}
	return 0;
}

void _int_(4) isr_T1(void)
{
	AD1CON1bits.ASAM = 1;
	IFS0bits.T1IF = 0;
}

void _int_(8) isr_T2(void)
{
	static uint U1TX_counter = 0;
	static uint value2send = 0;
	static uint cnt_idle = 0;
	++U1TX_counter;
	switch(state)
	{
		case 0:
			value2send = adc_hist.adc_value;
			if((U1TX_counter % 165) == 0)
			{
				puts("Consumo instantaneo: ");
				if(value2send/10 != 0)
					putc((value2send/10)+ '0');
				putc((value2send%10)+ '0');
				puts("l/100km\n");
			}
			break;
		case 1:
			value2send = adc_hist.adc_max;
			if((U1TX_counter % 165) == 0)
			{
				puts("Consumo máximo: ");
				if(value2send/10 != 0)
					putc((value2send/10)+ '0');
				putc((value2send%10)+ '0');
				puts("l/100km\n");
			}
			break;
		case 2:
			value2send = (adc_hist.data[0]+adc_hist.data[1]+adc_hist.data[2]+adc_hist.data[3])/4;
			if((U1TX_counter % 165) == 0)
			{
				puts("Consumo médio: ");
				if(value2send/10 != 0)
					putc((value2send/10)+ '0');
				putc((value2send%10)+ '0');
				puts("l/100km\n");
			}
			break;
		case 3:
			if((U1TX_counter % 165) == 0)
			{
				if(++cnt_idle < 5)
				{
					LATB = (LATB & 0xFC00) | 0x300;
					if((cnt_idle % 2))
						LATB |= dp;
				}
				else
				{
					DisableADC();
					adc_buffer_reset();
					cnt_idle = 0;
					LATB = (LATB & 0xFC00);
					while(((PORTE >> 4) & 0x3) == 3);
					EnableADC();
				}	
			}
			
			break;
		default:
			break;
	}
	if(state != 3)
	{
		uchar led = 0;
		if(0 < adc_hist.adc_value && adc_hist.adc_value <=5)
			led = 0x1;
		else if(0 < adc_hist.adc_value && adc_hist.adc_value <= 10)
			led = 0x3;
		else if(0 < adc_hist.adc_value && adc_hist.adc_value <= 15)
			led = 0x7;
		else if(0 < adc_hist.adc_value)
			led = 0xF;
		
		LATE = (LATE & 0xFFF0) | led;
		send2displays(tobcd(adc_hist.adc_value));
	}
	IFS0bits.T2IF = 0;
}

void _int_(24) isr_UART1(void)
{
	IFS0bits.U1RXIF = 0;
}

void _int_(27) isr_adc(void)
{
	static uint ADC_counter = 0;
	adc_hist.adc_value = (ADC1BUF0 * 20) / 1023;
	if(adc_hist.adc_value > adc_hist.adc_max)
		adc_hist.adc_max = adc_hist.adc_value;
	if((++ADC_counter % 20) == 0)
	{
		adc_hist.data[adc_hist.pointer++] = adc_hist.adc_value;
		adc_hist.pointer &= MASK_adc_hist;
	}
	IFS1bits.AD1IF = 0;
}
