/**
  ******************************************************************************
  * @file    ADC/ADC_DMA/main.h 
  * @author  MCD Application Team
  * @version V1.8.0
  * @date    04-November-2016
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADC_H
#define __ADC_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include <stdio.h>


/* Private define ------------------------------------------------------------*/
/* Used to display the ADC converted value on LCD */
#define USE_LCD
  /* If you are not using the LCD, you can monitor the converted value by adding
     the variable "uhADCxConvertedValue" to the debugger watch window */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
//#if defined (USE_STM324xG_EVAL)
  #define ADCx                     		ADC1
  #define ADC_CHANNEL_USE1            ADC_Channel_10
  #define ADC_CHANNEL_USE2            ADC_Channel_11
  #define ADC_CHANNEL_USE3            ADC_Channel_12
  #define ADC_CHANNEL_USE4            ADC_Channel_13	
  #define ADCx_CLK                 		RCC_APB2Periph_ADC1
  #define ADCx_CHANNEL_GPIO_CLK    		RCC_AHB1Periph_GPIOC
	
  #define GPIO_PIN_USE1               GPIO_Pin_0
  #define GPIO_PORT_USE1              GPIOC
  #define GPIO_PIN_USE2               GPIO_Pin_1
  #define GPIO_PORT_USE2              GPIOC
  #define GPIO_PIN_USE3               GPIO_Pin_2
  #define GPIO_PORT_USE3              GPIOC
  #define GPIO_PIN_USE4               GPIO_Pin_3
  #define GPIO_PORT_USE4              GPIOC	

	#define DMA_CHANNELx             		DMA_Channel_0
  #define DMA_STREAMx              		DMA2_Stream0
 //#define ADCx_DR_ADDRESS          ((uint32_t)0x4001224C)
	#define ADCx_DR_ADDRESS          		((uint32_t)0x4001204C)


//#elif defined (USE_STM324x7I_EVAL)   
//  #define ADCx                     ADC3
//  #define ADC_CHANNEL              ADC_Channel_7
//  #define ADCx_CLK                 RCC_APB2Periph_ADC3
//  #define ADCx_CHANNEL_GPIO_CLK    RCC_AHB1Periph_GPIOF
//  #define GPIO_PIN                 GPIO_Pin_9
//  #define GPIO_PORT                GPIOF
//  #define DMA_CHANNELx             DMA_Channel_2
//  #define DMA_STREAMx              DMA2_Stream0
//  #define ADCx_DR_ADDRESS          ((uint32_t)0x4001224C)

//#else
//  #define ADCx                     ADC3
//  #define ADC_CHANNEL              ADC_Channel_8
//  #define ADCx_CLK                 RCC_APB2Periph_ADC3
//  #define ADCx_CHANNEL_GPIO_CLK    RCC_AHB1Periph_GPIOF
//  #define GPIO_PIN                 GPIO_Pin_10
//  #define GPIO_PORT                GPIOF
//  #define DMA_CHANNELx             DMA_Channel_2
//  #define DMA_STREAMx              DMA2_Stream0
//  #define ADCx_DR_ADDRESS          ((uint32_t)0x4001224C)

//#endif
/* Exported functions ------------------------------------------------------- */


void ADC_Config(void);

extern __IO uint16_t uhADCxConvertedValue[4];

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
