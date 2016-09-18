/* 
 * Soubor: ImageViewerOpenGl.cpp
 * Popis:  VUT Brno FIT - Zpracovani obrazu (ZPO)
 *         Filtrove "efekty" v obrazu
 * Autori: Frantisek Nemec (xnemec61@stud.fit.vutbr.cz)
 *         Jan Opalka (xopalk01@stud.fit.vutbr.cz)
 * Datum:  2015-04-09
 * Zdroj: 
 *      http://www.robot-home.it/blog/software/tutorial-opencv-qt-opengl-widget-per-visualizzare-immagini-da-opencv-in-una-gui-con-qt/?lang=en 
 *      http://www.robot-home.it/blog/software/tutorial-opencv-qt-opengl-widget-per-visualizzare-immagini-da-opencv-in-una-gui-con-qt-seconda-parte/?lang=en
 */

#include "ImageViewerOpenGl.h"

#include <QtOpenGL/QGLWidget>

#include "Debug.h"

/**
 * Konstruktor.
 * 
 * @param parent rodicovska komponenta
 */
ImageViewerOpenGl::ImageViewerOpenGl(QWidget *parent)
    : QOpenGLWidget(parent)
{
    mSceneChanged = false;
    mBgColor = QColor::fromRgb(150, 150, 150);

    mOutH = 0;
    mOutW = 0;

    mPosX = 0;
    mPosY = 0;
    
    mScale = false;
    mTemporalScale = false;
}

/**
 * Inicializace OpenGl.
 */
void ImageViewerOpenGl::initializeGL()
{
    makeCurrent();
    glClearColor(1.0f,1.0f,1.0f,0.0f);
}

/**
 * Reakce na zmenu velikosti okna.
 * 
 * @param width nova sirka okna
 * @param height nova vyska okna
 */
void ImageViewerOpenGl::resizeGL(int width, int height)
{
    makeCurrent();
    glViewport(0, 0, (GLint) width, (GLint) height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(0, width, 0, height, 0, 1); // To Draw image in the center of the area

    glMatrixMode(GL_MODELVIEW);

    mTemporalScale = width < mRenderQtImg.width() 
                     || height < mRenderQtImg.height();

    // ---> Scaled Image Sizes
    if (mScale || mTemporalScale)
    {
        mOutH = width / mImgRatio;
        mOutW = width;

        if (mOutH > height)
        {
            mOutW = height * mImgRatio;
            mOutH = height;
        }
    }
    else
    {
        mOutW = mRenderQtImg.width();
        mOutH = mRenderQtImg.height();
    }

    emit imageSizeChanged(mOutW, mOutH);
    // <--- Scaled Image Sizes

    mPosX = (width - mOutW) / 2;
    mPosY = (height - mOutH) / 2;

    mSceneChanged = true;

    updateScene();
}

/**
 * Prepocitani velikosti snimku na zaklade velikosti komponenty.
 */
void ImageViewerOpenGl::forceImageResize()
{
    mTemporalScale = width() < mRenderQtImg.width() 
                     || height() < mRenderQtImg.height();
    
    if (mScale || mTemporalScale)
    {
        mOutH = width() / mImgRatio;
        mOutW = width();

        if (mOutH > height())
        {
            mOutW = height() * mImgRatio;
            mOutH = height();
        }
    }
    else
    {
        mOutW = mRenderQtImg.width();
        mOutH = mRenderQtImg.height();
    }
    
    mPosX = (width() - mOutW) / 2;
    mPosY = (height() - mOutH) / 2;
}

/**
 * Obnoveni sceny.
 */
void ImageViewerOpenGl::updateScene()
{
    if (mSceneChanged && this->isVisible())
    {
        update();
    }
}

/**
 * Vykresleni sceny.
 */
void ImageViewerOpenGl::paintGL()
{
    makeCurrent();

    if (!mSceneChanged)
        return;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderImage();

    mSceneChanged = false;
}

/**
 * Vykresleni snimku.
 */
void ImageViewerOpenGl::renderImage()
{
    makeCurrent();

    glClear(GL_COLOR_BUFFER_BIT);

    if (!mRenderQtImg.isNull())
    {
        glLoadIdentity();

        QImage image; // the image rendered

        glPushMatrix();
        {
            if (mScale || mTemporalScale)
            {
                int imW = mRenderQtImg.width();
                int imH = mRenderQtImg.height();

                // The image is to be resized to fit the widget?
                if (imW != this->size().width() &&
                        imH != this->size().height())
                {

                    image = mRenderQtImg.scaled(//this->size(),
                            QSize(mOutW, mOutH),
                            Qt::IgnoreAspectRatio,
                            Qt::SmoothTransformation
                            );

                    //qDebug( QString( "Image size: (%1x%2)").arg(imW).arg(imH).toAscii() );
                }
                else
                {
                    image = mRenderQtImg;
                }
                // ---> Centering image in draw area            
                glRasterPos2i(mPosX, mPosY);
                // <--- Centering image in draw area

                imW = image.width();
                imH = image.height();
                
                glDrawPixels(imW, imH, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());
            }
            else
            {
                glRasterPos2i(mPosX, mPosY);
                glDrawPixels(mRenderQtImg.width(), mRenderQtImg.height(),
                             GL_RGBA, GL_UNSIGNED_BYTE, mRenderQtImg.bits());
            }
        }
        glPopMatrix();

        // end
        glFlush();
    }
}

/**
 * Nastaveni noveho sniku k vykresleni.
 * 
 * @param image novy snimek
 * @return true - vse ok; false - neco se pokazilo
 */
bool ImageViewerOpenGl::showImage(cv::Mat image)
{
    bool needResize = image.cols != mOrigImage.cols || image.rows != mOrigImage.rows;

    image.copyTo(mOrigImage);
    
    mImgRatio = (float) image.cols / (float) image.rows;
    if (mOrigImage.channels() == 3)
    {
        mRenderQtImg = QImage((const unsigned char*) (mOrigImage.data),
                mOrigImage.cols, mOrigImage.rows,
                mOrigImage.step, QImage::Format_RGB888).rgbSwapped();
    }
    else if (mOrigImage.channels() == 1)
    {
        mRenderQtImg = QImage((const unsigned char*) (mOrigImage.data),
                mOrigImage.cols, mOrigImage.rows,
                mOrigImage.step, QImage::Format_Indexed8);
    }
    else
    {
        return false;
    }
    
    mRenderQtImg = QGLWidget::convertToGLFormat(mRenderQtImg);

    if (needResize)
    {
        forceImageResize();
    }
    
    mSceneChanged = true;

    updateScene();

    return true;
}

/**
 * Nastaveni zda se ma snimek roztahovat na cele okno.
 * 
 * @param scale scale
 */
void ImageViewerOpenGl::setScale(bool scale)
{
    if (mScale != scale)
    {
        mScale = scale;

        forceImageResize();

        mSceneChanged = true;
        updateScene();
    }
}
/**
 * Nastaveni zda se ma snimek roztahovat na cele okno.
 * 
 * @param scale scale
 */
void ImageViewerOpenGl::setScale(int scale)
{
    if (mScale != scale)
    {
        mScale = scale;

        forceImageResize();

        mSceneChanged = true;
        updateScene();
    }
}
