#ifndef _PPU_H_
#define _PPU_H_

#include <map>;
#include <vector>;

#include "memory.h";
#include "renderer.h"

//PPU Registers 
//http://wiki.nesdev.com/w/index.php/PPU_registers
#define PPUCTRL			0x2000
#define	PPUMASK			0x2001
#define PPUSTATUS		0x2002
#define OAMADDR			0x2003
#define OAMDATA			0x2004
#define PPUSCROLL		0x2005
#define PPUADDR			0x2006
#define PPUDATA			0x2007
#define OAMDMA			0x2008


//PPU Mask Bits
#define PPU_MASK_GRAYSCALE		0x01
#define PPU_MASK_BG_LEFT		0x02
#define PPU_MASK_SPRITE_LEFT	0x04
#define PPU_MASK_SHOW_BG		0x08
#define PPU_MASK_SHOW_SPRITES	0x10
#define PPU_MASK_INTENSE_RED	0x20
#define PPU_MASK_INTENSE_GREEN	0x40
#define PPU_MASK_INTENSE_BLUE	0x80

//PPU Status Bits
#define PPU_STATUS_LAST_PPU_WRITES1	0x01
#define PPU_STATUS_LAST_PPU_WRITES2	0x02
#define PPU_STATUS_LAST_PPU_WRITES3	0x04
#define PPU_STATUS_LAST_PPU_WRITES4	0x08
#define PPU_STATUS_LAST_PPU_WRITES5	0x10
#define PPU_STATUS_SPRITE_OVERFLOW	0x20
#define PPU_STATUS_SPRITE_ZERO_HIT	0x40
#define PPU_STATUS_VBLANK_STARTED	0x80

//PPU Control bits
#define PPU_CONTROL_NAMETABLE_ADDR1     0x01
#define PPU_CONTROL_NAMETABLE_ADDR2     0x02
#define PPU_CONTROL_VRAM_ADDR_INCR      0x04
#define PPU_CONTROL_SPRITE_PATTERN_ADDR 0x08
#define PPU_CONTROL_BG_PATTERN_ADDR     0x10
#define PPU_CONTROL_SPRITE_SIZE         0x20
#define PPU_CONTROL_PPU_SELECT          0x40
#define PPU_CONTROL_NMI                 0x80

#define SCREEN_WIDTH			256
#define SCREEN_HEIGHT			240

//Scanline rendering
#define PPU_PER_SCANLINE		341
#define SCANLINE_RENDER_START	0
#define SCANLINE_RENDER_END		239
#define SCANLINE_VBLANK_START	240
#define SCANLINE_VBLANK_END		259
#define SCANLINE_FRAME_END		261
#define SCANLINE_INIT			241

//Sprite evaluation
//http://wiki.nesdev.com/w/index.php/PPU_sprite_evaluation
#define FIRST_SPRITE		252
#define LEFT_PATTERN_TABLE	0x1000
#define RIGHT_PATTERN_TABLE	0x2000

//Background rendering
#define ADDR_PALETTE_BG			0x3F00


//Timing
#define PIXEL_CLOCK_PER_SCANLINE = 341

class ppu {
public:
	ppu();
	void init(Memory *memory);

	//Colours in palette table
	std::vector<palette_entry> palette_table = {
		{ 124,124,124,1 },{ 124,124,124,1 },{ 0,0,252,1 },{ 0,0,188,1 },{ 68,40,188,1 },
		{ 148,0,132,1 },{ 168,0,32,1 },{ 168,16,0,1 },{ 136,20,0,1 },
		{ 80,48,0,1 },{ 0,120,0,1 },{ 0,104,0,1 },{ 0,88,0,1 },
		{ 0,64,88,1 },{ 0,0,0,1 },{ 0,0,0,1 },{ 0,0,0,1 },
		{ 188,188,188,1 },{ 0,120,248,1 },{ 0,88,248,1 },{ 104,68,252,1 },
		{ 216,0,204,1 },{ 228,0,88,1 },{ 248,56,0,1 },{ 228,92,16,1 },
		{ 172,124,0,1 },{ 0,184,0,1 },{ 0,168,0,1 },{ 0,168,68,1 },
		{ 0,136,136,1 },{ 0,0,0,1 },{ 0,0,0,1 },{ 0,0,0,1 },
		{ 248,248,248,1 },{ 60,188,252,1 },{ 104,136,252,1 },{ 152,120,248,1 },
		{ 248,120,248,1 },{ 248,88,152,1 },{ 248,120,88,1 },{ 252,160,68,1 },
		{ 248,184,0,1 },{ 184,248,24,1 },{ 88,216,84,1 },{ 88,248,152,1 },
		{ 0,232,216,1 },{ 120,120,120,1 },{ 0,0,0,1 },{ 0,0,0,1 },
		{ 252,252,252,1 },{ 164,228,252,1 },{ 184,184,248,1 },{ 216,184,248,1 },
		{ 248,184,248,1 },{ 248,164,192,1 },{ 240,208,176,1 },{ 252,224,168,1 },
		{ 248,216,120,1 },{ 216,248,120,1 },{ 184,248,184,1 },{ 184,248,216,1 },
		{ 0,252,252,1 },{ 248,216,248,1 },{ 0,0,0,1 },{ 0,0,0,1 }
	};

	void runCycles(unsigned short cycles);

	void renderScanline();

	//Reading from PPU Registers
	unsigned char readPPUCTRL();
	unsigned char readPPUMASK();
	unsigned char readPPUSTATUS();
	unsigned char readOAMADDR();
	unsigned char readOAMDATA();
	unsigned char readPPUSCROLL();
	unsigned char readPPUADDR();
	unsigned char readPPUDATA();
	unsigned char readOAMDMA();
	
	//Writing to PPU Registers
	void writePPUCTRL(unsigned char value);
	void writePPUMASK(unsigned char value);
	void writePPUSTATUS(unsigned char value);
	void writeOAMADDR(unsigned char value);
	void writeOAMDATA(unsigned char value);
	void writePPUSCROLL(unsigned char value);
	void writePPUADDR(unsigned char value);
	void writePPUDATA(unsigned char value);
	void writeOAMDMA(unsigned char value);


private:
	//Memory
	Memory *_memory;
	renderer * _renderer;

	unsigned char *OAM;
	unsigned char *ppuMemory;
	unsigned short ppuAddress;
	unsigned short spriteAddress;
	unsigned short ppuLatch;
	unsigned short ppuDataLatch;
	unsigned short ppuCycles;
	bool transferLatch;
	bool transferLatchScroll;
	bool ppuMemoryAccess;

	//VRAM Reading 
	char getSpritePatternPlaneRow(unsigned baseAddress, unsigned short tileIndex, unsigned short tileRow, unsigned short plane);
	unsigned char read(unsigned short address);
	unsigned char readDirect();
	void write(unsigned short address, unsigned char value);
	void writeDirect(unsigned char value);
	unsigned short normalizeAddress(unsigned short address);
	

	//Registers
	unsigned char ppuctrl;
	unsigned char ppumask;
	unsigned char ppustatus;
	unsigned char oamdata;
	unsigned char ppuscroll;
	unsigned char ppudata;
	unsigned char oamdma;

	//Rendering
	unsigned short scanline;
	unsigned short lastScanline;
	bool inVBlank;
	bool executedNMI;
	char getPixelColourFromPatternTables(unsigned char patternPlane0, unsigned char patternPlane1, unsigned short paletteUpperBits, unsigned short pixelIndex, bool flip);

	//Background
	unsigned short scroll_x;

	void evaluateSprites();
	void renderBackground();



};

#endif