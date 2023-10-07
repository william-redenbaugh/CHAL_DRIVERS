#ifndef _SUNFOUNDER_I2C_MATRIX_H
#define _SUNFOUNDER_I2C_MATRIX_H

#include "CHAL_SHARED/csal_ledmatrix.h"
#include "CHAL_SHARED/os_i2c.h"

int os_init_sunfounder_i2c_matrix(os_ledmatrix_t *matrix, os_spi_t *spi_bus);

#endif