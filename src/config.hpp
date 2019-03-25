#ifndef _CONFIG_HPP_
#define _CONFIG_HPP_

#include <wx/wx.h>
#include "enums.hpp"

class ConfigPanel : public wxPanel
{
public:
	ConfigPanel(wxWindow *parent);

	void UpdateAccordingPreset(int preset);
	
	void UpdateGUI();
	
	void DisableGUI();
	
	struct Preset
	{
		const wxChar *name;
		int memcfg[MEMCFG_COUNT];
		int bootcfg[BOOTCFG_COUNT];
	};
	
	struct Configuration
	{
		const wxChar *name;
	};
	
	struct ConfigDescriptor
	{
		const wxChar *text;
		int nchoices;
		const struct Configuration *data;
	};
	
	const struct Configuration kversions[KERN_COUNT] =
	{
		[KERN_VERSION3] = { .name = wxT("Kernel 3") },
		[KERN_VERSION4] = { .name = wxT("Kernel 4") },
	};
	
	const struct Configuration bootdisks[DISK_COUNT] =
	{
		[DISK_SDCARD] = { .name = wxT("SD Card")       },
		[DISK_EMMC  ] = { .name = wxT("Internal EMMC") },
	};

	const struct Configuration execmodes[MODE_COUNT] =
	{
		[MODE_NORMAL  ] = { .name = wxT("Normal")   },
		[MODE_FLASHING] = { .name = wxT("Flashing") },
	};
	
	const struct ConfigDescriptor bootcfg[BOOTCFG_COUNT] =
	{
		[BOOTCFG_KVERSION] = 
		{
			.text = wxT("Linux Cmdline Params"), 
			.nchoices = KERN_COUNT,
			.data = kversions,
		},
		
		[BOOTCFG_BOOTDISK] = 
		{
			.text = wxT("Default Boot Disk"), 
			.nchoices = DISK_COUNT,
			.data = bootdisks,
		},
		
		[BOOTCFG_EXECMODE] = 
		{
			.text = wxT("Execution Mode"), 
			.nchoices = MODE_COUNT,
			.data = execmodes,
		},
	};
	
	const struct Configuration memtypes[MEM_TYPE_COUNT] =
	{
		[MEM_TYPE_LPDDR2] = { .name = wxT("LPDDR2") },
		[MEM_TYPE_DDR3  ] = { .name = wxT("DDR3"  ) },
		[MEM_TYPE_LPDDR3] = { .name = wxT("LPDDR3") },
	};

	const struct Configuration memcaps[MEM_CAP_COUNT] =
	{
		[MEM_CAP_128MB ] = { .name = wxT("128MB" ) },
		[MEM_CAP_256MB ] = { .name = wxT("256MB" ) },
		[MEM_CAP_512MB ] = { .name = wxT("512MB" ) },
		[MEM_CAP_1024MB] = { .name = wxT("1024MB") },
		[MEM_CAP_2048MB] = { .name = wxT("2048MB") },
	};

	const struct Configuration membanks[MEM_BANK_COUNT] =
	{
		[MEM_BANK_SINGLE] = { .name = wxT("Single") },
		[MEM_BANK_DOUBLE] = { .name = wxT("Double") },
	};

	const struct Configuration memwidth[MEM_WIDTH_COUNT] =
	{
		[MEM_WIDTH_16BITS] = { .name = wxT("16 bits") },
		[MEM_WIDTH_32BITS] = { .name = wxT("32 bits") },
	};

	const struct Configuration memvoltage[MEM_VOLTAGE_COUNT] =
	{
		[MEM_VOLTAGE_1_00] = { .name = wxT("1.00V") },
		[MEM_VOLTAGE_1_05] = { .name = wxT("1.05V") },
		[MEM_VOLTAGE_1_10] = { .name = wxT("1.10V") },
		[MEM_VOLTAGE_1_15] = { .name = wxT("1.15V") },
		[MEM_VOLTAGE_1_20] = { .name = wxT("1.20V") },
		[MEM_VOLTAGE_1_25] = { .name = wxT("1.25V") },
		[MEM_VOLTAGE_1_30] = { .name = wxT("1.30V") },
		[MEM_VOLTAGE_1_35] = { .name = wxT("1.35V") },
		[MEM_VOLTAGE_1_40] = { .name = wxT("1.40V") },
		[MEM_VOLTAGE_1_45] = { .name = wxT("1.45V") },
		[MEM_VOLTAGE_1_50] = { .name = wxT("1.50V") },
		[MEM_VOLTAGE_1_55] = { .name = wxT("1.55V") },
		[MEM_VOLTAGE_1_60] = { .name = wxT("1.60V") },
		[MEM_VOLTAGE_1_65] = { .name = wxT("1.65V") },
		[MEM_VOLTAGE_1_70] = { .name = wxT("1.70V") },
		[MEM_VOLTAGE_1_75] = { .name = wxT("1.75V") },
		[MEM_VOLTAGE_1_80] = { .name = wxT("1.80V") },
		[MEM_VOLTAGE_1_85] = { .name = wxT("1.85V") },
		[MEM_VOLTAGE_1_90] = { .name = wxT("1.90V") },
		[MEM_VOLTAGE_1_95] = { .name = wxT("1.95V") },
		[MEM_VOLTAGE_2_00] = { .name = wxT("2.00V") },
	};
	
	const struct Configuration memfreq[MEM_FREQ_COUNT] =
	{
		[MEM_FREQ_156MHZ] = { .name = wxT("156MHz") },
		[MEM_FREQ_396MHZ] = { .name = wxT("396MHz") },
		[MEM_FREQ_528MHZ] = { .name = wxT("528MHz") },
		[MEM_FREQ_600MHZ] = { .name = wxT("600Mhz") },
	};
	
	const struct ConfigDescriptor memcfg[MEMCFG_COUNT] =
	{
		[MEMCFG_TYPE] = 
		{
			.text = wxT("Memory Type"), 
			.nchoices = MEM_TYPE_COUNT,
			.data = memtypes,
		},
		
		[MEMCFG_CAP] = 
		{
			.text = wxT("Memory Capacity"), 
			.nchoices = MEM_CAP_COUNT,
			.data = memcaps,
		},
		
		[MEMCFG_BANK] = 
		{
			.text = wxT("Memory Bank"), 
			.nchoices = MEM_BANK_COUNT,
			.data = membanks,
		},
		
		[MEMCFG_BIT] = 
		{
			.text = wxT("Memory Bus Width"), 
			.nchoices = MEM_WIDTH_COUNT,
			.data = memwidth,
		},
		
		[MEMCFG_VOLT] = 
		{
			.text = wxT("Memory Voltage"), 
			.nchoices = MEM_VOLTAGE_COUNT,
			.data = memvoltage,
		},
		
		[MEMCFG_FREQ] = 
		{
			.text = wxT("Memory Frequency"), 
			.nchoices = MEM_FREQ_COUNT,
			.data = memfreq,
		},
	};
	
	const struct Preset mypresets[PRESET_COUNT] =
	{
		[PRESET_COREV1_NORMAL] =
		{
			.name = wxT("CoreV1 Normal"),
			.memcfg =
			{
				[MEMCFG_TYPE ] = MEM_TYPE_DDR3,
				[MEMCFG_CAP  ] = MEM_CAP_2048MB,
				[MEMCFG_BANK ] = MEM_BANK_DOUBLE,
				[MEMCFG_BIT  ] = MEM_WIDTH_32BITS,
				[MEMCFG_VOLT ] = MEM_VOLTAGE_1_40,
				[MEMCFG_FREQ ] = MEM_FREQ_396MHZ,
			},
			.bootcfg =
			{
				[BOOTCFG_KVERSION] = KERN_VERSION3,
				[BOOTCFG_BOOTDISK] = DISK_SDCARD,
				[BOOTCFG_EXECMODE] = MODE_NORMAL,
			},
		},
	
		[PRESET_COREV1_FLASHING] =
		{
			.name = wxT("CoreV1 Flashing"),
			.memcfg =
			{
				[MEMCFG_TYPE ] = MEM_TYPE_DDR3,
				[MEMCFG_CAP  ] = MEM_CAP_2048MB,
				[MEMCFG_BANK ] = MEM_BANK_DOUBLE,
				[MEMCFG_BIT  ] = MEM_WIDTH_32BITS,
				[MEMCFG_VOLT ] = MEM_VOLTAGE_1_40,
				[MEMCFG_FREQ ] = MEM_FREQ_396MHZ,
			},
			.bootcfg =
			{
				[BOOTCFG_KVERSION] = KERN_VERSION3,
				[BOOTCFG_BOOTDISK] = DISK_SDCARD,
				[BOOTCFG_EXECMODE] = MODE_FLASHING,
			},
		},
	
		[PRESET_COREV2_NORMAL] =
		{
			.name = wxT("CoreV2 Normal"),
			.memcfg =
			{
				[MEMCFG_TYPE ] = MEM_TYPE_LPDDR3,
				[MEMCFG_CAP  ] = MEM_CAP_2048MB,
				[MEMCFG_BANK ] = MEM_BANK_DOUBLE,
				[MEMCFG_BIT  ] = MEM_WIDTH_32BITS,
				[MEMCFG_VOLT ] = MEM_VOLTAGE_1_25,
				[MEMCFG_FREQ ] = MEM_FREQ_528MHZ,
			},
			.bootcfg =
			{
				[BOOTCFG_KVERSION] = KERN_VERSION3,
				[BOOTCFG_BOOTDISK] = DISK_SDCARD,
				[BOOTCFG_EXECMODE] = MODE_NORMAL,
			},
		},
	
		[PRESET_COREV2_FLASHING] =
		{
			.name = wxT("CoreV2 Flashing"),
			.memcfg =
			{
				[MEMCFG_TYPE ] = MEM_TYPE_LPDDR3,
				[MEMCFG_CAP  ] = MEM_CAP_2048MB,
				[MEMCFG_BANK ] = MEM_BANK_DOUBLE,
				[MEMCFG_BIT  ] = MEM_WIDTH_32BITS,
				[MEMCFG_VOLT ] = MEM_VOLTAGE_1_25,
				[MEMCFG_FREQ ] = MEM_FREQ_528MHZ,
			},
			.bootcfg =
			{
				[BOOTCFG_KVERSION] = KERN_VERSION3,
				[BOOTCFG_BOOTDISK] = DISK_SDCARD,
				[BOOTCFG_EXECMODE] = MODE_FLASHING,
			},
		},
	
		[PRESET_CUSTOM] =
		{
			.name = wxT("Custom"),
		},
	};
	
private:
	wxChoice *presetChoice;
	wxStaticText *presetText;
	wxPanel *presetPanel;
	
	wxChoice *memcfgChoice[MEMCFG_COUNT];
	wxStaticText *memcfgText[MEMCFG_COUNT];
	wxPanel *memcfgPanel[MEMCFG_COUNT];
	
	wxChoice *bootcfgChoice[BOOTCFG_COUNT];
	wxStaticText *bootcfgText[BOOTCFG_COUNT];
	wxPanel *bootcfgPanel[BOOTCFG_COUNT];
	
	wxPanel *leftPanel;
	wxPanel *rightPanel;
	
	void OnChoice(wxCommandEvent& event);
	
	int MatchPreset();
	
	wxPanel *CreatePresetPanel(wxWindow *parent, int &maxSize);
	wxPanel *CreateMemConfigPanel(wxWindow *parent, int &maxSize, int index);
	wxPanel *CreateBootConfigPanel(wxWindow *parent, int &maxSize, int index);
};

#endif

