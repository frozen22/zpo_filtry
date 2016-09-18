/* 
 * Soubor: Pixel.h
 * Popis:  VUT Brno FIT - Zpracovani obrazu (ZPO)
 *         Filtrove "efekty" v obrazu
 * Autori: Frantisek Nemec (xnemec61@stud.fit.vutbr.cz)
 *         Jan Opalka (xopalk01@stud.fit.vutbr.cz)
 * Datum:  2015-05-06
 */

#ifndef PIXEL_H
#define	PIXEL_H

class Pixel
{
    public:
        
        Pixel(int x, int y);

        int getX();
        int getY();

    private:
        
        int x;
        int y;

};

#endif	/* PIXEL_H */

