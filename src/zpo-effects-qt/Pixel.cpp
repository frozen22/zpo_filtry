/* 
 * Soubor: Pixel.cpp
 * Popis:  VUT Brno FIT - Zpracovani obrazu (ZPO)
 *         Filtrove "efekty" v obrazu
 * Autori: Frantisek Nemec (xnemec61@stud.fit.vutbr.cz)
 *         Jan Opalka (xopalk01@stud.fit.vutbr.cz)
 * Datum:  2015-05-06
 */

#include "Pixel.h"

Pixel::Pixel(int x, int y)
{
    this->x = x;
    this->y = y;
}

int Pixel::getY()
{
    return y;
}

int Pixel::getX()
{
    return x;
}
