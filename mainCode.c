#include <p18f452.h>
#pragma config WDT = OFF

#define		rs		PORTCbits.RC0
#define 	rw 		PORTCbits.RC1
#define 	en		PORTCbits.RC2
#define 	MAX 	20
#define 	LINE    16

unsigned char string[MAX], j = 0, state = 0, stack[MAX], postFix[MAX], k = 0;
signed float stackSecond[MAX];
signed char TOP = -1;

void initiaLcd(void);			
void delay250ms(void);		
void delay3us(void);			
void commandInst(void);
void busyFlag(void);				
void dataInst(void);				
void checkNumber(void);
void timer0Generation(void);
void checkInput(void);
void clearScreen(void);
void push(unsigned char value);
unsigned char pop(void);
void displayError(void);
void secondLine(void);
void clearSecondLine(void);
void firstLine(void);
void shiftCursor(void);
void convertInfixToPostfix(void);
void testOperator(unsigned char value);
void operandToList(unsigned char value);
signed float calculatePostfix(void);

#pragma interrupt myFunction
void myFunction(void)
{
	if(INTCONbits.RBIF == 1)
	{
		PORTB = PORTB;	
		if(PORTAbits.RA0 == 0)
		{
			if(PORTBbits.RB4 == 0)
			{
				LATD = 0x2B;
				string[j++] = LATD;
				dataInst();
				busyFlag();	
			}
			else if(PORTBbits.RB5 == 0)	
			{
				LATD = 0x2D;
				string[j++] = LATD;
				dataInst();
				busyFlag();
			}
			else if(PORTBbits.RB6 == 0)
			{
				LATD = 0x2A;
				string[j++] = LATD;	
				dataInst();
				busyFlag();	
			}
			else if(PORTBbits.RB7 == 0)
			{
				LATD = 0x2F;
				string[j++] = LATD;
				dataInst();
				busyFlag();
			}
		}
		else if(PORTAbits.RA1 == 0)
		{
			if(PORTBbits.RB4 == 0)
				checkInput();
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
				string[j++] = '5';
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
				clearScreen();
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
		INTCONbits.RBIF = 0;	
	}
	else if(INTCONbits.INT0IF == 1)
	{
		INTCONbits.INT0IF = 0;
		LATD = '(';
		string[j++] = '(';
		dataInst();
		busyFlag();
	}
	else if(INTCON3bits.INT1IF == 1)
	{
		INTCON3bits.INT1IF = 0;
		LATD = ')';
		string[j++] = ')';
		dataInst();
		busyFlag();
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
		TRISB = 0xF0;			
		TRISD = 0x00;		// PORTD TOTALY OUTPUT
		TRISC = 0xF8;
		TRISA = 0x70;
		ADCON1 = 0x07;	
		initiaLcd();
		INTCONbits.GIE = 1;				// GLOBAL INTERRUPT BIT IS ENABLED
		INTCONbits.PEIE = 1;			// PERIPHERALS INTERRUPTS
		INTCONbits.RBIE = 1;			// PORTB CHANGE INTERRUPT
		PIE1bits.TMR1IE = 1;			// TIMER1 INTERRUPT
		INTCONbits.INT0IE = 1;
		INTCON3bits.INT1IE = 1;
		INTCON3bits.INT1IF = 0;
		INTCONbits.INT0IF = 0;
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
		timer0Generation();		
}
void delay3us(void)
{
		T0CON = 0x48;
		TMR0L = 253;
		timer0Generation();
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
void timer0Generation(void)
{
	INTCONbits.TMR0IF = 0;
	T0CONbits.TMR0ON = 1;
	while(INTCONbits.TMR0IF == 0);
	INTCONbits.TMR0IF = 0;
	T0CONbits.TMR0ON = 0;	
}
void clearScreen(void)
{
	LATD = 0x01;
	commandInst();
	delay250ms();
}
void checkInput(void)
{
	unsigned char i = 0, flag = 1, value = 0;
	while(string[i] != '\0')
	{
		if(string[i] == '(')
			push(string[i]);
		else if(string[i] == ')')
		{
			if(TOP == -1)
			{
				flag = 0;
				break;
			}
			else
			{	
				value = pop();
				if(value == ')' && string[i] == ')')				
					flag = 0;
			}
		}
		++i;
	}
	if(TOP >= 0)
		flag = 0;
	if(flag == 0)
		displayError();
	if(flag)
	{
		signed float number = 0;
		signed int valeur = 0;
		convertInfixToPostfix();
		secondLine();
 		number = calculatePostfix();
		valeur = (signed int)number;
		LATD = valeur + 0x30;
		dataInst();
		busyFlag();
		LATD = '.';
		dataInst();
		busyFlag();
		number = (number - valeur) * 10;
		LATD  = ((signed int) number) + 0x30;
		dataInst();
		busyFlag();
	}
}
void push(unsigned char value)
{
	stack[++TOP] = value;
}
unsigned char pop(void)
{
	return stack[TOP--];
}
void displayError(void)
{
	unsigned char error[] = "_ERROR_", i = 0;
	secondLine();
	while(error[i] != '\0')
	{
		LATD = error[i++];
		dataInst();
		busyFlag();
	}
	delay250ms();
	delay250ms();
	clearSecondLine();
}
void secondLine(void)
{
	LATD = 0xC0;
	commandInst();
	busyFlag();
}
void clearSecondLine(void)
{
	unsigned char i = 0;
	secondLine();
	while(i<LINE)	
	{	
		LATD = 0x20;
		dataInst();
		busyFlag();
		++i;
	}
	firstLine();
	i = 0;		
	while(i<j)
	{
		shiftCursor();
		++i;
	}
}
void firstLine(void)
{
	LATD = 0x80;
	commandInst();
	busyFlag();
}
void shiftCursor(void)
{
	LATD = 0x14;
	commandInst();
	busyFlag();
}
void convertInfixToPostfix(void)
{
	unsigned char i = 0;
	TOP = -1;
	while(string[i] != '\0')
	{
		if(string[i] == '(' || string[i] == ')' || string[i] == '+' || string[i] == '-' || string[i] == '*' || string[i] == '/')
			testOperator(string[i]);
		else
			operandToList(string[i]);	
		++i;
	}
	while(TOP != -1)
		operandToList(pop());
}
void testOperator(unsigned char value)
{
	if(value == '(')
		stack[++TOP] = value;
	else if(value == '*' || value == '/')
	{		
		while(stack[TOP] != '+' && stack[TOP] != '-' && stack[TOP] != '(' && TOP != -1)
			operandToList(pop());
		stack[++TOP] = value;
	}
	else if(value == '+' || value == '-')
	{
		while(stack[TOP] != '(' && TOP != -1)
			operandToList(pop());
		stack[++TOP] = value;
	}
	else if(value == ')')	
	{
		while(stack[TOP] != '(')
			operandToList(pop());
		--TOP;
	}
}
void operandToList(unsigned char value)
{
	postFix[k++] = value;	
}
signed float calculatePostfix(void)
{
	unsigned char i = 0;
	signed float value_1 = 0, value_2 = 0;
	TOP = -1;
	while(postFix[i] != '\0')
	{
		if(postFix[i] != '+' && postFix[i] != '-' && postFix[i] != '/' && postFix[i] != '*')
			stackSecond[++TOP] = (float)(postFix[i]-0x30);
		else
		{
			value_1 = stackSecond[TOP--];
			value_2 = stackSecond[TOP--];
			switch(postFix[i])
			{
				case '+' :
					stackSecond[++TOP] = value_2 + value_1;
					break;
				case '-' :
					stackSecond[++TOP] = value_2 - value_1;
					break;
				case '/' :
					stackSecond[++TOP] = value_2 / value_1;
					break;
				case '*' :
					stackSecond[++TOP] = value_2 * value_1;
					break;
			}
		}
		++i;
	}
	return stackSecond[TOP--];
}