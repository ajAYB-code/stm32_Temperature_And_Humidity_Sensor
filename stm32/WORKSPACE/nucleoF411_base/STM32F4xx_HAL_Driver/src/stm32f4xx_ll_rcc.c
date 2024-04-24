/** 
  ==============================================================================
                      ##### RCC specific features #####
  ==============================================================================
    [..]  
      After reset the device is running from Internal High Speed oscillator 
      (HSI 16MHz) with Flash 0 wait state, Flash prefetch buffer, D-Cache 
      and I-Cache are disabled, and all peripherals are off except internal
      SRAM, Flash and JTAG.
      (+) There is no prescaler on High speed (AHB) and Low speed (APB) busses;
          all peripherals mapped on these busses are running at HSI speed.
      (+) The clock for all peripherals is switched off, except the SRAM and FLASH.
      (+) All GPIOs are in input floating state, except the JTAG pins which
          are assigned to be used for debug purpose.
    
    [..]          
      Once the device started from reset, the user application has to:        
      (+) Configure the clock source to be used to drive the System clock
          (if the application needs higher frequency/performance)
      (+) Configure the System clock frequency and Flash settings  
      (+) Configure the AHB and APB busses prescalers
      (+) Enable the clock for the peripheral(s) to be used
      (+) Configure the clock source(s) for peripherals which clocks are not
          derived from the System clock (I2S, RTC, ADC, USB OTG FS/SDIO/RNG)

                      ##### RCC Limitations #####
  ==============================================================================
    [..]  
      A delay between an RCC peripheral clock enable and the effective peripheral 
      enabling should be taken into account in order to manage the peripheral read/write 
      from/to registers.
      (+) This delay depends on the peripheral mapping.
      (+) If peripheral is mapped on AHB: the delay is 2 AHB clock cycle 
          after the clock enable bit is set on the hardware register
      (+) If peripheral is mapped on APB: the delay is 2 APB clock cycle 
          after the clock enable bit is set on the hardware register

    [..]  
      Possible Workarounds:
      (#) Enable the peripheral clock sometimes before the peripheral read/write 
          register is required.
      (#) For AHB peripheral, insert two dummy read to the peripheral register.
      (#) For APB peripheral, insert a dummy read to the peripheral register.
  */

#include "../inc/stm32f4xx_ll_rcc.h"

#include "include/board.h"

/* HPRE: AHB high-speed prescaler */
#define RCC_CFGR_HPRE_DIV_NONE			0x0
#define RCC_CFGR_HPRE_DIV_2				(0x8 + 0)
#define RCC_CFGR_HPRE_DIV_4				(0x8 + 1)
#define RCC_CFGR_HPRE_DIV_8				(0x8 + 2)
#define RCC_CFGR_HPRE_DIV_16			(0x8 + 3)
#define RCC_CFGR_HPRE_DIV_64			(0x8 + 4)
#define RCC_CFGR_HPRE_DIV_128			(0x8 + 5)
#define RCC_CFGR_HPRE_DIV_256			(0x8 + 6)
#define RCC_CFGR_HPRE_DIV_512			(0x8 + 7)

/* PPRE1/2: APB high-speed prescalers */
#define RCC_CFGR_PPRE_DIV_NONE			0x0
#define RCC_CFGR_PPRE_DIV_2				0x4
#define RCC_CFGR_PPRE_DIV_4				0x5
#define RCC_CFGR_PPRE_DIV_8				0x6
#define RCC_CFGR_PPRE_DIV_16			0x7

enum rcc_osc {
	RCC_HSI,
	RCC_HSE,
	RCC_PLL,
	RCC_PLLI2S,
	RCC_LSI,
	RCC_LSE
};

struct ClockConfig_t {
	uint8_t		type;
	uint8_t		pll_src;
	uint16_t	plln;
	uint8_t		pllm;
	uint8_t		pllp;
	uint8_t		pllq;
	uint8_t		pllr;
	uint8_t		hpre;
	uint8_t		ppre1;
	uint8_t		ppre2;
	uint8_t		power_save;
	uint32_t	flash_cfg;
	uint32_t	ahb_freq;
	uint32_t	apb1_freq;
	uint32_t	apb2_freq;
};

static struct ClockConfig_t _clock_config[] = {
	{ /* HSE 8MHz */
		.type = RCC_HSE,
		.hpre = RCC_CFGR_HPRE_DIV_NONE,
		.ppre1 = RCC_CFGR_PPRE_DIV_NONE,
		.ppre2 = RCC_CFGR_PPRE_DIV_NONE,
	},
	{ /* HSE 8MHz --> 48MHz */
		.type = RCC_PLL,
		.pll_src = RCC_HSE,
		.pllm = 8,
		.plln = 96,
		.pllp = 2,
		.pllq = 2,
		.hpre = RCC_CFGR_HPRE_DIV_NONE,
		.ppre1 = RCC_CFGR_PPRE_DIV_4,
		.ppre2 = RCC_CFGR_PPRE_DIV_2,
		.power_save = 1,							/* HCLK <= 64MHz */
		.flash_cfg = FLASH_ACR_LATENCY_3WS,
		.ahb_freq  = 48000000,
		.apb1_freq = 12000000,
		.apb2_freq = 24000000,
	},
	{ /* HSE 8MHz --> 84MHz */
		.type = RCC_PLL,
		.pll_src = RCC_HSE,
		.pllm = 8,
		.plln = 336,
		.pllp = 4,
		.pllq = 7,
		.power_save = 2,							/* HCLK <= 84MHz */
		.hpre = RCC_CFGR_HPRE_DIV_NONE,
		.ppre1 = RCC_CFGR_PPRE_DIV_2,
		.ppre2 = RCC_CFGR_PPRE_DIV_NONE,
		.flash_cfg = FLASH_ACR_LATENCY_2WS,
		.ahb_freq  = 84000000,
		.apb1_freq = 42000000,
		.apb2_freq = 84000000
	},
	{ /* HSE 8MHz --> 96MHz */
		.type = RCC_PLL,
		.pll_src = RCC_HSE,
		.pllm = 8,
		.plln = 192,
		.pllp = 2,
		.pllq = 2,
		.power_save = 3,							/* HCLK <= 100MHz */
		.hpre = RCC_CFGR_HPRE_DIV_NONE,
		.ppre1 = RCC_CFGR_PPRE_DIV_2,
		.ppre2 = RCC_CFGR_PPRE_DIV_NONE,
		.flash_cfg = FLASH_ACR_LATENCY_3WS,
		.ahb_freq  = 96000000,
		.apb1_freq = 48000000,
		.apb2_freq = 96000000
	
	},
	{ /* HSI 16MHz */
		.type = RCC_HSI,
		.hpre = RCC_CFGR_HPRE_DIV_NONE,
		.ppre1 = RCC_CFGR_PPRE_DIV_NONE,
		.ppre2 = RCC_CFGR_PPRE_DIV_NONE,
	},
	{ /* HSI 16MHz --> 84MHz */
		.type = RCC_PLL,
		.pll_src = RCC_HSI,
		.pllm = 8,
		.plln = 336,
		.pllp = 4,
		.pllq = 7,
		.power_save = 2,							/* HCLK <= 84MHz */
		.hpre = RCC_CFGR_HPRE_DIV_NONE,
		.ppre1 = RCC_CFGR_PPRE_DIV_2,
		.ppre2 = RCC_CFGR_PPRE_DIV_NONE,
		.flash_cfg = FLASH_ACR_LATENCY_2WS,
		.ahb_freq  = 84000000,
		.apb1_freq = 42000000,
		.apb2_freq = 84000000
	}
};

static void rcc_osc_on(enum rcc_osc osc)
{
	switch (osc) {
	case RCC_HSI:
		if (!(RCC->CR & RCC_CR_HSION)) {
			RCC->CR |= RCC_CR_HSION;
			while ((RCC->CR & RCC_CR_HSIRDY)==0);
		}
		break;
	case RCC_HSE:
		if (!(RCC->CR & RCC_CR_HSEON)) {
			RCC->CR |= (RCC_CR_HSEON | RCC_CR_HSEBYP | RCC_CR_CSSON);
			while ((RCC->CR & RCC_CR_HSERDY)==0);
		}
		break;
	case RCC_PLL:
		if (!(RCC->CR & RCC_CR_PLLON)) {
			RCC->CR |= RCC_CR_PLLON;
			while ((RCC->CR & RCC_CR_PLLRDY)==0);
		}
		break;
	case RCC_PLLI2S:
		if (!(RCC->CR & RCC_CR_PLLI2SON)) {
			RCC->CR |= RCC_CR_PLLI2SON;
			while ((RCC->CR & RCC_CR_PLLI2SRDY)==0);
		}
		break;
	case RCC_LSI:
		if (!(RCC->CSR & RCC_CSR_LSION)) {
			RCC->CSR |= RCC_CSR_LSION;
			while ((RCC->CSR & RCC_CSR_LSIRDY)==0);
		}
		break;
	case RCC_LSE:
		if (!(RCC->BDCR & RCC_BDCR_LSEON)) {
			RCC->BDCR |= RCC_BDCR_LSEON;
			while ((RCC->BDCR & RCC_BDCR_LSERDY)==0);
		}
		break;
	}
}

static void rcc_osc_off(enum rcc_osc osc)
{
	switch (osc) {
	case RCC_HSI:
		RCC->CR &= ~RCC_CR_HSION;
		break;
	case RCC_HSE:
		RCC->CR &= ~(RCC_CR_HSEON | RCC_CR_HSEBYP | RCC_CR_CSSON);
		break;
	case RCC_PLL:
		RCC->CR &= ~RCC_CR_PLLON;
		break;
	case RCC_PLLI2S:
		RCC->CR &= ~RCC_CR_PLLI2SON;
		break;
	case RCC_LSI:
		RCC->CSR &= ~RCC_CSR_LSION;
		break;
	case RCC_LSE:
		RCC->BDCR &= ~RCC_BDCR_LSEON;
		break;
	}
}

static void rcc_set_sysclk(enum rcc_osc osc)
{
	RCC->CFGR = (RCC->CFGR & ~0x3) | (osc & 3);
	while (((RCC->CFGR & 0xC)>>2) != osc);
}

void rcc_config_clock(uint32_t config, Clock_t *sysclks)
{
	struct ClockConfig_t *clk;
	
	if (config < CLOCK_CONFIG_END) {
		clk=&(_clock_config[config]);
	} else {
		clk=&(_clock_config[CLOCK_CONFIG_HSI_16MHz]);
	}
		
	if (clk->type == RCC_HSE) {			// HSE Clock
		rcc_osc_on(RCC_HSE);
		rcc_set_sysclk(RCC_HSE);
		rcc_osc_off(RCC_PLL);
		rcc_osc_off(RCC_HSI);
	
	} else if (clk->type == RCC_PLL) {
		// enable PWR module clocking
		RCC->APB1ENR |= 1<<28;
	
		if (clk->pll_src == RCC_HSE) {	// HSE Clock src
			rcc_osc_on(RCC_HSE);
			
			// set HSE Clock as PLL src
			RCC->PLLCFGR |= 1<<22;
		} else {						// Default: HSI Clock src
			rcc_osc_on(RCC_HSI);
			
			// set HSI Clock as PLL src
			RCC->PLLCFGR &= ~(1<<22);
		}
		
		// regulator voltage scaling : VOS[1:0], active when PLL is on
		//  0x1 -> Scale 3 mode: HCLK <= 64MHz
		//  0x2 -> Scale 2 mode: HCLK <= 84MHz
		//  0x3 -> Scale 1 mode: HCLK <= 100MHz
		PWR->CR = (PWR->CR & (~(3<<28))) | (clk->power_save << 28);
		
		// configure prescalers for 
		//  AHB: AHBCLK > 25MHz
		//  APB1: APB1CLK <= 42MHz
		//  APB2: APB2CLK <= 84MHz
		RCC->CFGR = ( RCC->CFGR & ~((0x3F<<10) | (0xF<<4)) ) |
					((clk->hpre & 0xF) << 4) |
					((clk->ppre1 & 0x7) << 10) |
					((clk->ppre2 & 0x7) << 13);
					
		// configure PLL
		RCC->PLLCFGR = ( RCC->PLLCFGR & ~((0xF<<24) | (3<<16) | (0x7FFF)) ) |
					((clk->pllm & 0x3F)) |
					((clk->plln & 0x1FF) << 6) |
					((((clk->pllp>>1) - 1) & 0x3) << 16) |
					((clk->pllq & 0x3F) << 24);
					
		// enable PLL oscillator
		rcc_osc_on(RCC_PLL);
		
		// set Flash timings
		FLASH->ACR = (FLASH->ACR & ~(0xF)) | 
					(FLASH_ACR_PRFTEN | FLASH_ACR_DCEN | FLASH_ACR_ICEN | (clk->flash_cfg & 0xF));
		
		// connect to PLL
		rcc_set_sysclk(RCC_PLL);
		
		// stop unused clock
		if ((clk->pll_src == RCC_HSE) && (RCC->CR & RCC_CR_HSION))
			rcc_osc_off(RCC_HSI);
		else
			rcc_osc_off(RCC_HSE);
			
	} else {							// Default: HSI Clock
		rcc_osc_on(RCC_HSI);
		rcc_set_sysclk(RCC_HSI);
		rcc_osc_off(RCC_PLL);
		rcc_osc_off(RCC_HSE);
	}
	
	sysclks->ahb_freq = clk->ahb_freq;
	sysclks->apb1_freq = clk->apb1_freq;
	sysclks->apb1_timer_freq = clk->ppre1==RCC_CFGR_PPRE_DIV_NONE ? clk->apb1_freq : 2*clk->apb1_freq;
	sysclks->apb2_freq = clk->apb2_freq;
	sysclks->apb2_timer_freq = clk->ppre2==RCC_CFGR_PPRE_DIV_NONE ? clk->apb2_freq : 2*clk->apb2_freq;
}
