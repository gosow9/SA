/*
 * GUI_Paint.h
 *
 *  Created on: Nov 17, 2019
 *      Author: crenda
 */


#ifndef __GUI_PAINT_H
#define __GUI_PAINT_H

//#include "DEV_Config.h"
#include "fonts.h"

/**
 * Image attributes
**/
typedef struct {
    uint8_t *add;
    uint16_t Width;
    uint16_t Height;
    uint16_t WidthMemory;
    uint16_t HeightMemory;
    uint16_t Color;
    uint16_t Rotate;
    uint16_t Mirror;
    uint16_t WidthByte;
    uint16_t HeightByte;
} IMAGE;
extern IMAGE Image;

/**
 * Display rotate
**/
#define ROTATE_0            0
#define ROTATE_90           90
#define ROTATE_180          180
#define ROTATE_270          270

/**
 * Display Flip
**/
typedef enum {
    MIRROR_NONE  = 0x00,
    MIRROR_HORIZONTAL = 0x01,
    MIRROR_VERTICAL = 0x02,
    MIRROR_ORIGIN = 0x03,
} MIRROR_IMAGE;
#define MIRROR_IMAGE_DFT MIRROR_NONE

/**
 * image color
**/
#define WHITE          0xFF
#define BLACK          0x00

#define IMAGE_BACKGROUND    WHITE
#define FONT_FOREGROUND     BLACK
#define FONT_BACKGROUND     WHITE

/**
 * The size of the point
**/
typedef enum {
    DOT_PIXEL_1X1  = 1,		// 1 x 1
    DOT_PIXEL_2X2  , 		// 2 X 2
    DOT_PIXEL_3X3  ,		// 3 X 3
    DOT_PIXEL_4X4  ,		// 4 X 4
    DOT_PIXEL_5X5  , 		// 5 X 5
    DOT_PIXEL_6X6  , 		// 6 X 6
    DOT_PIXEL_7X7  , 		// 7 X 7
    DOT_PIXEL_8X8  , 		// 8 X 8
} DOT_PIXEL;
#define DOT_PIXEL_DFT  DOT_PIXEL_1X1  //Default dot pilex

/**
 * Point size fill style
**/
typedef enum {
    DOT_FILL_AROUND  = 1,		// dot pixel 1 x 1
    DOT_FILL_RIGHTUP  , 		// dot pixel 2 X 2
} DOT_STYLE;
#define DOT_STYLE_DFT  DOT_FILL_AROUND  //Default dot pilex

/**
 * Line style, solid or dashed
**/
typedef enum {
    LINE_STYLE_SOLID = 0,
    LINE_STYLE_DOTTED,
} LINE_STYLE;

/**
 * Whether the graphic is filled
**/
typedef enum {
    DRAW_FILL_EMPTY = 0,
    DRAW_FILL_FULL,
} DRAW_FILL;


//init and Clear
void setNewImage(uint8_t *image, uint16_t Width, uint16_t Height, uint16_t Rotate, uint16_t Color);
void selectImage(uint8_t *image);
void setRotateImage(uint16_t Rotate);
void setMirroringImage(uint8_t mirror);
void setPixel(uint16_t Xpt, uint16_t Ypt, uint16_t Color);
void clear_Image(uint8_t Color);

//Drawing
void Paint_DrawPoint(uint16_t Xpt, uint16_t Ypt, uint16_t Color, DOT_PIXEL Dot_Pixel, DOT_STYLE Dot_FillWay);
void Paint_DrawLine(uint16_t xStart, uint16_t yStart, uint16_t Xend, uint16_t Yend, uint16_t Color, LINE_STYLE Line_Style, DOT_PIXEL Dot_Pixel);

//Display string
void drawChar(uint16_t xStart, uint16_t yStart, const char Acsii_Char, sFONT* Font, uint8_t cBackground, uint8_t cForeground);
void drawString(uint16_t xStart, uint16_t yStart, const char * pString, sFONT* Font, uint8_t cBackground, uint8_t cForeground);


#endif





