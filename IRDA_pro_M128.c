#include "ASA_Lib.h"
//
// //SPI
// #define SPI_PORT	PORTB
// #define SPI_PIN		PINB
// #define SPI_DDR		DDRB
// #define SPI_MISO	PB3
// #define SPI_MOSI	PB2
// #define SPI_SCK		PB1
// #define SPI_SS      PB0
//
// //ASA_ID
// #define ADDR_PORT	PORTB
// #define ADDR_PIN	PINB
// #define ADDR_DDR	DDRB
// #define ADDR0       PB5
// #define ADDR1       PB6
// #define ADDR2       PB7
//
// /*****SPI*****/
// void SPI_Init(void)
// {
// 	//set miso as inputs
// 	SPI_DDR =(0<<SPI_MISO)|(1<<SPI_MOSI)|(1<<SPI_SCK)|(1<<SPI_SS)|(1<<ADDR0)|(1<<ADDR1)|(1<<ADDR2);
// 	//set mosi, sck, /ss, Addr0, Addr1, Addr2 as outputs
// 	SPCR =(1<<SPE)|(0<<DORD)|(0<<CPOL)|(0<<CPHA)|(1<<MSTR)|(1<<SPR1)|(1<<SPR0);
// 	//SPI enable, master, f/128
// 	_delay_ms(10);
// }
//
// char SPI_MasterTransmit(char cData)
// {
// 	/* Start transmission */
// 	SPDR = cData;
// 	/* Wait for transmission complete */
// 	while(!(SPSR & (1<<SPIF)));
//
// 	return SPDR;
// }
// void SPI_MasterRecieve(void){
// 	DDRB = (0<<SPI_SCK)|(1<<SPI_MISO)|(0<<SPI_MOSI)|(0<<SPI_SS);
// 	SPCR = (1<<SPIE)|(1<<SPE)|(0<<DORD)|(0<<MSTR)|(0<<CPOL)|(0<<CPHA);
// }
// int main(void)
// {
// 	unsigned int temp;
// 	char Data,Data2,Tr,data_back;
// 	ASA_M128_set();
// 	SPI_Init();
// 	printf("start\n");
// 	DDRF=0x10;
//
// 	while(1)
// 	{
//
// 		scanf("%d",&Tr);
// 		ASA_ID_set(2);
// 		PORTF=0x10;
// 		Data=SPI_MasterTransmit(Tr);
// 		printf("Data= %d\n",Data);
// 		PORTF=0x00;
// 		while(Data==50)
// 		{
// 			PORTF=0x10;
// 			Data2=SPI_MasterTransmit(Tr);
// 			PORTF=0x00;
// 			printf("Data2= %d ",Data2);// Tr+1
// 			if(Data2==(Tr+1))
// 			{
// 				data_back=Data2-1;
// 				break;
// 			}
// 		}
//
// 		printf("Transmit= %d\n",Tr);
// 	  printf("BACK_Data= %d\n",data_back); //Tr
//
// 	}
// }
