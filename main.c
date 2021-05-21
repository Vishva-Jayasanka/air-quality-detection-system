#define D4 eS_PORTD4
#define D5 eS_PORTD5
#define D6 eS_PORTD6
#define D7 eS_PORTD7
#define RS eS_PORTC6
#define EN eS_PORTC7

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include "ledmatrix7219d88/ledmatrix7219d88.h"
#include "lcd.h" 
#define DHT11_PIN 1
uint8_t c=0,I_RH,D_RH,I_Temp,D_Temp,CheckSum;

uint8_t co2_cols[8] = {0b00000000,
				       0b00000000,
					   0b00000000,
					   0b00000000,
					   0b00000000,
					   0b00000000,
					   0b00000000,
					   0b00000000};
					   
uint8_t temp_cols[8] = {0b00000000,
					    0b00000000,
					    0b00000000,
					    0b00000000,
					    0b00000000,
					    0b00000000,
					    0b00000000,
					    0b00000000};	
						
					   
uint8_t hum_cols[8] = {0b00000000,
					   0b00000000,
					   0b00000000,
					   0b00000000,
					   0b00000000,
					   0b00000000,
					   0b00000000,
					   0b00000000};											   

void matrix (int co2, int temp, int hum) {
	
	uint8_t vals[9] = {0b00000000,
					   0b00000001,
					   0b00000011,
					   0b00000111,
					   0b00001111,
					   0b00011111,
					   0b00111111,
					   0b01111111,
					   0b11111111};
					   
	for(int i = 0; i < 7; i++){
		co2_cols[i] = co2_cols[i + 1];
	}
	
	if(co2 < 9) 
	{
		co2_cols[7] = vals[co2];
	}
	else 
	{
		co2_cols[7] = vals[8];
	}
	ledmatrix7219d88_setmatrix(0, co2_cols);
	
	for(int i = 0; i < 7; i++){
		temp_cols[i] = temp_cols[i + 1];
	}
	if(temp >= 8) 
	{
		temp_cols[7] = vals[8];
	}
	else if(temp < 0)
	{
		temp_cols[7] = vals[0];
	} 
	else 
	{
		temp_cols[7] = vals[temp];
	}
	ledmatrix7219d88_setmatrix(1, temp_cols);
	
	for(int i = 0; i < 7; i++){
		hum_cols[i] = hum_cols[i + 1];
	}
	if(hum < 0)
	{
		hum_cols[7] = vals[0];
	} 
	else 
	{
		hum_cols[7] = vals[hum];	
	}
	ledmatrix7219d88_setmatrix(2, hum_cols);
	
}

void test(){
		Lcd4_Clear();
		Lcd4_Set_Cursor(1,0);
		Lcd4_Write_String("Testing");	
		PORTB = 0x00;
		PORTD &= ~(1 << PD0);
		PORTA &= ~(1 << PA4) & ~(1 << PA5) & ~(1 << PA6);
		PORTA |= 1 << PA5;
		PORTB |= 1 << PB6 | 1 << PB1 | 1 << PB3 | 1 << PB0;
		
		_delay_ms(1000);
		Lcd4_Set_Cursor(1,7);
		Lcd4_Write_String(".");
		PORTA &= ~(1 << PA5);
		PORTB &= ~(1 << PB6) & ~(1 << PB1) & ~(1 << PB3) & ~(1 << PB0);
		PORTD |= 1 << PD0;
		PORTA |= 1 << PA6;
		PORTB |= 1 << PB4;
		
		_delay_ms(1000);
		Lcd4_Set_Cursor(1,8);
		Lcd4_Write_String(".");
		PORTD &= ~(1 << PD0);
		PORTA &= ~(1 << PA6);
		PORTB &= ~(1 << PB4);
		PORTA |= 1 << PA4;
		PORTB |= 1 << PB7 | 1 << PB2 | 1 << PB5 | 1 << PB0;
		
		_delay_ms(1000);
		Lcd4_Set_Cursor(1,9);
		Lcd4_Write_String(".");
		PORTA &= ~(1 << PD4);
		PORTB &= ~(1 << PB7) & ~(1 << PB2) & ~(1 << PB5) & ~(1 << PB0);
		_delay_ms(500);
}

void temp (int temp)
{
	if (temp > 40 || temp < 10){
		PORTA |= 1 << PA4;
		PORTA &= ~(1 << PA5);
		PORTA &= ~(1 << PA6);
	}
	else if (temp >= 20 && temp < 30){
		PORTA |= 1 << PA5;
		PORTA &= ~(1 << PA4);
		PORTA &= ~(1 << PA6);
	}
	else {
		PORTA |= 1 << PA6;
		PORTA &= ~(1 << PA4);
		PORTA &= ~(1 << PA5);
	}
}

void hum (int hum)
{
	if (hum >= 40 && hum <= 50){
		PORTB |= 1 << PB6;
		PORTB &= ~(1 << PB7);
		PORTD &= ~(1 << PD0);
	}
	else if (hum < 30 || hum > 70){
		PORTB |= 1 << PB7;
		PORTB &= ~(1 << PC6);
		PORTD &= ~(1 << PD0);
	}
	else{
		PORTD |= 1 << PD0;
		PORTB &= ~(1 << PB6);
		PORTB &= ~(1 << PB7);
	}
}

void lpgas (int lp)
{
	if (lp){
		PORTB |= 1 << PB1;
		PORTB &= ~(1 << PB2);
	}
	else{
		PORTB |= 1 << PB2;
		PORTB &= ~(1 << PB1);
	}
}

void co2 (int co2)
{
	if (co2 < 500){
		PORTB |= 1 << PB3;
		PORTB &= ~(1 << PB4);
		PORTB &= ~(1 << PB5);
	}
	else if (co2 >= 500 && co2 <= 800){
		PORTB |= 1 << PB4;
		PORTB &= ~(1 << PB3);
		PORTB &= ~(1 << PB5);
	}
	else{
		PORTB |= 1 << PB5;
		PORTB &= ~(1 << PB3);
		PORTB &= ~(1 << PB4);
	}
}

void buzzer(int i, int ppm)
{
	if(i)
	{
		switch (ppm/100){
			case 3:
				PORTB |= 1<<PINB0;
				_delay_ms(1000);
				break;
			case 4:
				PORTB |= 1<<PINB0;
				_delay_ms(900); 
				break;
			case 5:
				PORTB |= 1<<PINB0;
				_delay_ms(800);
				break;
			case 6:
				PORTB |= 1<<PINB0;
				_delay_ms(700);
				break;
			case 7:
				PORTB |= 1<<PINB0;
				_delay_ms(600);
				break;
			case 8:
				PORTB |= 1<<PINB0;
				_delay_ms(500);
				break;
			case 9:
				PORTB |= 1<<PINB0;
				_delay_ms(400);
				break;
			case 10:
				PORTB |= 1<<PINB0;
				_delay_ms(300);
				break;
			case 11:
				PORTB |= 1<<PINB0;
				_delay_ms(200);
				break;
			default:
				PORTB |= 1<<PINB0;
				_delay_ms(100);
				break;
		}
		PORTB &= ~(1<<PINB0);
		_delay_ms(100);
	}
	else
	PORTB &= ~(1<<PINB0);
}

void InitADC()
{
	ADMUX=(1<<REFS0) | (1<<REFS1);                      // For Aref=AVcc;
	ADCSRA=(1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); //Re-scalar div factor =128
}

uint16_t ReadADC(uint8_t ch)
{
	//Select ADC Channel ch must be 0-7
	ch = ch & 0b00000111;
	ADMUX |= ch;

	//Start Single conversion
	ADCSRA|=(1<<ADSC);

	//Wait for conversion to complete
	while(!(ADCSRA & (1<<ADIF)));

	//Clear ADIF by writing one to it
	//Note you may be wondering why we have write one to clear it
	//This is standard way of clearing bits in io as said in data sheets.
	//The code writes '1' but it result in setting bit to '0' !!!

	ADCSRA|=(1<<ADIF);
	uint16_t low = ADCL;
	uint16_t res = ADCH << 8 | low;
	return(res);
}

void Wait()
{
	uint8_t i;
	for(i=0;i<20;i++)
	_delay_loop_2(0);
}

void Request()
{
	DDRA |= (1<<DHT11_PIN);
	PORTA &= ~(1<<DHT11_PIN);
	_delay_ms(20);
	PORTA |= (1<<DHT11_PIN);
}

void Response()
{
	DDRA &= ~(1<<DHT11_PIN);
	while(PINA & (1<<DHT11_PIN));
	while((PINA & (1<<DHT11_PIN))==0);
	while(PINA & (1<<DHT11_PIN));
}

uint8_t Receive_data()
{
	for (int q=0; q<8; q++)
	{
		
		while((PINA & (1<<DHT11_PIN)) == 0);
		_delay_us(30);
		
		if(PINA & (1<<DHT11_PIN))
		c = (c<<1)|(0x01);
		else
		c = (c<<1);
		
		while(PINA & (1<<DHT11_PIN));
		
	}
	return c;
}

int main(void)
{
	_delay_ms(500);
	ledmatrix7219d88_init();
	int apparentTemp[15][21] = {{52, 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 },
								{49, 53, 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 },
								{47, 50, 55, 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 },
								{44, 47, 51, 55, 61, 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 },
								{42, 44, 47, 51, 55, 59, 64, 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 },
								{39, 42, 44, 46, 49, 53, 57, 62, 66, 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 },
								{37, 39, 41, 42, 44, 47, 51, 54, 58, 62, 66, 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 },
								{35, 36, 38, 39, 41, 43, 45, 48, 51, 54, 57, 61, 65, 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 },
								{33, 34, 35, 36, 37, 38, 40, 42, 43, 46, 49, 52, 56, 59, 62, 0 , 0 , 0 , 0 , 0 , 0 },
								{31, 32, 32, 33, 34, 34, 36, 37, 38, 40, 42, 43, 46, 48, 51, 54, 58, 0 , 0 , 0 , 0 },
								{28, 29, 29, 30, 31, 31, 32, 33, 34, 35, 36, 37, 38, 39, 41, 43, 45, 47, 50, 0 , 0 },
								{26, 26, 27, 27, 28, 28, 29, 29, 30, 31, 31, 32, 32, 33, 34, 35, 36, 37, 39, 41, 42},
								{23, 23, 24, 24, 25, 25, 26, 26, 26, 27, 27, 27, 28, 28, 29, 30, 30, 31, 31, 32, 33},
								{21, 21, 21, 22, 22, 22, 23, 23, 23, 23, 24, 24, 24, 24, 25, 25, 26, 26, 26, 26, 27},
								{18, 18, 18, 18, 19, 19, 19, 19, 20, 20, 21, 21, 21, 21, 21, 22, 22, 22, 22, 22, 22}};
	
	DDRD = 0xFF;
	DDRC = 0xFF;
	
	DDRB = 0xFF;
	DDRA &= ~(1 << DDA7);
	DDRA &= ~(1 << DDA2);
	DDRA &= ~(1 << DDA3);
	DDRA |= 1 << PA4 | 1 << PA5 | 1 << PA6;

	Lcd4_Init();
	
	char data[5];
	int adc_result, p = 1, lp, Hbased;
	InitADC();
		
	while(1)
	{
		Request();
		Response();
		I_RH=Receive_data();
		D_RH=Receive_data();
		I_Temp=Receive_data();
		D_Temp=Receive_data();
		CheckSum=Receive_data();
		
		adc_result=ReadADC(0);
		adc_result= (int)((adc_result * 7.94) - 3127.46);
		
		if (lp = (PINA & (1<<PA2)) && I_Temp < 40 && adc_result < 800 && (Hbased = (PINA & (1 <<PA3))))
		{
			if(p==1)
			{
				
				if ((I_RH + D_RH + I_Temp + D_Temp) != CheckSum)
				{
					Lcd4_Set_Cursor(1,0);
					Lcd4_Write_String("Error");
				}
				else
				{
					Lcd4_Clear();
					Lcd4_Set_Cursor(1,0);
					Lcd4_Write_String("Humidity =");
					Lcd4_Set_Cursor(2,0);
					Lcd4_Write_String("Temp = ");
					
					itoa(I_RH,data,10);
					Lcd4_Set_Cursor(1,11);
					Lcd4_Write_String(data);
					Lcd4_Write_String(".");
					
					itoa(D_RH,data,10);
					Lcd4_Write_String(data);
					Lcd4_Write_String("%");
					itoa(I_Temp,data,10);
					
					
					Lcd4_Set_Cursor(2,7);
					Lcd4_Write_String(data);
					Lcd4_Write_String(".");
					
					itoa(D_Temp,data,10);
					Lcd4_Write_String(data);
					Lcd4_Write_String(" C");
					
				}
				p = 2;
			}
			else if(p==2)
			{
				
				int hum = I_RH + D_RH/10.0;
				int tem = (I_Temp + D_Temp/10.0)*1.8 +32;
				itoa(apparentTemp[14 - (tem - 70) / 5][hum / 5], data, 10);
				Lcd4_Clear();
				Lcd4_Write_String("Apparent Temp");
				Lcd4_Set_Cursor(2,0);
				Lcd4_Write_String(data);
				Lcd4_Write_String(" C");
				p = 0;
			}
			else
			{
				
				Lcd4_Clear();
				Lcd4_Write_String("CO2 = ");
				sprintf(data, "%d", adc_result);
				Lcd4_Write_String(data);
				Lcd4_Write_String(" ppm");
				p = 1;
			}
		}
		else
		{
			if(I_Temp >= 40 )
			{
				Lcd4_Clear();
				Lcd4_Write_String("Temperature ");
				Lcd4_Set_Cursor(2,0);
				Lcd4_Write_String("Warning!");
				buzzer(1, 500);
			}
			
			if(!(PINA & (1<<PA2)))
			{
				Lcd4_Clear();
				Lcd4_Write_String("DETECTED");
				Lcd4_Set_Cursor(2,0);
				Lcd4_Write_String("LP GAS");
				buzzer(1, 500);
			}
			
			if(adc_result >= 800)
			{
				Lcd4_Clear();
				Lcd4_Write_String("CO2");
				Lcd4_Set_Cursor(2,0);
				Lcd4_Write_String("Warning!");
				buzzer(1, adc_result);
			}
			if(!(PINA & (1<<PA3))) {
				Lcd4_Clear();
				Lcd4_Write_String("DETECTED");
				Lcd4_Set_Cursor(2,0);
				Lcd4_Write_String("Hydrogen Sulfide");
				buzzer(1, 500);
			}
		}
		
		temp(I_Temp);
		hum(I_RH);
		co2(adc_result);
		lpgas(lp);
		
		if (!(PINA & (1 << PA7)))
		{
			test();
		}
		
		matrix(adc_result / 100, (I_Temp - 10) / 4, (I_RH - 20) / 10);
		
		_delay_ms(1500);
	}
}