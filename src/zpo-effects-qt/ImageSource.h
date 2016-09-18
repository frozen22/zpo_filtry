/* 
 * Soubor: ImageSource.h
 * Popis:  VUT Brno FIT - Zpracovani obrazu (ZPO)
 *         Filtrove "efekty" v obrazu
 * Autori: Frantisek Nemec (xnemec61@stud.fit.vutbr.cz)
 *         Jan Opalka (xopalk01@stud.fit.vutbr.cz)
 * Datum:  2015-04-09
 */

#ifndef IMAGESOURCE_H
#define	IMAGESOURCE_H

#include <QObject>
#include <QTimer>
#include <QThread>

#include <opencv2/imgproc/imgproc.hpp> // cv::Mat
#include <opencv2/highgui/highgui.hpp> // cv::VideoCapture

#include "ImageFilter.h"

class ImageSource : public QObject
{
    Q_OBJECT
    
    public:
        
        enum class SourceType
        {
            IMAGE,
            CAMERA,
            VIDEO,
            NOTHING
        };
    
    private:
        
        static constexpr double DEFAULT_FPS = 25.0;

        SourceType mActiveSourceType;
        ImageFilter::Type mFilterType;
        cv::Mat mImage;
        cv::Mat mFilteredImage;
        cv::VideoCapture* mCapture;
        int mFrameStep;
        
        QTimer* mVideoTimer;
        QTimer* mCameraTimer;
        QThread* mThread;
        
    public:

        ImageSource();
        virtual ~ImageSource();
        
        bool videoIsRunning();
        ImageSource::SourceType getSourceType();
        
    public slots:
        
        void setImage(cv::Mat image);
        void setVideo(std::string fileName);
        void setCamera();
        void setFilterType(ImageFilter::Type filterType);
        
        //? rename stopPlayback?
        void stopVideo();
        void startVideo();
        
    signals:
    
        void newImage(cv::Mat img);
        void errorMessage(std::string title, std::string msg);
    
    private slots:
        
        void newVideoFrame();
        void newCameraFrame();

    private:
        
        bool setVideoRaw(std::string fileName);
        bool setCameraRaw();
        
        void initTimers();
        void filterImage();
        int getVideoCaptureTimerInterval();

};

#endif	/* IMAGESOURCE_H */

