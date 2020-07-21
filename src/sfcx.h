#pragma once

#include <stdint.h>

//Registers
#define SFCX_CONFIG				0x00
#define SFCX_STATUS 			0x04
#define SFCX_COMMAND			0x08
#define SFCX_ADDRESS			0x0C
#define SFCX_DATA				0x10
#define SFCX_LOGICAL 			0x14
#define SFCX_PHYSICAL			0x18
#define SFCX_DPHYSADDR			0x1C
#define SFCX_MPHYSADDR			0x20
#define SFCX_PHISON				0xFC

#define META_TYPE_0				0x00 			//Pre Jasper
#define META_TYPE_1				0x01 			//Jasper 16MB
#define META_TYPE_2				0x02			//Jasper 256MB and 512MB Large Block

#define CONFIG_BLOCKS			0x04			//Number of blocks assigned for config data

#define SFCX_INITIALIZED		1

struct sfcx_config
{
	uint32_t initialized;
	uint32_t meta_type;

	uint32_t page_sz;
	uint32_t meta_sz;
	uint32_t page_sz_phys;

	uint32_t pages_in_block;
	uint32_t block_sz;
	uint32_t block_sz_phys;

	uint32_t size_mb;
	uint32_t size_bytes;
	uint32_t size_bytes_phys;
	uint32_t size_pages;
	uint32_t size_blocks;

	uint32_t blocks_per_lg_block;
	uint32_t size_usable_fs;
	uint32_t addr_config;
	uint32_t len_config;
};