/** @file application.c
 * Data is sampled at 10kHz. See timer configuration.
 */

#include "main.h"

/* TODO: fix this */
#if defined(STM32L432xx)
#include "stm32l4xx_hal.h"
#else
#include "stm32f0xx_hal.h"
#endif

#include <string.h>
#include "Adafruit_INA219.h"
#include "Adafruit_GFX.h"
#include "ssd1306.h"
#include "application.h"

#define GRAPH_HEIGTH (64-16)
#define GRAPH_WIDTH  (128)


/* TODO: fix this */
extern TIM_HandleTypeDef htim6;
extern UART_HandleTypeDef huart2;

int applicationInit(void)
{
	//setCalibration_16V_400mA();
	setCalibration_32V_1A();
	ssd1306Init();
	display();
	HAL_Delay(500);
	Adafruit_GFX(getWidth(), getHeight());
	setTextSize(3);
	setTextColor1(WHITE);
	
	return 0;
}


int applicationLoop(void)
{
	float current, busVoltage, shuntVoltage, power;
	char caption[32];
	int i, nSamples;
	int minVal, maxVal, tmp;
	float average;
	
	extern int16_t contBuffer[];

	setTextSize(3);
		
	/* Show instantaneous measurements values until button is pressed */
	while(0 && HAL_GPIO_ReadPin(BUTTON_PORT, BUTTON_PIN) == GPIO_PIN_SET){
		current = getCurrent_mA();
		busVoltage = getBusVoltage_V();
		shuntVoltage = getShuntVoltage_mV();
		power = getPower_mW();

		printf("%f mA, %f V %f mV %f mW\r\n", current, busVoltage, shuntVoltage, power);
		
		/* Print current and power on display */
		clearDisplay();

		setCursor(0, 0);
		sprintf(caption, "%04.1fmA", current);
		for(i=0; i<strlen(caption); i++)
			write(caption[i]);

		setCursor(0, 32);
		sprintf(caption, "%04.1fmW", power);
		for(i=0; i<strlen(caption); i++)
			write(caption[i]);
		
		display();
		
		HAL_Delay(2);
	}
	
	HAL_Delay(200);
	
	/* Show current average */
	while(HAL_GPIO_ReadPin(BUTTON_PORT, BUTTON_PIN) == GPIO_PIN_SET){

		/* Take continuous measurement values */
		contMeasureInit(INA219_REG_CURRENT);
		HAL_TIM_Base_Start_IT(&htim6);
		HAL_Delay(200);
		HAL_TIM_Base_Stop_IT(&htim6);
		
		/* Average values */
		nSamples = getNSamples();
		average = 0;
		for(i=1; i<nSamples; i++){
			average += convertMeasure(contBuffer[i]);
		}
		average /= (float)nSamples;
		
		/* Display */
		clearDisplay();
		setCursor(0, 0);

		if(nSamples != 0)
			sprintf(caption, "%04.1fmA", average);
		else
			sprintf(caption, "NaN");

		for(i=0; i<strlen(caption); i++)
			write(caption[i]);
		display();
	}
	
	HAL_Delay(200);
	
	/* Plot power graph within trigger hi/lo */

	/* Wait a trigger full pulse */
	while(HAL_GPIO_ReadPin(TRIGGER_PORT, TRIGGER_PIN) == GPIO_PIN_SET);
	while(HAL_GPIO_ReadPin(TRIGGER_PORT, TRIGGER_PIN) == GPIO_PIN_RESET);
		
	contMeasureInit(INA219_REG_POWER);	
	HAL_TIM_Base_Start_IT(&htim6);

	/* Wait until trigger is released */
	while(HAL_GPIO_ReadPin(TRIGGER_PORT, TRIGGER_PIN) == GPIO_PIN_SET);
	HAL_TIM_Base_Stop_IT(&htim6);
	
	nSamples = getNSamples();
	minVal = maxVal = contBuffer[0];
	for(i=1; i<nSamples; i++){
		if(contBuffer[i] < minVal)
			minVal = contBuffer[i];
		if(contBuffer[i] > maxVal)
			maxVal = contBuffer[i];
	}

	/* Normalize */
	for(i=0; i<nSamples; i++)
		contBuffer[i] -= minVal;

	for(i=0; i<nSamples; i++){
		tmp = contBuffer[i]; //Prevent overflow
		tmp *= GRAPH_HEIGTH;
		tmp /= (maxVal - minVal);
		contBuffer[i] = tmp;
	}

	
	/* Display min/max power */
	clearDisplay();

	setCursor(0, 56);
	setTextSize(1);
	sprintf(caption, "m/M: %04.1f/%04.1f mW", convertMeasure(minVal), convertMeasure(maxVal));
	for(i=0; i<strlen(caption); i++)
		write(caption[i]);
	
	/* Plot graph */
	for(i=0; i<GRAPH_WIDTH-1; i++){
		drawLine(i, GRAPH_HEIGTH-contBuffer[i], i+1, GRAPH_HEIGTH-contBuffer[i+1], WHITE);
	}
	display();
	
	/* Scroll the rest of the graph */
	scrollGraphInit(6);

	for(i=GRAPH_WIDTH; i<nSamples-1; i++)
		scrollGraphUpdateLine(GRAPH_HEIGTH-contBuffer[i], GRAPH_HEIGTH-contBuffer[i+1]);
	
	scrollGraphDeinit();

	/* Wait for button */
	while(HAL_GPIO_ReadPin(BUTTON_PORT, BUTTON_PIN) == GPIO_PIN_SET);
	
	return 0;
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	contMeasureUpdate();
	
	/* TODO: Fix this */
#if defined(STM32L432xx)
	HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);	
#else
	HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
#endif
}


int fputc(int c, FILE *stream)
{
	HAL_UART_Transmit(&huart2, (uint8_t*)&c, 1, HAL_MAX_DELAY);
	return c;
}
