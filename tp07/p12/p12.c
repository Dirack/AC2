#include <detpic32.h>
#include <ptrodrigues.h>

#define uart_baud 115200// UART1 baudrate
#define uart_par 'N'	// UART1 parity
#define uart_dbits 8	// UART1 data bits
#define uart_sbits 1	// UART1 stop bits

#define FREQT1 4	// ADC refresh rate
#define FREQT3 100 	// SSD refresh rate
#define preT1 3
#define preT3 2
#define ADCSAMPLES 8

#define T1_IP 1
#define T3_IP 4
#define UART1_IP 2
#define AD1_IP 3

volatile uint ADCvalue = 0;
volatile uint voltMin = 0x3FF;
volatile uint voltMax = 0x0;
volatile boolean SSD_status = true;

void putc(char byte2send)
{
	while(U1STAbits.UTXBF == 1);
	U1TXREG = byte2send;
}

void puts(char *str)
{
	do{
		putc(*str);
	}while(*++str != '\0');
}

char getc(void)
{
	if(U1STAbits.OERR)
		U1STAbits.OERR = 0;
	while(!U1STAbits.URXDA);
	if((U1STA & 0xC) != 0)
	{
		U1RXREG;
		return 0;
	}
	return U1RXREG;
}

char UART_decodeParity(char parity)
{
	switch(parity)
	{
		case 'N':
			return	0x0;
		case 'E':
			return 	0x1;
		case 'O':
			return	0x2;
		default:
			return 0xFF;
	}
}

boolean UART_checkArgs(uint baud, char parity, uint stopbits)
{
	boolean check = true;
	if(baud < 600 || baud > 115200)
	{
		puts("\nUART1 ERROR: 600 < Baudrate > 115200");
		check = false;
	}
	if(UART_decodeParity(parity) == -1)
	{	puts("\nUART1 ERROR: Parity: 'N', 'E', 'O'");
		check = false;
	}
	if(stopbits > 2)
	{
		puts("\nUART1 ERROR: Stop bits: 1/2");
		check = false;
	}
	putc('\n');
	return check;
}

void UART1_PrintCfg(uint baud, char parity, uint stopbits)
{
	U1STAbits.UTXBRK = 1;
	puts("//////// UART1 configuration ////////\nBaudrate: ");
	printInt10(baud);
	puts("\nParity: ");
	putc(parity);
	puts("\nStop bits: ");
	printInt10(stopbits);
	puts("\n/////////////////////////////////////\n");
}

void configUart(uint baud, char parity, uint stopbits)
{
	if(!UART_checkArgs(baud, parity, stopbits))
	{
		baud = 115200;
		parity = 'N';
		stopbits = 1;
	}
	
	U1BRG = ((PBCLK + 8 * baud) / (16 * baud)) - 1; // gerador baudrate
	U1MODEbits.BRGH = 0;
	U1MODEbits.PDSEL = UART_decodeParity(parity);
	U1MODEbits.STSEL = stopbits-1;
	U1STAbits.UTXEN = 1;
	U1STAbits.URXEN = 1;
	U1MODEbits.ON = 1;
	U1STAbits.URXISEL = 0;
	IFS0bits.U1RXIF = 0;
	IEC0bits.U1RXIE = 1;
	IEC0bits.U1EIE = 1;
	IPC6bits.U1IP = UART1_IP;

	UART1_PrintCfg(baud, parity, stopbits);
}

void _init_(uint nSamples)
{	
	U1STAbits.UTXBRK = 1;
	configUart(uart_baud,uart_par,uart_sbits);
	init_adc14(nSamples);
	TRISB &= 0xFC00;
	TRISE &= 0xFFFF;
	static uint preSclA[] = {1,2,64,256};
	static uint preSclB[] = {1,2,4,8,16,32,64,256};
	T1CONbits.TCKPS = preT1;
	PR1 = ((PBCLK/preSclA[preT1])/FREQT1)-1;
	TMR1 = 0;
	T1CONbits.TON = 1;
	IFS0bits.T1IF = 0;
	IEC0bits.T1IE = 1;
	IPC1bits.T1IP = T1_IP;

	T3CONbits.TCKPS = preT3;
	PR3 = ((PBCLK/preSclB[preT3])/FREQT3)-1;
	TMR3 = 0;
	T3CONbits.TON = 1;
	IFS0bits.T3IF = 0;
	IEC0bits.T3IE = 1;
	IPC3bits.T3IP = T3_IP;

	IFS1bits.AD1IF = 0;
	IEC1bits.AD1IE = 1;
	IPC6bits.AD1IP = AD1_IP;

	OC1CONbits.OCM = 6;
	OC1CONbits.OCTSEL = 1;
	OC1RS = 0;
	OC1CONbits.ON = 1;
}

uchar toBCD(uchar character)
{
	return (((character/10)<<4) + (character % 10));
}

void voltagetoASCII(char *voltage, uint value)
{
	//fills char buffer with voltage in ASCII: "3.3V\n\0"
	uchar adc_BCD = toBCD(value);
	voltage[0] = (adc_BCD >> 4) + '0'; 
	voltage[1] = '.';
	voltage[2] = (adc_BCD & 0xF) + '0';
	voltage[3] = 'V';
	voltage[4] = '\n';
	voltage[5] = '\0';
}

void send2SSD(uchar character)
{
	static uchar char_array[] = {
	ZERO,ONE,TWO,THREE,FOUR,FIVE,SIX,SEVEN,EIGHT,NINE,
	A,B,C,D,E,F,DP,0x00};
	static boolean high_display = false;

	if(high_display^1)
		character = char_array[character & 0xF];
	else
		character = char_array[character >> 4] | dp;

	PORTB = set_SSD_Hex(PORTB, character, high_display);
	high_display ^= 1;
}

void send2UART_adc()
{
	char v_buffer[6];
	puts("Current voltage: ");
	voltagetoASCII(v_buffer, ADCvalue);
	puts(v_buffer);
}

void send2UART_maxmin()
{
	char v_buffer[6];
	puts("Min Voltage: ");
	voltagetoASCII(v_buffer, voltMin);
	puts(v_buffer);
	puts("Max Voltage: ");
	voltagetoASCII(v_buffer, voltMax);
	puts(v_buffer);
}

void setPWM(uint DutyCycle)
{
	if(!(DutyCycle < 0 || DutyCycle > 100))
	{
		OC1RS = ((PR3+1) * DutyCycle) / 100;
	}
}

int main(void)
{
	uchar dip_status;
	static uchar led_brightness[] = {3, 15, 40, 90};
	_init_(ADCSAMPLES);
	EnableInterrupts();
	while(true)
	{
		dip_status = ((PORTE >> 4) & 0x3);
		if((dip_status >> 1)^0x1)
		{
			SSD_status = true;
			if(dip_status)
			{
				IEC0bits.T1IE = 0;
				setPWM(100);
			}
			else
			{
				IEC0bits.T1IE = 1;
				setPWM(0);
			}
		}
		else
		{
			SSD_status = false;
			if(dip_status & 0x1) // 11
			{
				// do nothing
			}
			else
			{
				IEC0bits.T1IE = 0;
				setPWM(led_brightness[((PORTE >> 6) & 0x3)]);
			}
		}
	}
	return 0;
}

void _int_(4) isr_T1(void)
{
	AD1CON1bits.ASAM = 1;
	IFS0bits.T1IF = 0;
}

void _int_(12) isr_T3(void)
{
	static uint counter = 0;
	if(SSD_status)
	{
		send2SSD(toBCD(ADCvalue));
		if(++counter == 100)
		{
			counter = 0;
			if(U1STAbits.TRMT == 1)
				send2UART_adc();
		}
	}
	else
		LATB = LATB & 0xFCFF;
	IFS0bits.T3IF = 0;
}

void _int_(24) isr_uart1(void)
{
	if(IFS0bits.U1EIF)
	{
		printStr("ERROR\n");
		if(U1STAbits.OERR)
			U1STAbits.OERR = 0;
		else
		{
			if((U1STA & 0xC) != 0)
				U1RXREG;
		}
		IFS0bits.U1EIF = 0;
	}
	else if(IFS0bits.U1RXIF)
	{	if(U1STAbits.TRMT == 1)
		{
			if(U1RXREG == 'L')
				send2UART_maxmin();
		}
	}
	
	IFS0bits.U1RXIF = 0;
}

void _int_(27) isr_adc(void)
{
	uint mean = 0;
	uint *ADCPOINTER  = (uint*)&ADC1BUF0;
	int j;
	for(j=0; j < ADCSAMPLES; j++, ADCPOINTER = ADCPOINTER + 4)
	{
		mean += *ADCPOINTER;
	}
	ADCvalue = ((mean / ADCSAMPLES)*33)/1023;
	if(ADCvalue > voltMax)
		voltMax = ADCvalue;
	if(ADCvalue < voltMin)
		voltMin = ADCvalue;
	IFS1bits.AD1IF = 0;
}
