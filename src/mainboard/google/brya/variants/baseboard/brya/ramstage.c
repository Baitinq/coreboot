/* SPDX-License-Identifier: GPL-2.0-or-later */

#include <acpi/acpi_device.h>
#include <baseboard/variants.h>
#include <console/console.h>
#include <device/pci_ops.h>
#include <soc/pci_devs.h>

#include <drivers/intel/dptf/chip.h>
#include <intelblocks/power_limit.h>

void variant_update_power_limits(const struct cpu_power_limits *limits, size_t num_entries)
{
	if (!num_entries)
		return;

	const struct device_path policy_path[] = {
		{ .type = DEVICE_PATH_PCI, .pci.devfn = SA_DEVFN_DPTF},
		{ .type = DEVICE_PATH_GENERIC, .generic.id = 0}
	};

	const struct device *policy_dev = find_dev_nested_path(pci_root_bus(),
							policy_path, ARRAY_SIZE(policy_path));
	if (!policy_dev)
		return;

	struct drivers_intel_dptf_config *config = policy_dev->chip_info;

	uint16_t mchid = pci_s_read_config16(PCI_DEV(0, 0, 0), PCI_DEVICE_ID);

	for (size_t i = 0; i < num_entries; i++) {
		if (mchid == limits[i].mchid) {
			struct dptf_power_limits *settings = &config->controls.power_limits;
			config_t *conf = config_of_soc();
			struct soc_power_limits_config *soc_config = conf->power_limits_config;
			settings->pl1.min_power = limits[i].pl1_min_power;
			settings->pl1.max_power = limits[i].pl1_max_power;
			settings->pl2.min_power = limits[i].pl2_min_power;
			settings->pl2.max_power = limits[i].pl2_max_power;
			soc_config->tdp_pl4 = limits[i].pl4_power;
			printk(BIOS_INFO, "Overriding power limits PL1 (%u, %u) PL2 (%u, %u) PL4 (%u)\n",
					limits[i].pl1_min_power,
					limits[i].pl1_max_power,
					limits[i].pl2_min_power,
					limits[i].pl2_max_power,
					limits[i].pl4_power);
		}
	}
}