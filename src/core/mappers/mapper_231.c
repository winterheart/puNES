/*
 * mapper_231.c
 *
 *  Created on: 06/feb/2011
 *      Author: fhorse
 */

#include "mappers.h"
#include "info.h"
#include "mem_map.h"

void map_init_231(void) {
	EXTCL_CPU_WR_MEM(231);

	if (info.reset >= HARD) {
		map_prg_rom_8k(4, 0, 0);
	}
}
void extcl_cpu_wr_mem_231(WORD address, BYTE value) {
	value = address & 0x1E;
	control_bank(info.prg.rom.max.banks_16k)
	map_prg_rom_8k(2, 0, value);

	value |= ((address >> 5) & 0x01);
	control_bank(info.prg.rom.max.banks_16k)
	map_prg_rom_8k(2, 2, value);

	map_prg_rom_8k_update();

	switch ((address & 0xC0) >> 6) {
		case 0:
			mirroring_SCR0();
			break;
		case 1:
			mirroring_V();
			break;
		case 2:
			mirroring_H();
			break;
		case 3:
			mirroring_SCR0x1_SCR1x3();
			break;
	}
}