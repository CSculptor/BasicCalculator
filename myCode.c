
/* STILL THE MULTIPLICATION AND THE DIVISION OPERATIONS */
#include <p18f452.h>
#pragma config WDT = OFF

#define		rs		PORTCbits.RC0
#define 	rw 		PORTCbits.RC1
#define 	en		PORTCbits.RC2
#define 	max 	9
#define 	size 	16
#define 	length  3
#define 	size_t 	2
#define 	len     10000

unsigned char string[max];
unsigned char error[] = "__ERROR__";


#pragma idata access state = 0x000
near unsigned char state = 0;
near unsigned char j = 0;
near unsigned char counter = 0;

void initiaLcd(void);				// INITIALIZATION O LCD
void delay250ms(void);				// DELAY OF 250 MS
void delay3us(void);			
void commandInst(void);
void busyFlag(void);				
void dataInst(void);				
void checkNumber(void);
void secondLine(void);
void clearEntire(void);
void justDisplayError(void);
void clearSecondLine(void);
void maxRight(unsigned char  mark);
char findOperator(void);
void displayResult(unsigned char state, unsigned char mark, unsigned int result);

#pragma interrupt myFunction
void myFunction(void)
{
	if(INTCONbits.RBIF == 1)
	{
		PORTB = PORTB;	
		if(j >= max-2)
		{
			if(PORTAbits.RA3 == 0)
			{
				if(PORTBbits.RB4 == 0)
					clearEntire();
			}
			else if(PORTAbits.RA1 == 0)
			{
				if(PORTBbits.RB4 == 0)
				{
					LATD = '=';
					string[j++] = '=';
					dataInst();
					busyFlag();	
					checkNumber();
				}
			}
		}
		else
		{
			if(PORTAbits.RA0 == 0)
			{
				if(PORTBbits.RB4 == 0)
				{
					LATD = 0x2B;
					string[j++] = LATD;
					dataInst();
					busyFlag();	
					++counter;
				}
				else if(PORTBbits.RB5 == 0)	
				{
					LATD = 0x2D;
					string[j++] = LATD;
					dataInst();
					busyFlag();
					++counter;	
				}
				else if(PORTBbits.RB6 == 0)
				{
					LATD = 0x2A;
					string[j++] = LATD;
					dataInst();
					busyFlag();	
					++counter;
				}
				else if(PORTBbits.RB7 == 0)
				{
					LATD = 0x2F;
					string[j++] = LATD;
					dataInst();
					busyFlag();
					++counter;	
				}
			}
			else if(PORTAbits.RA1 == 0)
			{
				if(PORTBbits.RB4 == 0)
				{
					LATD = '=';
					string[j++] = '=';
					dataInst();
					busyFlag();	
					checkNumber();
				}
				else if(PORTBbits.RB5 == 0)	
				{
					LATD = '3';
					string[j++] = '3';
					dataInst();
					busyFlag();
				}
				else if(PORTBbits.RB6 == 0)
				{
					LATD = '6';
					string[j++] = '6';
					dataInst();
					busyFlag();
				}
				else if(PORTBbits.RB7 == 0)
				{
					LATD = '9';
					string[j++] = '9';
					dataInst();
					busyFlag();
				}
			}
			else if(PORTAbits.RA2 == 0)
			{
				if(PORTBbits.RB4 == 0)
				{
					LATD = '0';
					string[j++] = '0';
					dataInst();
					busyFlag();
				}
				else if(PORTBbits.RB5 == 0)	
				{
					LATD = '2';
					string[j++] = '2';
					dataInst();
					busyFlag();
				}
				else if(PORTBbits.RB6 == 0)
				{
					LATD = '5';
					string[j++] = '5';;
					dataInst();
					busyFlag();
				}
				else if(PORTBbits.RB7 == 0)
				{
					LATD = '8';
					string[j++] = '8';
					dataInst();
					busyFlag();
				}
			}
			else if(PORTAbits.RA3 == 0)
			{
				if(PORTBbits.RB4 == 0)
					clearEntire();
				else if(PORTBbits.RB5 == 0)	
				{
					LATD = '1';
					string[j++] = '1';
					dataInst();
					busyFlag();
				}
				else if(PORTBbits.RB6 == 0)
				{
					LATD = '4';
					string[j++] = '4';
					dataInst();
					busyFlag();
				}
				else if(PORTBbits.RB7 == 0)
				{
					LATD = '7';
					string[j++] = '7';
					dataInst();
					busyFlag();
				}
			}	
		}
		INTCONbits.RBIF = 0;
	}
	else if(PIR1bits.TMR1IF == 1)		
	{
		PIR1bits.TMR1IF = 0;
		TMR1H = 0x3C;
		TMR1L = 0xB0;
		switch(state)
		{
			case 0 :
				state = 1;
				LATA = 0x7E;
				break;
			case 1:
				state = 2;
				LATA = 0x7D;
				break;
			case 2:
				state = 3;
				LATA = 0x7B;
				break;
			case 3:
				state = 0;
				LATA = 0x77;
				break;
		}
	}
}

/* HIGH LEVEL INTERRUPT */
#pragma code myInterruptVector = 0x00008
void myInterruptVector(void)
{
	_asm
		GOTO myFunction
	_endasm
}
#pragma code


/* MAIN FUNCTION */
void main(void)
{
		TRISB = 0xF1;			
		TRISD = 0x00;		// PORTD TOTALY OUTPUT
		TRISC = 0xF8;
		TRISA = 0x70;
		ADCON1 = 0x07;	
		initiaLcd();
		INTCONbits.GIE = 1;				// GLOBAL INTERRUPT BIT IS ENABLED
		INTCONbits.PEIE = 1;			// PERIPHERALS INTERRUPTS
		INTCONbits.RBIE = 1;			// PORTB CHANGE INTERRUPT
		PIE1bits.TMR1IE = 1;			// TIMER1 INTERRUPT
		PIR1bits.TMR1IF = 0;	
		INTCONbits.RBIF = 0;
		T1CON = 0x14;
		TMR1H = 0x3C;
		TMR1L = 0xB0;
		LATA = 0x7E;
		T1CONbits.TMR1ON = 1;			// START TIMER1
		while(1);						// STAY HERE
}
void initiaLcd(void)
{
		LATD = 0x38;
		commandInst();
		delay250ms();
		LATD = 0x01;
		commandInst();
		delay250ms();
		LATD = 0x0F;
		commandInst();
		delay250ms();
}
void delay250ms(void)
{
		T0CON = 0x01;
		TMR0H = 0x0B;
		TMR0L = 0xBC;
		INTCONbits.TMR0IF = 0;
		T0CONbits.TMR0ON = 1;
		while(INTCONbits.TMR0IF == 0);
		INTCONbits.TMR0IF = 0;
		T0CONbits.TMR0ON = 0;	
}
void delay3us(void)
{
		T0CON = 0x48;
		TMR0L = 253;
		INTCONbits.TMR0IF = 0;
		T0CONbits.TMR0ON = 1;
		while(INTCONbits.TMR0IF == 0);
		INTCONbits.TMR0IF = 0;
		T0CONbits.TMR0ON = 0;	
}
void commandInst(void)
{
		rs = 0;
		rw = 0;
		en = 1;
		delay3us();
		en = 0;
}
void dataInst(void)
{
		rs = 1;
		rw = 0;
		en = 1;
		delay3us();
		en = 0;
}
void busyFlag(void)
{
		rs = 0;
		rw = 1;	
		TRISDbits.TRISD7 = 1;
		do
		{
			en = 0;
			delay3us();
			en = 1;
		}while(PORTDbits.RD7 == 1);
		en = 0;
		TRISDbits.TRISD7 = 0;
}
void checkNumber(void)	
{
	if(counter >= 2)
		justDisplayError();				// DISPLAY ERROR INCASE THERE ARE MORE THEN ONE OPERATOR IN THE OPERATION
	else
	{
		char operator = findOperator();			// FIND THE OPERATOR
		unsigned int number[length], value = 0, result = 0;
		unsigned char i = 0, p = 0, state = 0, mark = 0, etat = 0,  k, a;
		while(string[i] != '\0')
		{
			if(string[i] != '+' && string[i] != '-' && string[i] != '/' && string[i] != '*' && string[i] != '=') // AS LONG AS, EXECUTE THE BODY
				number[p++] = (string[i]-0x30);		// COVERT THE CHARACTER TO INT NUMBER AND STORES IT INTO NUMBER ARRAY		
			else
			{
				switch(p)							// BASED ON NUMBER OF DIGIT WE CHOOSE THE K 
				{
					case 1 :
						k = 1;
						break;		
					case 2 :
						k = 10;
						break;
					case 3 :
						k = 100;
						break;	
				}
				a = 0;
				while(a<p)
				{
					value += number[a++] * k;				// CREAT THE NUMBER BY MULTIPLYING EACH POSITION WITH ITS BASE 	
					k /= 10;		
				}
				if(value >= 256)						// IF NUMBER GREATER THEN 499 PRINT ERROR
				{				
					state = 1;
					justDisplayError();
					break;
				}
				else
				{
					if(operator == '+')					// IF OPERATION IS ADDITION THEN ADD THE CONTENT
							result += value;
					else if(operator == '-')			// IF OPERATION IS SUBTRACTION THEN MARK IS VARIABLE USED TO MARK THE MINUS SIGN
				 	{
						if(etat == 0)
						{
							etat = 1;				
							result = value-result;		
						}
						else
						{
							if(result < value)			// IF A-B (A<B) THEN (B-A) IS THE RESULT + MARK MAKRS THE MINUS SIGN
							{
								mark = 1;
								value = value-result;
								result = value;
							}
							else
								result -= value;
						}
					}
				}
				value = 0;
				p = 0;
			}		
			++i;
		}
		displayResult(state, mark, result);
	}
}
void secondLine(void)
{
	LATD = 0xC0;
	commandInst();
	busyFlag();
}
void clearEntire(void)
{
	counter = 0;
	LATD = 0x01;
	commandInst();	
	delay250ms();
	j = 0;
	while(j<max)
		string[j++] = '\0';	
	j = 0;
}
void justDisplayError(void)
{
	unsigned char i = 0;
	clearSecondLine();
	while(error[i] != '\0')
	{
		LATD = error[i++];
		dataInst();
		busyFlag();
	}
	delay250ms();
	delay250ms();
	delay250ms();
	delay250ms();
	clearSecondLine();
}
void clearSecondLine(void)
{
	unsigned char i = 0;
	secondLine();
	while(i<size)
	{
		LATD = 0x20;
		dataInst();
		busyFlag();
		++i;
	}
	secondLine();
}
void maxRight(unsigned char  mark)
{
	unsigned char i = 0, limit = 0;
	if(mark == 1)
		limit = size-6;
	else
		limit = size-5;
	while(i<limit)		
	{	
		LATD = 0x14;
		commandInst();
		busyFlag();
		++i;	
	}
	if(mark == 1)
	{
		LATD = '-';
		dataInst();
		busyFlag();
	}
}
char findOperator(void)
{
	unsigned char i = 0;
	while(string[i] != '\0')
	{	
		if(string[i] == '+' || string[i] == '-' || string[i] == '/' || string[i] == '*' )
			return string[i];
		++i;
	}
}
void displayResult(unsigned char state, unsigned char mark, unsigned int result)
{
	if(!state)						
	{
		secondLine();
		maxRight(mark);		
		LATD = (result/len) + 0x30;
		dataInst();		
		busyFlag();
		LATD = ((result%len)/(len/10)) + 0x30;
		dataInst();		
		busyFlag();
		LATD = (((result%len)%(len/10))/(len/100)) + 0x30;
		dataInst();		
		busyFlag();
		LATD = ((((result%len)%(len/10))%(len/100))/(len/1000)) + 0x30;
		dataInst();		
		busyFlag();
		LATD = ((((result%len)%(len/10))%(len/100))%(len/1000)) + 0x30;
		dataInst();		
		busyFlag();
	}
}