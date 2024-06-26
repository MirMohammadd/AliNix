/**
 ** This file is part of AliNix.

**AliNix is free software: you can redistribute it and/or modify
**it under the terms of the GNU Affero General Public License as published by
**the Free Software Foundation, either version 3 of the License, or
**(at your option) any later version.

**AliNix is distributed in the hope that it will be useful,
**but WITHOUT ANY WARRANTY; without even the implied warranty of
**MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
**GNU Affero General Public License for more details.

**You should have received a copy of the GNU Affero General Public License
**along with AliNix. If not, see <https://www.gnu.org/licenses/>.
*/

#include <arch/x86-pc/io/keyboard.h>
#include <arch/x86/io-ports.h>
#include "vga.h"

/** Video RAM starting adress */
#define SCREEN_START 		0xB8000
/** Video screen page size: 4000 bytes or 4 Ko */
#define SCREEN_PAGE_SIZE	0xFA0
/** Video screen page maximal offset = SCREEN_START + SCREEN_PAGE_SIZE = 0xB8000 + 0xFA0 = 0xB8FA0 */
#define SCREEN_PAGE_LIMIT (SCREEN_START + SCREEN_PAGE_SIZE)

/** Maximum columns on a VGA screen */
#define VGA_COLUMNS 80

#define VGA_LINES_MAX_INDEX  24
#define VGA_COLUMNS_MAX_INDEX 79

/** VGA control register */
#define VGA_CONTROL_REGISTER    0x3D4
/** VGA data register */
#define VGA_DATA_REGISTER       0x3D5

struct vga_symbol
{
	/** Position X on the screen */
	uint8_t position_x;
	/** Position Y on the screen */
	uint8_t position_y;
	/** Represents a foreground/background color set */
	uint8_t attributes;
}__attribute__((packed, aligned(4)));

struct vga_symbol symbol;

void vga_clear(void)
{
	uint8_t *video;

	for (video = (uint8_t *)SCREEN_START;
			video <= (uint8_t *)SCREEN_PAGE_LIMIT;
			video = video + 2)
	{
		*video = 0;
		*(video + 1) = BG_BLACK | FG_WHITE;
	}

	/* Move the cursor back to the starting point */
	vga_set_position(0, 0);
	vga_update_cursor();
}

void vga_update_cursor(void)
{
	uint16_t position = (symbol.position_y * 80) + symbol.position_x;

	outb(VGA_CONTROL_REGISTER, 0x0F);
	outb(VGA_DATA_REGISTER, (unsigned char)(position & 0xFF));

	outb(VGA_CONTROL_REGISTER, 0x0E);
	outb(VGA_DATA_REGISTER, (unsigned char)((position >> 8) & 0xFF));
}

void vga_scroll_up(uint8_t nb_lines)
{
	uint8_t *video;
	uint8_t *tmp;

	for( video = (uint8_t *)SCREEN_START;
			video < (uint8_t *)SCREEN_PAGE_LIMIT;
			video++)
	{
		tmp = (uint8_t *)(video + nb_lines * VGA_COLUMNS * 2);

		if (tmp < (uint8_t *)SCREEN_PAGE_LIMIT)
			*video = *tmp;
		else
			*video = 0;
	}

	symbol.position_y -= nb_lines;
	vga_update_cursor();
}

void vga_set_position(uint8_t x, uint8_t y)
{
	symbol.position_x = x;
	symbol.position_y = y;
}

void vga_update_position(int8_t x, int8_t y)
{
	symbol.position_x += x;
	symbol.position_y += y;
}

void vga_set_attributes(uint8_t attributes)
{
	symbol.attributes = attributes;
}

void vga_display_character(uchar_t character)
{
	uint8_t* video = (uchar_t*)(SCREEN_START + 2 * symbol.position_x
			+ VGA_COLUMNS * 2 * symbol.position_y);


	switch(character)
	{
		case KBD_CR_NL: /* Carriage return or new line */
			symbol.position_x = 0;
			symbol.position_y++;

			if (symbol.position_y > VGA_LINES_MAX_INDEX)
				vga_scroll_up(symbol.position_y - VGA_LINES_MAX_INDEX);
			break;

		case KBD_BACKSPACE:
			if (symbol.position_x > 0)
			{
				symbol.position_x--;
				video = (uchar_t *)(SCREEN_START + 2
						* symbol.position_x
						+ VGA_COLUMNS * 2
						* symbol.position_y);
			}
			else if (symbol.position_y > 0)
			{
				symbol.position_y--;
				symbol.position_x = VGA_COLUMNS_MAX_INDEX;
				video = (uchar_t *)(SCREEN_START + 2
						* symbol.position_x
						+ VGA_COLUMNS * 2
						* symbol.position_y);
			}

			*video = ' ';
			*(video+1) = symbol.attributes;
			break;

		case KBD_TABULATION:
			if (symbol.position_x + 4 > VGA_COLUMNS)
				; /* What to do? */
			else
				symbol.position_x += 4;
			break;
		
		case KEY_ESCAPE:
			*video = "Welcome to the Heisen Kernel!";
			*(video+1) = symbol.attributes;

		if (symbol.position_x > VGA_COLUMNS_MAX_INDEX)
			{
				symbol.position_x = 0;
				symbol.position_y++;
			}

			if (symbol.position_y > VGA_LINES_MAX_INDEX)
				vga_scroll_up(symbol.position_y - VGA_LINES_MAX_INDEX);
	



		default: /* Other characters */
			*video     = character;
			*(video+1) = symbol.attributes;

			symbol.position_x++;

			if (symbol.position_x > VGA_COLUMNS_MAX_INDEX)
			{
				symbol.position_x = 0;
				symbol.position_y++;
			}

			if (symbol.position_y > VGA_LINES_MAX_INDEX)
				vga_scroll_up(symbol.position_y - VGA_LINES_MAX_INDEX);
	}

	vga_update_cursor();
}
