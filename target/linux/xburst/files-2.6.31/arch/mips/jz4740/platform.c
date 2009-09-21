/*
 * Platform device support for Jz4740 SoC.
 *
 * Copyright 2007, <yliu@ingenic.cn>
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2.  This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */

#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/resource.h>
#include <linux/mtd/jz4740_nand.h>
#include <linux/jz4740_fb.h>
#include <linux/input/matrix_keypad.h>
#include <linux/mtd/jz4740_nand.h>
#include <linux/spi/spi.h>
#include <linux/spi/spi_gpio.h>
#include <linux/power_supply.h>
#include <linux/jz4740_batt.h>

#include <asm/jzsoc.h>
#include <asm/gpio.h>

/* OHCI (USB full speed host controller) */
static struct resource jz_usb_ohci_resources[] = {
	[0] = {
		.start	= CPHYSADDR(UHC_BASE),
		.end	= CPHYSADDR(UHC_BASE) + 0x10000 - 1,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= JZ_IRQ_UHC,
		.end	= JZ_IRQ_UHC,
		.flags	= IORESOURCE_IRQ,
	},
};

/* The dmamask must be set for OHCI to work */
static u64 ohci_dmamask = ~(u32)0;

static struct platform_device jz_usb_ohci_device = {
	.name		= "jz-ohci",
	.id		= 0,
	.dev = {
		.dma_mask		= &ohci_dmamask,
		.coherent_dma_mask	= 0xffffffff,
	},
	.num_resources	= ARRAY_SIZE(jz_usb_ohci_resources),
	.resource	= jz_usb_ohci_resources,
};


/* UDC (USB gadget controller) */
static struct resource jz_usb_gdt_resources[] = {
	[0] = {
		.start	= CPHYSADDR(UDC_BASE),
		.end	= CPHYSADDR(UDC_BASE) + 0x10000 - 1,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= JZ_IRQ_UDC,
		.end	= JZ_IRQ_UDC,
		.flags	= IORESOURCE_IRQ,
	},
};

static u64 udc_dmamask = ~(u32)0;

static struct platform_device jz_usb_gdt_device = {
	.name		= "jz-udc",
	.id		= -1,
	.dev = {
		.dma_mask		= &udc_dmamask,
		.coherent_dma_mask	= 0xffffffff,
	},
	.num_resources	= ARRAY_SIZE(jz_usb_gdt_resources),
	.resource	= jz_usb_gdt_resources,
};
/*
static struct jz_mmc_platform_data jz_mmc_pdata = {
	.card_detect_gpio = JZ_GPIO_PORTD(0),
	.read_only_gpio = JZ_GPIO_PORTD(16),
	.power_gpio = JZ_GPIO_PORTD(2),
};*/

/** MMC/SD controller **/
static struct resource jz_mmc_resources[] = {
	[0] = {
		.start	= CPHYSADDR(MSC_BASE),
		.end	= CPHYSADDR(MSC_BASE) + 0x10000 - 1,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= JZ_IRQ_MSC,
		.end	= JZ_IRQ_MSC,
		.flags	= IORESOURCE_IRQ,
	}
};

static u64 jz_mmc_dmamask =  ~(u32)0;

static struct platform_device jz_mmc_device = {
	.name = "jz-mmc",
	.id = 0,
	.dev = {
		.dma_mask               = &jz_mmc_dmamask,
		.coherent_dma_mask      = 0xffffffff,
/*		.platform_data			= &jz_mmc_pdata,*/
	},
	.num_resources  = ARRAY_SIZE(jz_mmc_resources),
	.resource       = jz_mmc_resources,
};

static struct resource jz_rtc_resources[] = {
	[0] = {
		.start	= CPHYSADDR(RTC_BASE),
		.end	= CPHYSADDR(RTC_BASE) + 0x10,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start  = JZ_IRQ_RTC,
		.end	= JZ_IRQ_RTC,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct platform_device jz_rtc_device = {
	.name	= "jz4740-rtc",
	.id	= -1,
	.num_resources	= ARRAY_SIZE(jz_rtc_resources),
	.resource	= jz_rtc_resources,
};

/** I2C controller **/
static struct resource jz_i2c_resources[] = {
	[0] = {
		.start	= CPHYSADDR(I2C_BASE),
		.end	= CPHYSADDR(I2C_BASE) + 0x10000 - 1,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= JZ_IRQ_I2C,
		.end	= JZ_IRQ_I2C,
		.flags	= IORESOURCE_IRQ,
	}
};

static u64 jz_i2c_dmamask =  ~(u32)0;

static struct platform_device jz_i2c_device = {
	.name = "jz_i2c",
	.id = 0,
	.dev = {
		.dma_mask               = &jz_i2c_dmamask,
		.coherent_dma_mask      = 0xffffffff,
	},
	.num_resources  = ARRAY_SIZE(jz_i2c_resources),
	.resource       = jz_i2c_resources,
};

static struct resource jz_nand_resources[] = {
	[0] = {
		.start	= CPHYSADDR(EMC_BASE),
		.end	= CPHYSADDR(EMC_BASE) + 0x10000 - 1,
		.flags	= IORESOURCE_MEM,
	},
};

static struct nand_ecclayout qi_lb60_ecclayout = {
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

static struct mtd_partition qi_lb60_partitions[] = {
	{ .name =	"NAND BOOT partition",
	  .offset =	0 * 0x100000,
	  .size =	4 * 0x100000,
	},
	{ .name =	"NAND KERNEL partition",
	  .offset =	4 * 0x100000,
	  .size =	4 * 0x100000,
	},
	{ .name =	"NAND ROOTFS partition",
	  .offset =	8 * 0x100000,
	  .size =	20 * 0x100000,
	},
	{ .name =	"NAND DATA partition",
	  .offset =	100 * 0x100000,
	  .size =	20 * 0x100000,
	},
};

static struct jz_nand_platform_data jz_nand_platform_data = {
	.num_partitions = ARRAY_SIZE(qi_lb60_partitions),
	.partitions = qi_lb60_partitions,
	.ecc_layout = &qi_lb60_ecclayout,
	.busy_gpio = 94,
};

static struct platform_device jz_nand_device = {
	.name = "jz4740-nand",
	.num_resources = ARRAY_SIZE(jz_nand_resources),
	.resource = jz_nand_resources,
	.dev = {
		.platform_data = &jz_nand_platform_data,
	}
};
#define KEEP_UART_ALIVE

#define KEY_QI_QI		KEY_F13
#define KEY_QI_UPRED	KEY_F14
#define KEY_QI_VOLUP	KEY_F15
#define KEY_QI_VOLDOWN	KEY_F16

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
	KEY(4, 3, KEY_APOSTROPHE),	/* S37 */
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
	KEY(5, 6, KEY_LEFTCTRL),	/* S48 */
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
	KEY(7, 2, KEY_FN),	/* S60 */
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

static struct fb_videomode qi_lb60_video_modes[] = {
	{
		.name = "320x240",
		.xres = 320,
		.yres = 240,
		.pixclock = 700000,
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

static struct jz4740_fb_platform_data qi_lb60_fb_data = {
	.width	    = 60,
	.height	    = 45,
	.num_modes  = ARRAY_SIZE(qi_lb60_video_modes),
	.modes	    = qi_lb60_video_modes,
	.bpp	    = 24,
	.lcd_type   = JZ_LCD_TYPE_8BIT_SERIAL,
};

static struct resource fb_resources[] = {
	[0] = {
		.start	= CPHYSADDR(LCD_BASE),
		.end	= CPHYSADDR(LCD_BASE) + 0x10000 - 1,
		.flags	= IORESOURCE_MEM,
	},
};

static u64 jz_fb_dmamask = ~(u32)0;

static struct platform_device qi_lb60_fb = {
	.name = "jz4740-fb",
	.id = -1,
	.num_resources = ARRAY_SIZE(fb_resources),
	.resource = fb_resources,
	.dev = {
		.dma_mask = &jz_fb_dmamask,
		.coherent_dma_mask = 0xffffffff,
		.platform_data = &qi_lb60_fb_data,
	},
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

static struct resource i2s_resources[] = {
	[0] = {
		.start	= CPHYSADDR(AIC_BASE),
		.end	= CPHYSADDR(AIC_BASE) + 0x38 - 1,
		.flags	= IORESOURCE_MEM,
	},
};

static struct platform_device jz_i2s_device = {
	.name = "jz4740-i2s",
	.id = -1,
	.num_resources = ARRAY_SIZE(i2s_resources),
	.resource = i2s_resources,
};

static struct resource codec_resources[] = {
	[0] = {
		.start	= CPHYSADDR(AIC_BASE) + 0x80,
		.end	= CPHYSADDR(AIC_BASE) + 0x88 - 1,
		.flags	= IORESOURCE_MEM,
	},
};

static struct platform_device jz_codec_device = {
	.name		= "jz4740-codec",
	.id		= -1,
	.num_resources	= ARRAY_SIZE(codec_resources),
	.resource	= codec_resources,
};

static struct jz_batt_info jz_batt_gpio_platform_data = {
	.dc_dect_gpio	= GPIO_DC_DETE_N,
	.usb_dect_gpio	= GPIO_USB_DETE,
	.charg_stat_gpio  = GPIO_CHARG_STAT_N,

	.min_voltag	= 3600000,
	.max_voltag	= 4200000,
	.batt_tech	= POWER_SUPPLY_TECHNOLOGY_LIPO,
};

static struct platform_device batt_gpio_device = {
	.name = "batt_gpio",
	.id = -1,
	.dev = {
		.platform_data = &jz_batt_gpio_platform_data,
	},
};

/* All */
static struct platform_device *jz_platform_devices[] __initdata = {
	&jz_usb_ohci_device,
	&jz_usb_gdt_device,
	&jz_mmc_device,
	&jz_nand_device,
	&jz_i2c_device,
	&qi_lb60_keypad,
	&spigpio_device,
	&qi_lb60_fb,
	&jz_i2s_device,
	&jz_codec_device,
	&jz_rtc_device,
	&batt_gpio_device,
};

static int __init jz_platform_init(void)
{
	spi_register_board_info(qi_lb60_spi_board_info,
				ARRAY_SIZE(qi_lb60_spi_board_info));
	return platform_add_devices(jz_platform_devices, ARRAY_SIZE(jz_platform_devices));
}

arch_initcall(jz_platform_init);
