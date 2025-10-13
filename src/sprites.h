#ifndef __Sprites_h_INCLUDE
#define __Sprites_h_INCLUDE

/* Bank of tiles. */
#define SpritesBank 0

/* Super Gameboy palette 0 */
#define SpritesSGBPal0c0 6076
#define SpritesSGBPal0c1 8935
#define SpritesSGBPal0c2 6596
#define SpritesSGBPal0c3 5344

/* Super Gameboy palette 1 */
#define SpritesSGBPal1c0 6076
#define SpritesSGBPal1c1 8935
#define SpritesSGBPal1c2 6596
#define SpritesSGBPal1c3 5344

/* Super Gameboy palette 2 */
#define SpritesSGBPal2c0 6076
#define SpritesSGBPal2c1 8935
#define SpritesSGBPal2c2 6596
#define SpritesSGBPal2c3 5344

/* Super Gameboy palette 3 */
#define SpritesSGBPal3c0 6076
#define SpritesSGBPal3c1 8935
#define SpritesSGBPal3c2 6596
#define SpritesSGBPal3c3 5344

/* Gameboy Color palette 0 */
#define SpritesCGBPal0c0 32389
#define SpritesCGBPal0c1 6111
#define SpritesCGBPal0c2 4447
#define SpritesCGBPal0c3 0

/* Gameboy Color palette 1 */
#define SpritesCGBPal1c0 32389
#define SpritesCGBPal1c1 6111
#define SpritesCGBPal1c2 26527
#define SpritesCGBPal1c3 0

/* Gameboy Color palette 2 */
#define SpritesCGBPal2c0 32389
#define SpritesCGBPal2c1 5082
#define SpritesCGBPal2c2 5760
#define SpritesCGBPal2c3 0

/* Gameboy Color palette 3 */
#define SpritesCGBPal3c0 32389
#define SpritesCGBPal3c1 10271
#define SpritesCGBPal3c2 8834
#define SpritesCGBPal3c3 5136

/* Gameboy Color palette 4 */
#define SpritesCGBPal4c0 32389
#define SpritesCGBPal4c1 10271
#define SpritesCGBPal4c2 5456
#define SpritesCGBPal4c3 5136

/* Gameboy Color palette 5 */
#define SpritesCGBPal5c0 32389
#define SpritesCGBPal5c1 341
#define SpritesCGBPal5c2 6342
#define SpritesCGBPal5c3 0

/* Gameboy Color palette 6 */
#define SpritesCGBPal6c0 32389
#define SpritesCGBPal6c1 32767
#define SpritesCGBPal6c2 16957
#define SpritesCGBPal6c3 0

/* Gameboy Color palette 7 */
#define SpritesCGBPal7c0 32389
#define SpritesCGBPal7c1 32650
#define SpritesCGBPal7c2 29491
#define SpritesCGBPal7c3 32767

/* CGB palette entries. */
#define SpritesCGB0 0
#define SpritesCGB1 1
#define SpritesCGB2 0
#define SpritesCGB3 1
#define SpritesCGB4 2
#define SpritesCGB5 2
#define SpritesCGB6 2
#define SpritesCGB7 2
#define SpritesCGB8 4
#define SpritesCGB9 3
#define SpritesCGB10 3
#define SpritesCGB11 3
#define SpritesCGB12 5
#define SpritesCGB13 5
#define SpritesCGB14 5
#define SpritesCGB15 5
#define SpritesCGB16 6
#define SpritesCGB17 6
#define SpritesCGB18 6
#define SpritesCGB19 6
#define SpritesCGB20 6
#define SpritesCGB21 6

/* Sprite palette data. */
extern const unsigned short SpritePalette[32];

/* Start of tile array. */
extern const unsigned char Sprites[];

/* Sprite number entries. */
#define SpriteNumPlayer0_0 0
#define SpriteNumPlayer0_1 1
#define SpriteNumPlayer0_2 2
#define SpriteNumPlayer0_3 3
#define SpriteNumPlayer1_0 20
#define SpriteNumPlayer1_1 21
#define SpriteNumPlayer1_2 22
#define SpriteNumPlayer1_3 23
#define SpriteNumPlayer2_0 24
#define SpriteNumPlayer2_1 25
#define SpriteNumPlayer2_2 26
#define SpriteNumPlayer2_3 27

#define SpriteNumLeaf1TopLeft 4
#define SpriteNumLeaf1TopRight 5
#define SpriteNumLeaf1BottomLeft 6
#define SpriteNumLeaf1BottomRight 7

#define SpriteNumLeaf2TopLeft 8
#define SpriteNumLeaf2TopRight 9
#define SpriteNumLeaf2BottomLeft 10
#define SpriteNumLeaf2BottomRight 11

#define SpriteNumLeaf3TopLeft 12
#define SpriteNumLeaf3TopRight 13
#define SpriteNumLeaf3BottomLeft 14
#define SpriteNumLeaf3BottomRight 15

#define SpriteNumAppleBombTopLeft 16
#define SpriteNumAppleBombTopRight 17
#define SpriteNumAppleBombBottomLeft 18
#define SpriteNumAppleBombBottomRight 19

#endif

/* End of SPRITES.H */
