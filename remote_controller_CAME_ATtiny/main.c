/*
 * Created: 08.01.2021 21:20:14
 * Author : Andriu
 * �������� ��� ������� � 12 ������� ������� ����� ��� ������������ � ���������� CAME
 * ���������� �������� ���������� 4096, �� 0 �� 4095.
 */ 
# define F_CPU 1000000UL //������� ����������

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define start_delay 11520 //��������� �������� �� ��������� ��������� 11520���
#define one_stop 320 //�� ��������� ��������� �������� ������ ���� 320���
#define two_stop 640 //�� ��������� ��������� �������� ������ ���� 640���


int key = 546; //��� ������

int maska = 1; //������ ����� ������� ������� key �� ���� � ��������� ��� � ������ massiv
char massiv [12] = {0,0,0,0,0,0,0,0,0,0,0,0};
int a = 0; //�������������� ����������, �������� ���������� ���� ����� ��� �������� � ����
char z = 0;
	
	void stop_comparator (void); //������� ��������� ���������� ����������
	//void TC0_setup (void); //������� ��������� ������� ��������
	int razbivka (int z); //������� ��������� ��� ������ ����� maska ������ ��� ���������� z 
	void setup_ports (void);  //������� ��������� ��������� ������ ����������
	//void setup_mk (void); //������� �������� ���������� ����������������
	void one (void); //������� ������ ������� �������� 1 � ����������
	void zero (void); //������� ������ ������� �������� 0 � ����������
	//void test_frequency (void); //�������� ������� ���������� 
	void key_out (void); //������� ������ ����� � ����
	
// 	ISR (TIM0_COMPA_vect)
// 	{
// 		z = 1;
// 	}

int main(void) //������ ������� main
{
    //TC0_setup();
	setup_ports(); //�������� ������ ����������
	
	stop_comparator(); //��������� ���������� ����������
	
	for (int i = 11; i >= 0; --i) //� ����� ��������� ����� ������������ � ���������� key �� ��������� ���� � �������� ��� � ������ massiv
	{
		massiv[i] = razbivka(key);
		key = key/2;
		
	}
	
    while (1) // ������ ��������� ����� while
    {
		//test_frequency();
		
		if (~PINB & (1<<PB0)) //�������� ������� �������
		{
			PORTB |= (1<<PB2); //��������� ������������ ������ ������
			
			PORTB |= (1<<PB1);
			_delay_ms(5);
			PORTB &= ~(1<<PB1);
			_delay_us(start_delay); //�������� ������
			
// 			while(~PINB & (1<<PB0)) //��������� ���� while. ���� ������ ������ ������ �������� �������� ����� � ����
// 			{
	           
				
				PORTB |= (1<<PB1); //��������� ���
				_delay_us(one_stop);
				PORTB &= ~(1<<PB1);
				
				key_out();
				
				/*
					while(~PINB & (1<<PB0)) //������� ���� while. ���� ������ ������ ������ ������ � ���� 12 ������ �����
					{
						while (a<=12)
						{
							if (0 == (massiv[a] & (1<<0)))
							{
								zero();
							}
							
							else
							{
								one();
							}
							
							a++;
						}
						
						if (a>=12) break;
						
					} //����� �������� ����� while
	
					//if (a>=12) break;
	
			//} //����� ���������� ����� while  
			
			a = 0;
			*/
			PORTB &= ~(1<<PB1);
			_delay_ms(2);
			
		} //����� �������� ������� �������
		
		PORTB &= ~(1<<PB2); //��������� ��������� ������ ������
		
    } //����� ��������� ����� while
	
} //����� ������� main

void setup_ports(void) //������� ��������� ��������� ������ ����������
{
	PORTB = 0b00000;
	DDRB = 0b00110;
}

// void setup_mk (void) //������� �������� ���������� ����������������
// {
// 	
// }

void stop_comparator (void) //������� ��������� ���������� ����������
{
	ACSR |= (1<<ACD);
}

int razbivka (int z) // ������� ��������� ��� ������ ����� maska ������ ��� ���������� z
{
	z = z & maska;
	
	return z;
}

void zero (void) //������� ������ ������� �������� 0 � ����������
{
	PORTB &= ~(1<<PB1); //������ ��� "0"
	_delay_us(one_stop);
	PORTB |= (1<<PB1);
	_delay_us(one_stop);
	_delay_us(one_stop);
	//_delay_us(two_stop);
	
}

void one (void) //������� ������ ������� �������� 1 � ����������
{
	PORTB &= ~(1<<PB1); //������ ��� "1"
	//_delay_us(two_stop);
	_delay_us(one_stop);
	_delay_us(one_stop);
	PORTB |= (1<<PB1);
	_delay_us(one_stop);
	
}

// void test_frequency (void) //�������� ������� ����������
// {
// 	PORTB ^= (1<<PB2);
// 	_delay_ms(1000);
// }

// void TC0_setup (void) //������� ��������� ������� ��������
// {
// 	TCCR0A |= (1<<WGM01);
// 	TCCR0B &= ~((1<<CS02)|(1<<CS00));
// 	TCCR0B |= (1<<CS01);
// 	OCR0A = 0xFF;
// 	
// 	GTCCR |= (1<<TSM)|(1<<PSR0);
// 	TCNT0 = 0;
// 	GTCCR &= ~((1<<TSM) | (1<<PSR0));
// }

void key_out (void)
{
	massiv[0] == 0 ? zero() : one();
	massiv[1] == 0 ? zero() : one();
	massiv[2] == 0 ? zero() : one();
	massiv[3] == 0 ? zero() : one();
	massiv[4] == 0 ? zero() : one();
	massiv[5] == 0 ? zero() : one();
	massiv[6] == 0 ? zero() : one();
	massiv[7] == 0 ? zero() : one();
	massiv[8] == 0 ? zero() : one();
	massiv[9] == 0 ? zero() : one();
	massiv[10] == 0 ? zero() : one();
	massiv[11] == 0 ? zero() : one();
}
