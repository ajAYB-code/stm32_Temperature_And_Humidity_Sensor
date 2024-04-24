#include "stm32f4xx_hal_spi.h"

#include "stm32f4xx_hal_gpio.h"

#define SPI_ENABLE				(1<<6)
#define SPI_MASTER				(1<<2)

#if __DEBUG__
uint32_t cpt=0;
uint32_t cpt1=0;
uint32_t cpt2=0;
#endif
uint32_t sr;


void HAL_SPI_IRQHandler(SPI_HandleTypeDef* hspi)
{
	sr = hspi->Instance->SR;

	if ((sr & SPI_SR_TXE) && hspi->n_to_write) {	// transmit buffer empty
		if (hspi->n_wr>1) hspi->Instance->DR;				//    read received data (but not the first time)
		hspi->Instance->DR = hspi->pTxBuffPtr[hspi->n_wr++];			//    send next data
		hspi->n_to_write--;
#if __DEBUG__
		cpt1++;
#endif
	} else {
		hspi->Instance->DR;							//    read second last received data
		hspi->Instance->CR2 &= ~(1<<7);				//    disable transmit buffer empty IRQ
#if __DEBUG__
		cpt2++;
#endif
		hspi->status=SPI_OK;
	}
	cpt++;
}
//============================================================
int HAL_SPI_Init(SPI_HandleTypeDef *hspi )
{
	uint32_t freq = 0;

	freq = sysclks.apb2_freq;
	uint32_t presc = freq/(hspi->Init.max_rate); // find prescaler for optimum data transfer rate
	for (uint32_t br_cfg=0; br_cfg<8; br_cfg++) {
		if (presc <= (uint32_t)(1<<(br_cfg+1))) {
			// configure the SPI
			hspi->Instance->CR1 = (hspi->Init.cfg) | (br_cfg<<3) | SPI_MASTER | 3<<8;
			hspi->Instance->CR2 = 1<<2;
			hspi->Instance->CR1 |= SPI_ENABLE; // enable interface
			return (1<<br_cfg)*freq; // return effective data rate
		}
	}
	return -1;
}
//============================================================
#define SPI_IS_BUSY(hspi) (((hspi->Instance)->SR & (SPI_SR_TXE | SPI_SR_RXNE)) == 0 || ((hspi->Instance)->SR & SPI_SR_BSY))
//============================================================
int  HAL_SPI_Transmit_IT(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size)
//void spi_write(SPI_t *spi, uint8_t *data, uint32_t n)
{
	hspi->status=SPI_BUSY;
	hspi->pTxBuffPtr=pData;
	hspi->n_to_write=Size-1;
	hspi->n_wr=1;
	
	hspi->Instance->CR2 |= 1<<7; 	// enable transmit buffer empty IRQ
	hspi->Instance->DR = (hspi->pTxBuffPtr)[0]; 	//start transmission with first byte
   	
	while (SPI_IS_BUSY(hspi));    	// wait for end of tranfer
//while((((hspi->Instance)->SR & (SPI_SR_TXE | SPI_SR_RXNE)) == 0 || ((hspi->Instance)->SR & SPI_SR_BSY)));

	hspi->Instance->DR; 	// read last received data

	return 0;
}
//============================================================
int  HAL_SPI_Write_Byte(SPI_HandleTypeDef *hspi, uint8_t data)
{
	hspi->Instance->DR = data;
	while (SPI_IS_BUSY(hspi));
	//while((((hspi->Instance)->SR & (SPI_SR_TXE | SPI_SR_RXNE)) == 0 || ((hspi->Instance)->SR & SPI_SR_BSY)));
	hspi->Instance->DR;
	return 0;
}
//============================================================
