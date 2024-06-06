diff --git a/board/microchip/sam9x60_curiosity/fs-overlay/init b/board/microchip/sam9x60_curiosity/fs-overlay/init
new file mode 100755
index 0000000..8922c55
--- /dev/null
+++ b/board/microchip/sam9x60_curiosity/fs-overlay/init
@@ -0,0 +1,9 @@
+#!/bin/sh
+
+mount -t devtmpfs none /dev
+
+#echo Launch EGT
+cd /root
+devmem 0xF8038364 w 0x1
+./motorcycledash > /dev/ttyS0 &
+/sbin/getty -L console 0 vt100
diff --git a/board/microchip/sam9x60_curiosity/linux.config b/board/microchip/sam9x60_curiosity/linux.config
index a0fe5ce..3c02111 100644
--- a/board/microchip/sam9x60_curiosity/linux.config
+++ b/board/microchip/sam9x60_curiosity/linux.config
@@ -1,13 +1,21 @@
-# CONFIG_SWAP is not set
 CONFIG_SYSVIPC=y
 CONFIG_NO_HZ_IDLE=y
 CONFIG_HIGH_RES_TIMERS=y
-CONFIG_LOG_BUF_SHIFT=16
 CONFIG_CGROUPS=y
 CONFIG_CGROUP_CPUACCT=y
 CONFIG_BLK_DEV_INITRD=y
+CONFIG_INITRAMFS_SOURCE="${BR_BINARIES_DIR}/rootfs.cpio"
+# CONFIG_RD_GZIP is not set
+# CONFIG_RD_BZIP2 is not set
+# CONFIG_RD_LZMA is not set
+# CONFIG_RD_XZ is not set
+# CONFIG_RD_LZO is not set
+# CONFIG_RD_LZ4 is not set
+# CONFIG_RD_ZSTD is not set
+# CONFIG_PRINTK is not set
+# CONFIG_BUG is not set
+# CONFIG_KALLSYMS is not set
 CONFIG_EMBEDDED=y
-CONFIG_SLAB=y
 CONFIG_ARCH_MULTI_V4T=y
 CONFIG_ARCH_MULTI_V5=y
 # CONFIG_ARCH_MULTI_V7 is not set
@@ -15,7 +23,6 @@ CONFIG_ARCH_AT91=y
 CONFIG_SOC_AT91RM9200=y
 CONFIG_SOC_AT91SAM9=y
 CONFIG_SOC_SAM9X60=y
-CONFIG_SOC_SAM9X7=y
 # CONFIG_ATMEL_CLOCKSOURCE_PIT is not set
 CONFIG_AEABI=y
 CONFIG_UACCESS_WITH_MEMCPY=y
@@ -26,270 +33,52 @@ CONFIG_CPU_IDLE=y
 # CONFIG_ARM_AT91_CPUIDLE is not set
 CONFIG_PM_DEBUG=y
 CONFIG_PM_ADVANCED_DEBUG=y
-CONFIG_MODULES=y
-CONFIG_MODULE_UNLOAD=y
-# CONFIG_BLK_DEV_BSG is not set
-# CONFIG_CORE_DUMP_DEFAULT_ELF_HEADERS is not set
-CONFIG_NET=y
-CONFIG_PACKET=y
-CONFIG_UNIX=y
-CONFIG_INET=y
-CONFIG_IP_MULTICAST=y
-CONFIG_IP_PNP=y
-CONFIG_IP_PNP_DHCP=y
-# CONFIG_INET_DIAG is not set
-CONFIG_IPV6_SIT_6RD=y
-CONFIG_BRIDGE=m
-CONFIG_BRIDGE_VLAN_FILTERING=y
-CONFIG_NET_DSA=m
-CONFIG_NET_DSA_TAG_OCELOT=m
-CONFIG_NET_DSA_TAG_OCELOT_8021Q=m
-CONFIG_VLAN_8021Q=m
-CONFIG_CAN=y
-CONFIG_CAN_AT91=y
-CONFIG_CAN_M_CAN=y
-CONFIG_CAN_M_CAN_PLATFORM=y
-CONFIG_BT=y
-CONFIG_BT_RFCOMM=y
-CONFIG_BT_RFCOMM_TTY=y
-CONFIG_BT_BNEP=y
-CONFIG_BT_BNEP_MC_FILTER=y
-CONFIG_BT_BNEP_PROTO_FILTER=y
-CONFIG_BT_HIDP=y
-CONFIG_BT_HCIBTUSB=y
-CONFIG_BT_HCIUART=y
-CONFIG_BT_HCIUART_H4=y
-CONFIG_BT_HCIUART_3WIRE=y
-CONFIG_BT_HCIVHCI=y
-CONFIG_CFG80211=m
-# CONFIG_CFG80211_DEFAULT_PS is not set
-CONFIG_CFG80211_DEBUGFS=y
-CONFIG_CFG80211_WEXT=y
-CONFIG_MAC80211=m
-CONFIG_RFKILL=y
-CONFIG_RFKILL_INPUT=y
+# CONFIG_GCC_PLUGINS is not set
+# CONFIG_COREDUMP is not set
+CONFIG_SLAB=y
 CONFIG_DEVTMPFS=y
 CONFIG_DEVTMPFS_MOUNT=y
 # CONFIG_STANDALONE is not set
 # CONFIG_PREVENT_FIRMWARE_BUILD is not set
-CONFIG_MTD=y
-CONFIG_MTD_TESTS=m
-CONFIG_MTD_CMDLINE_PARTS=y
-CONFIG_MTD_CFI=y
-CONFIG_MTD_DATAFLASH=y
-CONFIG_MTD_RAW_NAND=y
-CONFIG_MTD_NAND_ATMEL=y
-CONFIG_MTD_SPI_NOR=y
-CONFIG_MTD_UBI=y
-CONFIG_MTD_UBI_FASTMAP=y
-CONFIG_MTD_UBI_BLOCK=y
-CONFIG_BLK_DEV_LOOP=y
-CONFIG_BLK_DEV_RAM=y
-CONFIG_BLK_DEV_RAM_COUNT=4
-CONFIG_BLK_DEV_RAM_SIZE=8192
-CONFIG_ATMEL_SSC=y
-CONFIG_EEPROM_AT24=y
-CONFIG_SCSI=y
-CONFIG_BLK_DEV_SD=y
-# CONFIG_SCSI_LOWLEVEL is not set
-CONFIG_NETDEVICES=y
-CONFIG_NET_DSA_MICROCHIP_KSZ_COMMON=m
-CONFIG_NET_DSA_MICROCHIP_KSZ9477_I2C=m
-CONFIG_NET_DSA_MICROCHIP_KSZ_SPI=m
-CONFIG_NET_DSA_MICROCHIP_KSZ8863_SMI=m
-CONFIG_NET_DSA_SMSC_LAN9303_I2C=m
-CONFIG_NET_DSA_SMSC_LAN9303_MDIO=m
-CONFIG_NET_DSA_VITESSE_VSC73XX_SPI=m
-CONFIG_NET_DSA_VITESSE_VSC73XX_PLATFORM=m
-# CONFIG_NET_VENDOR_BROADCOM is not set
-CONFIG_MACB=y
-CONFIG_DM9000=y
-# CONFIG_NET_VENDOR_FARADAY is not set
-# CONFIG_NET_VENDOR_INTEL is not set
-# CONFIG_NET_VENDOR_MARVELL is not set
-CONFIG_KS8842=m
-CONFIG_KS8851=m
-CONFIG_KS8851_MLL=m
-CONFIG_ENC28J60=m
-CONFIG_ENCX24J600=m
-# CONFIG_NET_VENDOR_MICROSOFT is not set
-# CONFIG_NET_VENDOR_NI is not set
-# CONFIG_NET_VENDOR_NATSEMI is not set
-# CONFIG_NET_VENDOR_SEEQ is not set
-CONFIG_SMC91X=m
-CONFIG_SMC911X=m
-CONFIG_SMSC911X=m
-# CONFIG_NET_VENDOR_STMICRO is not set
-CONFIG_DAVICOM_PHY=y
-CONFIG_MICREL_PHY=y
-CONFIG_MWIFIEX=m
-CONFIG_MWIFIEX_SDIO=m
-CONFIG_MWIFIEX_USB=m
-CONFIG_WILC_SDIO=m
-CONFIG_WILC_SPI=m
-CONFIG_RT2X00=m
-CONFIG_RT2500USB=m
-CONFIG_RT73USB=m
-CONFIG_RT2800USB=m
-CONFIG_RT2800USB_RT53XX=y
-CONFIG_RT2800USB_RT55XX=y
-CONFIG_RT2800USB_UNKNOWN=y
-CONFIG_MDIO_MSCC_MIIM=m
-CONFIG_USB_LAN78XX=m
-CONFIG_RTL8187=m
-CONFIG_INPUT_POLLDEV=y
-CONFIG_INPUT_JOYDEV=y
-CONFIG_INPUT_EVDEV=y
-# CONFIG_KEYBOARD_ATKBD is not set
-CONFIG_KEYBOARD_QT1070=y
-CONFIG_KEYBOARD_GPIO=y
+# CONFIG_ALLOW_DEV_COREDUMP is not set
+# CONFIG_INPUT_KEYBOARD is not set
 # CONFIG_INPUT_MOUSE is not set
-CONFIG_INPUT_TOUCHSCREEN=y
-CONFIG_TOUCHSCREEN_ADS7846=m
-CONFIG_TOUCHSCREEN_ATMEL_MXT=y
 # CONFIG_SERIO is not set
 CONFIG_LEGACY_PTY_COUNT=4
 CONFIG_SERIAL_ATMEL=y
 CONFIG_SERIAL_ATMEL_CONSOLE=y
 CONFIG_SERIAL_DEV_BUS=y
-CONFIG_HW_RANDOM=y
-CONFIG_I2C_CHARDEV=y
-CONFIG_I2C_AT91=y
-CONFIG_I2C_GPIO=y
-CONFIG_SPI=y
-CONFIG_SPI_ATMEL=y
-CONFIG_SPI_ATMEL_QUADSPI=y
-CONFIG_SPI_GPIO=y
-CONFIG_SPI_SPIDEV=m
-CONFIG_PINCTRL_MCP23S08=m
+# CONFIG_HW_RANDOM is not set
+# CONFIG_I2C_COMPAT is not set
+# CONFIG_I2C_HELPER_AUTO is not set
 CONFIG_GPIO_SYSFS=y
-CONFIG_GPIO_PCA953X=m
 CONFIG_POWER_RESET=y
-CONFIG_POWER_SUPPLY=y
-CONFIG_HWMON=m
-CONFIG_SENSORS_LM75=m
-CONFIG_WATCHDOG=y
-CONFIG_AT91SAM9X_WATCHDOG=y
-CONFIG_SAMA5D4_WATCHDOG=y
-CONFIG_MFD_ATMEL_FLEXCOM=y
+# CONFIG_HWMON is not set
 CONFIG_MFD_ATMEL_HLCDC=y
 CONFIG_REGULATOR=y
 CONFIG_REGULATOR_FIXED_VOLTAGE=y
-CONFIG_MEDIA_SUPPORT=y
-CONFIG_MEDIA_SUPPORT_FILTER=y
-# CONFIG_MEDIA_SUBDRV_AUTOSELECT is not set
-CONFIG_MEDIA_CAMERA_SUPPORT=y
-CONFIG_MEDIA_PLATFORM_SUPPORT=y
-CONFIG_MEDIA_USB_SUPPORT=y
-CONFIG_USB_VIDEO_CLASS=m
-CONFIG_V4L_PLATFORM_DRIVERS=y
-CONFIG_VIDEO_ATMEL_ISI=y
-CONFIG_VIDEO_MICROCHIP_CSI2DC=y
-CONFIG_DWC_MIPI_CSI2_HOST=m
-CONFIG_DWC_MIPI_DPHY_GEN3=m
-CONFIG_VIDEO_IMX219=m
-CONFIG_VIDEO_OV2640=m
-CONFIG_VIDEO_OV7740=m
-CONFIG_VIDEO_MT9V032=m
 CONFIG_DRM=y
 CONFIG_DRM_ATMEL_HLCDC=y
 CONFIG_DRM_PANEL_SIMPLE=y
 CONFIG_FB_ATMEL=y
 CONFIG_BACKLIGHT_ATMEL_LCDC=y
-CONFIG_BACKLIGHT_CLASS_DEVICE=y
 CONFIG_BACKLIGHT_PWM=y
-CONFIG_SOUND=y
-CONFIG_SND=y
-CONFIG_SND_USB_AUDIO=y
-CONFIG_SND_SOC=y
-CONFIG_SND_ATMEL_SOC=y
-CONFIG_SND_AT91_SOC_SAM9G20_WM8731=y
-CONFIG_SND_ATMEL_SOC_WM8904=y
-CONFIG_SND_AT91_SOC_SAM9X5_WM8731=y
-CONFIG_SND_ATMEL_SOC_CLASSD=y
-CONFIG_SND_SOC_MIKROE_PROTO=m
-CONFIG_SND_MCHP_SOC_I2S_MCC=y
-CONFIG_USB=y
-CONFIG_USB_ANNOUNCE_NEW_DEVICES=y
-CONFIG_USB_EHCI_HCD=y
-CONFIG_USB_OHCI_HCD=y
-CONFIG_USB_ACM=y
-CONFIG_USB_STORAGE=y
-CONFIG_USB_SERIAL=y
-CONFIG_USB_SERIAL_GENERIC=y
-CONFIG_USB_SERIAL_FTDI_SIO=y
-CONFIG_USB_SERIAL_PL2303=y
-CONFIG_USB_GADGET=y
-CONFIG_USB_ATMEL_USBA=m
-CONFIG_USB_CONFIGFS=m
-CONFIG_USB_CONFIGFS_ACM=y
-CONFIG_USB_CONFIGFS_RNDIS=y
-CONFIG_USB_CONFIGFS_MASS_STORAGE=y
-CONFIG_USB_ZERO=m
-CONFIG_USB_ETH=m
-CONFIG_USB_MASS_STORAGE=m
-CONFIG_USB_G_SERIAL=m
-CONFIG_MMC=y
-CONFIG_PWRSEQ_SD8787=m
-CONFIG_MMC_SDHCI=y
-CONFIG_MMC_SDHCI_PLTFM=y
-CONFIG_MMC_SDHCI_OF_AT91=y
-CONFIG_MMC_ATMELMCI=y
-CONFIG_MMC_SPI=y
-CONFIG_NEW_LEDS=y
-CONFIG_LEDS_CLASS=y
-CONFIG_LEDS_GPIO=y
-CONFIG_LEDS_PWM=y
-CONFIG_LEDS_TRIGGERS=y
-CONFIG_LEDS_TRIGGER_TIMER=y
-CONFIG_LEDS_TRIGGER_HEARTBEAT=y
-CONFIG_LEDS_TRIGGER_CPU=y
-CONFIG_LEDS_TRIGGER_GPIO=y
-CONFIG_RTC_CLASS=y
-CONFIG_RTC_DRV_AT91RM9200=y
-CONFIG_RTC_DRV_AT91SAM9=y
+# CONFIG_HID is not set
+# CONFIG_USB_SUPPORT is not set
 CONFIG_DMADEVICES=y
 CONFIG_AT_HDMAC=y
 CONFIG_AT_XDMAC=y
-CONFIG_MICROCHIP_PIT64B=y
+# CONFIG_VIRTIO_MENU is not set
+# CONFIG_VHOST_MENU is not set
 # CONFIG_IOMMU_SUPPORT is not set
-CONFIG_IIO=y
-CONFIG_AT91_ADC=y
-CONFIG_AT91_SAMA5D2_ADC=y
+# CONFIG_ATMEL_EBI is not set
 CONFIG_PWM=y
-CONFIG_PWM_ATMEL=y
 CONFIG_PWM_ATMEL_HLCDC_PWM=y
-CONFIG_PWM_ATMEL_TCB=y
-CONFIG_EXT4_FS=y
-CONFIG_FANOTIFY=y
-CONFIG_VFAT_FS=y
+# CONFIG_FILE_LOCKING is not set
+# CONFIG_DNOTIFY is not set
 CONFIG_TMPFS=y
-CONFIG_CONFIGFS_FS=y
-CONFIG_UBIFS_FS=y
-CONFIG_UBIFS_FS_ADVANCED_COMPR=y
-CONFIG_NFS_FS=y
-CONFIG_ROOT_NFS=y
-CONFIG_NLS_CODEPAGE_437=y
-CONFIG_NLS_CODEPAGE_850=y
-CONFIG_NLS_ISO8859_1=y
-CONFIG_NLS_UTF8=y
-CONFIG_CRYPTO_CCM=y
-CONFIG_CRYPTO_GCM=y
-CONFIG_CRYPTO_CBC=y
-CONFIG_CRYPTO_CFB=y
-CONFIG_CRYPTO_OFB=y
-CONFIG_CRYPTO_XTS=y
-CONFIG_CRYPTO_HMAC=y
-CONFIG_CRYPTO_SHA1=y
-CONFIG_CRYPTO_SHA512=y
-CONFIG_CRYPTO_ARC4=y
-CONFIG_CRYPTO_DES=y
-CONFIG_CRYPTO_USER_API_HASH=m
-CONFIG_CRYPTO_USER_API_SKCIPHER=m
-CONFIG_CRYPTO_DEV_ATMEL_AES=y
-CONFIG_CRYPTO_DEV_ATMEL_TDES=y
-CONFIG_CRYPTO_DEV_ATMEL_SHA=y
-CONFIG_CRC_CCITT=y
+# CONFIG_MISC_FILESYSTEMS is not set
+CONFIG_SYMBOLIC_ERRNAME=y
 CONFIG_STRIP_ASM_SYMS=y
 CONFIG_DEBUG_FS=y
 # CONFIG_SCHED_DEBUG is not set
diff --git a/configs/sam9x60_curiosity_graphics_fastboot_defconfig b/configs/sam9x60_curiosity_graphics_fastboot_defconfig
new file mode 100644
index 0000000..fa3ab89
--- /dev/null
+++ b/configs/sam9x60_curiosity_graphics_fastboot_defconfig
@@ -0,0 +1,48 @@
+BR2_arm=y
+BR2_CCACHE=y
+BR2_OPTIMIZE_FAST=y
+BR2_STATIC_LIBS=y
+BR2_GLOBAL_PATCH_DIR="$(BR2_EXTERNAL_MCHP_PATH)/patches"
+BR2_TOOLCHAIN_BUILDROOT_MUSL=y
+BR2_TOOLCHAIN_BUILDROOT_CXX=y
+BR2_PACKAGE_HOST_GDB=y
+BR2_PACKAGE_HOST_GDB_TUI=y
+BR2_PACKAGE_HOST_GDB_PYTHON3=y
+BR2_PACKAGE_HOST_GDB_SIM=y
+BR2_TARGET_GENERIC_HOSTNAME="sam9x60_curiosity"
+BR2_TARGET_GENERIC_ISSUE="Welcome to the Microchip SAM9X60 CURIOSITY Demo"
+BR2_ENABLE_LOCALE_WHITELIST=""
+BR2_ROOTFS_OVERLAY="$(BR2_EXTERNAL_MCHP_PATH)/board/microchip/sam9x60_curiosity/fs-overlay/"
+BR2_LINUX_KERNEL=y
+BR2_LINUX_KERNEL_CUSTOM_TARBALL=y
+BR2_LINUX_KERNEL_CUSTOM_TARBALL_LOCATION="$(call github,linux4microchip,linux,linux4microchip-2023.04)/linux-linux4microchip-2023.04.tar.gz"
+BR2_LINUX_KERNEL_USE_CUSTOM_CONFIG=y
+BR2_LINUX_KERNEL_CUSTOM_CONFIG_FILE="$(BR2_EXTERNAL_MCHP_PATH)/board/microchip/sam9x60_curiosity/linux.config"
+BR2_LINUX_KERNEL_DTS_SUPPORT=y
+BR2_LINUX_KERNEL_INTREE_DTS_NAME="at91-sam9x60_curiosity"
+# BR2_PACKAGE_LUA_32BITS is not set
+BR2_PACKAGE_CAIRO_PNG=y
+BR2_PACKAGE_FREETYPE=y
+# BR2_PACKAGE_IFUPDOWN_SCRIPTS is not set
+# BR2_PACKAGE_URANDOM_SCRIPTS is not set
+BR2_TARGET_ROOTFS_INITRAMFS=y
+BR2_TARGET_AT91BOOTSTRAP3=y
+BR2_TARGET_AT91BOOTSTRAP3_CUSTOM_GIT=y
+BR2_TARGET_AT91BOOTSTRAP3_CUSTOM_REPO_URL="https://github.com/MCHP-MPU-Solutions-SHA/at91bootstrap.git"
+BR2_TARGET_AT91BOOTSTRAP3_CUSTOM_REPO_VERSION="v4.0.6_fastboot"
+BR2_TARGET_AT91BOOTSTRAP3_DEFCONFIG="sam9x60_curiositynf_linux_image_dt_fastboot_initramfs"
+BR2_PACKAGE_HOST_DOSFSTOOLS=y
+BR2_PACKAGE_HOST_E2FSPROGS=y
+BR2_PACKAGE_HOST_GENIMAGE=y
+BR2_PACKAGE_HOST_MTOOLS=y
+BR2_PACKAGE_LIBM2D=y
+BR2_PACKAGE_LIBPLANES=y
+# BR2_PACKAGE_LIBPLANES_INSTALL_EXAMPLES is not set
+BR2_PACKAGE_EGT=y
+# BR2_PACKAGE_EGT_MEDIA_SUPPORT is not set
+# BR2_PACKAGE_EGT_CHART_SUPPORT is not set
+# BR2_PACKAGE_EGT_SVG_SUPPORT is not set
+# BR2_PACKAGE_EGT_FONTCONFIG_SUPPORT is not set
+# BR2_PACKAGE_EGT_INSTALL_EXAMPLES is not set
+# BR2_PACKAGE_EGT_INSTALL_ICONS is not set
+BR2_PACKAGE_EGT_EXTRA_CONFIG_OPTIONS="--without-readline --without-filesystem --without-librsvg --without-lua --enable-svgdeserial"
diff --git a/package/egt/egt.mk b/package/egt/egt.mk
index 7f57bdc..a08cfb1 100644
--- a/package/egt/egt.mk
+++ b/package/egt/egt.mk
@@ -4,8 +4,8 @@
 #
 ################################################################################
 
-EGT_VERSION = 1.7
-EGT_SITE = https://github.com/linux4sam/egt.git
+EGT_VERSION = br1.7
+EGT_SITE = https://github.com/MCHP-MPU-Solutions-SHA/egt.git
 EGT_SITE_METHOD = git
 EGT_GIT_SUBMODULES = YES
 EGT_LICENSE = Apache-2.0
diff --git a/patches/linux/0000-linux-sam9x60-curiosity-fastboot-based-on-linux4microchip-2023.04.patch b/patches/linux/0000-linux-sam9x60-curiosity-fastboot-based-on-linux4microchip-2023.04.patch
new file mode 100644
index 0000000..7c1877b
--- /dev/null
+++ b/patches/linux/0000-linux-sam9x60-curiosity-fastboot-based-on-linux4microchip-2023.04.patch
@@ -0,0 +1,273 @@
+diff --git a/arch/arm/boot/dts/at91-sam9x60_curiosity.dts b/arch/arm/boot/dts/at91-sam9x60_curiosity.dts
+index a2e3277297d4..cc09071ae444 100644
+--- a/arch/arm/boot/dts/at91-sam9x60_curiosity.dts
++++ b/arch/arm/boot/dts/at91-sam9x60_curiosity.dts
+@@ -138,12 +138,12 @@ &dbgu {
+ &ebi {
+ 	pinctrl-names = "default";
+ 	pinctrl-0 = <&pinctrl_ebi_addr_nand &pinctrl_ebi_data_0_7>;
+-	status = "okay";
++	status = "disabled";
+ 
+ 	nand_controller: nand-controller {
+ 		pinctrl-names = "default";
+ 		pinctrl-0 = <&pinctrl_nand_oe_we &pinctrl_nand_cs &pinctrl_nand_rb>;
+-		status = "okay";
++		status = "disabled";
+ 
+ 		nand@3 {
+ 			reg = <0x3 0x0 0x800000>;
+@@ -166,34 +166,19 @@ at91bootstrap@0 {
+ 					reg = <0x0 0x40000>;
+ 				};
+ 
+-				uboot@40000 {
+-					label = "u-boot";
+-					reg = <0x40000 0xc0000>;
+-				};
+-
+-				ubootenvred@100000 {
+-					label = "U-Boot Env Redundant";
+-					reg = <0x100000 0x40000>;
+-				};
+-
+-				ubootenv@140000 {
+-					label = "U-Boot Env";
+-					reg = <0x140000 0x40000>;
+-				};
+-
+ 				dtb@180000 {
+ 					label = "device tree";
+-					reg = <0x180000 0x80000>;
++					reg = <0x40000 0x40000>;
+ 				};
+ 
+ 				kernel@200000 {
+ 					label = "kernel";
+-					reg = <0x200000 0x600000>;
++					reg = <0x80000 0x580000>;
+ 				};
+ 
+ 				rootfs@800000 {
+ 					label = "rootfs";
+-					reg = <0x800000 0x1f800000>;
++					reg = <0x600000 0x1000000>;
+ 				};
+ 			};
+ 		};
+@@ -535,3 +520,5 @@ &usb2 {
+ &watchdog {
+ 	status = "okay";
+ };
++
++#include "sam9x60_curiosity_pda5.dtso"
+diff --git a/arch/arm/boot/dts/sam9x60_curiosity_pda5.dtso b/arch/arm/boot/dts/sam9x60_curiosity_pda5.dtso
+new file mode 100644
+index 000000000000..ff0702f0ff1e
+--- /dev/null
++++ b/arch/arm/boot/dts/sam9x60_curiosity_pda5.dtso
+@@ -0,0 +1,168 @@
++// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
++/*
++ * sam9x60_curiosity_pda5.dtso - Device Tree file for SAM9X60-CURIOSITY board
++ * overlay blob for PDA5000 LCD.
++ *
++ * Copyright (C) 2022 Microchip Technology, Inc. and its subsidiaries
++ * Author: Durai Manickam KR <durai.manickamkr@microchip.com>
++ *
++ */
++///dts-v1/;
++///plugin/;
++
++//#include "dt-bindings/pinctrl/at91.h"
++
++&i2c6 {
++	dmas = <0>, <0>;
++	#address-cells = <1>;
++	#size-cells = <0>;
++	status = "okay";
++
++	qt1070: keyboard@1b {
++		compatible = "qt1070";
++		reg = <0x1b>;
++		interrupt-parent = <&pioC>;
++		interrupts = <31 0x0>;
++		pinctrl-names = "default";
++		pinctrl-0 = <&pinctrl_qt1070_irq>;
++	};
++
++	mxt: atmel_mxt_ts@4a {
++		compatible = "atmel,atmel_mxt_ts";
++		reg = <0x4a>;
++		interrupt-parent = <&pioC>;
++		interrupts = <25 0x8>;
++		pinctrl-names = "default";
++		pinctrl-0 = <&pinctrl_mxt_irq>;
++		wakeup-source;
++	};
++};
++
++&hlcdc {
++	status = "okay";
++
++	hlcdc-display-controller {
++		pinctrl-names = "default";
++		pinctrl-0 = <&pinctrl_lcd_base &pinctrl_lcd_rgb666>;
++		port@0 {
++			#address-cells = <1>;
++			#size-cells = <0>;
++			hlcdc_panel_output: endpoint@0 {
++				reg = <0>;
++				remote-endpoint = <&panel_input>;
++			};
++		};
++	};
++
++	hlcdc_pwm: hlcdc-pwm {
++		pinctrl-names = "default";
++		pinctrl-0 = <&pinctrl_lcd_pwm>;
++	};
++};
++
++&gpu {
++	status = "okay";
++};
++
++&pinctrl {
++	qt1070 {
++		pinctrl_qt1070_irq: qt1070_irq {
++			atmel,pins = <AT91_PIOC 31 AT91_PERIPH_GPIO AT91_PINCTRL_PULL_UP_DEGLITCH>;
++		};
++	};
++
++	mxt {
++		pinctrl_mxt_irq: mxt_irq {
++			atmel,pins = <AT91_PIOC 25 AT91_PERIPH_GPIO AT91_PINCTRL_PULL_UP_DEGLITCH>;
++		};
++	};
++
++	lcd {
++		pinctrl_lcd_base: lcd-base-0 {
++			atmel,pins = <AT91_PIOC 27 AT91_PERIPH_A AT91_PINCTRL_NONE	/* LCDVSYNC */
++				      AT91_PIOC 28 AT91_PERIPH_A AT91_PINCTRL_NONE	/* LCDHSYNC */
++				      AT91_PIOC 24 AT91_PERIPH_A AT91_PINCTRL_NONE	/* LCDDISP */
++				      AT91_PIOC 29 AT91_PERIPH_A AT91_PINCTRL_NONE	/* LCDDEN */
++				      AT91_PIOC 30 AT91_PERIPH_A AT91_PINCTRL_NONE>;	/* LCDPCK */
++		};
++
++		pinctrl_lcd_rgb666: lcd-rgb-3 {
++			atmel,pins = <AT91_PIOC 2 AT91_PERIPH_A AT91_PINCTRL_NONE	/* LCDD2 pin */
++				      AT91_PIOC 3 AT91_PERIPH_A AT91_PINCTRL_NONE	/* LCDD3 pin */
++				      AT91_PIOC 4 AT91_PERIPH_A AT91_PINCTRL_NONE	/* LCDD4 pin */
++				      AT91_PIOC 5 AT91_PERIPH_A AT91_PINCTRL_NONE	/* LCDD5 pin */
++				      AT91_PIOC 6 AT91_PERIPH_A AT91_PINCTRL_NONE	/* LCDD6 pin */
++				      AT91_PIOC 7 AT91_PERIPH_A AT91_PINCTRL_NONE	/* LCDD7 pin */
++				      AT91_PIOC 10 AT91_PERIPH_A AT91_PINCTRL_NONE	/* LCDD10 pin */
++				      AT91_PIOC 11 AT91_PERIPH_A AT91_PINCTRL_NONE	/* LCDD11 pin */
++				      AT91_PIOC 12 AT91_PERIPH_A AT91_PINCTRL_NONE	/* LCDD12 pin */
++				      AT91_PIOC 13 AT91_PERIPH_A AT91_PINCTRL_NONE	/* LCDD13 pin */
++				      AT91_PIOC 14 AT91_PERIPH_A AT91_PINCTRL_NONE	/* LCDD14 pin */
++				      AT91_PIOC 15 AT91_PERIPH_A AT91_PINCTRL_NONE	/* LCDD15 pin */
++				      AT91_PIOC 18 AT91_PERIPH_A AT91_PINCTRL_NONE	/* LCDD18 pin */
++				      AT91_PIOC 19 AT91_PERIPH_A AT91_PINCTRL_NONE	/* LCDD19 pin */
++				      AT91_PIOC 20 AT91_PERIPH_A AT91_PINCTRL_NONE	/* LCDD20 pin */
++				      AT91_PIOC 21 AT91_PERIPH_A AT91_PINCTRL_NONE	/* LCDD21 pin */
++				      AT91_PIOC 22 AT91_PERIPH_A AT91_PINCTRL_NONE	/* LCDD22 pin */
++				      AT91_PIOC 23 AT91_PERIPH_A AT91_PINCTRL_NONE>;	/* LCDD23 pin */
++		};
++
++		pinctrl_lcd_pwm: lcd_pwm {
++			atmel,pins = <AT91_PIOC 26 AT91_PERIPH_A AT91_PINCTRL_NONE>;
++		};
++	};
++};
++
++&{/} {
++	model = "Microchip SAM9X60-CURIOSITY TM5000";
++
++	backlight: backlight {
++		compatible = "pwm-backlight";
++		pwms = <&hlcdc_pwm 0 50000 0>;
++		brightness-levels = <0 4 8 16 32 64 128 255>;
++		default-brightness-level = <7>;
++		power-supply = <&bl_reg>;
++		status = "okay";
++	};
++
++	bl_reg: backlight_regulator {
++		compatible = "regulator-fixed";
++		regulator-name = "backlight-power-supply";
++		regulator-min-microvolt = <5000000>;
++		regulator-max-microvolt = <5000000>;
++		status = "okay";
++	};
++
++	panel: panel {
++		compatible = "pda,91-00156-a0", "simple-panel";
++		backlight = <&backlight>;
++		power-supply = <&panel_reg>;
++		#address-cells = <1>;
++		#size-cells = <0>;
++		status = "okay";
++
++		port@0 {
++			reg = <0>;
++			#address-cells = <1>;
++			#size-cells = <0>;
++
++			panel_input: endpoint@0 {
++				reg = <0>;
++				remote-endpoint = <&hlcdc_panel_output>;
++			};
++		};
++	};
++
++	panel_reg: panel_regulator {
++		compatible = "regulator-fixed";
++		regulator-name = "panel-power-supply";
++		regulator-min-microvolt = <3300000>;
++		regulator-max-microvolt = <3300000>;
++		status = "okay";
++	};
++};
++
++&isi {
++	status = "disabled";
++};
++
+diff --git a/drivers/gpu/drm/atmel-hlcdc/atmel_hlcdc_plane.c b/drivers/gpu/drm/atmel-hlcdc/atmel_hlcdc_plane.c
+index daa508504f47..43c7352b436c 100644
+--- a/drivers/gpu/drm/atmel-hlcdc/atmel_hlcdc_plane.c
++++ b/drivers/gpu/drm/atmel-hlcdc/atmel_hlcdc_plane.c
+@@ -442,6 +442,7 @@ static void atmel_hlcdc_plane_update_buffers(struct atmel_hlcdc_plane *plane,
+ {
+ 	const struct atmel_hlcdc_layer_desc *desc = plane->layer.desc;
+ 	struct drm_framebuffer *fb = state->base.fb;
++	static int force_update = 1;
+ 	u32 sr;
+ 	int i;
+ 
+@@ -456,7 +457,8 @@ static void atmel_hlcdc_plane_update_buffers(struct atmel_hlcdc_plane *plane,
+ 					    ATMEL_HLCDC_LAYER_PLANE_HEAD(i),
+ 					    state->dscrs[i]->self);
+ 
+-		if (!(sr & ATMEL_HLCDC_LAYER_EN)) {
++		if ((!(sr & ATMEL_HLCDC_LAYER_EN)) || force_update) {
++			force_update = 0;
+ 			atmel_hlcdc_layer_write_reg(&plane->layer,
+ 					ATMEL_HLCDC_LAYER_PLANE_ADDR(i),
+ 					state->dscrs[i]->addr);
+diff --git a/drivers/video/backlight/pwm_bl.c b/drivers/video/backlight/pwm_bl.c
+index c0523a0269ee..fbb16cafdf04 100644
+--- a/drivers/video/backlight/pwm_bl.c
++++ b/drivers/video/backlight/pwm_bl.c
+@@ -533,6 +533,9 @@ static int pwm_backlight_probe(struct platform_device *pdev)
+ 	if (!state.period && (data->pwm_period_ns > 0))
+ 		state.period = data->pwm_period_ns;
+ 
++	state.enabled = 1;
++	state.duty_cycle = 50000;
++
+ 	ret = pwm_apply_state(pb->pwm, &state);
+ 	if (ret) {
+ 		dev_err(&pdev->dev, "failed to apply initial PWM state: %d\n",
