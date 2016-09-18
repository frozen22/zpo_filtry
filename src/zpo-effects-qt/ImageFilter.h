/* 
 * Soubor: ImageFilter.h
 * Popis:  VUT Brno FIT - Zpracovani obrazu (ZPO)
 *         Filtrove "efekty" v obrazu
 * Autori: Frantisek Nemec (xnemec61@stud.fit.vutbr.cz)
 *         Jan Opalka (xopalk01@stud.fit.vutbr.cz)
 * Datum:  2015-04-09
 */

#ifndef IMAGEFILTER_H
#define	IMAGEFILTER_H

#include <opencv2/core/core.hpp> // cv::Mat
#include <QMetaType> // Q_DECLARE_METATYPE

class ImageFilter
{
    public:
        
        enum class Type
        {
            NoFilter,
            
            EdgeGrayLeft,
            EdgeGrayRight,
            EdgeGrayDown,
            EdgeGrayUp,
            EdgeGrayFourDir,
            EdgeGrayFourMax,
            EdgeGrayFourDirEqu,
            
            SobelGray,
            SobelGray2,
            
            SobelColor,
            Comics,
            Glass,
        };

        static void filter(const cv::Mat& src, cv::Mat& dst, ImageFilter::Type filterType);
        
        static void noFilter(const cv::Mat& src, cv::Mat& dst);
        
        static void edgeGrayLeft(const cv::Mat& src, cv::Mat& dst);
        static void edgeGrayRight(const cv::Mat& src, cv::Mat& dst);
        static void edgeGrayDown(const cv::Mat& src, cv::Mat& dst);
        static void edgeGrayUp(const cv::Mat& src, cv::Mat& dst);
        static void edgeGrayFourDir(const cv::Mat& src, cv::Mat& dst);
        static void edgeGrayFourDirEqu(const cv::Mat& src, cv::Mat& dst);
        
        static void sobelGray(const cv::Mat& src, cv::Mat& dst);
        static void sobelGray2(const cv::Mat& src, cv::Mat& dst);
        
        static void sobelColor(const cv::Mat& src, cv::Mat& dst);
        static void comics(const cv::Mat& src, cv::Mat& dst);
        static void glass(const cv::Mat& src, cv::Mat& dst);
        static void edgeGrayFourMax(const cv::Mat& src, cv::Mat& dst);
};

// Registrace enum, aby bylo mozne ho pouzit v Qt komponentach.
Q_DECLARE_METATYPE(ImageFilter::Type)

#endif	/* IMAGEFILTER_H */

