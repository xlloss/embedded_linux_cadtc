export PATH=$PATH:/home/slash/project/rk3399/toolchain/gcc-linaro-6.3.1-2017.05-x86_64_aarch64-linux-gnu/bin
export ARCH=arm64
export CROSS_COMPILE=aarch64-linux-gnu-
export KERNELDIR=/home/slash/project/rk3399/kernel/rockchip-rk3399-nanopi-m4

# for u-boot
export ADDR2LINE_ARM64=aarch64-linux-gnu-addr2line
export OBJ_ARM64=aarch64-linux-gnu-objdump
export GCC_ARM64=aarch64-linux-gnu-
#make CROSS_COMPILE=${TOOLCHAIN_GCC} rk3399_defconfig
