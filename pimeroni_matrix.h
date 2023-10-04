#ifndef _PIMERONI_MATRIX_H
#define _PIMERONI_MATRIX_H
#include "CHAL_SHARED/csal_ledmatrix.h"
#include "CHAL_SHARED/os_spi.h"

int os_init_pimeroni_matrix(os_ledmatrix_t *matrix, os_spi_t *spi_bus);

#endif