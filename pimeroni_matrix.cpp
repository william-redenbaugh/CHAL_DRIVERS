#include "global_includes.h"
#include "pimeroni_matrix.h"

#define PIMERONI_DEBUGGING

#ifdef PIMERONI_DEBUGGING
#define PIMERONI_DEBUG(e, ...)                  \
    os_printf("[PIMERONI MATRIX DEBUGGING]: "); \
    os_printf(e);                               \
    os_printf("\n")
#else
#define PIMERONI_DEBUG(e, ...) (void)(e)
#endif
static const int PIMERONI_BYTES_PER_PIXEL = 3;
static const int PIMERONI_WIDTH = 16;
static const int PIMERONI_HEIGHT = 16;

static const uint8_t PIMERONI_SOF = 0x72;

typedef struct _os_pimeroni_t
{
    os_spi_t *spi_bus;
    os_device_t device;
    uint8_t output_buffer[PIMERONI_BYTES_PER_PIXEL * PIMERONI_WIDTH * PIMERONI_HEIGHT + 1];
} _os_pimeroni_t;

static int os_pimeroni_init(void *ptr, int width, int height)
{

    _os_pimeroni_t *pimeroni = (_os_pimeroni_t *)ptr;
    if (pimeroni == NULL)
    {
        Serial.println("failed to initialize matrix");
        PIMERONI_DEBUG("Pimeroni null pointer error");
        return OS_RET_NULL_PTR;
    }

    pimeroni->output_buffer[0] = PIMERONI_SOF;

    os_device_init_params params = {
        .dma_buf_size = sizeof(uint8_t) * (PIMERONI_WIDTH * PIMERONI_HEIGHT + 1),
        .spi_mode = 0,
        .cs_gpio = GPIO_NUM_9,
        .clk = 200000,
        .bus = pimeroni->spi_bus,
    };

    Serial.printf("SPI BUS Index PTR %d\n", pimeroni->spi_bus);
    int ret = os_spi_couple_device(params, &pimeroni->device);
    return ret;
}

static int os_pimeroni_set(void *ptr, int x, int y, uint8_t r, uint8_t g, uint8_t b)
{
    if (x >= 16 | y >= 16)
    {
        PIMERONI_DEBUG("Out of bounds pixel selection");
        return OS_RET_INVALID_PARAM;
    }

    int pos = ((PIMERONI_WIDTH * y + x)) * 3 + 1;

    _os_pimeroni_t *pimeroni = (_os_pimeroni_t *)ptr;
    if (pimeroni == NULL)
    {
        return OS_RET_NULL_PTR;
    }

    pimeroni->output_buffer[0] = PIMERONI_SOF;
    pimeroni->output_buffer[pos] = r;
    pimeroni->output_buffer[pos + 1] = g;
    pimeroni->output_buffer[pos + 2] = b;

    return OS_RET_OK;
}

static int os_pimeroni_update(void *ptr)
{

    _os_pimeroni_t *pimeroni = (_os_pimeroni_t *)ptr;
    if (pimeroni == NULL)
    {
        return OS_RET_NULL_PTR;
    }

    const size_t data_sent_out = sizeof(uint8_t) * PIMERONI_BYTES_PER_PIXEL * PIMERONI_WIDTH * PIMERONI_HEIGHT + 1;
    int ret = os_spi_send(&pimeroni->device, pimeroni->output_buffer, data_sent_out);
    return ret;
}

int os_init_pimeroni_matrix(os_ledmatrix_t *matrix, os_spi_t *spi_bus)
{

    os_ledmatrix_init_t init_params;
    init_params.height = 16;
    init_params.width = 16;

    init_params.init_func = os_pimeroni_init;
    init_params.setpixel_func = os_pimeroni_set;
    init_params.update_func = os_pimeroni_update;

    init_params.matrix_ptr = new _os_pimeroni_t;

    _os_pimeroni_t *pimeroni = (_os_pimeroni_t *)init_params.matrix_ptr;
    pimeroni->spi_bus = spi_bus;

    int ret = os_init_ledmatrix(init_params, matrix);
    return ret;
}