#include "../inc/stm32f4xx_hal_i2c.h"
#include "../inc/stm32f4xx_hal_gpio.h"

// I2C Operation
#define I2C_WRITE		0
#define I2C_READ		1

// I2C status register SR1 bits
#define I2C_IT_ERR		(1<<8)
#define I2C_IT_EVT		(1<<9)
#define I2C_IT_BUF		(1<<10)

#define I2C_SR1_RxNE	(1<<6)
#define I2C_SR1_TxE		(1<<7)

/* context of the I2C transfert */
int nb_irqs_i2c=0;


void HAL_I2C_EV_IRQHandler(I2C_HandleTypeDef *hi2c)
{
	uint32_t reg32 __attribute__((unused));
	
	I2C_t *i2c = hi2c->Instance;
	uint32_t sr1 = i2c->SR1;
	
	nb_irqs_i2c++;
	
	if (sr1 & I2C_SR1_SB) {					
		// Start or Repeated Start sent
		i2c->DR = (hi2c->Devaddress <<1) | hi2c->op;	//   send address + mode (R/_W)
	} else if (sr1 & I2C_SR1_ADDR) { 	// A slave acknowledged the address
		if (hi2c->op == I2C_WRITE) {				//   write the first data
			reg32 = i2c->SR2;
			i2c->DR = hi2c->pBuffPtr[hi2c->n_wr++];
			if (hi2c->n_wr == hi2c->n_to_write) {
				i2c->CR2 &= ~I2C_IT_BUF;
			}
	    } else if (hi2c->op==I2C_READ) {			// Some data to read?
	    	if (hi2c->n_to_read == 1) {			//   special case: 1 data to read
				i2c->CR1 &= ~I2C_CR1_ACK;
				reg32 = i2c->SR2;				//   irq acknowledge
			} else {							//   general case
				i2c->CR1 |= I2C_CR1_ACK;
				reg32 = i2c->SR2;				//   irq acknowledge
			}
		}
	} else if (sr1 & I2C_SR1_TxE) {				// some more data to write
		if (hi2c->n_wr < hi2c->n_to_write) {
			i2c->DR = hi2c->pBuffPtr[hi2c->n_wr++];
			if (hi2c->n_wr == hi2c->n_to_write) {
				i2c->CR2 &= ~I2C_IT_BUF;		// when no more data disable BUF irq to disallow TxE events
			} 
		} else if ((hi2c->n_wr == hi2c->n_to_write) && (sr1 & I2C_SR1_BTF)) {	// no more data to write and last written byte transmitted, ack received
			if (hi2c->n_to_read) {				//   anything to read?
				hi2c->op=I2C_READ;
				hi2c->n_rd=0;
				i2c->CR2 |= I2C_IT_BUF;
				i2c->CR1 |= I2C_CR1_START | I2C_CR1_ACK;	// send repeated start
			} else {							// no more data to write: stop
				i2c->CR1 |= I2C_CR1_STOP;		// send stop condition
				i2c->CR2 &= ~I2C_IT_EVT;
				hi2c->status = I2C_OK;
			}
		}
	} else if (sr1 & I2C_SR1_RxNE) {			// a data was received
		hi2c->pBuffPtr[hi2c->n_rd++] = i2c->DR;
			if (hi2c->n_rd == hi2c->n_to_read-1) {
				i2c->CR1 &= ~I2C_CR1_ACK;
			} else if (hi2c->n_rd == hi2c->n_to_read) {
				i2c->CR1 |= I2C_CR1_STOP;
				hi2c->status = I2C_OK;
			}
	}
}
void HAL_I2C_ER_IRQHandler(I2C_HandleTypeDef *hi2c)
//void err_handler(I2C_HandleTypeDef *hi2c)
{
	I2C_t * i2c = hi2c->Instance;
	uint32_t sr1 = i2c->SR1;
	
	if (sr1 & I2C_SR1_BERR) {
		i2c->SR1 &= ~I2C_SR1_BERR;
		hi2c->status = I2C_ERROR;
	} else if (sr1 & I2C_SR1_ARLO) {
		i2c->SR1 &= ~I2C_SR1_ARLO;
		hi2c->status = I2C_ARBITRATION_LOST;
	} else if (sr1 & I2C_SR1_AF) {
		i2c->SR1 &= ~I2C_SR1_AF;
		hi2c->status = I2C_SLAVE_ERROR;
	} else {
		i2c->SR1 &= ~0xDF00;
		hi2c->status = I2C_ERROR;
	}
	i2c->CR1 |= I2C_CR1_STOP;

}




/* i2c_master_init
 *
 * i2c interface initialized in master mode
 */

int  HAL_I2C_Init(I2C_HandleTypeDef *hi2c)
{
	// make a software reset to deal with spurious busy states
	hi2c->Instance->CR1|=1<<15; // SR2[1]==1 ...
	hi2c->Instance->CR1 = 0; // disable the peripheral before changing any configuration
	hi2c->Instance->CR2 = (sysclks.apb1_freq/1000000); // peripheral input clock frequency + Event & Buffer & Error Interrupt Enable
	
	// clock control register config : Fast Mode (400kHz), duty=0 (duty cycle 2/3)
	// Ti2c = (2+1)*CCR*Tapb1 ==> CCR = (apb1_freq/(3*400e3))
	if (hi2c->Init.ClockSpeed == 0) {hi2c->Init.ClockSpeed = 400000;}
	hi2c->Instance->CCR = (1<<15) | (sysclks.apb1_freq/(3* hi2c->Init.ClockSpeed));
	
	//Configure the rise time register (from ST Cube library)
	// rise time:
	//  standard mode: 1000ns
	//  fast mode    : 300ns
	// TRISE = risetime / Tapb1 = 300e-9 * apb1_freq = 300 * apb1_freq_MHz * 1e6 / 1e9
	//  =(((i2c_freq) <= 100000) ? ((apb1_freq/10e6) + 1) : ((((apb1_freq/10e6) * 300) / 1000) + 1))
	hi2c->Instance->TRISE =(((sysclks.apb1_freq/1000000) * 300) / 1000) + 1;
	hi2c->Instance->FLTR = hi2c->Instance->FLTR & (~0x1F); // analog noise filter on, digital filter off
	hi2c->Instance->CR1 |= 1; // enable the peripheral
		
	return I2C_OK;
}

/* i2c_write : write n bytes from buf to slave identified by addr
 *
 */
int HAL_I2C_Master_Transmit_IT(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
	hi2c->status = I2C_BUSY;

	hi2c->Devaddress=DevAddress;
	hi2c->pBuffPtr = pData;
	hi2c->n_to_read  = 0;
	hi2c->n_to_write = Size;
	hi2c->n_wr = 0;
	hi2c->n_rd = 0;
	hi2c->op = I2C_WRITE;
		
	hi2c->Instance->CR2 |= I2C_IT_ERR | I2C_IT_EVT | I2C_IT_BUF;
	hi2c->Instance->CR1 |= I2C_CR1_START;					// send start condition
	
	while (hi2c->status == I2C_BUSY) ;			// wait for the transaction to be done
	
	for (int i=0;i<100;i++)
		__asm volatile("nop");
			
	return hi2c->status;
	
}
//========================================================================
int HAL_I2C_Master_Receive_IT(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
	hi2c->status = I2C_BUSY;

	hi2c->Devaddress=DevAddress;
	hi2c->pBuffPtr = pData;
	hi2c->n_to_read  = Size;
	hi2c->n_to_write = 0;
	hi2c->n_wr = 0;
	hi2c->n_rd = 0;
	hi2c->op = I2C_READ;
		
	hi2c->Instance->CR2 |= I2C_IT_ERR | I2C_IT_EVT | I2C_IT_BUF;
	hi2c->Instance->CR1 |= I2C_CR1_START;					// send start condition
	
	while (hi2c->status == I2C_BUSY) ;			// wait for the transaction to be done
	
	for (int i=0;i<100;i++)
		__asm volatile("nop");
			
	return hi2c->status;
}

/* i2c_write_read : write nwr bytes from buf to slave identified by addr,
 *                  then read nrd bytes to buf
 *
 */

int HAL_I2C_Master_Transmit_Receive_IT(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint32_t nwr, uint32_t nrd, uint32_t Timeout)
{
	
	hi2c->status = I2C_BUSY;

	hi2c->Devaddress=DevAddress;
	hi2c->pBuffPtr = pData;
	hi2c->n_to_read  = nrd;
	hi2c->n_to_write = nwr;
	hi2c->n_wr = 0;
	hi2c->n_rd = 0;
	hi2c->op = I2C_WRITE;
		
	hi2c->Instance->CR2 |= I2C_IT_ERR | I2C_IT_EVT | I2C_IT_BUF;
	hi2c->Instance->CR1 |= I2C_CR1_START;					// send start condition

	while (hi2c->status == I2C_BUSY) ;			// wait for the transaction to be done
	
	for (int i=0;i<100;i++)
		__asm volatile("nop");
			
	return hi2c->status;



	
}
