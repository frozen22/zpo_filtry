/* 
 * Soubor: ImageViewerOpenGl.h
 * Popis:  VUT Brno FIT - Zpracovani obrazu (ZPO)
 *         Filtrove "efekty" v obrazu
 * Autori: Frantisek Nemec (xnemec61@stud.fit.vutbr.cz)
 *         Jan Opalka (xopalk01@stud.fit.vutbr.cz)
 * Datum:  2015-04-09
 * Zdroj: 
 *      http://www.robot-home.it/blog/software/tutorial-opencv-qt-opengl-widget-per-visualizzare-immagini-da-opencv-in-una-gui-con-qt/?lang=en 
 *      http://www.robot-home.it/blog/software/tutorial-opencv-qt-opengl-widget-per-visualizzare-immagini-da-opencv-in-una-gui-con-qt-seconda-parte/?lang=en
 */

#ifndef CQTOPENCVVIEWERGL_H
#define CQTOPENCVVIEWERGL_H

#include <QOpenGLWidget>
#include <opencv2/core/core.hpp>

class ImageViewerOpenGl : public QOpenGLWidget
{
    Q_OBJECT
    
    private:

        bool mTemporalScale;
        bool mScale;
        bool mSceneChanged; /// Indicates when OpenGL view is to be redrawn

        QImage mRenderQtImg; /// Qt image to be rendered
        cv::Mat mOrigImage; /// original OpenCV image to be shown

        QColor mBgColor; /// Background color

        int mOutH; /// Resized Image height
        int mOutW; /// Resized Image width
        float mImgRatio; /// height/width ratio

        int mPosX; /// Top left X position to render image in the center of widget
        int mPosY; /// Top left Y position to render image in the center of widget
        
    public:
        
        explicit ImageViewerOpenGl(QWidget *parent = 0);

    signals:
        
        void imageSizeChanged( int outW, int outH ); /// Used to resize the image outside the widget

    public slots:

        bool showImage(cv::Mat image); /// Used to set the image to be viewed
        void setScale(bool scale);
        void setScale(int scale);

    protected:

        void initializeGL(); /// OpenGL initialization
        void paintGL(); /// OpenGL Rendering
        void resizeGL(int width, int height); /// Widget Resize Event

        void updateScene();
        void renderImage();
        void forceImageResize();

};

#endif // CQTOPENCVVIEWERGL_H
