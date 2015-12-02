#include "ppu.h"
#include "memory.h"
#include <iostream>


ppu::ppu() {
	//TODO - Initializer for PPU
}

void ppu::init(Memory * memory) {
	_memory = memory;
	//Initialize sprite memory
	OAM = new unsigned char[256];
	ppuMemory = new unsigned char[16384];

	_renderer = new renderer();

	_renderer->init();
}

void ppu::runCycles(unsigned short cycles) {
	ppuCycles += cycles * 3; 

	//Do we have enough cycles to finish the scanline?
	if (ppuCycles >= PPU_PER_SCANLINE) {
		//Increment our scanline
		scanline++;// = ppuCycles / PPU_PER_SCANLINE;
		//Use the remaining cycles next time
		ppuCycles = ppuCycles % PPU_PER_SCANLINE;
	}

	//Rendering period, scanlines 0 - 240
	if (scanline >= 0 && scanline <= SCANLINE_RENDER_END) {

		bool drawSprites = ((ppumask & PPU_MASK_SHOW_SPRITES) || (ppumask & PPU_MASK_SHOW_BG));

		//Have we transferred from ppuLatch yet?
		if (!transferLatch && drawSprites) {
			ppuAddress = ppuLatch;
			transferLatch = true;
		}

		//Have we already rendered this scanline?
		if (scanline != lastScanline) {
			renderScanline();

			lastScanline = scanline;

			if (drawSprites) {
				//TODO Figure out what this actually does
				ppuAddress = (ppuAddress & (~0x1F & ~(1 << 10))) | (ppuLatch & (0x1F | (1 << 10)));
			}
		}
	}
	//Vblank start period
	//Vblank consists from scanlines 241 - 260, PPU makes no memory accesses during these cycles 
	else if (scanline == SCANLINE_VBLANK_START) {
		if (!inVBlank) {
			inVBlank = true;
			ppustatus |= PPU_STATUS_VBLANK_STARTED;
		}
	}

	//End of frame period
	else if (scanline > SCANLINE_FRAME_END) {
		//Set spriteoverflow and zerohit to false
		ppustatus &= ~PPU_STATUS_SPRITE_OVERFLOW;
		ppustatus &= ~PPU_STATUS_SPRITE_ZERO_HIT;

		//TODO - Update screen, we're all done drawing the scanlines
		_renderer->update();

		transferLatch = false;
		transferLatchScroll = false;

		scanline = 0;
		lastScanline = 0;
	}
	//End of Vblank period
	else if (scanline > SCANLINE_VBLANK_END) {
		if (inVBlank) {
			inVBlank = false;
			ppustatus |= ~PPU_STATUS_VBLANK_STARTED;
		}
	}

	if (inVBlank && !executedNMI && (ppuctrl & PPU_CONTROL_NMI) && (ppustatus & PPU_STATUS_VBLANK_STARTED)) {
		executedNMI = true;
		//TODO - Tell CPU to run an NMI
	}
}

void ppu::renderScanline() {
	//TODO - Account for 8x8 vs 8x16 sprites
	renderBackground();
	evaluateSprites();
}
/*
	Run every scanline, renders the background pixels for the scanline and 
	sends their colour and index to the renderer.
*/

void ppu::renderBackground() {
	//Which pattern table to use, 4th bit of PPUCTRL
	unsigned short patternTableAddr = ((ppuctrl >> 4) & 1 ? 0x0000 : 0x1000);

	unsigned short nameTableAddr, attributeTableAddr, tileAddress;

	unsigned short tileX, tileY, tileScrollY, tileScrollX;
	
	//Render tiles in nametable, 32x30
	for (int tileNum = 0; tileNum < 32; tileNum++) {
		//Bit 10 of the ppu_address dictates what kind of mirroring to use
		//Either 0x2000, 0x2400, 0x2800, 0x2C00
		nameTableAddr = 0x2000 + ((ppuAddress >> 10) & 0x03) * 0x400;
		//Attribute table exists 0x03C0 to the right of the name table
		attributeTableAddr = nameTableAddr + 0x03C0;

		tileX = ppuAddress & 0x1F;
		tileY = (ppuAddress >> 5) & 0x1F;
		tileScrollY = (ppuAddress >> 12) & 0x07;
		tileScrollX = scroll_x;

		tileAddress = nameTableAddr | (ppuAddress & 0x03FF);


		unsigned short tileIndex;
		unsigned char patternPlane1, patternPlane2, paletteUpperBits, attributeValue;
		unsigned short attributeAddress, groupIndex, pixelColourIndex;


		//Tiles are 8x8
		for (int pixelNum = 0; pixelNum < 8; pixelNum++) {
			tileIndex = read(tileAddress);
			patternPlane1 = read(patternTableAddr + (tileIndex << 4) + tileScrollY);
			patternPlane2 = read(patternTableAddr + (tileIndex << 4) + tileScrollY + 8);

			//value = (topleft << 0) | (topright << 2) | (bottomleft << 4) | (bottomright << 6)
			attributeAddress = attributeTableAddr | (((((tileY * 8) + tileScrollY) / 32) * (SCREEN_WIDTH / 32)) + (((tileX * 8) + tileScrollX) / 32));
			attributeValue = read(attributeAddress);

			groupIndex = (((tileX % 4) & 0x2) >> 1) + ((tileY % 4) & 0x2);
			paletteUpperBits = ((attributeValue >> (groupIndex << 1)) & 0x3) << 2;

			//Get the color of this pixel
			pixelColourIndex = getPixelColourFromPatternTables(patternPlane1, patternPlane2, paletteUpperBits, pixelNum, false);

			//Transparent pixel
			if (pixelColourIndex == 0) {
				int x = (tileNum << 3) + pixelNum;
				int y = scanline;
				palette_entry color = palette_entry();
				color.r = 0;
				color.g = 0;
				color.b = 0;
				color.a = 0;
				_renderer->put_pixel(PixelType::BACKGROUND_TILE, x, scanline, color);
			}
			else {
				int x = (tileNum << 3) + pixelNum;
				int y = scanline;
				palette_entry color = palette_table.at(pixelColourIndex);
				_renderer->put_pixel(PixelType::BACKGROUND_TILE, x, scanline, color);
			}
		}
	}
}



/* Run every scanline. Reads through the OAM to determine which sprites must be drawn on that scanline
Sprites found to be within range are copied into a second OAM which is used to initialize the sprites.*/
void ppu::evaluateSprites() {
	unsigned short activeSprites = 0;

	//Which sprite we are currently evaluating
	int currentSprite;

	//Data store in OAM about each sprite
	unsigned short spriteX, spriteY, tileIndex, spriteAttribute, pixelColourUpperBits;

	//The Y position of the sprite, relative to the current scanline
	unsigned short scanlineRange;

	//Pattern plane (top or bottom)
	unsigned char patternPlane0, patternPlane1;

	//First decide which half pattern table to use
	unsigned short patternTableHalf = ((ppuctrl >> 3) & 1 ? LEFT_PATTERN_TABLE : RIGHT_PATTERN_TABLE);

	//Iterate through the sprites, starting at the last sprite (252) first.
	//Each sprite is 4 bytes
	for (currentSprite = FIRST_SPRITE; currentSprite >= 0; currentSprite -= 4) {
		tileIndex = OAM[currentSprite + 1];
		spriteAttribute = OAM[currentSprite + 2];
		spriteX = OAM[currentSprite + 3];
		//Y is located at currentSprite + 0 but we have to add 1
		spriteY = OAM[currentSprite] + 1;

		//Sprite colour palette 
		//Shift it left by two because our final address is of the form:
		/*	43210
			|||||
			|||++- Pixel value from tile data
			|++--- Palette number from attribute table or OAM
			+----- Background/Sprite select
		*/
		pixelColourUpperBits = (spriteAttribute & 0b11) << 2;
		

		//Check 6th bit for horizontal flip
		bool horizontalFlip = (spriteAttribute >> 6) & 1;
		//Check 7th bit for vertical flip
		bool verticalFlip = (spriteAttribute >> 7) & 1;

		scanlineRange = scanline - spriteY;

		//Is the sprite in range of our current scanline?
		if (scanlineRange < 8) {

			//Get the pattern data for that sprite
			patternPlane0 = ppu::getSpritePatternPlaneRow(patternTableHalf, tileIndex, scanlineRange, 0);
			patternPlane1 = ppu::getSpritePatternPlaneRow(patternTableHalf, tileIndex, scanlineRange, 1);


			//Iterate through the pattern pixel by pixel (8x8 pattern)
			unsigned short pixelColourIndex;
			for (int pixel = 0; pixel < 8; pixel++) {
				
				pixelColourIndex = pixelColourUpperBits;

				//Calculate the pixel colour to use.
				//There are four different values, 0, 1, 2, 3
				//http://wiki.nesdev.com/w/index.php/PPU_pattern_tables
				//0 uses the background colour

				//If we have a horizontal flip, we start on the last pixel first
				if (horizontalFlip) {
					//Important to note: 1 | 2 = 3
					//Check the first plane
					pixelColourIndex |= patternPlane0 & (0x1 << pixel) ? 1 : 0;
					//Check the second plane
					pixelColourIndex |= patternPlane1 & (0x1 << pixel) ? 2 : 0;
				}
				else {
					//Check the first plane
					pixelColourIndex |= patternPlane0 & (0x80 >> pixel) ? 1 : 0;
					//Check the second plane
					pixelColourIndex |= patternPlane1 & (0x80 >> pixel) ? 2 : 0;
				}

				//If our colourIndex is 0, we have a transparent pixel
				if (pixelColourIndex & 0x3 == 0) {
					int x = spriteX + pixel;
					int y = scanline;
					palette_entry color = palette_entry();
					color.r = 0;
					color.g = 0;
					color.b = 0;
					color.a = 0;
					_renderer->put_pixel(PixelType::BACKGROUND_TILE, x, scanline, color);

				}

				else {
					//TODO - Check for zero-hit sprite and update PPU status
					int x = spriteX + pixel;
					int y = scanline;
					palette_entry color = palette_table.at(pixelColourIndex);
					_renderer->put_pixel(PixelType::BACKGROUND_TILE, x, scanline, color);
				}

			}

		}
	}

}
/*
Get the index in the palette (colour) of the passed pixel index and its corresponding pattern planes.
*/

char ppu::getPixelColourFromPatternTables(unsigned char patternPlane0, unsigned char patternPlane1, unsigned short paletteUpperBits, unsigned short pixelIndex, bool flip) {
	
	//Horizontally flip the palette?
	if (flip) pixelIndex = (0x80 >> pixelIndex);
	else pixelIndex = (0x1 << pixelIndex);

	unsigned short pixelColourIndex = paletteUpperBits;
	pixelColourIndex |= patternPlane0 & pixelIndex ? 0x1 : 0x0;
	pixelColourIndex |= patternPlane1 & pixelIndex ? 0x2 : 0x0;
	return pixelColourIndex;
}

char ppu::getSpritePatternPlaneRow(unsigned baseAddress, unsigned short tileIndex, unsigned short tileRow, unsigned short plane) {
	return	_memory->read(baseAddress + (tileIndex << 4) + tileRow + plane*8);
}


unsigned char ppu::read(unsigned short address) {
	//TODO - Make compatible with different mappers

	return ppuMemory[address];
}

unsigned char ppu::readDirect() {
	return ppuMemory[ppuAddress];
}

void ppu::writeDirect(unsigned char value) {
	ppuMemory[ppuAddress] = value;
}

void ppu::write(unsigned short address, unsigned char value) {
	ppuMemory[address] = value;
}



unsigned short ppu::normalizeAddress(unsigned short address) {
	//TODO - Normalize address based on mapper
	return 0;
}


/************
Reading PPU Registers
*************/


/* Read the PPUCTRL register */
unsigned char ppu::readPPUCTRL()
{
	return ppuctrl;
}

/* Read the PPUMASK register */
unsigned char ppu::readPPUMASK()
{
	return ppumask;
}

/* Read the PPUSTATUS register */
unsigned char ppu::readPPUSTATUS()
{
	return ppustatus;
}

/* Read the OAMADDR register */
unsigned char ppu::readOAMADDR()
{
	return oamaddr;
}

/* Read the OAMDATA register */
unsigned char ppu::readOAMDATA()
{
	return oamdata;
}

/* Read the PPUSCROLL register */
unsigned char ppu::readPPUSCROLL()
{
	return ppuscroll;
}

/* Read the PPUADDR register */
unsigned char ppu::readPPUADDR()
{
	return ppuaddr;
}

/* Read the PPUDATA register */
unsigned char ppu::readPPUDATA()
{
	return ppudata;
}

/* Read the OAMDMA register */
unsigned char ppu::readOAMDMA()
{
	return oamdma;
}

/************
Writing PPU Registers
*************/

/* Write the PPUCTRL register */
void ppu::writePPUCTRL(unsigned char value)
{
	ppuctrl = value;
}

/* Write the PPUMASK register */
void ppu::writePPUMASK(unsigned char value)
{
	ppumask = value;
}

/* Write the PPUSTATUS register */
void ppu::writePPUSTATUS(unsigned char value)
{
	ppustatus = value;
}

/* Write the OAMADDR register */
void ppu::writeOAMADDR(unsigned char value)
{
	oamaddr = value;
}

/* Write the OAMDATA register */
void ppu::writeOAMDATA(unsigned char value)
{
	oamdata = value;
}

/* Write the PPUSCROLL register */
void ppu::writePPUSCROLL(unsigned char value)
{
	ppuscroll = value;
}

/* Write the PPUADDR register */
void ppu::writePPUADDR(unsigned char value)
{
	ppuaddr = value;
}

/* Write the PPUDATA register */
void ppu::writePPUDATA(unsigned char value)
{
	ppudata = value;
}

/* Write the OAMDMA register */
void ppu::writeOAMDMA(unsigned char value)
{
	oamdata = value;
}
