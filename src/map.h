#ifndef __Map_h_INCLUDE
#define __Map_h_INCLUDE

#define MapTitleWidth 11U
#define MapTitleHeight 18U
#define MapGameWidth 11U
#define MapGameHeight 32U
#define MapBank 0U

#define MAP_ATTR_WIDTH 11U
#define MAP_ATTR_HEIGHT 32U

#define MAP_ATTR_TREE_COLS_COUNT 7U
#define MAP_ATTR_RANDOM_COLS_COUNT 4U

#define WORLD_ATTR_RANDOM_PALETTE_MIN 2U
#define WORLD_ATTR_RANDOM_PALETTE_MAX 5U

extern const unsigned char MapTreePalette[MAP_ATTR_HEIGHT];
extern const unsigned char MapAttrTreeCols[MAP_ATTR_TREE_COLS_COUNT];
extern const unsigned char MapAttrRandomCols[MAP_ATTR_RANDOM_COLS_COUNT];

extern const unsigned char MapTitle[];
extern const unsigned char MapGame[];

#endif

/* End of MAP.H */
