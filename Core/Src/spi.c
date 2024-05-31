/**
 ******************************************************************************
 * @file    spi.c
 * @brief   This file provides code for the configuration
 *          of the SPI instances.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "spi.h"

/* USER CODE BEGIN 0 */
SPI_HandleTypeDef hspi2;
DMA_HandleTypeDef hdma_spi2_tx;

void SPI2fast()
{
	__HAL_RCC_SPI2_CLK_ENABLE();
	
	hspi2.Instance = SPI2;
	hspi2.Init.Mode = SPI_MODE_MASTER;
	hspi2.Init.Direction = SPI_DIRECTION_1LINE;
	hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi2.Init.CLKPolarity = SPI_POLARITY_HIGH;
	hspi2.Init.CLKPhase = SPI_PHASE_2EDGE;
	hspi2.Init.NSS = SPI_NSS_SOFT;
	hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
	hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
	if (HAL_SPI_Init(&hspi2) != HAL_OK)
	{
		Error_Handler();
	}
	
	__HAL_RCC_DMA1_CLK_ENABLE();
	
	hdma_spi2_tx.Instance = DMA1_Channel5;
  hdma_spi2_tx.Init.Request = DMA_REQUEST_1;
  hdma_spi2_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
  hdma_spi2_tx.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_spi2_tx.Init.MemInc = DMA_MINC_ENABLE;
  hdma_spi2_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
  hdma_spi2_tx.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
  hdma_spi2_tx.Init.Mode = DMA_NORMAL;
  hdma_spi2_tx.Init.Priority = DMA_PRIORITY_MEDIUM;
  if (HAL_DMA_Init(&hdma_spi2_tx) != HAL_OK)
  {
    Error_Handler();
  }

  __HAL_LINKDMA(&hspi2, hdmatx, hdma_spi2_tx);
	
	
	
	
	
}



/* USER CODE END 0 */


/* SPI2 init function */
void MX_SPI2_Init(void)
{

	/* USER CODE BEGIN SPI2_Init 0 */

	/* USER CODE END SPI2_Init 0 */

	/* USER CODE BEGIN SPI2_Init 1 */

	/* USER CODE END SPI2_Init 1 */
	hspi2.Instance = SPI2;
	hspi2.Init.Mode = SPI_MODE_MASTER;
	hspi2.Init.Direction = SPI_DIRECTION_1LINE;
	hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi2.Init.CLKPolarity = SPI_POLARITY_HIGH;
	hspi2.Init.CLKPhase = SPI_PHASE_2EDGE;
	hspi2.Init.NSS = SPI_NSS_SOFT;
	hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
	hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
	if (HAL_SPI_Init(&hspi2) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN SPI2_Init 2 */

	/* USER CODE END SPI2_Init 2 */
}




void HAL_SPI_MspInit(SPI_HandleTypeDef *spiHandle)
{

	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* USER CODE BEGIN SPI2_MspInit 0 */

	/* USER CODE END SPI2_MspInit 0 */
	/* SPI2 clock enable */
	__HAL_RCC_SPI2_CLK_ENABLE();

	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	/**SPI2 GPIO Configuration
	PC3     ------> SPI2_MOSI
	PB13     ------> SPI2_SCK
	*/
	GPIO_InitStruct.Pin = GPIO_PIN_3;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_13;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/* USER CODE BEGIN SPI2_MspInit 1 */
	   /* SPI2_TX Init */
  hdma_spi2_tx.Instance = DMA1_Channel5;
  hdma_spi2_tx.Init.Request = DMA_REQUEST_1;
  hdma_spi2_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
  hdma_spi2_tx.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_spi2_tx.Init.MemInc = DMA_MINC_ENABLE;
  hdma_spi2_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma_spi2_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
  hdma_spi2_tx.Init.Mode = DMA_NORMAL;
  hdma_spi2_tx.Init.Priority = DMA_PRIORITY_MEDIUM;
  if (HAL_DMA_Init(&hdma_spi2_tx) != HAL_OK)
  {
    Error_Handler();
  }

  __HAL_LINKDMA(&hspi2, hdmatx, hdma_spi2_tx);
	
	/* USER CODE END SPI2_MspInit 1 */
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef *spiHandle)
{

	/* USER CODE BEGIN SPI2_MspDeInit 0 */

	/* USER CODE END SPI2_MspDeInit 0 */
	/* Peripheral clock disable */
	__HAL_RCC_SPI2_CLK_DISABLE();

	/**SPI2 GPIO Configuration
	PC3     ------> SPI2_MOSI
	PB13     ------> SPI2_SCK
	*/
	HAL_GPIO_DeInit(GPIOC, GPIO_PIN_3);

	HAL_GPIO_DeInit(GPIOB, GPIO_PIN_13);

	HAL_DMA_DeInit(spiHandle->hdmatx);

	/* USER CODE BEGIN SPI2_MspDeInit 1 */

	/* USER CODE END SPI2_MspDeInit 1 */
}

/* USER CODE BEGIN 1 */
uint8_t SPI2_WriteByte(uint8_t *data, uint16_t size)
{
	//return HAL_SPI_Transmit_DMA(&hspi2, data, size);
	return HAL_SPI_Transmit(&hspi2, data, size,0xff);
}
/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/