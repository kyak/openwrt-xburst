/*
 * Copyright (c) 2009 Qi Hardware Inc.,
 * Author: Xiangfu Liu <xiangfu@qi-hardware.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __ASM_JZ4740_QI_LB60_H__
#define __ASM_JZ4740_QI_LB60_H__

#include <linux/gpio.h>
/*
 * Frequencies of on-board oscillators
 */
#define JZ_EXTAL		12000000  /* Main extal freq: 12 MHz */
#define JZ_EXTAL_RTC		32768     /* RTC extal freq: 32.768 KHz */

/*
 * GPIO
 */
#define GPIO_DC_DETE_N          JZ_GPIO_PORTC(26)
#define GPIO_CHARG_STAT_N       JZ_GPIO_PORTC(27)
#define GPIO_LED_EN             JZ_GPIO_PORTC(28)
#define GPIO_LCD_CS             JZ_GPIO_PORTC(21)
#define GPIO_DISP_OFF_N         JZ_GPIO_PORTD(21)
#define GPIO_PWM                JZ_GPIO_PORTD(27)
#define GPIO_WAKEUP_N           JZ_GPIO_PORTD(29)

#define GPIO_AMP_EN             JZ_GPIO_PORTD(4)

#define GPIO_SD_CD_N            JZ_GPIO_PORTD(0)
#define GPIO_SD_VCC_EN_N        JZ_GPIO_PORTD(2)
#define GPIO_SD_WP              JZ_GPIO_PORTD(16)

#define GPIO_USB_DETE           JZ_GPIO_PORTD(28)
#define GPIO_BUZZ_PWM           JZ_GPIO_PORTD(27)
#define GPIO_UDC_HOTPLUG        GPIO_USB_DETE

#define GPIO_AUDIO_POP          JZ_GPIO_PORTB(29)
#define GPIO_COB_TEST           JZ_GPIO_PORTB(30)

#define GPIO_KEYOUT_BASE        JZ_GPIO_PORTC(10)
#define GPIO_KEYIN_BASE         JZ_GPIO_PORTD(18)
#define GPIO_KEYIN_8            JZ_GPIO_PORTD(26)

/*
 * MMC/SD
 */
#define MSC_WP_PIN		GPIO_SD_WP
#define MSC_HOTPLUG_PIN		GPIO_SD_CD_N
#define MSC_HOTPLUG_IRQ		(IRQ_GPIO_0 + GPIO_SD_CD_N)

#define __msc_init_io()				\
do {						\
	__gpio_as_output(GPIO_SD_VCC_EN_N);	\
	__gpio_as_input(GPIO_SD_CD_N);		\
} while (0)

#define __msc_enable_power()			\
do {						\
	__gpio_clear_pin(GPIO_SD_VCC_EN_N);	\
} while (0)

#define __msc_disable_power()			\
do {						\
	__gpio_set_pin(GPIO_SD_VCC_EN_N);	\
} while (0)

#define __msc_card_detected(s)			\
({						\
	int detected = 1;			\
	if (!__gpio_get_pin(GPIO_SD_CD_N))	\
		detected = 0;			\
	detected;				\
})

#endif /* __ASM_JZ4740_QI_LB60_H__ */
