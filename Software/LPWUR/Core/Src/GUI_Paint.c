/*
 * GUI_Image.c
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

IMAGE Image;
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
    Image.add = NULL;
    Image.add = image;

    Image.WidthMemory = Width;
    Image.HeightMemory = Height;
    Image.Color = Color;
    //Image.WidthByte = (Width % 8 == 0)? (Width / 8 ): (Width / 8 + 1);
    Image.WidthByte = Width;
    Image.HeightByte = Height;
    //printf("WidthByte = %d, HeightByte = %d\r\n", Image.WidthByte, Image.HeightByte);
    //printf(" EPD_WIDTH / 8 = %d\r\n",  122 / 8);
   
    Image.Rotate = Rotate;
    Image.Mirror = MIRROR_NONE;
    
    if(Rotate == ROTATE_0 || Rotate == ROTATE_180) {
        Image.Width = Width;
        Image.Height = Height;
    } else {
        Image.Width = Height;
        Image.Height = Width;
    }
}

/******************************************************************************
//function:	Select Image
//parameter:
//    image   :   Pointer to the image cache
******************************************************************************/
void selectImage(uint8_t *image)
{
    Image.add = image;
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
        Image.Rotate = Rotate;
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
        Image.Mirror = mirror;
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
//    Color   :   Imageed colors
******************************************************************************/
void setPixel(uint16_t Xpt, uint16_t Ypt, uint16_t Color)
{
    if(Xpt > Image.Width || Ypt > Image.Height){
//        Debug("Exceeding display boundaries\r\n");
        return;
    }      
    uint16_t X, Y;

    switch(Image.Rotate) {
    case 0:
        X = Xpt;
        Y = Ypt;
        break;
    case 90:
        X = Image.WidthMemory - Ypt - 1;
        Y = Xpt;
        break;
    case 180:
        X = Image.WidthMemory - Xpt - 1;
        Y = Image.HeightMemory - Ypt - 1;
        break;
    case 270:
        X = Ypt;
        Y = Image.HeightMemory - Xpt - 1;
        break;
		
    default:
        return;
    }
    
    switch(Image.Mirror) {
    case MIRROR_NONE:
        break;
    case MIRROR_HORIZONTAL:
        X = Image.WidthMemory - X - 1;
        break;
    case MIRROR_VERTICAL:
        Y = Image.HeightMemory - Y - 1;
        break;
    case MIRROR_ORIGIN:
        X = Image.WidthMemory - X - 1;
        Y = Image.HeightMemory - Y - 1;
        break;
    default:
        return;
    }

    if(X > Image.WidthMemory || Y > Image.HeightMemory){
//        Debug("Exceeding display boundaries\r\n");
        return;
    }
    
   // uint32_t Addr = X / 8 + Y * Image.WidthByte;
    uint32_t Addr = X + Y * Image.WidthByte;
   // uint8_t Rdata = Image.Image[Addr];
    if(Color == BLACK)
        //Image.Image[Addr] = Rdata & ~(0x80 >> (X % 8));
    	Image.add[Addr] = BLACK;
    else
        //Image.Image[Addr] = Rdata | (0x80 >> (X % 8));
    	Image.add[Addr] = Color;
}

/******************************************************************************
//function:	Clear the color of the picture
//parameter:
//    Color   :   Imageed colors
******************************************************************************/
void clear_Image(uint8_t Color)
{
    for (uint16_t Y = 0; Y < Image.HeightByte; Y++) {
        for (uint16_t X = 0; X < Image.WidthByte; X++ ) {//8 pixel =  1 byte
            uint32_t Addr = X + Y*Image.WidthByte;
            Image.add[Addr] = Color;
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
void Image_DrawPoint(uint16_t Xpt, uint16_t Ypt, uint16_t Color,
                     DOT_PIXEL Dot_Pixel, DOT_STYLE DOT_STYLE)
{
    if (Xpt > Image.Width || Ypt > Image.Height) {
//        Debug("Image_DrawPoint Input exceeds the normal display range\r\n");
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
void Image_DrawLine(uint16_t xStart, uint16_t yStart, uint16_t Xend, uint16_t Yend,
                    uint16_t Color, LINE_STYLE Line_Style, DOT_PIXEL Dot_Pixel)
{
    if (xStart > Image.Width || yStart > Image.Height ||
        Xend > Image.Width || Yend > Image.Height) {
//        Debug("Image_DrawLine Input exceeds the normal display range\r\n");
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
        //Imageed dotted line, 2 point is really virtual
        if (Line_Style == LINE_STYLE_DOTTED && Dotted_Len % 3 == 0) {
            //Debug("LINE_DOTTED\r\n");
            Image_DrawPoint(Xpt, Ypt, IMAGE_BACKGROUND, Dot_Pixel, DOT_STYLE_DFT);
            Dotted_Len = 0;
        } else {
            Image_DrawPoint(Xpt, Ypt, Color, Dot_Pixel, DOT_STYLE_DFT);
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

    if (Xpt > Image.Width || Ypt > Image.Height) {
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
                    // Image_DrawPoint(Xpt + Column, Ypt + Page, cForeground, DOT_PIXEL_DFT, DOT_STYLE_DFT);
            } else {
                if (*ptr & (0x80 >> (Column%8))) {
                    setPixel(Xpt + Column, Ypt + Page, cForeground);
                    // Image_DrawPoint(Xpt + Column, Ypt + Page, cForeground, DOT_PIXEL_DFT, DOT_STYLE_DFT);
                } else {
                    setPixel(Xpt + Column, Ypt + Page, cBackground);
                    // Image_DrawPoint(Xpt + Column, Ypt + Page, cBackground, DOT_PIXEL_DFT, DOT_STYLE_DFT);
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

    if (xStart > Image.Width || yStart > Image.Height) {
//        Debug("drawString Input exceeds the normal display range\r\n");
        return;
    }

    while (* pString != '\0') {
        //if X direction filled , reposition to(xStart,Ypt),Ypt is Y direction plus the Height of the character
        if ((Xpt + Font->Width ) > Image.Width ) {
            Xpt = xStart;
            Ypt += Font->Height;
        }

        // If the Y direction is full, reposition to(xStart, yStart)
        if ((Ypt  + Font->Height ) > Image.Height ) {
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


