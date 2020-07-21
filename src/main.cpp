#include <Arduino.h>
#include <SPI.h>
#include "sfcx.h"

extern "C" {
  #include "XSPI.h"
  #include "Flasher.h"
  #include "XNAND.h"
}

void get_sfcx_config(sfcx_config *sfc) {
  DWORD config = XNANDGetConfig();

  sfc->initialized = SFCX_INITIALIZED;
	sfc->meta_type = 0;
	sfc->page_sz = 0x200;
	sfc->meta_sz = 0x10;
	sfc->page_sz_phys = sfc->page_sz + sfc->meta_sz;

//  Serial.print("config: 0x");
//  Serial.println(config, HEX);

//  Serial.print("((config >> 17) & 0x03): 0x");
//  Serial.println(((config >> 17) & 0x03), HEX);

//  Serial.print("((config >> 4) & 0x3): 0x");
//  Serial.println(((config >> 4) & 0x3), HEX);

  switch ((config >> 17) & 0x03) {
	  case 0: // Small block original SFC (pre jasper)
      sfc->meta_type = META_TYPE_0;
      sfc->blocks_per_lg_block = 8;

      switch ((config >> 4) & 0x3) {
        case 3: // 64MB
          sfc->block_sz = 0x4000; // 16 KB
          sfc->size_blocks = 0x1000;
          sfc->size_bytes = sfc->size_blocks << 0xE;
          sfc->size_usable_fs = 0xF80;
          sfc->addr_config = (sfc->size_usable_fs - CONFIG_BLOCKS) * sfc->block_sz;
          break;
      }

      break;
  }

  sfc->len_config = sfc->block_sz * 0x04; //4 physical blocks

	sfc->pages_in_block = sfc->block_sz / sfc->page_sz;
	sfc->block_sz_phys = sfc->pages_in_block * sfc->page_sz_phys;

	sfc->size_pages = sfc->size_bytes / sfc->page_sz;
	sfc->size_blocks = sfc->size_bytes / sfc->block_sz;

	sfc->size_bytes_phys = sfc->block_sz_phys * sfc->size_blocks;
	sfc->size_mb = sfc->size_bytes >> 20;

//  Serial.print("config: 0x");
//  Serial.println(config, HEX);

//  Serial.print("sfc->page_sz: 0x");
//  Serial.println(sfc->page_sz, HEX);

//  Serial.print("sfc->meta_sz: 0x");
//  Serial.println(sfc->meta_sz, HEX);

//  Serial.print("sfc->page_sz_phys: 0x");
//  Serial.println(sfc->page_sz_phys, HEX);

//  Serial.print("sfc->pages_in_block: 0x");
//  Serial.println(sfc->pages_in_block, HEX);

//  Serial.print("sfc->block_sz: 0x");
//  Serial.println(sfc->block_sz, HEX);

//  Serial.print("sfc->block_sz_phys: 0x");
//  Serial.println(sfc->block_sz_phys, HEX);

//  Serial.print("sfc->size_mb: ");
//  Serial.print(sfc->size_mb, DEC);
//  Serial.println("mb");

//  Serial.print("sfc->size_bytes: 0x");
//  Serial.println(sfc->size_bytes, HEX);

//  Serial.print("sfc->size_bytes_phys: 0x");
//  Serial.println(sfc->size_bytes_phys, HEX);

//  Serial.print("sfc->size_pages: 0x");
//  Serial.println(sfc->size_pages, HEX);

//  Serial.print("sfc->size_blocks: 0x");
//  Serial.println(sfc->size_blocks, HEX);

//  Serial.print("sfc->blocks_per_lg_block: 0x");
//  Serial.println(sfc->blocks_per_lg_block, HEX);

//  Serial.print("sfc->size_usable_fs: 0x");
//  Serial.println(sfc->size_usable_fs, HEX);

//  Serial.print("sfc->addr_config: 0x");
//  Serial.println(sfc->addr_config, HEX);

//  Serial.print("sfc->len_config: 0x");
//  Serial.println(sfc->len_config, HEX);
}

void Dump32(uint32_t val) {
  for (size_t i = 0; i < 4; i++) {
    if(val == 0x00) {
      Serial.print("0");
    }

    if(val < 0x10) {
      Serial.print("0");
    }

    Serial.print(val & 0xff, HEX);
    val >>= 8;
  }
  
}

WORD NandReadCB(uint32_t block, uint32_t len) {
	len /= 4;

  DWORD nextBlock = block;
  BYTE wordsLeft = 0;
  WORD status = 0;

	while (len) {
		BYTE readnow;

		if (!wordsLeft) {
        status = XNANDReadStart(nextBlock);
        nextBlock++;
        wordsLeft = 0x84;

        if (status != 0x200) {
        if (status & 0x40) {
          //Serial.print(" * Bad block found at: 0x");
          //Serial.println(block, HEX);
          return status;
        }
        else if (status & 0x1c) {
          //Serial.print(" * (corrected) ECC error 0x");
          //Serial.print(block, HEX);
          //Serial.print(" : 0x");
          //Serial.println(status, HEX);
          return status;
        }
        /*else if (!raw && (status & 0x800)) {
          //Serial.print(" * illegal logical block 0x");
          //Serial.print(block, HEX);
          //Serial.print(" : 0x");
          //Serial.println(status, HEX);
        }*/
      else {
        //Serial.print(" * Unknown error at %08x: %08x. Please worry 0x");
        //Serial.print(block, HEX);
        //Serial.print(" : 0x");
        //Serial.println(status, HEX);
        return status;
      }
    }
    else {
      //Serial.print("Read page 0x");
      //Serial.println(block, HEX);
    }

    }
      

    DWORD buffer = 0;
		readnow = 1;		

		XNANDReadProcess((BYTE *)&buffer, 1);
    
    //Dump32(buffer);
    BYTE *p = (BYTE *)&buffer;
    Serial.write(*p++);
    Serial.write(*p++);
    Serial.write(*p++);
    Serial.write(*p++);

		wordsLeft-=readnow;
		len-=readnow;
	}

  return 0;
}

void DumpHex(uint8_t *buffer, size_t len) {
  const int cols = 32;
  uint8_t *p = buffer;

  for(int y = 0; y < (len / cols); y++) {
    for(int x = 0; x < cols; x++) {
      Serial.print(*p++, HEX);
      Serial.print(" ");
    }
  }

  for(int i = 0; i < (len % cols); i++) {
    Serial.print(*p++, HEX);
    Serial.print(" ");
  }
}

void setup() {
  Serial.begin(115200);

  XSPIInit();

  XSPIEnterFlashMode();

/*
  

  sfcx_config sfc = {0};

  get_sfcx_config(&sfc);

  //BYTE *buffer = new BYTE[sfc.page_sz_phys];

  for (uint32_t i = 0; i < sfc.size_pages; i++) {
    NandReadCB(i, 0x210);
  }
  */
}

struct Command {
  u8 Id;
  u32 ArgA;
};

static const u8 CMD_CONFIG = 0x01;
static const u8 CMD_READ_PAGE = 0x02;

void writeLength(u32 length) {
  Serial.write((length >> 0) & 0xff);
  Serial.write((length >> 8) & 0xff);
  Serial.write((length >> 16) & 0xff);
  Serial.write((length >> 24) & 0xff);
}

u32 read32() {
  u32 result = 0;
  Serial.readBytes((uint8_t *)&result, 4);
  return result;
}

void loop() {
  switch(Serial.read()) {
    case CMD_CONFIG: {
      sfcx_config config {0};
      get_sfcx_config(&config);
      u32 len = (u32)sizeof(sfcx_config);
      
      writeLength(len);

      u8 *p = (u8 *)&config;
      for (u32 i = 0; i < sizeof(sfcx_config); i++) {
        Serial.write(*p++);
      }

      break;
    }

    case CMD_READ_PAGE: {
      u32 len = (u32)0x210ul;

      u32 page = read32();

      writeLength(len);
      NandReadCB(page, 0x210);

      break;
    }
  }
}