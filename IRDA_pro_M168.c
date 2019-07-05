#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "printrf.h"

/*SPI*/
#define SPI_CS		PB2
#define SPI_MOSI	PB3
#define SPI_MISO	PB4
#define SPI_SCK		PB5

volatile char Data,flag=0,Data_RX,flag_break;

void PWM1_init(void){
  TCCR1A=(1<<COM1A1)|(0<<COM1A0)|(0<<COM1B1)|(0<<COM1B0)|(1<<WGM11)|(0<<WGM10);
  ///// COM1A1,COM1A0 /////
  //  control output compare pins (OC1A and OC1B respectively) behavior
  //  ->  Clear OC1A/OC1B on compare match when up-counting.Set OC1A/OC1B on compare match when downcounting
  TCCR1B=(1<<WGM13)|(0<<WGM12)|(0<<CS12)|(0<<CS11)|(1<<CS10);
  //  WGM13,WGM12,WGM11,WGM10 -> set Timer/Counter Mode of Operation (PWM, phase correct)
	ICR1=104;
  // freq=38k ->  mode of face constance-> f_clock/2/N/TOP  -> 8*10^5/2/1/105;
  //  N->  (Clock Select Bit Description) , TOP -> ICR1dddd
	OCR1A=52;
}

// transmit to m128 //
void spi_slave_init(void){
	DDRB = (0<<SPI_SCK)|(1<<SPI_MISO)|(0<<SPI_MOSI)|(0<<SPI_CS);
	SPCR = (1<<SPIE)|(1<<SPE)|(0<<DORD)|(0<<MSTR)|(0<<CPOL)|(0<<CPHA);
}

#define FOSC 8000000L// Clock Speed
#define BAUD 38400
#define MYUBRR 8000000L/16/BAUD-1

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

ISR(SPI_STC_vect){
  cli();
	Data =SPDR;
	flag=1;
  flag_break=1;
  sei();
  printf("ISR_STC\n");
}

ISR(USART_RX_vect){
  cli();
	Data_RX =USART_Receive();
	flag=2;
  sei();
  printf("ISR_RX\n");
}

int main (void){
  ASA_M128_set();
	spi_slave_init();
  //set PWM to out put
	DDRB|=0x02;
  DDRD=1<<1;
	PWM1_init();
	USART_Init(MYUBRR);
  sei();
  printf("Start\n");

  while(1)
	{

    SPDR=50;
    while(SPDR!=11)   /// SPDR==Tr(Master)
    {
      // recept from m128 , transmit to Resciver
  		if(flag==1){
  			USART_Transmit(Data);
        printf("flag=1\n");
        //USART_Transmit(flag);
  			flag=0;
  		}
      while(flag==2){
        SPDR=Data_RX+1;
        printf("flag=2\n");
        if(flag_break==1){
          printf("flag_break=1\n");
          flag_break=0;
          break;  }
      }
      flag=0;

    }
	}

}
