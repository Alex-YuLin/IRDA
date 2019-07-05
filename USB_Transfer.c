#include "printrf.h"

#define FOSC 8000000// Clock Speed
#define BAUD 38400
#define MYUBRR FOSC/16/BAUD-1

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
void spi_slave_init(void){
	DDRB = (0<<SPI_SCK)|(1<<SPI_MISO)|(0<<SPI_MOSI)|(0<<SPI_CS);
	SPCR = (1<<SPIE)|(1<<SPE)|(0<<DORD)|(0<<MSTR)|(0<<CPOL)|(0<<CPHA);
}
void SPI_Master_Init(void)
{
	//set miso as inputs
	SPI_DDR =(0<<SPI_MISO)|(1<<SPI_MOSI)|(1<<SPI_SCK)|(1<<SPI_SS)|(1<<ADDR0)|(1<<ADDR1)|(1<<ADDR2);
	//set mosi, sck, /ss, Addr0, Addr1, Addr2 as outputs
	SPCR =(1<<SPE)|(0<<DORD)|(0<<CPOL)|(0<<CPHA)|(1<<MSTR)|(1<<SPR1)|(1<<SPR0);
	//SPI enable, master, f/128
	_delay_ms(10);
}

char SPI_Transmit(char cData)
{
	/* Start transmission */
	SPDR = cData;
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)));

	return SPDR;
}
ISR(USART_RX_vect){
  cli();
  USART_data=UDR0;
  flag=1;
  sei();
}
int main(){
  ASA_M128_set();
  SPI_Master_Init();
  USART_Init();
  sei();
  printf("start");
  while(1){
    if(flag==1){
      printf("data=%x",USART_data);
      USART_Transmit(USART_data);
      SPI_Transmit(USART_data);
      flag=0;
     }
  }
}
