#!/bin/sh

make -C ./sd_fuse

./sd_fuse/mkbl2 ./u-boot.bin bl2.bin 14336
cat ./bl2.bin ./u-boot.bin > bl2_uboot.bin
