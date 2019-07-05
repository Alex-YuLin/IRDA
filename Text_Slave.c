#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
/*SPI*/
#define SPI_CS		PB2
#define SPI_MOSI	PB3
#define SPI_MISO	PB4
#define SPI_SCK		PB5

volatile char Data,flag=0,Data_RX;

void spi_slave_init(void){
	DDRB = (0<<SPI_SCK)|(1<<SPI_MISO)|(0<<SPI_MOSI)|(0<<SPI_CS);
	SPCR = (1<<SPIE)|(1<<SPE)|(0<<DORD)|(0<<MSTR)|(0<<CPOL)|(0<<CPHA);
}
void SPI_SlaveTransmit(char data){
	SPDR=data;
	/* Wait for reception complete */
	while(!(SPSR & (1<<SPIF)));
	/* Return Data Register */
}
char SPI_SlaveReceive(void){
	/* Wait for reception complete */
	while(!(SPSR & (1<<SPIF)));
	/* Return Data Register */
	return SPDR;
}

ISR(SPI_STC_vect){
	cli();
	Data =SPDR;
	SPDR=Data+5;
	sei();
}
int main(){

  spi_slave_init();
	sei();
	while(1){
		// printf(" ");
		//SPI_SlaveReceive();
	}
}
