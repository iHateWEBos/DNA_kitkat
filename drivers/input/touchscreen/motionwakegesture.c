/*
 * drivers/input/touchscreen/motionwakegesture.c
 *
 *
 * Copyright (c) 2013, Ryan Wallace <wallace.ryan10@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/delay.h>
#include <linux/init.h>
#include <linux/err.h>
#include <linux/input/motionwakegesture.h>
#include <linux/slab.h>
#include <linux/workqueue.h>
#include <linux/input.h>
#ifdef CONFIG_POCKET_DETECT
#include <linux/input/pocket_detect.h>
#endif
#ifndef CONFIG_HAS_EARLYSUSPEND
#include <linux/lcd_notify.h>
#else
#include <linux/earlysuspend.h>
#endif
#include <linux/hrtimer.h>

/* uncomment since no touchscreen defines android touch, do that here */
//#define ANDROID_TOUCH_DECLARED

/* Version, author, desc, etc */
#define DRIVER_AUTHOR "Ryan Wallace <wallace.ryan10@gmail.com>"
#define DRIVER_DESCRIPTION "Motion Launch Gestures for almost any device"
#define DRIVER_VERSION "1.0"
#define LOGTAG "[MotionWakeGesture]: "

MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESCRIPTION);
MODULE_VERSION(DRIVER_VERSION);
MODULE_LICENSE("GPLv2");

#define DEFAULT_GESTURE 0

int gesture_switch = DEFAULT_GESTURE;

/* Read cmdline for mlg */
 static int __init read_mlg_cmdline(char *mlg)
{
	if (strcmp(mlg, "1") == 0) {
 		pr_info("[cmdline_mlg]: Wake Gestures enabled. | mlg='%s'\n", mlg);
 		gesture_switch = 1;
 	} else if (strcmp(mlg, "0") == 0) {
 		pr_info("[cmdline_mlg]: Wake Gestures disabled. | mlg='%s'\n", mlg);
 		gesture_switch = 0;
        } else {
 		pr_info("[cmdline_mlg]: No valid input found. Going with default: | mlg='%u'\n", gesture_switch);
 	}
 	return 1;
 }
 __setup("mlg=", read_mlg_cmdline);