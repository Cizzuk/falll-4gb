#ifndef __World_h_INCLUDE
#define __World_h_INCLUDE

#define WorldWidth 20U
#define WorldHeight 32U
#define WorldBank 0U

#define WORLD_ATTR_RANDOM_COL_COUNT 4U
#define WORLD_ATTR_RANDOM_PALETTE_MIN 2U
#define WORLD_ATTR_RANDOM_PALETTE_MAX 5U

extern unsigned char WorldPalette[WorldWidth];
extern const unsigned char WorldAttrRandomColumns[WORLD_ATTR_RANDOM_COL_COUNT];

extern const unsigned char World[];

#endif

/* End of WORLD.H */
