# Simple graphical ammeter
This project implements a very simple graphical ammeter/powermeter on STM32 devices.

Hardware shopping list:

* Nucleo-32F070RB available from http://www.st.com/en/evaluation-tools/nucleo-f070rb.html. Any other Nucleo board can be easily adapted as STM32 Cube APIs are used
* INA219 breakout board available from https://www.adafruit.com/product/904
* SSD1306 128x64 display available from https://www.adafruit.com/product/938


|Signal        | STM32 IO | Nucleo connector | 
|--------------|:--------:|:----------------:| 
| INA219 SCL   |   PB8    |      CN10-3      | 
| INA219 SDA   |   PB9    |      CN10-5      |
| SSD1306 SCL  |   PB10   |      CN10-25     |
| SSD1306 SDA  |   PB11   |      CN10-18     |


At reset the values of current and power are displayed in text form on the display.
When pressing the Nucleo blue button, a power measurement acquisition at 1kHz sampling rate is started. 
When acqusition is finished, either because the button is released or the interal buffer is filled, the acquired 
data is displayed.

