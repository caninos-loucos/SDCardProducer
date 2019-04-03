# SDCardProducer
Software that install the Caninos Bootloader into compatible SD Card Disk Images.

## Description
1. It makes existing compatible sd card disk images bootable by any Caninos Board.
2. It turns existing compatible sd card disk images into flash images that will install it's contents to the core board's internal emmc chip as soon as the board is started in ADFU mode with the sd card inserted.
3. It writes the basic boot configuration to the disk image, such as: memory capacity, memory banks, memory bit width, memory type (lpddr2, ddr3, lpddr3), memory voltage, memory frequency, kernel version (3 or 4), amount of sectors to copy, default boot disk (sd or emmc) and execution mode (flash or normal boot).

# Installation

1. Install wx-widget (see below)
2. Run `make`
3. Run `linux/cardproducer`

#### Install wx-widget

If you are on a debian-based system, run:

```
sudo apt install libwxgtk3.0-dev
```

If you are on ubuntu, you may also need to install the following packages:

```
sudo apt-get install --reinstall overlay-scrollbar-gtk2
sudo apt-get install libcanberra-gtk-module
```

## What are compatible disk images?
1. The image file must have ".img" extension and be a RAW uncompressed disk image.
2. The image file must be formated with a MBR (not UEFI) and only have primary partitions (not logical partitions).
3. The image must have a partition marked as bootable and use LBA addressing instead of CHS.
4. The first partition must start at least at the 1MB disk mark (normaly it is the default configuration in any modern disk).
