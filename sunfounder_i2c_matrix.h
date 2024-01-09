#ifndef _SUNFOUNDER_I2C_MATRIX_H
#define _SUNFOUNDER_I2C_MATRIX_H

#include "CHAL_SHARED/csal_ledmatrix.h"
#include "CHAL_SHARED/os_i2c.h"

/**
 * @brief Base interaface for handling the sunfounder i2c based matrix
 * @param os_ledmatrix_t *matrix module
 * @param os_spi_t *spi interface we are using
*/
int os_init_sunfounder_i2c_matrix(os_ledmatrix_t *matrix, os_i2c_t *i2c_bus);

#endif