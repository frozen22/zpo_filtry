/* 
 * Soubor: VoronoiPoint.h
 * Popis:  VUT Brno FIT - Zpracovani obrazu (ZPO)
 *         Filtrove "efekty" v obrazu
 * Autori: Frantisek Nemec (xnemec61@stud.fit.vutbr.cz)
 *         Jan Opalka (xopalk01@stud.fit.vutbr.cz)
 * Datum:  2015-05-06
 */

#include "Pixel.h"
#include <vector>
        
#ifndef VORONOIPOINT_H
#define	VORONOIPOINT_H

class VoronoiPoint
{
    public:
        
        VoronoiPoint(int x, int y);
        ~VoronoiPoint();
        
        void CalculateAverages();
        void AddPixel(Pixel *pixel, int r, int g, int b);
        int getX();
        int getY();
        int getR();
        int getG();
        int getB();
        std::vector<Pixel*> getPixels();

    private:

        int X;
        int Y;
        long long totalR;
        long long totalG;
        long long totalB;
        int avgR;
        int avgG;
        int avgB;
        std::vector<Pixel*> pixelOfRegion;
};

#endif	/* VORONOIPOINT_H */

