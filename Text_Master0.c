#include "ASA_Lib.h"
//SPI
#define SPI_PORT	PORTB
#define SPI_PIN		PINB
#define SPI_DDR		DDRB
#define SPI_MISO	PB3
#define SPI_MOSI	PB2
#define SPI_SCK		PB1
#define SPI_SS      PB0

//ASA_ID
#define ADDR_PORT	PORTB
#define ADDR_PIN	PINB
#define ADDR_DDR	DDRB
#define ADDR0       PB5
#define ADDR1       PB6
#define ADDR2       PB7
void SPI_Init(void)
{
	//set miso as inputs   // SS set output!!!!
	SPI_DDR =(0<<SPI_MISO)|(1<<SPI_MOSI)|(1<<SPI_SCK)|(1<<SPI_SS)|(1<<ADDR0)|(1<<ADDR1)|(1<<ADDR2);
	//set mosi, sck, /ss, Addr0, Addr1, Addr2 as outputs
	SPCR =(1<<SPE)|(0<<DORD)|(0<<CPOL)|(0<<CPHA)|(1<<MSTR)|(1<<SPR1)|(1<<SPR0);
	//SPI enable, master, f/128
	_delay_ms(10);
}
char SPI_MasterTransmit(char cData)
{
	/* Start transmission */
	SPDR = cData;
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)));

	return SPDR;
}
int main(){
  ASA_M128_set();
  char Tr,Data,Data1;
  printf("start\n");

  SPI_Init();
  DDRF=0x10;

  while(1){
    scanf("%d",&Tr);
    ASA_ID_set(2);
    PORTF=0x10;
		Data1=SPI_MasterTransmit(Tr);
    PORTF=0x00;
		_delay_ms(50);
		PORTF=0x10;
		Data=SPI_MasterTransmit(23);
    PORTF=0x00;
		printf("Transmit= %d\n",Tr);
		printf("BACK_Data= %d\n",Data1);
		printf("BACK_Data= %d\n",Data);

  }
}
