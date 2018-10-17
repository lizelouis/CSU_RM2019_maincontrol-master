// I2Cdev library collection - MPU6050 I2C device class
// Based on InvenSense MPU-6050 register map document rev. 2.0, 5/19/2011 (RM-MPU-6000A-00)
// 10/3/2011 by Jeff Rowberg <jeff@rowberg.net>
// Updates should (hopefully) always be available at https://github.com/jrowberg/i2cdevlib
//
// Changelog:
//     ... - ongoing debug release

// NOTE: THIS IS ONLY A PARIAL RELEASE. THIS DEVICE CLASS IS CURRENTLY UNDERGOING ACTIVE
// DEVELOPMENT AND IS STILL MISSING SOME IMPORTANT FEATURES. PLEASE KEEP THIS IN MIND IF
// YOU DECIDE TO USE THIS PARTICULAR CODE FOR ANYTHING.

/* ChibiOS I2Cdev MPU6050 device class conversion 2/5/2013 by Jan Schlemminger - C conversion, ChibiOS compliance
 * First release. I just tested a few functions so this should be considered HIGHLY EXPERIMENTAL!!!
 * Feel free to test and report bugs. Updates at https://github.com/jevermeister/MPU6050-ChibiOS
*/

/* ============================================
ChibiOS I2Cdev MPU6050 device class code is placed under the MIT license
Copyright (c) 2012 Jan Schlemminger

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
===============================================
*/

#ifndef _MPU6050_H_
#define _MPU6050_H_

#include "stdbool.h"
#include "stdint.h"

#define TRUE 1
#define FALSE 0

#define MPU6050_ADDRESS_AD0_LOW     0x68 // address pin low (GND), default for InvenSense evaluation board
#define MPU6050_ADDRESS_AD0_HIGH    0x69 // address pin high (VCC)
#define MPU6050_DEFAULT_ADDRESS     MPU6050_ADDRESS_AD0_LOW

#define MPU6050_RA_XG_OFFS_TC       0x00 //[7] PWR_MODE, [6:1] XG_OFFS_TC, [0] OTP_BNK_VLD
#define MPU6050_RA_YG_OFFS_TC       0x01 //[7] PWR_MODE, [6:1] YG_OFFS_TC, [0] OTP_BNK_VLD
#define MPU6050_RA_ZG_OFFS_TC       0x02 //[7] PWR_MODE, [6:1] ZG_OFFS_TC, [0] OTP_BNK_VLD
#define MPU6050_RA_X_FINE_GAIN      0x03 //[7:0] X_FINE_GAIN
#define MPU6050_RA_Y_FINE_GAIN      0x04 //[7:0] Y_FINE_GAIN
#define MPU6050_RA_Z_FINE_GAIN      0x05 //[7:0] Z_FINE_GAIN
#define MPU6050_RA_XA_OFFS_H        0x06 //[15:0] XA_OFFS
#define MPU6050_RA_XA_OFFS_L_TC     0x07
#define MPU6050_RA_YA_OFFS_H        0x08 //[15:0] YA_OFFS
#define MPU6050_RA_YA_OFFS_L_TC     0x09
#define MPU6050_RA_ZA_OFFS_H        0x0A //[15:0] ZA_OFFS
#define MPU6050_RA_ZA_OFFS_L_TC     0x0B
#define MPU6050_RA_XG_OFFS_USRH     0x13 //[15:0] XG_OFFS_USR
#define MPU6050_RA_XG_OFFS_USRL     0x14
#define MPU6050_RA_YG_OFFS_USRH     0x15 //[15:0] YG_OFFS_USR
#define MPU6050_RA_YG_OFFS_USRL     0x16
#define MPU6050_RA_ZG_OFFS_USRH     0x17 //[15:0] ZG_OFFS_USR
#define MPU6050_RA_ZG_OFFS_USRL     0x18
#define MPU6050_RA_SMPLRT_DIV       0x19
#define MPU6050_RA_CONFIG           0x1A
#define MPU6050_RA_GYRO_CONFIG      0x1B
#define MPU6050_RA_ACCEL_CONFIG     0x1C
#define MPU6050_RA_FF_THR           0x1D
#define MPU6050_RA_FF_DUR           0x1E
#define MPU6050_RA_MOT_THR          0x1F
#define MPU6050_RA_MOT_DUR          0x20
#define MPU6050_RA_ZRMOT_THR        0x21
#define MPU6050_RA_ZRMOT_DUR        0x22
#define MPU6050_RA_FIFO_EN          0x23
#define MPU6050_RA_I2C_MST_CTRL     0x24
#define MPU6050_RA_I2C_SLV0_ADDR    0x25
#define MPU6050_RA_I2C_SLV0_REG     0x26
#define MPU6050_RA_I2C_SLV0_CTRL    0x27
#define MPU6050_RA_I2C_SLV1_ADDR    0x28
#define MPU6050_RA_I2C_SLV1_REG     0x29
#define MPU6050_RA_I2C_SLV1_CTRL    0x2A
#define MPU6050_RA_I2C_SLV2_ADDR    0x2B
#define MPU6050_RA_I2C_SLV2_REG     0x2C
#define MPU6050_RA_I2C_SLV2_CTRL    0x2D
#define MPU6050_RA_I2C_SLV3_ADDR    0x2E
#define MPU6050_RA_I2C_SLV3_REG     0x2F
#define MPU6050_RA_I2C_SLV3_CTRL    0x30
#define MPU6050_RA_I2C_SLV4_ADDR    0x31
#define MPU6050_RA_I2C_SLV4_REG     0x32
#define MPU6050_RA_I2C_SLV4_DO      0x33
#define MPU6050_RA_I2C_SLV4_CTRL    0x34
#define MPU6050_RA_I2C_SLV4_DI      0x35
#define MPU6050_RA_I2C_MST_STATUS   0x36
#define MPU6050_RA_INT_PIN_CFG      0x37
#define MPU6050_RA_INT_ENABLE       0x38
#define MPU6050_RA_DMP_INT_STATUS   0x39
#define MPU6050_RA_INT_STATUS       0x3A
#define MPU6050_RA_ACCEL_XOUT_H     0x3B
#define MPU6050_RA_ACCEL_XOUT_L     0x3C
#define MPU6050_RA_ACCEL_YOUT_H     0x3D
#define MPU6050_RA_ACCEL_YOUT_L     0x3E
#define MPU6050_RA_ACCEL_ZOUT_H     0x3F
#define MPU6050_RA_ACCEL_ZOUT_L     0x40
#define MPU6050_RA_TEMP_OUT_H       0x41
#define MPU6050_RA_TEMP_OUT_L       0x42
#define MPU6050_RA_GYRO_XOUT_H      0x43
#define MPU6050_RA_GYRO_XOUT_L      0x44
#define MPU6050_RA_GYRO_YOUT_H      0x45
#define MPU6050_RA_GYRO_YOUT_L      0x46
#define MPU6050_RA_GYRO_ZOUT_H      0x47
#define MPU6050_RA_GYRO_ZOUT_L      0x48
#define MPU6050_RA_EXT_SENS_DATA_00 0x49
#define MPU6050_RA_EXT_SENS_DATA_01 0x4A
#define MPU6050_RA_EXT_SENS_DATA_02 0x4B
#define MPU6050_RA_EXT_SENS_DATA_03 0x4C
#define MPU6050_RA_EXT_SENS_DATA_04 0x4D
#define MPU6050_RA_EXT_SENS_DATA_05 0x4E
#define MPU6050_RA_EXT_SENS_DATA_06 0x4F
#define MPU6050_RA_EXT_SENS_DATA_07 0x50
#define MPU6050_RA_EXT_SENS_DATA_08 0x51
#define MPU6050_RA_EXT_SENS_DATA_09 0x52
#define MPU6050_RA_EXT_SENS_DATA_10 0x53
#define MPU6050_RA_EXT_SENS_DATA_11 0x54
#define MPU6050_RA_EXT_SENS_DATA_12 0x55
#define MPU6050_RA_EXT_SENS_DATA_13 0x56
#define MPU6050_RA_EXT_SENS_DATA_14 0x57
#define MPU6050_RA_EXT_SENS_DATA_15 0x58
#define MPU6050_RA_EXT_SENS_DATA_16 0x59
#define MPU6050_RA_EXT_SENS_DATA_17 0x5A
#define MPU6050_RA_EXT_SENS_DATA_18 0x5B
#define MPU6050_RA_EXT_SENS_DATA_19 0x5C
#define MPU6050_RA_EXT_SENS_DATA_20 0x5D
#define MPU6050_RA_EXT_SENS_DATA_21 0x5E
#define MPU6050_RA_EXT_SENS_DATA_22 0x5F
#define MPU6050_RA_EXT_SENS_DATA_23 0x60
#define MPU6050_RA_MOT_DETECT_STATUS    0x61
#define MPU6050_RA_I2C_SLV0_DO      0x63
#define MPU6050_RA_I2C_SLV1_DO      0x64
#define MPU6050_RA_I2C_SLV2_DO      0x65
#define MPU6050_RA_I2C_SLV3_DO      0x66
#define MPU6050_RA_I2C_MST_DELAY_CTRL   0x67
#define MPU6050_RA_SIGNAL_PATH_RESET    0x68
#define MPU6050_RA_MOT_DETECT_CTRL      0x69
#define MPU6050_RA_USER_CTRL        0x6A
#define MPU6050_RA_PWR_MGMT_1       0x6B
#define MPU6050_RA_PWR_MGMT_2       0x6C
#define MPU6050_RA_BANK_SEL         0x6D
#define MPU6050_RA_MEM_START_ADDR   0x6E
#define MPU6050_RA_MEM_R_W          0x6F
#define MPU6050_RA_DMP_CFG_1        0x70
#define MPU6050_RA_DMP_CFG_2        0x71
#define MPU6050_RA_FIFO_COUNTH      0x72
#define MPU6050_RA_FIFO_COUNTL      0x73
#define MPU6050_RA_FIFO_R_W         0x74
#define MPU6050_RA_WHO_AM_I         0x75

#define MPU6050_TC_PWR_MODE_BIT     7
#define MPU6050_TC_OFFSET_BIT       6
#define MPU6050_TC_OFFSET_LENGTH    6
#define MPU6050_TC_OTP_BNK_VLD_BIT  0

#define MPU6050_VDDIO_LEVEL_VLOGIC  0
#define MPU6050_VDDIO_LEVEL_VDD     1

#define MPU6050_CFG_EXT_SYNC_SET_BIT    5
#define MPU6050_CFG_EXT_SYNC_SET_LENGTH 3
#define MPU6050_CFG_DLPF_CFG_BIT    2
#define MPU6050_CFG_DLPF_CFG_LENGTH 3

#define MPU6050_EXT_SYNC_DISABLED       0x0
#define MPU6050_EXT_SYNC_TEMP_OUT_L     0x1
#define MPU6050_EXT_SYNC_GYRO_XOUT_L    0x2
#define MPU6050_EXT_SYNC_GYRO_YOUT_L    0x3
#define MPU6050_EXT_SYNC_GYRO_ZOUT_L    0x4
#define MPU6050_EXT_SYNC_ACCEL_XOUT_L   0x5
#define MPU6050_EXT_SYNC_ACCEL_YOUT_L   0x6
#define MPU6050_EXT_SYNC_ACCEL_ZOUT_L   0x7

#define MPU6050_DLPF_BW_256         0x00
#define MPU6050_DLPF_BW_188         0x01
#define MPU6050_DLPF_BW_98          0x02
#define MPU6050_DLPF_BW_42          0x03
#define MPU6050_DLPF_BW_20          0x04
#define MPU6050_DLPF_BW_10          0x05
#define MPU6050_DLPF_BW_5           0x06

#define MPU6050_GCONFIG_FS_SEL_BIT      4
#define MPU6050_GCONFIG_FS_SEL_LENGTH   2

#define MPU6050_GYRO_FS_250         0x00
#define MPU6050_GYRO_FS_500         0x01
#define MPU6050_GYRO_FS_1000        0x02
#define MPU6050_GYRO_FS_2000        0x03

#define MPU6050_ACONFIG_XA_ST_BIT           7
#define MPU6050_ACONFIG_YA_ST_BIT           6
#define MPU6050_ACONFIG_ZA_ST_BIT           5
#define MPU6050_ACONFIG_AFS_SEL_BIT         4
#define MPU6050_ACONFIG_AFS_SEL_LENGTH      2
#define MPU6050_ACONFIG_ACCEL_HPF_BIT       2
#define MPU6050_ACONFIG_ACCEL_HPF_LENGTH    3

#define MPU6050_ACCEL_FS_2          0x00
#define MPU6050_ACCEL_FS_4          0x01
#define MPU6050_ACCEL_FS_8          0x02
#define MPU6050_ACCEL_FS_16         0x03

#define MPU6050_DHPF_RESET          0x00
#define MPU6050_DHPF_5              0x01
#define MPU6050_DHPF_2P5            0x02
#define MPU6050_DHPF_1P25           0x03
#define MPU6050_DHPF_0P63           0x04
#define MPU6050_DHPF_HOLD           0x07

#define MPU6050_TEMP_FIFO_EN_BIT    7
#define MPU6050_XG_FIFO_EN_BIT      6
#define MPU6050_YG_FIFO_EN_BIT      5
#define MPU6050_ZG_FIFO_EN_BIT      4
#define MPU6050_ACCEL_FIFO_EN_BIT   3
#define MPU6050_SLV2_FIFO_EN_BIT    2
#define MPU6050_SLV1_FIFO_EN_BIT    1
#define MPU6050_SLV0_FIFO_EN_BIT    0

#define MPU6050_MULT_MST_EN_BIT     7
#define MPU6050_WAIT_FOR_ES_BIT     6
#define MPU6050_SLV_3_FIFO_EN_BIT   5
#define MPU6050_I2C_MST_P_NSR_BIT   4
#define MPU6050_I2C_MST_CLK_BIT     3
#define MPU6050_I2C_MST_CLK_LENGTH  4

#define MPU6050_CLOCK_DIV_348       0x0
#define MPU6050_CLOCK_DIV_333       0x1
#define MPU6050_CLOCK_DIV_320       0x2
#define MPU6050_CLOCK_DIV_308       0x3
#define MPU6050_CLOCK_DIV_296       0x4
#define MPU6050_CLOCK_DIV_286       0x5
#define MPU6050_CLOCK_DIV_276       0x6
#define MPU6050_CLOCK_DIV_267       0x7
#define MPU6050_CLOCK_DIV_258       0x8
#define MPU6050_CLOCK_DIV_500       0x9
#define MPU6050_CLOCK_DIV_471       0xA
#define MPU6050_CLOCK_DIV_444       0xB
#define MPU6050_CLOCK_DIV_421       0xC
#define MPU6050_CLOCK_DIV_400       0xD
#define MPU6050_CLOCK_DIV_381       0xE
#define MPU6050_CLOCK_DIV_364       0xF

#define MPU6050_I2C_SLV_RW_BIT      7
#define MPU6050_I2C_SLV_ADDR_BIT    6
#define MPU6050_I2C_SLV_ADDR_LENGTH 7
#define MPU6050_I2C_SLV_EN_BIT      7
#define MPU6050_I2C_SLV_BYTE_SW_BIT 6
#define MPU6050_I2C_SLV_REG_DIS_BIT 5
#define MPU6050_I2C_SLV_GRP_BIT     4
#define MPU6050_I2C_SLV_LEN_BIT     3
#define MPU6050_I2C_SLV_LEN_LENGTH  4

#define MPU6050_I2C_SLV4_RW_BIT         7
#define MPU6050_I2C_SLV4_ADDR_BIT       6
#define MPU6050_I2C_SLV4_ADDR_LENGTH    7
#define MPU6050_I2C_SLV4_EN_BIT         7
#define MPU6050_I2C_SLV4_INT_EN_BIT     6
#define MPU6050_I2C_SLV4_REG_DIS_BIT    5
#define MPU6050_I2C_SLV4_MST_DLY_BIT    4
#define MPU6050_I2C_SLV4_MST_DLY_LENGTH 5

#define MPU6050_MST_PASS_THROUGH_BIT    7
#define MPU6050_MST_I2C_SLV4_DONE_BIT   6
#define MPU6050_MST_I2C_LOST_ARB_BIT    5
#define MPU6050_MST_I2C_SLV4_NACK_BIT   4
#define MPU6050_MST_I2C_SLV3_NACK_BIT   3
#define MPU6050_MST_I2C_SLV2_NACK_BIT   2
#define MPU6050_MST_I2C_SLV1_NACK_BIT   1
#define MPU6050_MST_I2C_SLV0_NACK_BIT   0

#define MPU6050_INTCFG_INT_LEVEL_BIT        7
#define MPU6050_INTCFG_INT_OPEN_BIT         6
#define MPU6050_INTCFG_LATCH_INT_EN_BIT     5
#define MPU6050_INTCFG_INT_RD_CLEAR_BIT     4
#define MPU6050_INTCFG_FSYNC_INT_LEVEL_BIT  3
#define MPU6050_INTCFG_FSYNC_INT_EN_BIT     2
#define MPU6050_INTCFG_I2C_BYPASS_EN_BIT    1
#define MPU6050_INTCFG_CLKOUT_EN_BIT        0

#define MPU6050_INTMODE_ACTIVEHIGH  0x00
#define MPU6050_INTMODE_ACTIVELOW   0x01

#define MPU6050_INTDRV_PUSHPULL     0x00
#define MPU6050_INTDRV_OPENDRAIN    0x01

#define MPU6050_INTLATCH_50USPULSE  0x00
#define MPU6050_INTLATCH_WAITCLEAR  0x01

#define MPU6050_INTCLEAR_STATUSREAD 0x00
#define MPU6050_INTCLEAR_ANYREAD    0x01

#define MPU6050_INTERRUPT_FF_BIT            7
#define MPU6050_INTERRUPT_MOT_BIT           6
#define MPU6050_INTERRUPT_ZMOT_BIT          5
#define MPU6050_INTERRUPT_FIFO_OFLOW_BIT    4
#define MPU6050_INTERRUPT_I2C_MST_INT_BIT   3
#define MPU6050_INTERRUPT_PLL_RDY_INT_BIT   2
#define MPU6050_INTERRUPT_DMP_INT_BIT       1
#define MPU6050_INTERRUPT_DATA_RDY_BIT      0

// TODO: figure out what these actually do
// UMPL source code is not very obivous
#define MPU6050_DMPINT_5_BIT            5
#define MPU6050_DMPINT_4_BIT            4
#define MPU6050_DMPINT_3_BIT            3
#define MPU6050_DMPINT_2_BIT            2
#define MPU6050_DMPINT_1_BIT            1
#define MPU6050_DMPINT_0_BIT            0

#define MPU6050_MOTION_MOT_XNEG_BIT     7
#define MPU6050_MOTION_MOT_XPOS_BIT     6
#define MPU6050_MOTION_MOT_YNEG_BIT     5
#define MPU6050_MOTION_MOT_YPOS_BIT     4
#define MPU6050_MOTION_MOT_ZNEG_BIT     3
#define MPU6050_MOTION_MOT_ZPOS_BIT     2
#define MPU6050_MOTION_MOT_ZRMOT_BIT    0

#define MPU6050_DELAYCTRL_DELAY_ES_SHADOW_BIT   7
#define MPU6050_DELAYCTRL_I2C_SLV4_DLY_EN_BIT   4
#define MPU6050_DELAYCTRL_I2C_SLV3_DLY_EN_BIT   3
#define MPU6050_DELAYCTRL_I2C_SLV2_DLY_EN_BIT   2
#define MPU6050_DELAYCTRL_I2C_SLV1_DLY_EN_BIT   1
#define MPU6050_DELAYCTRL_I2C_SLV0_DLY_EN_BIT   0

#define MPU6050_PATHRESET_GYRO_RESET_BIT    2
#define MPU6050_PATHRESET_ACCEL_RESET_BIT   1
#define MPU6050_PATHRESET_TEMP_RESET_BIT    0

#define MPU6050_DETECT_ACCEL_ON_DELAY_BIT       5
#define MPU6050_DETECT_ACCEL_ON_DELAY_LENGTH    2
#define MPU6050_DETECT_FF_COUNT_BIT             3
#define MPU6050_DETECT_FF_COUNT_LENGTH          2
#define MPU6050_DETECT_MOT_COUNT_BIT            1
#define MPU6050_DETECT_MOT_COUNT_LENGTH         2

#define MPU6050_DETECT_DECREMENT_RESET  0x0
#define MPU6050_DETECT_DECREMENT_1      0x1
#define MPU6050_DETECT_DECREMENT_2      0x2
#define MPU6050_DETECT_DECREMENT_4      0x3

#define MPU6050_USERCTRL_DMP_EN_BIT             7
#define MPU6050_USERCTRL_FIFO_EN_BIT            6
#define MPU6050_USERCTRL_I2C_MST_EN_BIT         5
#define MPU6050_USERCTRL_I2C_IF_DIS_BIT         4
#define MPU6050_USERCTRL_DMP_RESET_BIT          3
#define MPU6050_USERCTRL_FIFO_RESET_BIT         2
#define MPU6050_USERCTRL_I2C_MST_RESET_BIT      1
#define MPU6050_USERCTRL_SIG_COND_RESET_BIT     0

#define MPU6050_PWR1_DEVICE_RESET_BIT   7
#define MPU6050_PWR1_SLEEP_BIT          6
#define MPU6050_PWR1_CYCLE_BIT          5
#define MPU6050_PWR1_TEMP_DIS_BIT       3
#define MPU6050_PWR1_CLKSEL_BIT         2
#define MPU6050_PWR1_CLKSEL_LENGTH      3

#define MPU6050_CLOCK_INTERNAL          0x00
#define MPU6050_CLOCK_PLL_XGYRO         0x01
#define MPU6050_CLOCK_PLL_YGYRO         0x02
#define MPU6050_CLOCK_PLL_ZGYRO         0x03
#define MPU6050_CLOCK_PLL_EXT32K        0x04
#define MPU6050_CLOCK_PLL_EXT19M        0x05
#define MPU6050_CLOCK_KEEP_RESET        0x07

#define MPU6050_PWR2_LP_WAKE_CTRL_BIT       7
#define MPU6050_PWR2_LP_WAKE_CTRL_LENGTH    2
#define MPU6050_PWR2_STBY_XA_BIT            5
#define MPU6050_PWR2_STBY_YA_BIT            4
#define MPU6050_PWR2_STBY_ZA_BIT            3
#define MPU6050_PWR2_STBY_XG_BIT            2
#define MPU6050_PWR2_STBY_YG_BIT            1
#define MPU6050_PWR2_STBY_ZG_BIT            0

#define MPU6050_WAKE_FREQ_1P25      0x0
#define MPU6050_WAKE_FREQ_2P5       0x1
#define MPU6050_WAKE_FREQ_5         0x2
#define MPU6050_WAKE_FREQ_10        0x3

#define MPU6050_BANKSEL_PRFTCH_EN_BIT       6
#define MPU6050_BANKSEL_CFG_USER_BANK_BIT   5
#define MPU6050_BANKSEL_MEM_SEL_BIT         4
#define MPU6050_BANKSEL_MEM_SEL_LENGTH      5

#define MPU6050_WHO_AM_I_BIT        6
#define MPU6050_WHO_AM_I_LENGTH     6

#define MPU6050_DMP_MEMORY_BANKS        8
#define MPU6050_DMP_MEMORY_BANK_SIZE    256
#define MPU6050_DMP_MEMORY_CHUNK_SIZE   16

// note: DMP code memory blocks defined at end of header file

/*        MPU6050(); */
        void MPU6050(uint8_t address);

        void MPUinitialize(void);
        bool MPUtestConnection(void);

        // AUX_VDDIO register
        uint8_t MPUgetAuxVDDIOLevel(void);
        void MPUsetAuxVDDIOLevel(uint8_t level);

        // SMPLRT_DIV register
        uint8_t MPUgetRate(void);
        void MPUsetRate(uint8_t rate);

        // CONFIG register
        uint8_t MPUgetExternalFrameSync(void);
        void MPUsetExternalFrameSync(uint8_t sync);
        uint8_t MPUgetDLPFMode(void);
        void MPUsetDLPFMode(uint8_t bandwidth);

        // GYRO_CONFIG register
        uint8_t MPUgetFullScaleGyroRange(void);
        void MPUsetFullScaleGyroRange(uint8_t range);

        // ACCEL_CONFIG register
        bool MPUgetAccelXSelfTest(void);
        void MPUsetAccelXSelfTest(bool enabled);
        bool MPUgetAccelYSelfTest(void);
        void MPUsetAccelYSelfTest(bool enabled);
        bool MPUgetAccelZSelfTest(void);
        void MPUsetAccelZSelfTest(bool enabled);
        uint8_t MPUgetFullScaleAccelRange(void);
        void MPUsetFullScaleAccelRange(uint8_t range);
        uint8_t MPUgetDHPFMode(void);
        void MPUsetDHPFMode(uint8_t mode);

        // FF_THR register
        uint8_t MPUgetFreefallDetectionThreshold(void);
        void MPUsetFreefallDetectionThreshold(uint8_t threshold);

        // FF_DUR register
        uint8_t MPUgetFreefallDetectionDuration(void);
        void MPUsetFreefallDetectionDuration(uint8_t duration);

        // MOT_THR register
        uint8_t MPUgetMotionDetectionThreshold(void);
        void MPUsetMotionDetectionThreshold(uint8_t threshold);

        // MOT_DUR register
        uint8_t MPUgetMotionDetectionDuration(void);
        void MPUsetMotionDetectionDuration(uint8_t duration);

        // ZRMOT_THR register
        uint8_t MPUgetZeroMotionDetectionThreshold(void);
        void MPUsetZeroMotionDetectionThreshold(uint8_t threshold);

        // ZRMOT_DUR register
        uint8_t MPUgetZeroMotionDetectionDuration(void);
        void MPUsetZeroMotionDetectionDuration(uint8_t duration);

        // FIFO_EN register
        bool MPUgetTempFIFOEnabled(void);
        void MPUsetTempFIFOEnabled(bool enabled);
        bool MPUgetXGyroFIFOEnabled(void);
        void MPUsetXGyroFIFOEnabled(bool enabled);
        bool MPUgetYGyroFIFOEnabled(void);
        void MPUsetYGyroFIFOEnabled(bool enabled);
        bool MPUgetZGyroFIFOEnabled(void);
        void MPUsetZGyroFIFOEnabled(bool enabled);
        bool MPUgetAccelFIFOEnabled(void);
        void MPUsetAccelFIFOEnabled(bool enabled);
        bool MPUgetSlave2FIFOEnabled(void);
        void MPUsetSlave2FIFOEnabled(bool enabled);
        bool MPUgetSlave1FIFOEnabled(void);
        void MPUsetSlave1FIFOEnabled(bool enabled);
        bool MPUgetSlave0FIFOEnabled(void);
        void MPUsetSlave0FIFOEnabled(bool enabled);

        // I2C_MST_CTRL register
        bool MPUgetMultiMasterEnabled(void);
        void MPUsetMultiMasterEnabled(bool enabled);
        bool MPUgetWaitForExternalSensorEnabled(void);
        void MPUsetWaitForExternalSensorEnabled(bool enabled);
        bool MPUgetSlave3FIFOEnabled(void);
        void MPUsetSlave3FIFOEnabled(bool enabled);
        bool MPUgetSlaveReadWriteTransitionEnabled(void);
        void MPUsetSlaveReadWriteTransitionEnabled(bool enabled);
        uint8_t MPUgetMasterClockSpeed(void);
        void MPUsetMasterClockSpeed(uint8_t speed);

        // I2C_SLV* registers (Slave 0-3)
        uint8_t MPUgetSlaveAddress(uint8_t num);
        void MPUsetSlaveAddress(uint8_t num, uint8_t address);
        uint8_t MPUgetSlaveRegister(uint8_t num);
        void MPUsetSlaveRegister(uint8_t num, uint8_t reg);
        bool MPUgetSlaveEnabled(uint8_t num);
        void MPUsetSlaveEnabled(uint8_t num, bool enabled);
        bool MPUgetSlaveWordByteSwap(uint8_t num);
        void MPUsetSlaveWordByteSwap(uint8_t num, bool enabled);
        bool MPUgetSlaveWriteMode(uint8_t num);
        void MPUsetSlaveWriteMode(uint8_t num, bool mode);
        bool MPUgetSlaveWordGroupOffset(uint8_t num);
        void MPUsetSlaveWordGroupOffset(uint8_t num, bool enabled);
        uint8_t MPUgetSlaveDataLength(uint8_t num);
        void MPUsetSlaveDataLength(uint8_t num, uint8_t length);

        // I2C_SLV* registers (Slave 4)
        uint8_t MPUgetSlave4Address(void);
        void MPUsetSlave4Address(uint8_t address);
        uint8_t MPUgetSlave4Register(void);
        void MPUsetSlave4Register(uint8_t reg);
        void MPUsetSlave4OutputByte(uint8_t data);
        bool MPUgetSlave4Enabled(void);
        void MPUsetSlave4Enabled(bool enabled);
        bool MPUgetSlave4InterruptEnabled(void);
        void MPUsetSlave4InterruptEnabled(bool enabled);
        bool MPUgetSlave4WriteMode(void);
        void MPUsetSlave4WriteMode(bool mode);
        uint8_t MPUgetSlave4MasterDelay(void);
        void MPUsetSlave4MasterDelay(uint8_t delay);
        uint8_t MPUgetSlate4InputByte(void);

        // I2C_MST_STATUS register
        bool MPUgetPassthroughStatus(void);
        bool MPUgetSlave4IsDone(void);
        bool MPUgetLostArbitration(void);
        bool MPUgetSlave4Nack(void);
        bool MPUgetSlave3Nack(void);
        bool MPUgetSlave2Nack(void);
        bool MPUgetSlave1Nack(void);
        bool MPUgetSlave0Nack(void);

        // INT_PIN_CFG register
        bool MPUgetInterruptMode(void);
        void MPUsetInterruptMode(bool mode);
        bool MPUgetInterruptDrive(void);
        void MPUsetInterruptDrive(bool drive);
        bool MPUgetInterruptLatch(void);
        void MPUsetInterruptLatch(bool latch);
        bool MPUgetInterruptLatchClear(void);
        void MPUsetInterruptLatchClear(bool clear);
        bool MPUgetFSyncInterruptLevel(void);
        void MPUsetFSyncInterruptLevel(bool level);
        bool MPUgetFSyncInterruptEnabled(void);
        void MPUsetFSyncInterruptEnabled(bool enabled);
        bool MPUgetI2CBypassEnabled(void);
        void MPUsetI2CBypassEnabled(bool enabled);
        bool MPUgetClockOutputEnabled(void);
        void MPUsetClockOutputEnabled(bool enabled);

        // INT_ENABLE register
        uint8_t MPUgetIntEnabled(void);
        void MPUsetIntEnabled(uint8_t enabled);
        bool MPUgetIntFreefallEnabled(void);
        void MPUsetIntFreefallEnabled(bool enabled);
        bool MPUgetIntMotionEnabled(void);
        void MPUsetIntMotionEnabled(bool enabled);
        bool MPUgetIntZeroMotionEnabled(void);
        void MPUsetIntZeroMotionEnabled(bool enabled);
        bool MPUgetIntFIFOBufferOverflowEnabled(void);
        void MPUsetIntFIFOBufferOverflowEnabled(bool enabled);
        bool MPUgetIntI2CMasterEnabled(void);
        void MPUsetIntI2CMasterEnabled(bool enabled);
        bool MPUgetIntDataReadyEnabled(void);
        void MPUsetIntDataReadyEnabled(bool enabled);

        // INT_STATUS register
        uint8_t MPUgetIntStatus(void);
        bool MPUgetIntFreefallStatus(void);
        bool MPUgetIntMotionStatus(void);
        bool MPUgetIntZeroMotionStatus(void);
        bool MPUgetIntFIFOBufferOverflowStatus(void);
        bool MPUgetIntI2CMasterStatus(void);
        bool MPUgetIntDataReadyStatus(void);

        // ACCEL_*OUT_* registers
        void MPUgetMotion9(int16_t* ax, int16_t* ay, int16_t* az, int16_t* gx, int16_t* gy, int16_t* gz, int16_t* mx, int16_t* my, int16_t* mz);
        void MPUgetMotion6(int16_t* ax, int16_t* ay, int16_t* az, int16_t* gx, int16_t* gy, int16_t* gz);
        void MPUgetAcceleration(int16_t* x, int16_t* y, int16_t* z);
        int16_t MPUgetAccelerationX(void);
        int16_t MPUgetAccelerationY(void);
        int16_t MPUgetAccelerationZ(void);

        // TEMP_OUT_* registers
        int16_t MPUgetTemperature(void);

        // GYRO_*OUT_* registers
        void MPUgetRotation(int16_t* x, int16_t* y, int16_t* z);
        int16_t MPUgetRotationX(void);
        int16_t MPUgetRotationY(void);
        int16_t MPUgetRotationZ(void);

        // EXT_SENS_DATA_* registers
        uint8_t MPUgetExternalSensorByte(int position);
        uint16_t MPUgetExternalSensorWord(int position);
        uint32_t MPUgetExternalSensorDWord(int position);

        // MOT_DETECT_STATUS register
        bool MPUgetXNegMotionDetected(void);
        bool MPUgetXPosMotionDetected(void);
        bool MPUgetYNegMotionDetected(void);
        bool MPUgetYPosMotionDetected(void);
        bool MPUgetZNegMotionDetected(void);
        bool MPUgetZPosMotionDetected(void);
        bool MPUgetZeroMotionDetected(void);

        // I2C_SLV*_DO register
        void MPUsetSlaveOutputByte(uint8_t num, uint8_t data);

        // I2C_MST_DELAY_CTRL register
        bool MPUgetExternalShadowDelayEnabled(void);
        void MPUsetExternalShadowDelayEnabled(bool enabled);
        bool MPUgetSlaveDelayEnabled(uint8_t num);
        void MPUsetSlaveDelayEnabled(uint8_t num, bool enabled);

        // SIGNAL_PATH_RESET register
        void MPUresetGyroscopePath(void);
        void MPUresetAccelerometerPath(void);
        void MPUresetTemperaturePath(void);

        // MOT_DETECT_CTRL register
        uint8_t MPUgetAccelerometerPowerOnDelay(void);
        void MPUsetAccelerometerPowerOnDelay(uint8_t delay);
        uint8_t MPUgetFreefallDetectionCounterDecrement(void);
        void MPUsetFreefallDetectionCounterDecrement(uint8_t decrement);
        uint8_t MPUgetMotionDetectionCounterDecrement(void);
        void MPUsetMotionDetectionCounterDecrement(uint8_t decrement);

        // USER_CTRL register
        bool MPUgetFIFOEnabled(void);
        void MPUsetFIFOEnabled(bool enabled);
        bool MPUgetI2CMasterModeEnabled(void);
        void MPUsetI2CMasterModeEnabled(bool enabled);
        void MPUswitchSPIEnabled(bool enabled);
        void MPUresetFIFO(void);
        void MPUresetI2CMaster(void);
        void MPUresetSensors(void);

        // PWR_MGMT_1 register
        void MPUreset(void);
        bool MPUgetSleepEnabled(void);
        void MPUsetSleepEnabled(bool enabled);
        bool MPUgetWakeCycleEnabled(void);
        void MPUsetWakeCycleEnabled(bool enabled);
        bool MPUgetTempSensorEnabled(void);
        void MPUsetTempSensorEnabled(bool enabled);
        uint8_t MPUgetClockSource(void);
        void MPUsetClockSource(uint8_t source);

        // PWR_MGMT_2 register
        uint8_t MPUgetWakeFrequency(void);
        void MPUsetWakeFrequency(uint8_t frequency);
        bool MPUgetStandbyXAccelEnabled(void);
        void MPUsetStandbyXAccelEnabled(bool enabled);
        bool MPUgetStandbyYAccelEnabled(void);
        void MPUsetStandbyYAccelEnabled(bool enabled);
        bool MPUgetStandbyZAccelEnabled(void);
        void MPUsetStandbyZAccelEnabled(bool enabled);
        bool MPUgetStandbyXGyroEnabled(void);
        void MPUsetStandbyXGyroEnabled(bool enabled);
        bool MPUgetStandbyYGyroEnabled(void);
        void MPUsetStandbyYGyroEnabled(bool enabled);
        bool MPUgetStandbyZGyroEnabled(void);
        void MPUsetStandbyZGyroEnabled(bool enabled);

        // FIFO_COUNT_* registers
        uint16_t MPUgetFIFOCount(void);

        // FIFO_R_W register
        uint8_t MPUgetFIFOByte(void);
        void MPUsetFIFOByte(uint8_t data);
        void MPUgetFIFOBytes(uint8_t *data, uint8_t length);

        // WHO_AM_I register
        uint8_t MPUgetDeviceID(void);
        void MPUsetDeviceID(uint8_t id);
        
        // ======== UNDOCUMENTED/DMP REGISTERS/METHODS ========
        
        // XG_OFFS_TC register
        uint8_t MPUgetOTPBankValid(void);
        void MPUsetOTPBankValid(bool enabled);
        int8_t MPUgetXGyroOffset(void);
        void MPUsetXGyroOffset(int8_t offset);

        // YG_OFFS_TC register
        int8_t MPUgetYGyroOffset(void);
        void MPUsetYGyroOffset(int8_t offset);

        // ZG_OFFS_TC register
        int8_t MPUgetZGyroOffset(void);
        void MPUsetZGyroOffset(int8_t offset);

        // X_FINE_GAIN register
        int8_t MPUgetXFineGain(void);
        void MPUsetXFineGain(int8_t gain);

        // Y_FINE_GAIN register
        int8_t MPUgetYFineGain(void);
        void MPUsetYFineGain(int8_t gain);

        // Z_FINE_GAIN register
        int8_t MPUgetZFineGain(void);
        void MPUsetZFineGain(int8_t gain);

        // XA_OFFS_* registers
        int16_t MPUgetXAccelOffset(void);
        void MPUsetXAccelOffset(int16_t offset);

        // YA_OFFS_* register
        int16_t MPUgetYAccelOffset(void);
        void MPUsetYAccelOffset(int16_t offset);

        // ZA_OFFS_* register
        int16_t MPUgetZAccelOffset(void);
        void MPUsetZAccelOffset(int16_t offset);

        // XG_OFFS_USR* registers
        int16_t MPUgetXGyroOffsetUser(void);
        void MPUsetXGyroOffsetUser(int16_t offset);

        // YG_OFFS_USR* register
        int16_t MPUgetYGyroOffsetUser(void);
        void MPUsetYGyroOffsetUser(int16_t offset);

        // ZG_OFFS_USR* register
        int16_t MPUgetZGyroOffsetUser(void);
        void MPUsetZGyroOffsetUser(int16_t offset);
        
        // INT_ENABLE register (DMP functions)
        bool MPUgetIntPLLReadyEnabled(void);
        void MPUsetIntPLLReadyEnabled(bool enabled);
        bool MPUgetIntDMPEnabled(void);
        void MPUsetIntDMPEnabled(bool enabled);
        
        // DMP_INT_STATUS
        bool MPUgetDMPInt5Status(void);
        bool MPUgetDMPInt4Status(void);
        bool MPUgetDMPInt3Status(void);
        bool MPUgetDMPInt2Status(void);
        bool MPUgetDMPInt1Status(void);
        bool MPUgetDMPInt0Status(void);

        // INT_STATUS register (DMP functions)
        bool MPUgetIntPLLReadyStatus(void);
        bool MPUgetIntDMPStatus(void);
        
        // USER_CTRL register (DMP functions)
        bool MPUgetDMPEnabled(void);
        void MPUsetDMPEnabled(bool enabled);
        void MPUresetDMP(void);
        
        // BANK_SEL register
        void MPUsetMemoryBank(uint8_t bank, bool prefetchEnabled, bool userBank);
        
        // MEM_START_ADDR register
        void MPUsetMemoryStartAddress(uint8_t address);
        
        // MEM_R_W register
        uint8_t MPUreadMemoryByte(void);
        void MPUwriteMemoryByte(uint8_t data);
        void MPUreadMemoryBlock(uint8_t *data, uint16_t dataSize, uint8_t bank, uint8_t address);
        bool MPUwriteMemoryBlock(const uint8_t *data, uint16_t dataSize, uint8_t bank, uint8_t address, bool verify, bool useProgMem);
        bool MPUwriteProgMemoryBlock(const uint8_t *data, uint16_t dataSize, uint8_t bank, uint8_t address, bool verify);

        bool MPUwriteDMPConfigurationSet(const uint8_t *data, uint16_t dataSize, bool useProgMem);
        bool MPUwriteProgDMPConfigurationSet(const uint8_t *data, uint16_t dataSize);

        // DMP_CFG_1 register
        uint8_t MPUgetDMPConfig1(void);
        void MPUsetDMPConfig1(uint8_t config);

        // DMP_CFG_2 register
        uint8_t MPUgetDMPConfig2(void);
        void MPUsetDMPConfig2(uint8_t config);
				

        // special methods for MotionApps 2.0 implementation
        #ifdef MPU6050_INCLUDE_DMP_MOTIONAPPS20
            uint8_t *MPUdmpPacketBuffer;
            static const uint16_t MPUdmpPacketSize = 42; // no variable-length arrays at runtime in C

            uint8_t MPUdmpInitialize(void);
            bool MPUdmpPacketAvailable(void);

            uint8_t MPUdmpSetFIFORate(uint8_t fifoRate);
            uint8_t MPUdmpGetFIFORate(void);
            uint8_t MPUdmpGetSampleStepSizeMS(void);
            uint8_t MPUdmpGetSampleFrequency(void);
            int32_t MPUdmpDecodeTemperature(int8_t tempReg);
            
            // Register callbacks after a packet of FIFO data is processed
            //uint8_t dmpRegisterFIFORateProcess(inv_obj_func func, int16_t priority);
            //uint8_t dmpUnregisterFIFORateProcess(inv_obj_func func);
            uint8_t MPUdmpRunFIFORateProcesses(void);
            
            // Setup FIFO for various output
            uint8_t MPUdmpSendQuaternion(uint_fast16_t accuracy);
            uint8_t MPUdmpSendGyro(uint_fast16_t elements, uint_fast16_t accuracy);
            uint8_t MPUdmpSendAccel(uint_fast16_t elements, uint_fast16_t accuracy);
            uint8_t MPUdmpSendLinearAccel(uint_fast16_t elements, uint_fast16_t accuracy);
            uint8_t MPUdmpSendLinearAccelInWorld(uint_fast16_t elements, uint_fast16_t accuracy);
            uint8_t MPUdmpSendControlData(uint_fast16_t elements, uint_fast16_t accuracy);
            uint8_t MPUdmpSendSensorData(uint_fast16_t elements, uint_fast16_t accuracy);
            uint8_t MPUdmpSendExternalSensorData(uint_fast16_t elements, uint_fast16_t accuracy);
            uint8_t MPUdmpSendGravity(uint_fast16_t elements, uint_fast16_t accuracy);
            uint8_t MPUdmpSendPacketNumber(uint_fast16_t accuracy);
            uint8_t MPUdmpSendQuantizedAccel(uint_fast16_t elements, uint_fast16_t accuracy);
            uint8_t MPUdmpSendEIS(uint_fast16_t elements, uint_fast16_t accuracy);

            // Get Fixed Point data from FIFO
            uint8_t MPUdmpGetAccel32(int32_t *data, const uint8_t* packet);
            uint8_t MPUdmpGetAccel16(int16_t *data, const uint8_t* packet);
            uint8_t MPUdmpGetAccel(VectorInt16 *v, const uint8_t* packet);
            uint8_t MPUdmpGetQuaternion32(int32_t *data, const uint8_t* packet);
            uint8_t MPUdmpGetQuaternion16(int16_t *data, const uint8_t* packet);
            uint8_t MPUdmpGetQuaternion(Quaternion *q, const uint8_t* packet);
            uint8_t MPUdmpGet6AxisQuaternion32(int32_t *data, const uint8_t* packet);
            uint8_t MPUdmpGet6AxisQuaternion16(int16_t *data, const uint8_t* packet);
            uint8_t MPUdmpGet6AxisQuaternion(Quaternion *q, const uint8_t* packet);
            uint8_t MPUdmpGetRelativeQuaternion32(int32_t *data, const uint8_t* packet);
            uint8_t MPUdmpGetRelativeQuaternion16(int16_t *data, const uint8_t* packet);
            uint8_t MPUdmpGetRelativeQuaternion(Quaternion *data, const uint8_t* packet);
            uint8_t MPUdmpGetGyro32(int32_t *data, const uint8_t* packet);
            uint8_t MPUdmpGetGyro16(int16_t *data, const uint8_t* packet);
            uint8_t MPUdmpGetGyro(VectorInt16 *v, const uint8_t* packet);
            uint8_t MPUdmpSetLinearAccelFilterCoefficient(float coef);
            uint8_t MPUdmpGetLinearAccel32(int32_t *data, const uint8_t* packet);
            uint8_t MPUdmpGetLinearAccel16(int16_t *data, const uint8_t* packet);
            uint8_t MPUdmpGetLinearAccel(VectorInt16 *v, const uint8_t* packet);
            uint8_t MPUdmpGetLinearAccelVect(VectorInt16 *v, VectorInt16 *vRaw, VectorFloat *gravity);
            uint8_t MPUdmpGetLinearAccelInWorld32(int32_t *data, const uint8_t* packet);
            uint8_t MPUdmpGetLinearAccelInWorld16(int16_t *data, const uint8_t* packet);
            uint8_t MPUdmpGetLinearAccelInWorld(VectorInt16 *v, const uint8_t* packet);
            uint8_t MPUdmpGetLinearAccelInWorldVect(VectorInt16 *v, VectorInt16 *vReal, Quaternion *q);
            uint8_t MPUdmpGetGyroAndAccelSensor32(int32_t *data, const uint8_t* packet);
            uint8_t MPUdmpGetGyroAndAccelSensor16(int16_t *data, const uint8_t* packet);
            uint8_t MPUdmpGetGyroAndAccelSensor(VectorInt16 *g, VectorInt16 *a, const uint8_t* packet);
            uint8_t MPUdmpGetGyroSensor32(int32_t *data, const uint8_t* packet);
            uint8_t MPUdmpGetGyroSensor16(int16_t *data, const uint8_t* packet);
            uint8_t MPUdmpGetGyroSensor(VectorInt16 *v, const uint8_t* packet);
            uint8_t MPUdmpGetControlData(int32_t *data, const uint8_t* packet);
            uint8_t MPUdmpGetTemperature(int32_t *data, const uint8_t* packet);
            uint8_t MPUdmpGetGravity32(int32_t *data, const uint8_t* packet);
            uint8_t MPUdmpGetGravity16(int16_t *data, const uint8_t* packet);
            uint8_t MPUdmpGetGravity(VectorInt16 *v, const uint8_t* packet);
            uint8_t MPUdmpGetGravityVect(VectorFloat *v, Quaternion *q);
            uint8_t MPUdmpGetUnquantizedAccel32(int32_t *data, const uint8_t* packet);
            uint8_t MPUdmpGetUnquantizedAccel16(int16_t *data, const uint8_t* packet);
            uint8_t MPUdmpGetUnquantizedAccel(VectorInt16 *v, const uint8_t* packet);
            uint8_t MPUdmpGetQuantizedAccel32(int32_t *data, const uint8_t* packet);
            uint8_t MPUdmpGetQuantizedAccel16(int16_t *data, const uint8_t* packet);
            uint8_t MPUdmpGetQuantizedAccel(VectorInt16 *v, const uint8_t* packet);
            uint8_t MPUdmpGetExternalSensorData(int32_t *data, uint16_t size, const uint8_t* packet);
            uint8_t MPUdmpGetEIS(int32_t *data, const uint8_t* packet);
            
            uint8_t MPUdmpGetEuler(float *data, Quaternion *q);
            uint8_t MPUdmpGetYawPitchRoll(float *data, Quaternion *q, VectorFloat *gravity);

            // Get Floating Point data from FIFO
            uint8_t MPUdmpGetAccelFloat(float *data, const uint8_t* packet);
            uint8_t MPUdmpGetQuaternionFloat(float *data, const uint8_t* packet);

            uint8_t MPUdmpProcessFIFOPacket(const unsigned char *dmpData);
            uint8_t MPUdmpReadAndProcessFIFOPacket(uint8_t numPackets, uint8_t *processed);

            uint8_t MPUdmpSetFIFOProcessedCallback(void (*func) (void));

            uint8_t MPUdmpInitFIFOParam(void);
            uint8_t MPUdmpCloseFIFO(void);
            uint8_t MPUdmpSetGyroDataSource(uint8_t source);
            uint8_t MPUdmpDecodeQuantizedAccel(void);
            uint32_t MPUdmpGetGyroSumOfSquare(void);
            uint32_t MPUdmpGetAccelSumOfSquare(void);
            void MPUdmpOverrideQuaternion(long *q);
            uint16_t MPUdmpGetFIFOPacketSize(void);
        #endif

        // special methods for MotionApps 4.1 implementation
        #ifdef MPU6050_INCLUDE_DMP_MOTIONAPPS41
            uint8_t *MPUdmpPacketBuffer;
            uint16_t MPUdmpPacketSize;

            uint8_t MPUdmpInitialize(void);
            bool MPUdmpPacketAvailable(void);

            uint8_t MPUdmpSetFIFORate(uint8_t fifoRate);
            uint8_t MPUdmpGetFIFORate(void);
            uint8_t MPUdmpGetSampleStepSizeMS(void);
            uint8_t MPUdmpGetSampleFrequency(void);
            int32_t MPUdmpDecodeTemperature(int8_t tempReg);
            
            // Register callbacks after a packet of FIFO data is processed
            //uint8_t dmpRegisterFIFORateProcess(inv_obj_func func, int16_t priority);
            //uint8_t dmpUnregisterFIFORateProcess(inv_obj_func func);
            uint8_t MPUdmpRunFIFORateProcesses(void);
            
            // Setup FIFO for various output
            uint8_t MPUdmpSendQuaternion(uint_fast16_t accuracy);
            uint8_t MPUdmpSendGyro(uint_fast16_t elements, uint_fast16_t accuracy);
            uint8_t MPUdmpSendAccel(uint_fast16_t elements, uint_fast16_t accuracy);
            uint8_t MPUdmpSendLinearAccel(uint_fast16_t elements, uint_fast16_t accuracy);
            uint8_t MPUdmpSendLinearAccelInWorld(uint_fast16_t elements, uint_fast16_t accuracy);
            uint8_t MPUdmpSendControlData(uint_fast16_t elements, uint_fast16_t accuracy);
            uint8_t MPUdmpSendSensorData(uint_fast16_t elements, uint_fast16_t accuracy);
            uint8_t MPUdmpSendExternalSensorData(uint_fast16_t elements, uint_fast16_t accuracy);
            uint8_t MPUdmpSendGravity(uint_fast16_t elements, uint_fast16_t accuracy);
            uint8_t MPUdmpSendPacketNumber(uint_fast16_t accuracy);
            uint8_t MPUdmpSendQuantizedAccel(uint_fast16_t elements, uint_fast16_t accuracy);
            uint8_t MPUdmpSendEIS(uint_fast16_t elements, uint_fast16_t accuracy);

            // Get Fixed Point data from FIFO
            uint8_t MPUdmpGetAccel(int32_t *data, const uint8_t* packet);
            uint8_t MPUdmpGetAccel(int16_t *data, const uint8_t* packet);
            uint8_t MPUdmpGetAccel(VectorInt16 *v, const uint8_t* packet);
            uint8_t MPUdmpGetQuaternion(int32_t *data, const uint8_t* packet);
            uint8_t MPUdmpGetQuaternion(int16_t *data, const uint8_t* packet);
            uint8_t MPUdmpGetQuaternion(Quaternion *q, const uint8_t* packet);
            uint8_t MPUdmpGet6AxisQuaternion(int32_t *data, const uint8_t* packet);
            uint8_t MPUdmpGet6AxisQuaternion(int16_t *data, const uint8_t* packet);
            uint8_t MPUdmpGet6AxisQuaternion(Quaternion *q, const uint8_t* packet);
            uint8_t MPUdmpGetRelativeQuaternion(int32_t *data, const uint8_t* packet);
            uint8_t MPUdmpGetRelativeQuaternion(int16_t *data, const uint8_t* packet);
            uint8_t MPUdmpGetRelativeQuaternion(Quaternion *data, const uint8_t* packet);
            uint8_t MPUdmpGetGyro(int32_t *data, const uint8_t* packet);
            uint8_t MPUdmpGetGyro(int16_t *data, const uint8_t* packet);
            uint8_t MPUdmpGetGyro(VectorInt16 *v, const uint8_t* packet);
            uint8_t MPUdmpGetMag(int16_t *data, const uint8_t* packet);
            uint8_t MPUdmpSetLinearAccelFilterCoefficient(float coef);
            uint8_t MPUdmpGetLinearAccel(int32_t *data, const uint8_t* packet);
            uint8_t MPUdmpGetLinearAccel(int16_t *data, const uint8_t* packet);
            uint8_t MPUdmpGetLinearAccel(VectorInt16 *v, const uint8_t* packet);
            uint8_t MPUdmpGetLinearAccel(VectorInt16 *v, VectorInt16 *vRaw, VectorFloat *gravity);
            uint8_t MPUdmpGetLinearAccelInWorld(int32_t *data, const uint8_t* packet);
            uint8_t MPUdmpGetLinearAccelInWorld(int16_t *data, const uint8_t* packet);
            uint8_t MPUdmpGetLinearAccelInWorld(VectorInt16 *v, const uint8_t* packet);
            uint8_t MPUdmpGetLinearAccelInWorld(VectorInt16 *v, VectorInt16 *vReal, Quaternion *q);
            uint8_t MPUdmpGetGyroAndAccelSensor(int32_t *data, const uint8_t* packet);
            uint8_t MPUdmpGetGyroAndAccelSensor(int16_t *data, const uint8_t* packet);
            uint8_t MPUdmpGetGyroAndAccelSensor(VectorInt16 *g, VectorInt16 *a, const uint8_t* packet);
            uint8_t MPUdmpGetGyroSensor(int32_t *data, const uint8_t* packet);
            uint8_t MPUdmpGetGyroSensor(int16_t *data, const uint8_t* packet);
            uint8_t MPUdmpGetGyroSensor(VectorInt16 *v, const uint8_t* packet);
            uint8_t MPUdmpGetControlData(int32_t *data, const uint8_t* packet);
            uint8_t MPUdmpGetTemperature(int32_t *data, const uint8_t* packet);
            uint8_t MPUdmpGetGravity(int32_t *data, const uint8_t* packet);
            uint8_t MPUdmpGetGravity(int16_t *data, const uint8_t* packet);
            uint8_t MPUdmpGetGravity(VectorInt16 *v, const uint8_t* packet);
            uint8_t MPUdmpGetGravity(VectorFloat *v, Quaternion *q);
            uint8_t MPUdmpGetUnquantizedAccel(int32_t *data, const uint8_t* packet);
            uint8_t MPUdmpGetUnquantizedAccel(int16_t *data, const uint8_t* packet);
            uint8_t MPUdmpGetUnquantizedAccel(VectorInt16 *v, const uint8_t* packet);
            uint8_t MPUdmpGetQuantizedAccel(int32_t *data, const uint8_t* packet);
            uint8_t MPUdmpGetQuantizedAccel(int16_t *data, const uint8_t* packet);
            uint8_t MPUdmpGetQuantizedAccel(VectorInt16 *v, const uint8_t* packet);
            uint8_t MPUdmpGetExternalSensorData(int32_t *data, uint16_t size, const uint8_t* packet);
            uint8_t MPUdmpGetEIS(int32_t *data, const uint8_t* packet);
            
            uint8_t MPUdmpGetEuler(float *data, Quaternion *q);
            uint8_t MPUdmpGetYawPitchRoll(float *data, Quaternion *q, VectorFloat *gravity);

            // Get Floating Point data from FIFO
            uint8_t MPUdmpGetAccelFloat(float *data, const uint8_t* packet);
            uint8_t MPUdmpGetQuaternionFloat(float *data, const uint8_t* packet);

            uint8_t MPUdmpProcessFIFOPacket(const unsigned char *dmpData);
            uint8_t MPUdmpReadAndProcessFIFOPacket(uint8_t numPackets, uint8_t *processed=NULL);

            uint8_t MPUdmpSetFIFOProcessedCallback(void (*func) (void));

            uint8_t MPUdmpInitFIFOParam(void);
            uint8_t MPUdmpCloseFIFO(void);
            uint8_t MPUdmpSetGyroDataSource(uint8_t source);
            uint8_t MPUdmpDecodeQuantizedAccel(void);
            uint32_t MPUdmpGetGyroSumOfSquare(void);
            uint32_t MPUdmpGetAccelSumOfSquare(void);
            void MPUdmpOverrideQuaternion(long *q);
            uint16_t MPUdmpGetFIFOPacketSize(void);
        #endif

        extern uint8_t MPUdevAddr;
        extern uint8_t MPUbuffer[14];
				
				extern uint16_t MPUfifoCount;     	// count of all bytes currently in FIFO
				extern uint8_t  MPUfifoBuffer[64];	// FIFO storage buffer
				
				static uint8_t MPUverifyBuffer[MPU6050_DMP_MEMORY_CHUNK_SIZE];

#endif /* _MPU6050_H_ */
