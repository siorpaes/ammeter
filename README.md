# Simple graphical ammeter/powermeter
This project implements a very simple graphical ammeter/powermeter on STM32 devices.

Hardware shopping list:

* [Nucleo-F070RB](http://www.st.com/en/evaluation-tools/nucleo-f070rb.html) or [Nucleo-L432KC](https://www.amazon.it/STM32-St-nucleo-Development-Board/dp/B077GFHLFS). Any other Nucleo board can be easily adapted as STM32 Cube APIs are used
* [INA219 High Side DC Current Sensor Breakout](https://www.adafruit.com/product/904)
* [SSD1306 128x64 display](https://www.adafruit.com/product/938)


Pinout for Nucleo-F070RB board


|Signal        | STM32 IO | Nucleo connector |
|--------------|:--------:|:----------------:| 
| INA219  SCL1 |   PB8    |      CN10-3      | 
| INA219  SDA1 |   PB9    |      CN10-5      |
| SSD1306 SCL2 |   PB10   |      CN10-25     |
| SSD1306 SDA2 |   PB11   |      CN10-18     |



Pinout for Nucleo-L432KC board


|Signal        | STM32 IO | Nucleo connector |
|--------------|:--------:|:----------------:| 
| INA219  SCL1 |   PA9    |      CN3-1       | 
| INA219  SDA1 |   PA10   |      CN3-2       |
| SSD1306 SCL3 |   PA7    |      CN4-6       |
| SSD1306 SDA3 |   PB4    |      CN3-15      |
| Button       |   PA3    |      CN4-10      |


At reset the instantaneous values of current and power are displayed in text form on the display.
When pressing the button a first time, a current measurement is performed at 10kHz and the mean value of the acquisition is displayed.
When pressing the second button a second time, a power measurement acquisition at 10kHz sampling rate is started.
When acqusition is finished, either because the button is released or the interal buffer is filled, the acquired data is displayed.

