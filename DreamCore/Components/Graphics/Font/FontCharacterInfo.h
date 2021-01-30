#pragma once

namespace octronic::dream
{
	typedef struct
	{
		float ax; // advance.x
		float ay; // advance.y
		float bw; // bitmap.width;
		float bh; // bitmap.rows;
		float bl; // bitmap_left;
		float bt; // bitmap_top;
		float tx; // x offset of glyph in texture coordinates
	} FontCharacterInfo;
}
