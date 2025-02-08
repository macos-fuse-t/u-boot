// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (c) 2017 Tuomas Tynkkynen
 */

#include <config.h>
#include <dm.h>
#include <init.h>
#include <fdtdec.h>
#include <virtio.h>

#include <asm/armv8/mmu.h>

DECLARE_GLOBAL_DATA_PTR;

static struct mm_region scorpi_arm64_mem_map[] = {
	 {
		/* PCI IO */
		.virt = 0x2000,
		.phys = 0x2000,
		.size = 0xe000,
		.attrs = PTE_BLOCK_MEMTYPE(MT_DEVICE_NGNRNE) |
			 PTE_BLOCK_NON_SHARE |
			 PTE_BLOCK_PXN | PTE_BLOCK_UXN
	}, {
		/* Flash */
		.virt = 0xFC000000UL,
		.phys = 0xFC000000UL,
		.size = 0x04000000UL,
		.attrs = PTE_BLOCK_MEMTYPE(MT_NORMAL) |
			 PTE_BLOCK_INNER_SHARE
	}, {
		/* Peripherals */
		.virt = 0x10000UL,
		.phys = 0x10000UL,
		.size = 0x4000UL,
		.attrs = PTE_BLOCK_MEMTYPE(MT_DEVICE_NGNRNE) |
			 PTE_BLOCK_NON_SHARE |
			 PTE_BLOCK_PXN | PTE_BLOCK_UXN
	}, {
		/* GICv3 */
		.virt = 0x2f000000UL,
		.phys = 0x2f000000UL,
		.size = 0x10000UL,
		.attrs = PTE_BLOCK_MEMTYPE(MT_DEVICE_NGNRNE) |
			 PTE_BLOCK_NON_SHARE |
			 PTE_BLOCK_PXN | PTE_BLOCK_UXN
	}, {
		/* GICv3 */
		.virt = 0x2f100000UL,
		.phys = 0x2f100000UL,
		.size = 0xf00000UL,
		.attrs = PTE_BLOCK_MEMTYPE(MT_DEVICE_NGNRNE) |
			 PTE_BLOCK_NON_SHARE |
			 PTE_BLOCK_PXN | PTE_BLOCK_UXN
	}, {
		/* PCI */
		.virt = 0xa0000000UL,
		.phys = 0xa0000000UL,
		.size = 0x50000000UL,
		.attrs = PTE_BLOCK_MEMTYPE(MT_DEVICE_NGNRNE) |
			 PTE_BLOCK_NON_SHARE |
			 PTE_BLOCK_PXN | PTE_BLOCK_UXN
	}, {
		/* RAM */
		.virt = 0x100000000UL,
		.phys = 0x100000000UL,
		.size = 252UL * SZ_1G,
		.attrs = PTE_BLOCK_MEMTYPE(MT_NORMAL) |
			 PTE_BLOCK_INNER_SHARE
	}, {
		/* List terminator */
		0,
	}
};

struct mm_region *mem_map = scorpi_arm64_mem_map;

int board_init(void)
{
	return 0;
}

int board_late_init(void)
{
	/* init display */
	pci_init();
	uclass_probe_all(UCLASS_VIDEO);

	/*
	 * Make sure virtio bus is enumerated so that peripherals
	 * on the virtio bus can be discovered by their drivers
	 */
	virtio_init();

	return 0;
}

int dram_init(void)
{
	if (fdtdec_setup_mem_size_base() != 0)
		return -EINVAL;

	return 0;
}

int dram_init_banksize(void)
{
	fdtdec_setup_memory_banksize();

	return 0;
}

void *board_fdt_blob_setup(int *err)
{
	*err = 0;

	/* XXX: Pass in the address from the host */
	return (void *)0xFFFF0000;
}
