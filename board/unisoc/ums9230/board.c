// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) 2025 Otto Pflüger
 */

#include <env.h>
#include <fdt_support.h>
#include <net.h>

int ft_board_setup(void *blob, struct bd_info *bd)
{
	u8 tmp[ETH_ALEN], mac[ETH_ALEN];
	int i;

	if (eth_env_get_enetaddr("wifiaddr", mac)) {
		do_fixup_by_compat(blob, "sprd,sc2355-wlan",
				   "local-mac-address", mac, ETH_ALEN, 1);
	}

	eth_env_get_enetaddr("bdaddr", tmp);

	for (i = 0; i < ETH_ALEN; ++i)
		mac[i] = tmp[ETH_ALEN - i - 1];

	do_fixup_by_compat(blob, "sprd,bluetooth-sipc",
			   "local-bd-address", mac, ETH_ALEN, 1);

	/*
	 * The UFS controller is only described as usable on units whose internal
	 * storage U-Boot found on it (ufs_present, set by the preboot script). On
	 * an eMMC unit the kernel never touches the controller.
	 */
	do_fixup_by_compat(blob, "sprd,ums9230-ufs", "status",
			   env_get("ufs_present") ? "okay" : "disabled",
			   env_get("ufs_present") ? 5 : 9, 1);

	return 0;
}
