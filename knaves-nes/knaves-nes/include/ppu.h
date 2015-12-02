#include <list>;
#include <map>;

#include "memory.h";
#include "cpu.h";

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



//Timing
#define PIXEL_CLOCK_PER_SCANLINE = 341

class ppu {
public:
	ppu();
	void init(Memory *memory);

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
	unsigned char *OAM;
	unsigned char *ppuMemory;
	unsigned short ppuAddress;
	unsigned short ppuLatch;
	unsigned short ppuCycles;
	bool transferLatch;
	bool transferLatchScroll;

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
	unsigned char oamaddr;
	unsigned char oamdata;
	unsigned char ppuscroll;
	unsigned char ppuaddr;
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