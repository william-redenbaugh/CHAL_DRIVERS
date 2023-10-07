#include "sunfounder_i2c_matrix.h"
#include "global_includes.h"

#define CONFIGURE_CMD_PAGE 0XFD
#define FRAME1_PAGE 0X00
#define FRAME2_PAGE 0X01
#define FUNCTION_PAGE 0X0B
#define DETECTION_PAGE 0X0C
#define LED_VAF_PAGE 0X0D

#define CONFIGURATION_REG 0X00
#define PICTURE_DISPLAY_REG 0X01
#define DISPLAY_OPTION_REG 0X05
#define BREATH_CTL_REG 0X08
#define BREATH_CTL_REG2 0X09
#define SW_SHUT_DOWN_REG 0X0A

#define AUDIO_GAIN_CTL_REG 0X0B
#define STAGGERED_DELAY_REG 0X0D
#define SLEW_RATE_CTL_REG 0X0E
#define CURRENT_CTL_REG 0X0F
#define VAF_CTL_REG 0X14
#define VAF_CTL_REG2 0X15

#define MSKSTD1 (0x3 << 0)
#define MSKSTD2 (0x3 << 2)
#define MSKSTD3 (0x3 << 4)
#define MSKSTD4 (0x3 << 6)
#define CONST_STD_GROUP1 0x00
#define CONST_STD_GROUP2 0x55
#define CONST_STD_GROUP3 0xAA
#define CONST_STD_GROUP4 0xFF

#define MSKVAF1 (0x4 << 0)
#define MSKVAF2 (0x4 << 4)
#define MSKVAF3 (0x4 << 0)

#define MSKFORCEVAFTIME_CONST (0x0 << 3)
#define MSKFORCEVAFCTL_ALWAYSON (0x0 << 6)
#define MSKFORCEVAFCTL_DISABLE (0x2 << 6)
#define MSKCURRENT_CTL_EN (0x1 << 7)
#define CONST_CURRENT_STEP_20mA (0x19 << 0)
#define MSKBLINK_FRAME_300 (0x0 << 6)
#define MSKBLINK_EN (0x1 << 3)
#define MSKBLINK_DIS (0x0 << 3)
#define MSKBLINK_PERIOD_TIME_CONST (0x7 << 0)

#define RGB_MATRIX_ADDRESS 0x74

uint8_t tabLED_Type3Vaf[64] = {
    // Reference SLED1735 Datasheet Type3 Circuit Map
    // Frame 1
    0x50, 0x55, 0x55, 0x55, // C1-A ~ C1-P
    0x00, 0x00, 0x00, 0x00, // C2-A ~ C2-P ,
    0x00, 0x00, 0x00, 0x00, // C3-A ~ C3-P
    0x15, 0x54, 0x55, 0x55, // C4-A ~ C4-P
    0x00, 0x00, 0x00, 0x00, // C5-A ~ C5-P
    0x00, 0x00, 0x00, 0x00, // C6-A ~ C6-P
    0x55, 0x05, 0x55, 0x55, // C7-A ~ C7-P
    0x00, 0x00, 0x00, 0x00, // C8-A ~ C8-P
    // Frame 2
    0x00, 0x00, 0x00, 0x00, // C9-A ~ C9-P
    0x55, 0x55, 0x41, 0x55, // C10-A ~ C10-P
    0x00, 0x00, 0x00, 0x00, // C11-A ~ C11-P
    0x00, 0x00, 0x00, 0x00, // C12-A ~ C12-P
    0x55, 0x55, 0x55, 0x50, // C13-A ~ C13-P
    0x00, 0x00, 0x00, 0x00, // C14-A ~ C14-P
    0x00, 0x00, 0x00, 0x00, // C15-A ~ C15-P
    0x00, 0x00, 0x00, 0x00, // C16-A ~ C16-P
};
// Table Index means the number of LED, and the data of table means PWM control ram address
uint8_t tabLED_Type3PWMCTLAnodeRed[70] = {
    // Reference SLED1735 Datasheet Type3 Map
    0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, // Red D1~D14 PWM CTL Mapping address
    0x50, 0x51, 0x52, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F, // Red D15~D28 PWM CTL Mapping address
    0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F, // Red D29~D42 PWM CTL Mapping address
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, // Red D43~D56 PWM CTL Mapping address
    0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6E, 0x6F, // Red D57~D70 PWM CTL Mapping address
};

// Table Index means the number of LED, and the data of table means PWM control ram address
uint8_t tabLED_Type3PWMCTLAnodeGreen[70] = {
    // Reference SLED1735 Datasheet Type3 Map
    0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, // Green D1~D14 PWM CTL Mapping address
    0x60, 0x61, 0x62, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, // Green D15~D28 PWM CTL Mapping address
    0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F, // Green D29~D42 PWM CTL Mapping address
    0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, // Green D43~D56 PWM CTL Mapping address
    0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7E, 0x7F, // Green D57~D70 PWM CTL Mapping address
};

// Table Index means the number of LED, and the data of table means PWM control ram address
uint8_t tabLED_Type3PWMCTLAnodeBlue[70] = {
    // Reference SLED1735 Datasheet Type3 Map
    0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, // Blue D1~D14 PWM CTL Mapping address
    0x70, 0x71, 0x72, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F, // Blue D15~D28 PWM CTL Mapping address
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, // Blue D29~D42 PWM CTL Mapping address
    0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F, // Blue D43~D56 PWM CTL Mapping address
    0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8E, 0x8F, // Blue D57~D70 PWM CTL Mapping address
};

typedef struct _os_sunfounder_i2c_t
{
    uint8_t rgb_test[64][3] = {};
    os_i2c_t *bus;
} _os_sunfounder_i2c_t;

static void write_cmd(os_i2c_t *i2c_bus, uint8_t reg, uint8_t cmd)
{
    uint8_t data[2] = {reg, cmd};

    os_i2c_send(i2c_bus, RGB_MATRIX_ADDRESS, data, sizeof(data));
}

static void write_Ndata(os_i2c_t *i2c_bus, uint8_t startaddr, uint8_t data, uint8_t data_len)
{
    for (int i = 0; i < data_len; i++)
    {
        write_cmd(i2c_bus, startaddr, data);
        startaddr++;
    }
}

static void write_array_data(os_i2c_t *i2c_bus, uint8_t startaddr, uint8_t *data, uint8_t data_len)
{
    for (int i = 0; i < data_len; i++)
    {
        write_cmd(i2c_bus, startaddr, data[i]);
        startaddr++;
    }
}

static int os_sunfounder_i2c_init(void *ptr, int width, int height)
{
    _os_sunfounder_i2c_t *matrix = (_os_sunfounder_i2c_t *)ptr;
    os_i2c_t *i2c_bus = matrix->bus;

    // Wire.begin(); // join i2c bus (address optional for master)
    write_cmd(i2c_bus, CONFIGURE_CMD_PAGE, FUNCTION_PAGE);
    write_cmd(i2c_bus, SW_SHUT_DOWN_REG, 0x0);                                                                                                                            // System must go to SW shutdowm mode when initialization
    write_cmd(i2c_bus, PICTURE_DISPLAY_REG, 0x10);                                                                                                                        // Setting Matrix Type = Type3
    write_cmd(i2c_bus, STAGGERED_DELAY_REG, ((MSKSTD4 & CONST_STD_GROUP4) | (MSKSTD3 & CONST_STD_GROUP3) | (MSKSTD2 & CONST_STD_GROUP2) | (MSKSTD1 & CONST_STD_GROUP1))); // Setting Staggered Delay
    write_cmd(i2c_bus, SLEW_RATE_CTL_REG, 0x1);                                                                                                                           // Enable Slew Rate control
    write_cmd(i2c_bus, VAF_CTL_REG, (MSKVAF2 | MSKVAF1));                                                                                                                 // VAF Control settings base on the LED type.
    write_cmd(i2c_bus, VAF_CTL_REG2, (MSKFORCEVAFCTL_DISABLE | MSKFORCEVAFTIME_CONST | MSKVAF3));
    write_cmd(i2c_bus, CURRENT_CTL_REG, (MSKCURRENT_CTL_EN | CONST_CURRENT_STEP_20mA)); // Setting LED driving current = 20mA and Enable current control
    write_cmd(i2c_bus, CONFIGURE_CMD_PAGE, FRAME1_PAGE);                                // Init Frame1Page(Clear all Ram) Setting SLED1735 Ram Page to Frame 1 Page
    write_Ndata(i2c_bus, 0x00, 0X00, 0XB3);
    write_cmd(i2c_bus, CONFIGURE_CMD_PAGE, FRAME2_PAGE);
    write_Ndata(i2c_bus, 0x00, 0X00, 0XB3);
    write_cmd(i2c_bus, CONFIGURE_CMD_PAGE, LED_VAF_PAGE);
    write_array_data(i2c_bus, 0X00, tabLED_Type3Vaf, 0X40);

    write_cmd(i2c_bus, CONFIGURE_CMD_PAGE, FUNCTION_PAGE);
    write_cmd(i2c_bus, SW_SHUT_DOWN_REG, 0x1); // After initialization , system back to SW Normal mode.
    write_cmd(i2c_bus, CONFIGURE_CMD_PAGE, FRAME1_PAGE);
    write_Ndata(i2c_bus, 0X00, 0XFF, 0X10); // Clear LED CTL Registers (Frame1Page)
    write_Ndata(i2c_bus, 0x20, 0x00, 0X80);
    write_cmd(i2c_bus, CONFIGURE_CMD_PAGE, FRAME2_PAGE);
    write_Ndata(i2c_bus, 0X00, 0XFF, 0X10); // Clear LED CTL Registers (Frame1Page)
    write_Ndata(i2c_bus, 0x20, 0x00, 0X80);

    return OS_RET_OK;
}

static int os_sunfounder_i2c_set(void *ptr, int x, int y, uint8_t r, uint8_t g, uint8_t b)
{
    if (x >= 8 | y >= 8 | x < 0 | y < 0)
    {
        return OS_RET_INVALID_PARAM;
    }
    _os_sunfounder_i2c_t *matrix = (_os_sunfounder_i2c_t *)ptr;
    matrix->rgb_test[8 * x + y][0] = b;
    matrix->rgb_test[8 * x + y][1] = g;
    matrix->rgb_test[8 * x + y][2] = r;

    return OS_RET_OK;
}

static int os_sunfounder_i2c_update(void *ptr)
{
    _os_sunfounder_i2c_t *matrix = (_os_sunfounder_i2c_t *)ptr;
    os_i2c_t *i2c_bus = matrix->bus;

    uint8_t revert_image[3][64] = {};
    for (int i = 0; i < 64; i++)
    {
        revert_image[0][i] = matrix->rgb_test[i][1];
        revert_image[1][i] = matrix->rgb_test[i][0];
        revert_image[2][i] = matrix->rgb_test[i][2];
    }

    uint8_t reg = 0x20; // 一页的寄存器起始地址
    uint8_t empty = 0;  // 寄存器地址空缺的位置（写入的数据需要补0）
    uint8_t pos = 0;    // 数据列表需要写入的数据所对应的索引
    // print("r: %s, g: %s, b: %s"%(len(reds), len(greens), len(blues)))
    for (int i = 0; i < 15; i++)
    {
        if (i == 0)
        {
            // print("Write Page 1")
            write_cmd(i2c_bus, CONFIGURE_CMD_PAGE, FRAME1_PAGE); // 设置写入的页
        }
        else if (reg == 0x20)
        {
            // print("Write Page 2")
            write_cmd(i2c_bus, CONFIGURE_CMD_PAGE, FRAME2_PAGE);
        }

        uint8_t color = i % 3;
        /*
        uint8_t revert_image_data[64] = {};
        for(int i=0; i<64; i++){
           revert_image_data[i] = revert_image[color][i];
        }
        */
        uint8_t pos_data = pos * 14;
        int data_left = 64 - (pos + 1) * 14;

        uint8_t data_lenth = 0;
        if (data_left > 0)
        {
            data_lenth = 14;
        }
        else
        {
            data_lenth = 8;
            pos_data = 56;
        }
        uint8_t data[data_lenth] = {};
        for (int i = 0; i < data_lenth; i++)
        {
            // data[i] = revert_image_data[pos_data+i];
            data[i] = revert_image[color][pos_data + i];
        }
        uint8_t insert_data[data_lenth + 2] = {0};
        for (int i = 0; i < (data_lenth + 2); i++)
        {
            if (i < empty)
            {
                insert_data[i] = data[i];
            }
            else if (i == empty || i == (empty + 1))
            {
                insert_data[i] = 0;
            }

            else if (i > (empty + 1))
            {
                insert_data[i] = data[i - 2];
            }
        }
        if (data_lenth == 8)
        {
            insert_data[8] = 0;
            insert_data[9] = 0;
        }

        // Wire.beginTransmission(RGB_MATRIX_ADDRESS); // transmit to device #4
        // Wire.write(reg);
        // Wire.write(insert_data, data_lenth + 2);
        // Wire.endTransmission(); // stop transmitting

        if (color == 2)
        {
            empty += 3;
            pos += 1;
        }
        reg += 0x10;
        if (reg == 0xA0)
        {
            //          Serial.println("reg == 0xA0");
            reg = 0x20;
        }
    }

    return OS_RET_OK;
}

int os_init_sunfounder_i2c_matrix(os_ledmatrix_t *matrix, os_i2c_t *i2c_bus)
{

    os_ledmatrix_init_t init_params;
    init_params.height = 16;
    init_params.width = 16;

    init_params.init_func = os_sunfounder_i2c_init;
    init_params.setpixel_func = os_sunfounder_i2c_set;
    init_params.update_func = os_sunfounder_i2c_update;

    init_params.matrix_ptr = new _os_sunfounder_i2c_t;

    _os_sunfounder_i2c_t *pimeroni = (_os_sunfounder_i2c_t *)init_params.matrix_ptr;
    pimeroni->bus = i2c_bus;

    int ret = os_init_ledmatrix(init_params, matrix);
    return ret;
}