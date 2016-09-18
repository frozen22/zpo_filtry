/* 
 * Soubor: ImageSource.cpp
 * Popis:  VUT Brno FIT - Zpracovani obrazu (ZPO)
 *         Filtrove "efekty" v obrazu
 * Autori: Frantisek Nemec (xnemec61@stud.fit.vutbr.cz)
 *         Jan Opalka (xopalk01@stud.fit.vutbr.cz)
 * Datum:  2015-04-09
 */

#include "ImageSource.h"

#include <QString>

#include "ImageFilter.h"
#include "Debug.h"

ImageSource::ImageSource()
    : mActiveSourceType(SourceType::NOTHING)
    , mFilterType(ImageFilter::Type::NoFilter)
    , mCapture(new cv::VideoCapture())
{
    mThread = new QThread(this);
    this->moveToThread(mThread);
    
    initTimers();
    
    mThread->start();
}

ImageSource::~ImageSource()
{
    mThread->quit();
    mThread->wait();

    delete mVideoTimer;
    delete mCameraTimer;

    delete mCapture;
}

/**
 * Nastaveni typu filtru.
 * 
 * @param filterType typ filtru
 */
void ImageSource::setFilterType(ImageFilter::Type filterType)
{
    mFilterType = filterType;
    
    if (mActiveSourceType == SourceType::IMAGE)
    {
        filterImage();
    }
}

/**
 * Inicializace casovacu pro zobrazeni videa a kamery.
 */
void ImageSource::initTimers()
{
    //? Pri destrukci mImageSource je chyba v uvolnovani QTimer. QTimer musi
    //  byt uvolnet ve stejnem vlakne jako je vytvoren tj. ve stejnym vlakne
    //  ve kterem bezi this objekt. Nicmene jak se videt QTimer je presunut
    //  do noveho vlakna a tak je treba jej pred uvolnenim presunou zpet.
    
    mVideoTimer = new QTimer(0);
    mVideoTimer->setTimerType(Qt::PreciseTimer);
    mVideoTimer->moveToThread(mThread);
    connect(mVideoTimer, SIGNAL(timeout()), this, SLOT(newVideoFrame()));
    
    mCameraTimer = new QTimer(0);
    mCameraTimer->setTimerType(Qt::PreciseTimer);
    mCameraTimer->moveToThread(mThread);
    connect(mCameraTimer, SIGNAL(timeout()), this, SLOT(newCameraFrame()));
}

/**
 * Ziskani intervalu mezi zobrazeni snimku na zaklade fps zdroje.
 * 
 * @return interval mezi zobrazeni sniku
 */
int ImageSource::getVideoCaptureTimerInterval()
{
    double fps = mCapture->get(CV_CAP_PROP_FPS);
    if (fps <= 0)
    {
        fps = DEFAULT_FPS;
    }
    
    return round(1000.0 / fps);
}

/**
 * Nastaveni obrazku k filtraci.
 * 
 * @param image obrazek
 */
void ImageSource::setImage(cv::Mat image)
{
    mActiveSourceType = SourceType::IMAGE;
    mVideoTimer->stop();
    mCameraTimer->stop();
    mImage = image;
    filterImage();
}

/**
 * Nastaveni videa k filtrace.
 * 
 * @param fileName cesta k souboru s videem
 */
void ImageSource::setVideo(std::string fileName)
{
    if (!setVideoRaw(fileName))
    {
        QString msg = QString("Could not load video: \"%1\"").arg(QString::fromStdString(fileName));
        std::string stdMsg = msg.toStdString();
        emit errorMessage("Error", stdMsg);
    }
}

/**
 * Nastaveni kamery.
 */
void ImageSource::setCamera()
{
    if (!setCameraRaw())
    {
        emit errorMessage("Error", "Camera not found!");
    }
}

/**
 * Nastaveni videa k filtraci.
 * 
 * @param fileName cesta k souboru s videem
 * @return false - video se nepodarilo nacist
 */
bool ImageSource::setVideoRaw(std::string fileName)
{
    mCameraTimer->stop();
    mVideoTimer->stop();
    
    cv::VideoCapture* vc = new cv::VideoCapture();
            
    if (vc->open(fileName))
    {
        delete mCapture;
        mCapture = vc;
        
        mActiveSourceType = SourceType::VIDEO;
        mFrameStep = getVideoCaptureTimerInterval();
        mVideoTimer->start(mFrameStep);
        return true;
    }
    else
    {
        return false;
    }
}

/**
 * Nastaveni kamery.
 * 
 * @return false - kamera se nepovedla inicializovat
 */
bool ImageSource::setCameraRaw()
{
    mVideoTimer->stop();
    mCameraTimer->stop();
    
    cv::VideoCapture* vc = new cv::VideoCapture();
            
    if (vc->open(0))
    {
        delete mCapture;
        mCapture = vc;

        mActiveSourceType = SourceType::CAMERA;
        mFrameStep = getVideoCaptureTimerInterval();
        mCameraTimer->start(mFrameStep);
        return true;
    }
    else
    {
        return false;
    }
}

/**
 * Zastaveni prehravani video/kamery.
 */
void ImageSource::stopVideo()
{
    if (mActiveSourceType == SourceType::VIDEO)
    {
        mVideoTimer->stop();
    }
    
    if (mActiveSourceType == SourceType::CAMERA)
    {
        mCameraTimer->stop();
    }
}

/**
 * Start prehravani video/kamery.
 */
void ImageSource::startVideo()
{
    if (mActiveSourceType == SourceType::VIDEO)
    {
        mVideoTimer->start(mFrameStep);
    }
    if (mActiveSourceType == SourceType::CAMERA)
    {
        mCameraTimer->start(mFrameStep);
    }
}

/**
 * Provedeni filtrace obrazku a emitovani signalu s filtrovanym obrazkem.
 */
void ImageSource::filterImage()
{
    cv::Mat filteredImage;
    
    ImageFilter::filter(mImage, filteredImage, mFilterType);
    
    emit newImage(filteredImage); 
}

/**
 * Provedeni filtrace noveho snimku vide.
 */
void ImageSource::newVideoFrame()
{
    cv::Mat sourceImage;
    
    if (mCapture->read(sourceImage))
    {
        cv::Mat filteredImage; 
        ImageFilter::filter(sourceImage, filteredImage, mFilterType);
        emit newImage(filteredImage);

        if (mCapture->get(CV_CAP_PROP_POS_FRAMES) == mCapture->get(CV_CAP_PROP_FRAME_COUNT))
        {
            mCapture->set(CV_CAP_PROP_POS_AVI_RATIO, 0);
        }
    }
}

/**
 * Provedeni filtrace noveho snimku z kamery.
 */
void ImageSource::newCameraFrame()
{
    cv::Mat sourceImage;

    if (mCapture->read(sourceImage))
    {
        cv::Mat filteredImage;
        ImageFilter::filter(sourceImage, filteredImage, mFilterType);
        emit newImage(filteredImage);
    }
}

/**
 * Test na bez video nebo kamery.
 * 
 * @return true - kamera nebo video bezi
 */
bool ImageSource::videoIsRunning()
{
    return mVideoTimer->isActive() || mCameraTimer->isActive();
}

/**
 * Ziskani typu aktualne filtrovaneho media.
 * 
 * @return typ filtrovaneho media
 */
ImageSource::SourceType ImageSource::getSourceType()
{
    return mActiveSourceType;
}
