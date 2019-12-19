/*
 * GUI_Paint.c
 *
 *  Created on: Nov 17, 2019
 *      Author: crenda
 */
#include "GUI_Paint.h"
//#include "DEV_Config.h"
//#include "Debug.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h> //memset()
#include <math.h>

PAINT Paint;
/******************************************************************************
//function:	Create Image
//parameter:
//    image   :   Pointer to the image cache
//    width   :   The width of the picture
//    Height  :   The height of the picture
//    Color   :   Whether the picture is inverted
******************************************************************************/
void setNewImage(uint8_t *image, uint16_t Width, uint16_t Height, uint16_t Rotate, uint16_t Color)
{
    Paint.Image = NULL;
    Paint.Image = image;

    Paint.WidthMemory = Width;
    Paint.HeightMemory = Height;
    Paint.Color = Color;    
    //Paint.WidthByte = (Width % 8 == 0)? (Width / 8 ): (Width / 8 + 1);
    Paint.WidthByte = Width;
    Paint.HeightByte = Height;    
    //printf("WidthByte = %d, HeightByte = %d\r\n", Paint.WidthByte, Paint.HeightByte);
    //printf(" EPD_WIDTH / 8 = %d\r\n",  122 / 8);
   
    Paint.Rotate = Rotate;
    Paint.Mirror = MIRROR_NONE;
    
    if(Rotate == ROTATE_0 || Rotate == ROTATE_180) {
        Paint.Width = Width;
        Paint.Height = Height;
    } else {
        Paint.Width = Height;
        Paint.Height = Width;
    }
}

/******************************************************************************
//function:	Select Image
//parameter:
//    image   :   Pointer to the image cache
******************************************************************************/
void selectImage(uint8_t *image)
{
    Paint.Image = image;
}

/******************************************************************************
//function:	Select Image Rotate
//parameter:
//    Rotate   :   0,90,180,270
******************************************************************************/
void setRotateImage(uint16_t Rotate)
{
    if(Rotate == ROTATE_0 || Rotate == ROTATE_90 || Rotate == ROTATE_180 || Rotate == ROTATE_270) {
        //Debug("Set image Rotate %d\r\n", Rotate);
        Paint.Rotate = Rotate;
    } else {
        //Debug("rotate = 0, 90, 180, 270\r\n");
    }
}

/******************************************************************************
//function:	Select Image mirror
//parameter:
//mirror   :       Not mirror,Horizontal mirror,Vertical mirror,Origin mirror
******************************************************************************/
void setMirroringImage(uint8_t mirror)
{
    if(mirror == MIRROR_NONE || mirror == MIRROR_HORIZONTAL || 
        mirror == MIRROR_VERTICAL || mirror == MIRROR_ORIGIN) {
//        Debug("mirror image x:%s, y:%s\r\n",(mirror & 0x01)? "mirror":"none", ((mirror >> 1) & 0x01)? "mirror":"none");
        Paint.Mirror = mirror;
//    } else {
////        Debug("mirror should be MIRROR_NONE, MIRROR_HORIZONTAL,
//       // MIRROR_VERTICAL or MIRROR_ORIGIN\r\n");
  }
}

/******************************************************************************
//function:	Draw Pixels
//parameter:
//    Xpt  :   At point X
//    Ypt  :   At point Y
//    Color   :   Painted colors
******************************************************************************/
void setPixel(uint16_t Xpt, uint16_t Ypt, uint16_t Color)
{
    if(Xpt > Paint.Width || Ypt > Paint.Height){
//        Debug("Exceeding display boundaries\r\n");
        return;
    }      
    uint16_t X, Y;

    switch(Paint.Rotate) {
    case 0:
        X = Xpt;
        Y = Ypt;
        break;
    case 90:
        X = Paint.WidthMemory - Ypt - 1;
        Y = Xpt;
        break;
    case 180:
        X = Paint.WidthMemory - Xpt - 1;
        Y = Paint.HeightMemory - Ypt - 1;
        break;
    case 270:
        X = Ypt;
        Y = Paint.HeightMemory - Xpt - 1;
        break;
		
    default:
        return;
    }
    
    switch(Paint.Mirror) {
    case MIRROR_NONE:
        break;
    case MIRROR_HORIZONTAL:
        X = Paint.WidthMemory - X - 1;
        break;
    case MIRROR_VERTICAL:
        Y = Paint.HeightMemory - Y - 1;
        break;
    case MIRROR_ORIGIN:
        X = Paint.WidthMemory - X - 1;
        Y = Paint.HeightMemory - Y - 1;
        break;
    default:
        return;
    }

    if(X > Paint.WidthMemory || Y > Paint.HeightMemory){
//        Debug("Exceeding display boundaries\r\n");
        return;
    }
    
   // uint32_t Addr = X / 8 + Y * Paint.WidthByte;
    uint32_t Addr = X + Y * Paint.WidthByte;
   // uint8_t Rdata = Paint.Image[Addr];
    if(Color == BLACK)
        //Paint.Image[Addr] = Rdata & ~(0x80 >> (X % 8));
    	Paint.Image[Addr] = BLACK;
    else
        //Paint.Image[Addr] = Rdata | (0x80 >> (X % 8));
    	Paint.Image[Addr] = Color;
}

/******************************************************************************
//function:	Clear the color of the picture
//parameter:
//    Color   :   Painted colors
******************************************************************************/
void clear_Image(uint8_t Color)
{
    for (uint16_t Y = 0; Y < Paint.HeightByte; Y++) {
        for (uint16_t X = 0; X < Paint.WidthByte; X++ ) {//8 pixel =  1 byte
            uint32_t Addr = X + Y*Paint.WidthByte;
            Paint.Image[Addr] = Color;
        }
    }
}


/******************************************************************************
//function:	Draw Point(Xpt, Ypt) Fill the color
//parameter:
//    Xpt		:   The Xpt coordinate of the point
//    Ypt		:   The Ypt coordinate of the point
//    Color		:   Set color
//    Dot_Pixel	:	point size
******************************************************************************/
void Paint_DrawPoint(uint16_t Xpt, uint16_t Ypt, uint16_t Color,
                     DOT_PIXEL Dot_Pixel, DOT_STYLE DOT_STYLE)
{
    if (Xpt > Paint.Width || Ypt > Paint.Height) {
//        Debug("Paint_DrawPoint Input exceeds the normal display range\r\n");
        return;
    }

    int16_t XDir_Num , YDir_Num;
    if (DOT_STYLE == DOT_FILL_AROUND) {
        for (XDir_Num = 0; XDir_Num < 2 * Dot_Pixel - 1; XDir_Num++) {
            for (YDir_Num = 0; YDir_Num < 2 * Dot_Pixel - 1; YDir_Num++) {
                if(Xpt + XDir_Num - Dot_Pixel < 0 || Ypt + YDir_Num - Dot_Pixel < 0)
                    break;
                setPixel(Xpt + XDir_Num - Dot_Pixel, Ypt + YDir_Num - Dot_Pixel, Color);
            }
        }
    } else {
        for (XDir_Num = 0; XDir_Num <  Dot_Pixel; XDir_Num++) {
            for (YDir_Num = 0; YDir_Num <  Dot_Pixel; YDir_Num++) {
                setPixel(Xpt + XDir_Num - 1, Ypt + YDir_Num - 1, Color);
            }
        }
    }
}

/******************************************************************************
//function:	Draw a line of arbitrary slope
//parameter:
//    xStart ：Starting Xpt point coordinates
//    yStart ：Starting Xpt point coordinates
//    Xend   ：End point Xpt coordinate
//    Yend   ：End point Ypt coordinate
//    Color  ：The color of the line segment
******************************************************************************/
void Paint_DrawLine(uint16_t xStart, uint16_t yStart, uint16_t Xend, uint16_t Yend,
                    uint16_t Color, LINE_STYLE Line_Style, DOT_PIXEL Dot_Pixel)
{
    if (xStart > Paint.Width || yStart > Paint.Height ||
        Xend > Paint.Width || Yend > Paint.Height) {
//        Debug("Paint_DrawLine Input exceeds the normal display range\r\n");
        return;
    }

    uint16_t Xpt = xStart;
    uint16_t Ypt = yStart;
    int dx = (int)Xend - (int)xStart >= 0 ? Xend - xStart : xStart - Xend;
    int dy = (int)Yend - (int)yStart <= 0 ? Yend - yStart : yStart - Yend;

    // Increment direction, 1 is positive, -1 is counter;
    int XAddway = xStart < Xend ? 1 : -1;
    int YAddway = yStart < Yend ? 1 : -1;

    //Cumulative error
    int Esp = dx + dy;
    char Dotted_Len = 0;

    for (;;) {
        Dotted_Len++;
        //Painted dotted line, 2 point is really virtual
        if (Line_Style == LINE_STYLE_DOTTED && Dotted_Len % 3 == 0) {
            //Debug("LINE_DOTTED\r\n");
            Paint_DrawPoint(Xpt, Ypt, IMAGE_BACKGROUND, Dot_Pixel, DOT_STYLE_DFT);
            Dotted_Len = 0;
        } else {
            Paint_DrawPoint(Xpt, Ypt, Color, Dot_Pixel, DOT_STYLE_DFT);
        }
        if (2 * Esp >= dy) {
            if (Xpt == Xend)
                break;
            Esp += dy;
            Xpt += XAddway;
        }
        if (2 * Esp <= dx) {
            if (Ypt == Yend)
                break;
            Esp += dx;
            Ypt += YAddway;
        }
    }
}


/******************************************************************************
//function:	Show characters
//parameter:
//    Xpt           ：X coordinate
//    Ypt           ：Y coordinate
//    Acsii_Char       ：To display characters
//    Font             ：A structure pointer that displays a character size
//    cBackground : Select the background color of the English character
//    cForeground : Select the foreground color of the English character
******************************************************************************/
void drawChar(uint16_t Xpt, uint16_t Ypt, const char Acsii_Char,
                    sFONT* Font, uint8_t cBackground, uint8_t cForeground)
{
    uint16_t Page, Column;

    if (Xpt > Paint.Width || Ypt > Paint.Height) {
        return;
    }

    uint32_t Char_Offset = (Acsii_Char - ' ') * Font->Height * (Font->Width / 8 + (Font->Width % 8 ? 1 : 0));
    const unsigned char *ptr = &Font->table[Char_Offset];

    for (Page = 0; Page < Font->Height; Page ++ ) {
        for (Column = 0; Column < Font->Width; Column ++ ) {

            //To determine whether the font background color and screen background color is consistent
            if (FONT_BACKGROUND == cBackground) { //this process is to speed up the scan
                //if (*ptr & (0x80 >> (Column % 8)))
                if (*ptr & (0x80 >> (Column % 8)))
                    setPixel(Xpt + Column, Ypt + Page, cForeground);
                    // Paint_DrawPoint(Xpt + Column, Ypt + Page, cForeground, DOT_PIXEL_DFT, DOT_STYLE_DFT);
            } else {
                if (*ptr & (0x80 >> (Column%8))) {
                    setPixel(Xpt + Column, Ypt + Page, cForeground);
                    // Paint_DrawPoint(Xpt + Column, Ypt + Page, cForeground, DOT_PIXEL_DFT, DOT_STYLE_DFT);
                } else {
                    setPixel(Xpt + Column, Ypt + Page, cBackground);
                    // Paint_DrawPoint(Xpt + Column, Ypt + Page, cBackground, DOT_PIXEL_DFT, DOT_STYLE_DFT);
                }
            }
            //One pixel is 8 bits
            if (Column % 8 == 7)
                ptr++;
        }// Write a line
        if (Font->Width % 8 != 0)
            ptr++;
    }// Write all
}

/******************************************************************************
//function:	Display the string
//parameter:
//    xStart           ：X coordinate
//    yStart           ：Y coordinate
//    pString          ：The first address of the string to be displayed
//    Font             ：A structure pointer that displays a character size
//    cBackground : Select the background color of the English character
//    cForeground : Select the foreground color of the English character
******************************************************************************/
void drawString(uint16_t xStart, uint16_t yStart, const char * pString,
                         sFONT* Font, uint8_t cBackground, uint8_t cForeground )
{
    uint16_t Xpt = xStart;
    uint16_t Ypt = yStart;

    if (xStart > Paint.Width || yStart > Paint.Height) {
//        Debug("drawString Input exceeds the normal display range\r\n");
        return;
    }

    while (* pString != '\0') {
        //if X direction filled , reposition to(xStart,Ypt),Ypt is Y direction plus the Height of the character
        if ((Xpt + Font->Width ) > Paint.Width ) {
            Xpt = xStart;
            Ypt += Font->Height;
        }

        // If the Y direction is full, reposition to(xStart, yStart)
        if ((Ypt  + Font->Height ) > Paint.Height ) {
            Xpt = xStart;
            Ypt = yStart;
        }
        drawChar(Xpt, Ypt, * pString, Font, cBackground, cForeground);

        //The next character of the address
        pString ++;

        //The next word of the abscissa increases the font of the broadband
        Xpt += Font->Width;
    }
}


