/* 
 * Soubor: VoronoiPoint.cpp
 * Popis:  VUT Brno FIT - Zpracovani obrazu (ZPO)
 *         Filtrove "efekty" v obrazu
 * Autori: Frantisek Nemec (xnemec61@stud.fit.vutbr.cz)
 *         Jan Opalka (xopalk01@stud.fit.vutbr.cz)
 * Datum:  2015-05-06
 */

#include "VoronoiPoint.h"
#include <iostream>

using std::vector;
using std::cout;
using std::endl;

VoronoiPoint::VoronoiPoint(int x, int y)
{
    Y = y;
    X = x;
    totalR = 0;
    totalG = 0;
    totalB = 0;
}


VoronoiPoint::~VoronoiPoint()
{
    for (Pixel* p : pixelOfRegion)
    {
        delete p;
    }
}

/**
 * Pridani pixelu pixel.
 * 
 * @param pixel pixel
 * @param r red
 * @param g green
 * @param b blue
 */
void VoronoiPoint::AddPixel(Pixel *pixel, int r, int g, int b)
{
    totalR += r;
    totalG += g;
    totalB += b;
    pixelOfRegion.push_back(pixel);
}

/**
 * Vypocet prumerne hodnoty RGB slozek vsech ylozenych pixelu.
 */
void VoronoiPoint::CalculateAverages()
{
    if (pixelOfRegion.size() > 0)
    {
        int size = pixelOfRegion.size();
        avgB = (totalB / size) > 255 ? 255 : (totalB / size);
        avgG = (totalG / size) > 255 ? 255 : (totalG / size);
        avgR = (totalR / size) > 255 ? 255 : (totalR / size);
    }
}

int VoronoiPoint::getY()
{
    return Y;
}

int VoronoiPoint::getX()
{
    return X;
}

int VoronoiPoint::getR()
{
    return avgR;
}

int VoronoiPoint::getG()
{
    return avgG;
}

int VoronoiPoint::getB()
{
    return avgB;
}

vector<Pixel*> VoronoiPoint::getPixels()
{
    return pixelOfRegion;
}
