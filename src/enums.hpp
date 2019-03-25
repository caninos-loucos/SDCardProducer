#ifndef _ENUMS_HPP_
#define _ENUMS_HPP_

enum
{
	MEMCFG_TYPE  = 0,
	MEMCFG_CAP   = 1,
	MEMCFG_BANK  = 2,
	MEMCFG_BIT   = 3,
	MEMCFG_VOLT  = 4,
	MEMCFG_FREQ  = 5,
	MEMCFG_COUNT = 6,
};

enum
{
	BOOTCFG_KVERSION = 0,
	BOOTCFG_BOOTDISK = 1,
	BOOTCFG_EXECMODE = 2,
	BOOTCFG_COUNT    = 3,
};

enum
{
	KERN_VERSION3 = 0,
	KERN_VERSION4 = 1,
	KERN_COUNT    = 2,
};

enum
{
	DISK_SDCARD = 0,
	DISK_EMMC   = 1,
	DISK_COUNT  = 2,
};

enum
{
	MODE_NORMAL   = 0,
	MODE_FLASHING = 1,
	MODE_COUNT    = 2,
};

enum
{
	PRESET_COREV1_NORMAL   = 0,
	PRESET_COREV1_FLASHING = 1,
	PRESET_COREV2_NORMAL   = 2,
	PRESET_COREV2_FLASHING = 3,
	PRESET_CUSTOM          = 4,
	PRESET_COUNT           = 5,
};

enum
{
	MEM_TYPE_LPDDR2 = 0,
	MEM_TYPE_DDR3   = 1,
	MEM_TYPE_LPDDR3 = 2,
	MEM_TYPE_COUNT  = 3,
};

enum
{
	MEM_CAP_128MB  = 0,
	MEM_CAP_256MB  = 1,
	MEM_CAP_512MB  = 2,
	MEM_CAP_1024MB = 3,
	MEM_CAP_2048MB = 4,
	MEM_CAP_COUNT  = 5,
};

enum
{
	MEM_BANK_SINGLE = 0,
	MEM_BANK_DOUBLE = 1,
	MEM_BANK_COUNT  = 2,
};

enum
{
	MEM_WIDTH_16BITS = 0,
	MEM_WIDTH_32BITS = 1,
	MEM_WIDTH_COUNT  = 2,
};

enum
{
	MEM_VOLTAGE_1_00  = 0,
	MEM_VOLTAGE_1_05  = 1,
	MEM_VOLTAGE_1_10  = 2,
	MEM_VOLTAGE_1_15  = 3,
	MEM_VOLTAGE_1_20  = 4,
	MEM_VOLTAGE_1_25  = 5,
	MEM_VOLTAGE_1_30  = 6,
	MEM_VOLTAGE_1_35  = 7,
	MEM_VOLTAGE_1_40  = 8,
	MEM_VOLTAGE_1_45  = 9,
	MEM_VOLTAGE_1_50  = 10,
	MEM_VOLTAGE_1_55  = 11,
	MEM_VOLTAGE_1_60  = 12,
	MEM_VOLTAGE_1_65  = 13,
	MEM_VOLTAGE_1_70  = 14,
	MEM_VOLTAGE_1_75  = 15,
	MEM_VOLTAGE_1_80  = 16,
	MEM_VOLTAGE_1_85  = 17,
	MEM_VOLTAGE_1_90  = 18,
	MEM_VOLTAGE_1_95  = 19,
	MEM_VOLTAGE_2_00  = 20,
	MEM_VOLTAGE_COUNT = 21,
};
	
enum
{
	MEM_FREQ_156MHZ = 0,
	MEM_FREQ_396MHZ = 1,
	MEM_FREQ_528MHZ = 2,
	MEM_FREQ_600MHZ = 3,
	MEM_FREQ_COUNT  = 4,
};

#endif

