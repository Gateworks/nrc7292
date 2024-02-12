/* SPDX-License-Identifier: GPL-2.0 */
/*
 * FTDI FT232H USB-SPI bridge driver
 *
 * Copyright (C) Newracom, Inc. <www.newracom.com>
 *
 */

#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/gpio/driver.h>
#include <linux/spi/spi.h>
#include <linux/usb/ch9.h>
#include <linux/usb.h>

#include "ft232h-intf.h"


#define VID_FTDI	0x0403
#define PID_FT232H	0x6014


extern int ft232h_intf_probe(struct usb_interface *intf, const struct usb_device_id *id);
extern void ft232h_intf_disconnect(struct usb_interface *intf);

extern int ft232h_spi_probe(struct platform_device *pdev);
extern int ft232h_spi_remove(struct platform_device *pdev);

/* GW16146 is an FT232H USB to SPI controller connected to an NRC7292 802.11ah radio */
static struct spi_board_info gw16146_board_info = {
	.modalias	= "nrc80211",
	.max_speed_hz	= 30000000,
	.chip_select	= 0,
	.mode		= SPI_MODE_0,
	.irq		= -1,
};

extern struct ft232h_intf_ops ft232h_intf_ops;

static const struct mpsse_spi_platform_data gw16146_spi_cfg_plat_data = {
	.ops			= &ft232h_intf_ops,
	.info			= &gw16146_board_info,
	/* active-high reset on FTDI pin 29 (GPIOH5, gpio offset 10) */
	.reset_gpio = 10,
	.reset_active_high = 1,
	.reset_assert_ms = 100,
	.reset_deassert_ms = 100,
};

static struct usb_device_id ft232h_usb_table[] = {
#ifndef CONFIG_USB_SERIAL_FTDI_SIO
	{ USB_DEVICE( VID_FTDI, PID_FT232H ), },
#endif
	{ USB_DEVICE(0x2beb, 0x0146), .driver_info = (kernel_ulong_t)&gw16146_spi_cfg_plat_data},
	{}
};

MODULE_DEVICE_TABLE(usb, ft232h_usb_table);

static int ft232h_usb_probe(struct usb_interface *intf, const struct usb_device_id *id)
{
	int ret;

	ret = ft232h_intf_probe(intf, id);
	if (ret == 0) {
		struct ft232h_intf_priv *priv = usb_get_intfdata(intf);

		ret = ft232h_spi_probe(priv->spi_pdev);
	}

	return ret;
}

static void ft232h_usb_disconnect(struct usb_interface *intf)
{
	struct ft232h_intf_priv *priv = usb_get_intfdata(intf);

	ft232h_spi_remove(priv->spi_pdev);
	ft232h_intf_disconnect(priv->intf);
}

static struct usb_driver ft232h_usb_driver = {
	.name		= KBUILD_MODNAME,
	.id_table	= ft232h_usb_table,
	.probe		= ft232h_usb_probe,
	.disconnect	= ft232h_usb_disconnect,
};

module_usb_driver(ft232h_usb_driver);

MODULE_ALIAS("ft232h-usb-spi");
MODULE_AUTHOR("Newracom, Inc. <www.newracom.com>");
MODULE_AUTHOR("Sangbeom Kim <sb.kim@newracom.com>");
MODULE_AUTHOR("Anatolij Gustschin <agust@denx.de>");
MODULE_DESCRIPTION("FTDI FT232H USB-SPI bridge driver");
MODULE_LICENSE("GPL v2");

