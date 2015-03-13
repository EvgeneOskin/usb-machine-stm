/*
 * UsbDevice.c
 *
 *  Created on: 13 марта 2015 г.
 *      Author: eoskin
 */

#include "UsbDevice.h"

#include "stm32f4xx_ll_usb.h"

USB_OTG_GlobalTypeDef OTGDef;
USB_OTG_CfgTypeDef OTGCfg;
USB_OTG_EPTypeDef InEP;
USB_OTG_EPTypeDef OutEP;


void power_up();

void init_device() {
	OTGCfg.dev_endpoints = 3;
	OTGCfg.Host_channels = 2;
	OTGCfg.speed = USB_OTG_SPEED_HIGH;
	OTGCfg.dma_enable = 1;
	OTGCfg.ep0_mps = DEP0CTL_MPS_64;
	OTGCfg.phy_itface = USB_OTG_ULPI_PHY;
	OTGCfg.Sof_enable = 1;           /*!< Enable or disable the output of the SOF signal.                        */

	OTGCfg.low_power_enable = 0;     /*!< Enable or disable the low power mode.                                  */

	OTGCfg.vbus_sensing_enable = 0;  /*!< Enable or disable the VBUS Sensing feature.                            */

	OTGCfg.use_dedicated_ep1 = 1;    /*!< Enable or disable the use of the dedicated EP1 interrupt.              */

	OTGCfg.use_external_vbus = 0;    /*!< Enable or disable the use of the external VBUS.                        */

	InEP.is_in = 1;
	InEP.num = 1;
	InEP.type = EP_TYPE_INTR;
	InEP.maxpacket = MAX_PACKAGE_SIZE;

	OutEP.is_in = 0;
	OutEP.num = 1;
	OutEP.type = EP_TYPE_INTR;
	OutEP.maxpacket = MAX_PACKAGE_SIZE;

	power_up();
}

void power_up() {
	HAL_StatusTypeDef status;
	status = USB_CoreInit(&OTGDef, OTGCfg);
	if (status == HAL_OK) {
		status = USB_DevInit(&OTGDef, OTGCfg);
		if (status == HAL_OK) {
			status = USB_ActivateEndpoint(&OTGDef, &InEP);
			if (status == HAL_OK) {
				status = USB_ActivateEndpoint(&OTGDef, &OutEP);
			}
		}
	}
}
