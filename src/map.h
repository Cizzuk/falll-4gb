#ifndef __Map_h_INCLUDE
#define __Map_h_INCLUDE

#define MapWidth 11U
#define MapHeight 32U
#define MapBank 0U

#define MAP_BASE_WIDTH 11U
#define MAP_BASE_HEIGHT 16U
#define MAP_BASE_AREA (MAP_BASE_WIDTH * MAP_BASE_HEIGHT)

#define MAP_TREE_CURTAIN 0x01
#define MAP_TREE_X 2U
#define MAP_TREE_Y 3U
#define MAP_TREE_WIDTH 7U
#define MAP_TREE_HEIGHT 15U
#define MAP_TREE_AREA (MAP_TREE_WIDTH * MAP_TREE_HEIGHT)

#define MAP_ATTR_TREE_COLS_COUNT 7U
#define MAP_ATTR_RANDOM_COLS_COUNT 4U

#define MAP_ATTR_RANDOM_PALETTE_MIN 2U
#define MAP_ATTR_RANDOM_PALETTE_MAX 5U

extern const unsigned char MapTreePalette[MapHeight];
extern const unsigned char MapAttrTreeCols[MAP_ATTR_TREE_COLS_COUNT];
extern const unsigned char MapAttrRandomCols[MAP_ATTR_RANDOM_COLS_COUNT];

extern const unsigned char MapBase[MAP_BASE_AREA];
extern const unsigned char MapObjTree[MAP_TREE_AREA];
extern const unsigned char MapObjTreeCurtain[MAP_TREE_AREA];

#endif

/* End of MAP.H */
