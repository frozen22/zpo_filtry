/* 
 * Soubor: ImageFilter.cpp
 * Popis:  VUT Brno FIT - Zpracovani obrazu (ZPO)
 *         Filtrove "efekty" v obrazu
 * Autori: Frantisek Nemec (xnemec61@stud.fit.vutbr.cz)
 *         Jan Opalka (xopalk01@stud.fit.vutbr.cz)
 * Datum:  2015-04-09
 */

#include "ImageFilter.h"

#include <opencv2/imgproc/imgproc.hpp>
#include "Debug.h"
#include "VoronoiPoint.h"
#include <cmath>
#include <algorithm>
#include <vector>

using std::vector;

/**
 * Filtrace obrazu.
 * 
 * @param src vstupni obraz
 * @param dst vystupni filtrovany obraz
 * @param filterType typ filtru
 */
void ImageFilter::filter(const cv::Mat& src, cv::Mat& dst, ImageFilter::Type filterType)
{
    switch (filterType)
    {
        case ImageFilter::Type::NoFilter:           noFilter(src,dst);           break;
        case ImageFilter::Type::EdgeGrayLeft:       edgeGrayLeft(src,dst);       break;
        case ImageFilter::Type::EdgeGrayRight:      edgeGrayRight(src,dst);      break;
        case ImageFilter::Type::EdgeGrayDown:       edgeGrayDown(src,dst);       break;
        case ImageFilter::Type::EdgeGrayUp:         edgeGrayUp(src,dst);         break;
        case ImageFilter::Type::EdgeGrayFourDir:    edgeGrayFourDir(src,dst);    break;
        case ImageFilter::Type::EdgeGrayFourDirEqu: edgeGrayFourDirEqu(src,dst); break;
        case ImageFilter::Type::SobelGray:          sobelGray(src,dst);          break;
        case ImageFilter::Type::SobelGray2:         sobelGray2(src,dst);         break;
        case ImageFilter::Type::SobelColor:         sobelColor(src,dst);         break;
        case ImageFilter::Type::Comics:             comics(src,dst);             break;
        case ImageFilter::Type::Glass:              glass(src,dst);              break;
        case ImageFilter::Type::EdgeGrayFourMax:    edgeGrayFourMax(src,dst);    break;
        default: 
            DBG("filter - filterType case not set")
            noFilter(src,dst);
    }
}

/**
 * Kopie vstupniho obrazu na vystup.
 * 
 * @param src vstupni obraz
 * @param dst kopie vstupniho obrazu
 */
void ImageFilter::noFilter(const cv::Mat& src, cv::Mat& dst)
{
    dst = src.clone();
}

/**
 * Jednoduchy hranovy filtr s maskou:
 * 
 *      1, 0, -1
 *      2, 0, -2
 *      1, 0, -1
 * 
 * @param src vstupni obraz
 * @param dst vystupni (filtrovany obraz)
 */
void ImageFilter::edgeGrayLeft(const cv::Mat& src, cv::Mat& dst)
{
    cv::Mat src_gray;

    cvtColor(src, src_gray, CV_BGR2GRAY);

    float ker[9] = {1, 0, -1, 
                    2, 0, -2, 
                    1, 0, -1};
    
    cv::Mat kernel(3, 3, CV_32FC1, ker);

    cv::flip(kernel, kernel, -1);
    cv::filter2D(src_gray, dst, -1, kernel);
}

/**
 * Jednoduchy hranovy filtr s maskou:
 * 
 *      -1, 0, 1 
 *      -2, 0, 2
 *      -1, 0, 1
 * 
 * @param src vstupni obraz
 * @param dst vystupni (filtrovany obraz)
 */
void ImageFilter::edgeGrayRight(const cv::Mat& src, cv::Mat& dst)
{
    cv::Mat src_gray;

    cvtColor(src, src_gray, CV_BGR2GRAY);

    float ker[9] = {-1, 0, 1, 
                    -2, 0, 2, 
                    -1, 0, 1};
    
    cv::Mat kernel(3, 3, CV_32FC1, ker);

    cv::flip(kernel, kernel, -1);
    cv::filter2D(src_gray, dst, -1, kernel);
}

/**
 * Jednoduchy hranovy filtr s maskou:
 * 
 *      -1, -2, -1
 *       0,  0,  0
 *       1,  2,  1
 * 
 * @param src vstupni obraz
 * @param dst vystupni (filtrovany obraz)
 */
void ImageFilter::edgeGrayDown(const cv::Mat& src, cv::Mat& dst)
{
    cv::Mat src_gray;

    cvtColor(src, src_gray, CV_BGR2GRAY);

    float ker[9] = {-1, -2, -1, 
                     0,  0,  0, 
                     1,  2,  1};
    
    cv::Mat kernel(3, 3, CV_32FC1, ker);

    cv::flip(kernel, kernel, -1);
    cv::filter2D(src_gray, dst, CV_8UC1, kernel);
}

/**
 * Jednoduchy hranovy filtr s maskou:
 * 
 *       1,  2,  1
 *       0,  0,  0
 *      -1, -2, -1
 * 
 * @param src vstupni obraz
 * @param dst vystupni (filtrovany obraz)
 */
void ImageFilter::edgeGrayUp(const cv::Mat& src, cv::Mat& dst)
{
    cv::Mat src_gray;

    cvtColor(src, src_gray, CV_BGR2GRAY);

    float ker[9] = { 1,  2,  1, 
                     0,  0,  0, 
                    -1, -2, -1};
    
    cv::Mat kernel(3, 3, CV_32FC1, ker);

    cv::flip(kernel, kernel, -1);
    cv::filter2D(src_gray, dst, -1, kernel);
}

/**
 * Jednoduchy hranovy filtr pro vsechny smery hran.
 * 
 *       1,  2,  1
 *       0,  0,  0
 *      -1, -2, -1
 * 
 * @param src vstupni obraz
 * @param dst vystupni (filtrovany obraz)
 */
void ImageFilter::edgeGrayFourDir(const cv::Mat& src, cv::Mat& dst)
{
    cv::Mat d1;
    cv::Mat d2;
    cv::Mat d3;
    cv::Mat d4;
   
    edgeGrayDown(src, d1);
    edgeGrayUp(src, d2);
    edgeGrayRight(src, d3);
    edgeGrayLeft(src, d4);
    
    dst = cv::Mat::zeros(src.rows,src.cols,d2.type());
    
    for (int i = 0; i < d1.cols; i++)
    {
        for (int j = 0; j < d1.rows; j++)
        {
            int v = d1.at<uchar>(j,i) + d2.at<uchar>(j,i)  + d3.at<uchar>(j,i) + d4.at<uchar>(j,i);
            v /= 4;
            
            dst.at<uchar>(j,i) = (uchar)v;
        }
    }
}

/**
 * Emboss filter.
 * 
 * @param src vstupni obraz
 * @param dst vystupni (filtrovany obraz)
 */
void ImageFilter::edgeGrayFourMax(const cv::Mat& src, cv::Mat& dst)
{
    int kerDim = 3;
    float ker[3][3] = {-1, -1, 0, 
                       -1,  0, 1, 
                        0,  1, 1};
    
    cv::Mat kernel(3, 3, CV_32FC1, ker);

    //cv::flip(kernel, kernel, -1);
    //cv::filter2D(src, dst, CV_8UC1, kernel);
    int bias = 128;
    dst = cv::Mat::zeros(src.rows,src.cols,src.type());
    
    int red = 0, green = 0, blue =0;
    for (int i = 0; i < src.cols; i++)
    {
        for (int j = 0; j < src.rows; j++)
        {
            cv::Vec3b q;
            red = 0, green = 0, blue =0;
            for(int x = 0; x < kerDim; x++)
            {
                for(int y = 0; y < kerDim; y++) 
                { 
                    int imgX = (j - kerDim / 2 + x + src.rows) % src.rows; 
                    int imgY = (i - kerDim / 2 + y + src.cols) % src.cols;
                    //if( imgX > 500 || imgY > 500)
                    //std::cout << "x " << imgX << " rx" << src.rows << " y " << imgY << " ry " << src.cols << std::endl;
                    if(imgX < 0){
                        imgX = 0;
                    }
                    if(imgY < 0){
                        imgY = 0;
                    }
                    if(imgX > src.rows){
                        imgX = src.rows;
                    }
                    if(imgY > src.cols){
                        imgY = src.cols;
                    }
                    q = src.at<cv::Vec3b>(imgX,imgY);
                    
                    red += q[2] * ker[x][y]; 
                    green += q[1] * ker[x][y]; 
                    blue += q[0] * ker[x][y]; 
                    //std::cout << "r " << red << " g " << green << std::endl;
                } 
            }
            red = std::min(std::max(red + bias,0),255);
            green = std::min(std::max(green + bias,0),255);
            blue = std::min(std::max(blue + bias,0),255);
            
            
            
            q[2] = red; 
            q[1] = green; 
            q[0] = blue; 
            //std::cout << j << std::endl;
            dst.at<cv::Vec3b>(j,i) = q;
        }
    }    
    
    cvtColor(dst, dst, CV_BGR2GRAY);
}

void ImageFilter::edgeGrayFourDirEqu(const cv::Mat& src, cv::Mat& dst)
{
    edgeGrayFourDir(src,dst);
    cv::equalizeHist(dst, dst);
}

/**
 * Sobel filtr (vsesmerovy hranovy filtr) s prevodem na sedotonovy obraz.
 * 
 * @param src vstupni obraz
 * @param dst vystupni (filtrovany obraz)
 */
void ImageFilter::sobelGray(const cv::Mat& src, cv::Mat& dst)
{
    cv::Mat d1 = cv::Mat::zeros(src.rows, src.cols, CV_32FC1);
    cv::Mat d2 = cv::Mat::zeros(src.rows, src.cols, CV_32FC1);
    cv::Mat d3 = cv::Mat::zeros(src.rows, src.cols, CV_32FC1);
    cv::Mat d4 = cv::Mat::zeros(src.rows, src.cols, CV_32FC1);
    cv::Mat src_gray;
    float ker1[9] = {-1,  0,  1, 
                     -1,  0,  1, 
                     -1,  0,  1};
    
    float ker2[9] = {-1, -1, -1, 
                      0,  0,  0, 
                      1,  1,  1};
    
    cvtColor(src, src_gray, CV_BGR2GRAY);
    
    cv::Mat kernel1(3, 3, CV_32FC1, ker1);
    cv::Mat kernel2(3, 3, CV_32FC1, ker2);
    
    cv::flip(kernel1, kernel1, -1);
    cv::flip(kernel2, kernel2, -1);
    
    cv::filter2D(src_gray, d1, CV_32FC1, kernel1);
    cv::filter2D(src_gray, d2, CV_32FC1, kernel2);
    
    dst = cv::Mat::zeros(src.rows, src.cols, CV_8U);
    
    for (int j = 0; j < d1.cols; j++)
    {
        for (int i = 0; i < d1.rows; i++)
        {
            float f1 = d1.at<float>(i,j) * d1.at<float>(i,j);
            float f2 = d2.at<float>(i,j) * d2.at<float>(i,j);
            float f3 = d3.at<float>(i,j) * d3.at<float>(i,j);
            float f4 = d4.at<float>(i,j) * d4.at<float>(i,j);
            
            float v = sqrt(f1 + f2 + f3 + f4);
            
            if (v > 255)
            {
                v = 255;
            }
            
            dst.at<uchar>(i,j) = (uchar)round(v);
        }
    }
}

/**
 * Sobel filtr (vsesmerovy hranovy filtr) s prevodem na sedotonovy obraz.
 * 
 * @param src vstupni obraz
 * @param dst vystupni (filtrovany obraz)
 */
void ImageFilter::sobelGray2(const cv::Mat& src, cv::Mat& dst)
{
    cv::Mat d1 = cv::Mat::zeros(src.rows, src.cols, CV_32FC1);
    cv::Mat d2 = cv::Mat::zeros(src.rows, src.cols, CV_32FC1);
    cv::Mat d3 = cv::Mat::zeros(src.rows, src.cols, CV_32FC1);
    cv::Mat d4 = cv::Mat::zeros(src.rows, src.cols, CV_32FC1);
    
    cv::Mat src_gray;
    
    float ker1[9] = {-1,  0,  1, 
                     -2,  0,  2, 
                     -1,  0,  1};
    
    float ker2[9] = {-1, -2, -1, 
                      0,  0,  0, 
                      1,  2,  1};
    
    cvtColor(src, src_gray, CV_BGR2GRAY);
    
    cv::Mat kernel1(3, 3, CV_32FC1, ker1);
    cv::Mat kernel2(3, 3, CV_32FC1, ker2);
    
    cv::flip(kernel1, kernel1, -1);
    cv::flip(kernel2, kernel2, -1);
    
    cv::filter2D(src_gray, d1, CV_32FC1, kernel1);
    cv::filter2D(src_gray, d2, CV_32FC1, kernel2);
    
    dst = cv::Mat::zeros(src.rows, src.cols, CV_8U);
    
    for (int j = 0; j < d1.cols; j++)
    {
        for (int i = 0; i < d1.rows; i++)
        {
            float f1 = d1.at<float>(i,j) * d1.at<float>(i,j);
            float f2 = d2.at<float>(i,j) * d2.at<float>(i,j);
            float f3 = d3.at<float>(i,j) * d3.at<float>(i,j);
            float f4 = d4.at<float>(i,j) * d4.at<float>(i,j);
            
            float v = sqrt(f1 + f2 + f3 + f4);
            
            if (v > 255)
            {
                v = 255;
            }
            
            dst.at<uchar>(i,j) = (uchar)round(v);
        }
    }
}

/**
 * Barevny sobel, vytvari obraz, ktery se sklada z barevnych hran puvodniho obrazu a cernych plosek,
 * ktere reprezentuji barevne plochy v puvodnim obraze
 * 
 * @param src vstupni obraz
 * @param dst vystupni (filtrovany obraz)
 */
void ImageFilter::sobelColor(const cv::Mat& src, cv::Mat& dst)
{
    cv::Mat tmp = src.clone();
    cv::Mat blur = src.clone();
    //dst = src.clone();
    
    float mean_sum = 25.0;
    float mean5x5[25] = {
        1.0f/mean_sum, 1.0f/mean_sum, 1.0f/mean_sum, 1.0f/mean_sum, 1.0f/mean_sum,
        1.0f/mean_sum, 1.0f/mean_sum, 1.0f/mean_sum, 1.0f/mean_sum, 1.0f/mean_sum,
        1.0f/mean_sum, 1.0f/mean_sum, 1.0f/mean_sum, 1.0f/mean_sum, 1.0f/mean_sum,
        1.0f/mean_sum, 1.0f/mean_sum, 1.0f/mean_sum, 1.0f/mean_sum, 1.0f/mean_sum,
        1.0f/mean_sum, 1.0f/mean_sum, 1.0f/mean_sum, 1.0f/mean_sum, 1.0f/mean_sum,
    };
    
    cv::Mat meankernel5x5(5, 5, CV_32FC1, mean5x5);

    cv::flip(meankernel5x5, meankernel5x5, -1);
    cv::filter2D(src, blur, -1, meankernel5x5);
    //dst = blur.clone();
    edgeGrayFourDir(blur,tmp);
    //cv::filter2D(src, dst, -1, meankernel5x5);
    
    dst = blur.clone();
                
    for (int i = 0; i < dst.cols; i++)
    {
        for (int j = 0; j < dst.rows; j++)
        {
            cv::Vec3b p = dst.at<cv::Vec3b>(j,i);    
            if (tmp.at<uchar>(j,i) < 14)
            {
                dst.at<cv::Vec3b>(j,i) = {20,20,20};
            } else {
                if((p.val[0] + tmp.at<uchar>(j,i)*0.299) < 255){
                    p.val[0] += tmp.at<uchar>(j,i)*0.299;
                } else {
                    p.val[0] = 255;
                }
                if((p.val[1] + tmp.at<uchar>(j,i)*0.587) < 255){
                    p.val[1] += tmp.at<uchar>(j,i)*0.587;
                } else {
                    p.val[1] = 255;
                }
                if((p.val[2] + tmp.at<uchar>(j,i)*0.114) < 255){
                    p.val[2] += tmp.at<uchar>(j,i)*0.114;
                } else {
                    p.val[2] = 255;
                }
                
                dst.at<cv::Vec3b>(j,i) = p;
            }    
        }
    }
    
    cv::filter2D(dst, blur, -1, meankernel5x5);
    dst = blur.clone();
}

/**
 * Comic filtr vytvari na obrazu komixovy efekt, vyuziva gradienty a rozostrovaci filtr.
 * 
 * @param src vstupni obraz
 * @param dst vystupni (filtrovany obraz)
 */
void ImageFilter::comics(const cv::Mat& src, cv::Mat& dst)
{
    cv::Mat tmp = dst.clone();
    cv::Mat blur = dst.clone();
    dst = src.clone();
    float ker[9] = {
        1.0/16.0, 1.5/8.0, 1.0/16.0,
        1.5/8.0, 0.0, 1.5/8.0,
        1.0/16.0, 1.5/8.0, 1.0/16.0,
    };
    
    //rozostrovaci filtry
    float gaussian_sum = 52.0;
    float ker5x5[25] = {
        1.0f/gaussian_sum, 1.0f/gaussian_sum, 2.0f/gaussian_sum, 1.0f/gaussian_sum, 1.0f/gaussian_sum,
        1.0f/gaussian_sum, 2.0f/gaussian_sum, 4.0f/gaussian_sum, 2.0f/gaussian_sum, 1.0f/gaussian_sum,
        2.0f/gaussian_sum, 4.0f/gaussian_sum, 8.0f/gaussian_sum, 4.0f/gaussian_sum, 2.0f/gaussian_sum,
        1.0f/gaussian_sum, 2.0f/gaussian_sum, 4.0f/gaussian_sum, 2.0f/gaussian_sum, 1.0f/gaussian_sum,
        1.0f/gaussian_sum, 1.0f/gaussian_sum, 2.0f/gaussian_sum, 1.0f/gaussian_sum, 1.0f/gaussian_sum,
    };
    
    float mean_sum = 25.0;
    float mean5x5[25] = {
        1.0f/mean_sum, 1.0f/mean_sum, 1.0f/mean_sum, 1.0f/mean_sum, 1.0f/mean_sum,
        1.0f/mean_sum, 1.0f/mean_sum, 1.0f/mean_sum, 1.0f/mean_sum, 1.0f/mean_sum,
        1.0f/mean_sum, 1.0f/mean_sum, 1.0f/mean_sum, 1.0f/mean_sum, 1.0f/mean_sum,
        1.0f/mean_sum, 1.0f/mean_sum, 1.0f/mean_sum, 1.0f/mean_sum, 1.0f/mean_sum,
        1.0f/mean_sum, 1.0f/mean_sum, 1.0f/mean_sum, 1.0f/mean_sum, 1.0f/mean_sum,
    };
    
    cv::Mat kernel(3, 3, CV_32FC1, ker);
    cv::Mat kernel5x5(5, 5, CV_32FC1, ker5x5);
    cv::Mat meankernel5x5(5, 5, CV_32FC1, mean5x5);

    cv::flip(kernel, kernel, -1);
    cv::flip(kernel5x5, kernel5x5, -1);
    cv::flip(meankernel5x5, meankernel5x5, -1);
    cv::filter2D(src, blur, -1, kernel5x5);
    
    edgeGrayFourDirEqu(blur,tmp);
    int d = 30;
//    int d = 125;

    for (int i = 0; i < dst.cols; i++)
    {
        for (int j = 0; j < dst.rows; j++)
        {
            cv::Vec3b p = blur.at<cv::Vec3b>(j, i);
            cv::Vec3b q;

            if (tmp.at<uchar>(j, i) > 220)
            {
                q.val[0] = d;
                q.val[1] = d;
                q.val[2] = d;
            }
            else
            {
                q.val[0] = p.val[0];
                q.val[1] = p.val[1];
                q.val[2] = p.val[2];
            }
                
            dst.at<cv::Vec3b>(j,i) = q;
        }
    }
    
    //cv::filter2D(dst, dst, -1, kernel);
}

/**
 * Vytvari efekt skla na obrazku.
 * 
 * @param src vstupni obraz
 * @param dst vystupni (filtrovany obraz)
 */
void ImageFilter::glass(const cv::Mat& src, cv::Mat& dst)
{
    dst = src.clone();
    int region_size = 10;

    int sumR, sumG, sumB;
    int vx, vy, nearest;
    VoronoiPoint* vp;
    VoronoiPoint* nearestVP = nullptr;
    Pixel* pixel;

    vector<VoronoiPoint*> voronoiPoints;

    for (int i = 0; i + region_size < src.cols; i = i + region_size)
    {
        for (int j = 0; j + region_size < src.rows; j = j + region_size)
        {
            //vypocet seeds
            //std::cout << "test jk" << << std::endl;
            sumR = 0;
            sumG = 0;
            sumB = 0;
            for (int k = i; k < (i + region_size); k++)
            {
                for (int l = j; l < (j + region_size); l++)
                {
                    cv::Vec3b p = src.at<cv::Vec3b>(l, k);
                    sumR += p.val[2];
                    sumG += p.val[1];
                    sumB += p.val[0];
                }
            }

            srand((sumR + sumG + sumB) / 3);
            vx = rand() % region_size + i;
            vy = rand() % region_size + j;
            vp = new VoronoiPoint(vx, vy);

            voronoiPoints.push_back(vp);
        }
    }

    for (int i = 0; i < src.cols; i++)
    {
        for (int j = 0; j < src.rows; j++)
        {
            nearest = src.cols + src.rows;
            
            //pixel ziska nejlepsi hodnoceni
            for (size_t itp = 0; itp != voronoiPoints.size(); itp++)
            {
                vp = voronoiPoints[itp];

                int dist = abs(vp->getX() - i) + abs(vp->getY() - j);

                if (nearest > dist)
                {
                    nearest = dist;
                    nearestVP = vp;
                }
            }
            
            cv::Vec3b p = src.at<cv::Vec3b>(j, i);
            nearestVP->AddPixel(new Pixel(i, j), (int) p.val[2], (int) p.val[1], (int) p.val[0]);
        }
    }

    for (size_t it = 0; it != voronoiPoints.size(); it++)
    {
        vp = voronoiPoints[it];
        vp->CalculateAverages();

        std::vector<Pixel *> pixels = vp->getPixels();

        for (size_t itp = 0; itp != pixels.size(); itp++)
        {
            pixel = pixels[itp];
            cv::Vec3b q;

            q.val[2] = vp->getR(); //R
            q.val[1] = vp->getG(); //G
            q.val[0] = vp->getB(); //B

            dst.at<cv::Vec3b>(pixel->getY(), pixel->getX()) = q;
        }
    }
    
    for (VoronoiPoint* vp : voronoiPoints)
    {
        delete vp;
    }
}
