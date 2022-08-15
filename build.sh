#!/bin/bash

. edksetup.sh BaseTools

rm -rf build/*

export GCC5_ARM_PREFIX=arm-linux-gnueabihf-

build -a ARM -t GCC5 -p TrollSurface/TrollSurface.dsc

cp -v Build/EfiApps/DEBUG_GCC5/ARM/TrollSurface/TrollSurface/OUTPUT/MyHelloWorld.efi image/bootarm.efi
dd if=/dev/zero of=image/trollsurface.img bs=512 count=93750
mformat -i image/trollsurface.img -v "SeshOS" -f 1440 ::
mmd -i image/trollsurface.img ::/EFI
mmd -i image/trollsurface.img ::/EFI/BOOT
mcopy -i image/trollsurface.img image/bootarm.efi ::/EFI/BOOT
#
#
#qemu-system-x86_64 -bios /usr/share/ovmf/OVMF.fd -hda image/trollsurface.img