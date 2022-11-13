#
CROSS_COMPILE=/home/slash/project/rk3399/linuxsdk-friendlyelec/buildroot/output/rockchip_rk3399/host/bin/aarch64-linux-
PKG_CONFIG=/home/slash/project/rk3399/linuxsdk-friendlyelec/buildroot/output/rockchip_rk3399/host/bin/pkg-config
all:
	${CROSS_COMPILE}gcc -o overscan overscan.c `${PKG_CONFIG} --cflags --libs libdrm` -Wall -O0 -g
