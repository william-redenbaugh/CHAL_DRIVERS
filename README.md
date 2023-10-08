# CHAL_DRIVERS
Specific Drivers for random peripherals, utilizes the CSAL and CHAL framework for everything

## Current List of drivers
- Sunfounder 8x8 I2C RGB Matrix display
    - LED Matrix display driving controlling the sunfounder 8x8 RGB display. Utilizes I2C transmission to send over data. 
    - Relies on the "csal_ledmatrix.h" module from "CSAL_SHARED" to operate correctly. As this is essentially a parent class to that

- Pimeroni 16x16 SPI Matrix display
    - LED Matrix display driving controlling the pimeroni  16x16 RGB display. Utilizes SPI transmission to send over data. 
    - Relies on the "csal_ledmatrix.h" module from "CSAL_SHARED" to operate correctly.