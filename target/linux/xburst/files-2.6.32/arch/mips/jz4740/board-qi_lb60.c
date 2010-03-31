/*
 * linux/arch/mips/jz4740/board-qi_lb60.c
 *
 * QI_LB60 setup routines.
 *
 * Copyright (c) 2009 Qi Hardware inc.,
 * Author: Xiangfu Liu <xiangfu@qi-hardware.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 or later
 * as published by the Free Software Foundation.
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/gpio.h>

#include <asm/mach-jz4740/board-qi_lb60.h>
#include <asm/mach-jz4740/platform.h>

#include <linux/input.h>
#include <linux/gpio_keys.h>
#include <linux/mtd/jz4740_nand.h>
#include <linux/jz4740_fb.h>
#include <linux/input/matrix_keypad.h>
#include <linux/mtd/jz4740_nand.h>
#include <linux/spi/spi.h>
#include <linux/spi/spi_gpio.h>
#include <linux/power_supply.h>
#include <linux/power/jz4740-battery.h>
#include <linux/power/gpio-charger.h>
#include <linux/mmc/jz4740_mmc.h>

#include "clock.h"

/* NAND */
static struct nand_ecclayout qi_lb60_ecclayout_1gb = {
	.eccbytes = 36,
	.eccpos = {
		6,  7,  8,  9,  10, 11, 12, 13,
		14, 15, 16, 17, 18, 19, 20, 21,
		22, 23, 24, 25, 26, 27, 28, 29,
		30, 31, 32, 33, 34, 35, 36, 37,
		38, 39, 40, 41},
	.oobfree = {
		{.offset = 2,
		 .length = 4},
		{.offset = 42,
		 .length = 22}}
};

static struct mtd_partition qi_lb60_partitions_1gb[] = {
	{ .name = "NAND BOOT partition",
	  .offset = 0 * 0x100000,
	  .size = 4 * 0x100000,
 	},
	{ .name = "NAND KERNEL partition",
	  .offset = 4 * 0x100000,
	  .size = 4 * 0x100000,
 	},
	{ .name = "NAND ROOTFS partition",
	  .offset = 8 * 0x100000,
	  .size = (504 + 512) * 0x100000,
 	},
};

static struct nand_ecclayout qi_lb60_ecclayout_2gb = {
	.eccbytes = 72,
	.eccpos = {
		12, 13, 14, 15, 16, 17, 18, 19,
		20, 21, 22, 23, 24, 25, 26, 27, 
		28, 29, 30, 31, 32, 33, 34, 35,
		36, 37, 38, 39, 40, 41, 42, 43,
		44, 45, 46, 47, 48, 49, 50, 51, 
		52, 53, 54, 55, 56, 57, 58, 59, 
		60, 61, 62, 63, 64, 65, 66, 67, 
		68, 69, 70, 71, 72, 73, 74, 75, 
		76, 77, 78, 79, 80, 81, 82, 83},
	.oobfree = {
 		{.offset = 2,
		 .length = 10},
		{.offset = 84,
		 .length = 44}}
};

static struct mtd_partition qi_lb60_partitions_2gb[] = {
	{ .name = "NAND BOOT partition",
	  .offset = 0 * 0x100000,
	  .size = 4 * 0x100000,
 	},
	{ .name = "NAND KERNEL partition",
	  .offset = 4 * 0x100000,
	  .size = 4 * 0x100000,
 	},
	{ .name = "NAND ROOTFS partition",
	  .offset = 8 * 0x100000,
	  .size = (504 + 512 + 1024) * 0x100000,
 	},
};

static void qi_lb60_nand_ident(struct platform_device *pdev,
				struct nand_chip *chip, 
				struct mtd_partition **partitions,
				int *num_partitions)
{
	if (chip->page_shift == 12) {
		chip->ecc.layout = &qi_lb60_ecclayout_2gb;
		*partitions = qi_lb60_partitions_2gb;
		*num_partitions = ARRAY_SIZE(qi_lb60_partitions_2gb);
	} else {
		chip->ecc.layout = &qi_lb60_ecclayout_1gb;
		*partitions = qi_lb60_partitions_1gb;
		*num_partitions = ARRAY_SIZE(qi_lb60_partitions_1gb);
	}
}

static struct jz_nand_platform_data qi_lb60_nand_pdata = {
	.ident_callback = qi_lb60_nand_ident,
	.busy_gpio = 94,
};

/* Keyboard*/

/* #define KEEP_UART_ALIVE
 * don't define this. the keyboard and keyboard both work
 */

#define KEY_QI_VOLUP	KEY_F11
#define KEY_QI_VOLDOWN	KEY_F12
#define KEY_QI_QI	KEY_F13
#define KEY_QI_ACUTE	KEY_GRAVE
#define KEY_QI_FN	KEY_LEFTCTRL
#define KEY_QI_UPRED	KEY_RIGHTALT

static const uint32_t qi_lb60_keymap[] = {
	KEY(0, 0, KEY_F1),	/* S2 */
	KEY(0, 1, KEY_F2),	/* S3 */
	KEY(0, 2, KEY_F3),	/* S4 */
	KEY(0, 3, KEY_F4),	/* S5 */
	KEY(0, 4, KEY_F5),	/* S6 */
	KEY(0, 5, KEY_F6),	/* S7 */
	KEY(0, 6, KEY_F7),	/* S8 */

	KEY(1, 0, KEY_Q),	/* S10 */
	KEY(1, 1, KEY_W),	/* S11 */
	KEY(1, 2, KEY_E),	/* S12 */
	KEY(1, 3, KEY_R),	/* S13 */
	KEY(1, 4, KEY_T),	/* S14 */
	KEY(1, 5, KEY_Y),	/* S15 */
	KEY(1, 6, KEY_U),	/* S16 */
	KEY(1, 7, KEY_I),	/* S17 */
	KEY(2, 0, KEY_A),	/* S18 */
	KEY(2, 1, KEY_S),	/* S19 */
	KEY(2, 2, KEY_D),	/* S20 */
	KEY(2, 3, KEY_F),	/* S21 */
	KEY(2, 4, KEY_G),	/* S22 */
	KEY(2, 5, KEY_H),	/* S23 */
	KEY(2, 6, KEY_J),	/* S24 */
	KEY(2, 7, KEY_K),	/* S25 */
	KEY(3, 0, KEY_ESC),	/* S26 */
	KEY(3, 1, KEY_Z),	/* S27 */
	KEY(3, 2, KEY_X),	/* S28 */
	KEY(3, 3, KEY_C),	/* S29 */
	KEY(3, 4, KEY_V),	/* S30 */
	KEY(3, 5, KEY_B),	/* S31 */
	KEY(3, 6, KEY_N),	/* S32 */
	KEY(3, 7, KEY_M),	/* S33 */
	KEY(4, 0, KEY_TAB),	/* S34 */
	KEY(4, 1, KEY_CAPSLOCK),	/* S35 */
	KEY(4, 2, KEY_BACKSLASH),	/* S36 */
	KEY(4, 3, KEY_QI_ACUTE),	/* S37 */
	KEY(4, 4, KEY_COMMA),	/* S38 */
	KEY(4, 5, KEY_DOT),	/* S39 */
	KEY(4, 6, KEY_SLASH),	/* S40 */
	KEY(4, 7, KEY_UP),	/* S41 */
	KEY(5, 0, KEY_O),	/* S42 */
	KEY(5, 1, KEY_L),	/* S43 */
	KEY(5, 2, KEY_EQUAL),	/* S44 */
	KEY(5, 3, KEY_QI_UPRED),	/* S45 */
	KEY(5, 4, KEY_SPACE),	/* S46 */
	KEY(5, 5, KEY_QI_QI),	/* S47 */
	KEY(5, 6, KEY_RIGHTCTRL),	/* S48 */
	KEY(5, 7, KEY_LEFT),	/* S49 */
	KEY(6, 0, KEY_F8),	/* S50 */
	KEY(6, 1, KEY_P),	/* S51 */
	KEY(6, 2, KEY_BACKSPACE),/* S52 */
	KEY(6, 3, KEY_ENTER),	/* S53 */
	KEY(6, 4, KEY_QI_VOLUP),	/* S54 */
	KEY(6, 5, KEY_QI_VOLDOWN),	/* S55 */
	KEY(6, 6, KEY_DOWN),	/* S56 */
	KEY(6, 7, KEY_RIGHT),	/* S57 */

#ifndef KEEP_UART_ALIVE
	KEY(7, 0, KEY_LEFTSHIFT),	/* S58 */
	KEY(7, 1, KEY_LEFTALT),	/* S59 */
	KEY(7, 2, KEY_QI_FN),	/* S60 */
#endif
};

static const struct matrix_keymap_data qi_lb60_keymap_data = {
	.keymap		= qi_lb60_keymap,
	.keymap_size	= ARRAY_SIZE(qi_lb60_keymap),
};

static const unsigned int qi_lb60_keypad_cols[] = {
	74, 75, 76, 77, 78, 79, 80, 81,
};

static const unsigned int qi_lb60_keypad_rows[] = {
	114, 115, 116, 117, 118, 119, 120,
#ifndef KEEP_UART_ALIVE
	122,
#endif
};

static struct matrix_keypad_platform_data qi_lb60_pdata = {
	.keymap_data = &qi_lb60_keymap_data,
	.col_gpios	= qi_lb60_keypad_cols,
	.row_gpios	= qi_lb60_keypad_rows,
	.num_col_gpios	= ARRAY_SIZE(qi_lb60_keypad_cols),
	.num_row_gpios	= ARRAY_SIZE(qi_lb60_keypad_rows),
	.col_scan_delay_us	= 10,
	.debounce_ms		= 10,
	.wakeup			= 1,
	.active_low		= 1,
};

static struct platform_device qi_lb60_keypad = {
	.name		= "matrix-keypad",
	.id		= -1,
	.dev		= {
		.platform_data = &qi_lb60_pdata,
	},
};

/* Display */
static struct fb_videomode qi_lb60_video_modes[] = {
	{
		.name = "320x240",
		.xres = 320,
		.yres = 240,
		.refresh = 30,
		.left_margin = 140,
		.right_margin = 273,
		.upper_margin = 20,
		.lower_margin = 2,
		.hsync_len = 1,
		.vsync_len = 1,
		.sync = 0,
		.vmode = FB_VMODE_NONINTERLACED,
	},
};

static struct jz4740_fb_platform_data qi_lb60_fb_pdata = {
	.width		= 60,
	.height		= 45,
	.num_modes	= ARRAY_SIZE(qi_lb60_video_modes),
	.modes		= qi_lb60_video_modes,
	.bpp		= 24,
	.lcd_type	= JZ_LCD_TYPE_8BIT_SERIAL,
	.pixclk_falling_edge = 1,
};


struct spi_gpio_platform_data spigpio_platform_data = {
	.sck = JZ_GPIO_PORTC(23),
	.mosi = JZ_GPIO_PORTC(22),
	.miso = JZ_GPIO_PORTC(22),
	.num_chipselect = 1,
};

static struct platform_device spigpio_device = {
	.name = "spi_gpio",
	.id   = 1,
	.dev = {
		.platform_data = &spigpio_platform_data,
	},
};

static struct spi_board_info qi_lb60_spi_board_info[] = {
	{
		.modalias = "gpm940b0",
		.controller_data = (void*)JZ_GPIO_PORTC(21),
		.chip_select = 0,
		.bus_num = 1,
		.max_speed_hz = 30 * 1000,
	},
};

/* Battery */
static struct jz_battery_platform_data qi_lb60_battery_pdata = {
	.gpio_charge = GPIO_CHARG_STAT_N,
	.gpio_charge_active_low = 1,
	.info = {
		.name = "battery",
		.technology = POWER_SUPPLY_TECHNOLOGY_LIPO,
		.voltage_max_design = 4200000,
		.voltage_min_design = 3600000,
	},
};

static char *qi_lb60_batteries[] = {
	"battery",
};

static struct gpio_charger_platform_data qi_lb60_charger_pdata = {
	.name = "USB",
	.type = POWER_SUPPLY_TYPE_USB,
	.gpio = GPIO_USB_DETE,
	.gpio_active_low = 1,
	.batteries = qi_lb60_batteries,
	.num_batteries = ARRAY_SIZE(qi_lb60_batteries),
};

static struct platform_device qi_lb60_charger_device = {
	.name = "gpio-charger",
	.dev = {
		.platform_data = &qi_lb60_charger_pdata,
	},
};

/* GPIO Key: power */
static struct gpio_keys_button qi_lb60_gpio_keys_buttons[] = {
	[0] = {
		.code		= KEY_POWER,
		.gpio		= GPIO_WAKEUP_N,
		.active_low	= 1,
		.desc		= "Power",
		.wakeup		= 1,
	},
};

static struct gpio_keys_platform_data qi_lb60_gpio_keys_data = {
	.nbuttons = ARRAY_SIZE(qi_lb60_gpio_keys_buttons),
	.buttons = qi_lb60_gpio_keys_buttons,
};

static struct platform_device qi_lb60_gpio_keys = {
	.name =	"gpio-keys",
	.id =	-1,
	.dev = {
		.platform_data = &qi_lb60_gpio_keys_data,
	}
};

static struct jz4740_mmc_platform_data qi_lb60_mmc_pdata = {
	.gpio_card_detect	= JZ_GPIO_PORTD(0),
	.gpio_read_only		= -1,
	.gpio_power		= JZ_GPIO_PORTD(2),
	.power_active_low	= 1,
};

static struct platform_device *jz_platform_devices[] __initdata = {
	&jz4740_usb_ohci_device,
	&jz4740_usb_gdt_device,
	&jz4740_mmc_device,
	&jz4740_nand_device,
	&qi_lb60_keypad,
	&spigpio_device,
	&jz4740_framebuffer_device,
	&jz4740_i2s_device,
	&jz4740_codec_device,
	&jz4740_rtc_device,
	&jz4740_adc_device,
	&jz4740_battery_device,
	&qi_lb60_gpio_keys,
	&qi_lb60_charger_device,
};

static void __init board_gpio_setup(void)
{
	/* We only need to enable/disable pullup here for pins used in generic
	 * drivers. Everything else is done by the drivers themselfs. */
	jz_gpio_disable_pullup(GPIO_SD_VCC_EN_N);
	jz_gpio_disable_pullup(GPIO_SD_CD_N);
}

static int __init qi_lb60_init_platform_devices(void)
{
	jz4740_framebuffer_device.dev.platform_data = &qi_lb60_fb_pdata;
	jz4740_nand_device.dev.platform_data = &qi_lb60_nand_pdata;
	jz4740_battery_device.dev.platform_data = &qi_lb60_battery_pdata;
	jz4740_mmc_device.dev.platform_data = &qi_lb60_mmc_pdata;

	spi_register_board_info(qi_lb60_spi_board_info,
				ARRAY_SIZE(qi_lb60_spi_board_info));

	return platform_add_devices(jz_platform_devices,
					ARRAY_SIZE(jz_platform_devices));

}

struct jz4740_clock_board_data jz4740_clock_bdata = {
	.ext_rate = 12000000,
	.rtc_rate = 32768,
};

extern int jz_gpiolib_init(void);

static __init int board_avt2(char *str)
{
	qi_lb60_mmc_pdata.card_detect_active_low = 1;
	qi_lb60_mmc_pdata.power_active_low	= 1;

	return 1;
}

__setup("avt2", board_avt2);

static int __init qi_lb60_board_setup(void)
{
	printk("Qi Hardware JZ4740 QI_LB60 setup\n");
	if (jz_gpiolib_init())
		panic("Failed to initalize jz gpio\n");

	jz4740_clock_init();
	board_gpio_setup();

	if (qi_lb60_init_platform_devices())
		panic("Failed to initalize platform devices\n");

	return 0;
}

arch_initcall(qi_lb60_board_setup);
