<h1 align="center">Welcome to ADT74x0-STM32 👋</h1>
<p>
  <a href="https://github.com/axoulc/ADT74x0-STM32/blob/main/LICENSE" target="_blank">
    <img alt="License: MIT License" src="https://img.shields.io/badge/License-MIT License-yellow.svg" />
  </a>
</p>

> STM32 library for the I2C temperature sensor Analog Device ADT7410 or ADT7420

## Default Configuration (on startup or after reset)
- Conversion mode : Continous
- Interrupt Mode
- Resolution : 13 bits

## What is not implemented ?
- Continous Conversion

## I2C Configuration
- SCL frequency : up to 400 kHz

## INT and CT pins
These legs can be configured according to the temperature detection registers (high, low, critical and hysteresis). You will find all associated functions in the source code.

## Resolution
This IC can be configured in 13bits or 16bits. The temperature value will be automatically calculated according to the resolution setting.

## Usage

```C
#include "ADT74x0.h"

ADT74X0 temp;

int main(void) {
	temp.adti2c = &hi2c1; // Your I2C Handler
	
	ADT74x0_Init(&temp, 0x48); // I2C address depends of A0 and A1 pins
	ADT74x0_Reset(&temp); // Optional
	ADT74x0_SetResolution(&temp, ADT74X0_16BITS); // Put the device in 16 bits resolution
	
	if (ADT74x0_ReadTemp(&temp) != HAL_OK) {
	    Error_Handler();
	}
	printf("Raw value : %d - Temperature Value : %f\r\n", temp.raw_data, temp.deg_data);
}
```

## Author

👤 **Axel Chabot**

* Github: [@axoulc](https://github.com/axoulc)

## Show your support

Give a ⭐️ if this project helped you!

## 📝 License

Copyright © 2021 [Axel Chabot](https://github.com/axoulc).<br />
This project is [MIT](https://github.com/axoulc/ADT74x0-STM32/blob/main/LICENSE) licensed.

***
_This README was generated with ❤️ by [readme-md-generator](https://github.com/kefranabg/readme-md-generator)_
