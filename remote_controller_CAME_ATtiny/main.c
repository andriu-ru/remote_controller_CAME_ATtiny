/*
 * Created: 08.01.2021 21:20:14
 * Author : Andriu
 * Прошивка для пультов с 12 битными ключами таких как используются в шлагбаумах CAME
 * Колличетво возможны комбмнаций 4096, от 0 до 4095.
 */ 
# define F_CPU 1000000UL //Частота процессора

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define start_delay 11520 //Начальная задержка по стандарту протокола 11520мкс
#define one_stop 320 //по стандарту протокола задержка должна быть 320мкс
#define two_stop 640 //по стандарту протокола задержка должна быть 640мкс


int key = 546; //Код пульта

int maska = 1; //Данная маска поможет разбить key на биты и поместить его в массив massiv
char massiv [12] = {0,0,0,0,0,0,0,0,0,0,0,0};
int a = 0; //дополнительная переменная, содержит количество байт ключа при передаче в эфир
char z = 0;
	
	void stop_comparator (void); //Функция отключает аналоговый компаратор
	//void TC0_setup (void); //Функция настройки таймера счетчика
	int razbivka (int z); //Функция считывает при помощи маски maska первый бит переменной z 
	void setup_ports (void);  //Функция начальной настройки портов процессора
	//void setup_mk (void); //Функция настроки параметров микроконтроллера
	void one (void); //Вызывая данную функцию получаем 1 в интерфейсе
	void zero (void); //Вызывая данную функцию получаем 0 в интерфейсе
	//void test_frequency (void); //Проверка частоты процессора 
	void key_out (void); //Функция выдает ключь в эфир
	
// 	ISR (TIM0_COMPA_vect)
// 	{
// 		z = 1;
// 	}

int main(void) //Начало функции main
{
    //TC0_setup();
	setup_ports(); //Насройка портов процессора
	
	stop_comparator(); //Отключаем аналоговый компаратор
	
	for (int i = 11; i >= 0; --i) //в цикле разбиваем число содержащееся в переменной key на отдельные биты и помещаем его в массив massiv
	{
		massiv[i] = razbivka(key);
		key = key/2;
		
	}
	
    while (1) // Начало основного цикла while
    {
		//test_frequency();
		
		if (~PINB & (1<<PB0)) //Проверка нажатой клавиши
		{
			PORTB |= (1<<PB2); //Светодиод индицирующий работу пульта
			
			PORTB |= (1<<PB1);
			_delay_ms(5);
			PORTB &= ~(1<<PB1);
			_delay_us(start_delay); //Пилотный период
			
// 			while(~PINB & (1<<PB0)) //Стартовый цикл while. Если кнопка пульта нажата начинаем генерить ключь в эфир
// 			{
	           
				
				PORTB |= (1<<PB1); //Стартовый бит
				_delay_us(one_stop);
				PORTB &= ~(1<<PB1);
				
				key_out();
				
				/*
					while(~PINB & (1<<PB0)) //Кодовый цикл while. Если кнопка пульта нажата выдаем в эфир 12 битный ключь
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
						
					} //Конец кодового цикла while
	
					//if (a>=12) break;
	
			//} //Конец стартового цикла while  
			
			a = 0;
			*/
			PORTB &= ~(1<<PB1);
			_delay_ms(2);
			
		} //Конец проверки нажатия клавиши
		
		PORTB &= ~(1<<PB2); //Отключаем индикацию работы пульта
		
    } //Конец основного цикла while
	
} //Конец функции main

void setup_ports(void) //Функция начальной настройки портов процессора
{
	PORTB = 0b00000;
	DDRB = 0b00110;
}

// void setup_mk (void) //Функция настроки параметров микроконтроллера
// {
// 	
// }

void stop_comparator (void) //Функция отключает аналоговый компаратор
{
	ACSR |= (1<<ACD);
}

int razbivka (int z) // Функция считывает при помощи маски maska первый бит переменной z
{
	z = z & maska;
	
	return z;
}

void zero (void) //Вызывая данную функцию получаем 0 в интерфейсе
{
	PORTB &= ~(1<<PB1); //Первый бит "0"
	_delay_us(one_stop);
	PORTB |= (1<<PB1);
	_delay_us(one_stop);
	_delay_us(one_stop);
	//_delay_us(two_stop);
	
}

void one (void) //Вызывая данную функцию получаем 1 в интерфейсе
{
	PORTB &= ~(1<<PB1); //Третий бит "1"
	//_delay_us(two_stop);
	_delay_us(one_stop);
	_delay_us(one_stop);
	PORTB |= (1<<PB1);
	_delay_us(one_stop);
	
}

// void test_frequency (void) //Проверка частоты процессора
// {
// 	PORTB ^= (1<<PB2);
// 	_delay_ms(1000);
// }

// void TC0_setup (void) //Функция настройки таймера счетчика
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
