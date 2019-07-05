/*
 * IRDA_mega88_168.c
 *
 * Created: 2018/6/11 下午 07:30:27
 * Author : LinChen
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


#define FCPU 8000000
#define Pulse 38000
#define Baud 4800
#define MYUBRR 8000000/16/Baud-1

#define PB0 0
#define PB1 1
#define PB2 2
#define PB3 3
#define PB4 4
#define PB5 5
#define PB6 6
#define PB7 7

unsigned char Data_buffer;
unsigned char tamp;
int i=0;

void Pulse_set(){
	// Clear Flag //
	TCCR1A &= ~(1<<COM1A1) | (1<<COM1A0) | (1<<WGM11)| (1<<WGM10);
	TCCR1B &= ~(1<<WGM13) | (1<<WGM12)| (1<<CS12) | (1<<CS11)| (1<<CS10);
	
	
	// Setting Flag //
	// SET LOW LEVEL ; Fast PWM (ICR1) ; clock /1
	TCCR1A |= (1<<COM1A1) | (0<<COM1A0) | (1<<WGM11) | (0<<WGM10);
	TCCR1B |= (1<<WGM13) | (0<<WGM12) | (0<<CS12) | (0<<CS11)| (1<<CS10);
	ICR1 = FCPU/Pulse/2+5;
	OCR1A = ICR1/2;
	//37K
}
void USART_Init( unsigned int ubrr ){
	/* Set baud rate */
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXCIE0)|(1<<TXCIE0)|(1<<RXEN0)|(1<<TXEN0);
	/* Set frame format: 8data, 2stop bit */
	UCSR0C = (0<<USBS0)|(3<<UCSZ00);
	
}
void USART_Transmit( unsigned char data ){
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) );
	/* Put data into buffer, sends the data */
	UDR0 = data;
}

unsigned char USART_Receive( void ){
	/* Wait for data to be received */
	while ( !( UCSR0A & (1<<RXC0)) );
	/* Get and return received data from buffer */
	return UDR0;
}
ISR(USART_RX_vect){
	
	Data_buffer=UDR0;
	USART_Transmit(Data_buffer);
	PORTD |=  (1<<2);
}


int main(void)
{
	//set Pulse Output
	DDRB |= (1<<PB1);	
	// set UART Output Input 
	DDRD |= (1<<1);
	DDRD &= ~(1<<0);
	DDRD |= (1<<2);
	Pulse_set();
	USART_Init(MYUBRR);
	sei();
    while (1) 
    {
		PORTD &= ~(1<<2);
		
    }
}

