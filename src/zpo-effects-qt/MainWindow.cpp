/* 
 * Soubor: MainWindow.cpp
 * Popis:  VUT Brno FIT - Zpracovani obrazu (ZPO)
 *         Filtrove "efekty" v obrazu
 * Autori: Frantisek Nemec (xnemec61@stud.fit.vutbr.cz)
 *         Jan Opalka (xopalk01@stud.fit.vutbr.cz)
 * Datum:  2015-04-09
 */

#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <iostream>
#include <string>

#include <QDesktopWidget> // center
#include <QFileDialog>
#include <QMessageBox>
#include <QMetaObject>
#include <QMetaEnum>
#include <QVariant>

#include "Debug.h"
#include "QListWidgetItemFilterType.hpp"

MainWindow::MainWindow(QWidget *parent) 
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , mLoadingDialog(nullptr)
{
    ui->setupUi(this);
    
    initListFilterType();
    
    initImageViewer();
    initImageSource();
    
    ui->listWidgetFilterType->setEnabled(false);
    ui->checkBoxScale->setEnabled(false);
    
    center();
    initImage();
}

MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * Inicializace propojeni.
 */
void MainWindow::initImageViewer()
{
    connect(ui->checkBoxScale, SIGNAL(stateChanged(int)), ui->openGLWidget, SLOT(setScale(int)));
    connect(&mImageSource, SIGNAL(newImage(cv::Mat)), ui->openGLWidget, SLOT(showImage(cv::Mat)));
    connect(&mImageSource, SIGNAL(newImage(cv::Mat)), this, SLOT(imageFiltred()));
    
//    connect(ui->checkBoxScale, SIGNAL(stateChanged(int)), ui->imageWidget, SLOT(setScale(int)));
//    connect(&mImageSource, SIGNAL(newImage(cv::Mat)), ui->imageWidget, SLOT(showImage(cv::Mat)));
}

/**
 * Inicializace propojeni se zdrojem snimku.
 */
void MainWindow::initImageSource()
{
    connect(&mImageSource, SIGNAL(errorMessage(std::string,std::string)), 
            this, SLOT(errorMessage(std::string,std::string)));
}

/**
 * Posunuti okna na stred obrozovky.
 */
void MainWindow::center()
{
    this->move(QApplication::desktop()->screen()->rect().center() 
                - this->rect().center() - QPoint(0,40));
}

/**
 * Nacteni pocatecniho obrazku.
 */
void MainWindow::initImage()
{
    bool ok;
    cv::Mat img = loadImage("data/amber_rose.jpg", &ok);

    if (ok)
    {
        QMetaObject::invokeMethod(&mImageSource, "setImage", Qt::DirectConnection, Q_ARG(cv::Mat, img));
    }
}

/**
 * Naplneni seznamu filtru.
 */
void MainWindow::initListFilterType()
{
    // Bylo by vhodne ihned oznacil jednu polozku seznamu. ALE
    // Pri nastaveni vybrane polozky s konstruktoru to spadne, protoze se 
    // vyvola udalost itemChanged a v metode pro obslouzeni se pracuje
    // se samotny jeste NEINICIALIZOCANYM listem
    
    auto item = new QListWidgetItemFilterType(ImageFilter::Type::NoFilter,
                                              "No filter",
                                              ui->listWidgetFilterType);
    ui->listWidgetFilterType->addItem(item);
            
    item = new QListWidgetItemFilterType(ImageFilter::Type::Comics,
                                         "Comic",
                                         ui->listWidgetFilterType);
    ui->listWidgetFilterType->addItem(item);
    
    item = new QListWidgetItemFilterType(ImageFilter::Type::EdgeGrayLeft,
                                         "Edge left",
                                         ui->listWidgetFilterType);
    ui->listWidgetFilterType->addItem(item);
    
    item = new QListWidgetItemFilterType(ImageFilter::Type::EdgeGrayRight,
                                         "Edge right",
                                         ui->listWidgetFilterType);
    ui->listWidgetFilterType->addItem(item);
    
    item = new QListWidgetItemFilterType(ImageFilter::Type::EdgeGrayDown,
                                         "Edge down",
                                         ui->listWidgetFilterType);
    ui->listWidgetFilterType->addItem(item);
    
    item = new QListWidgetItemFilterType(ImageFilter::Type::EdgeGrayUp,
                                         "Edge up",
                                         ui->listWidgetFilterType);
    ui->listWidgetFilterType->addItem(item);
    
    item = new QListWidgetItemFilterType(ImageFilter::Type::EdgeGrayFourDir,
                                         "Edge all",
                                         ui->listWidgetFilterType);
    ui->listWidgetFilterType->addItem(item);
    
    item = new QListWidgetItemFilterType(ImageFilter::Type::EdgeGrayFourDirEqu,
                                         "Edge all equ",
                                         ui->listWidgetFilterType);
    ui->listWidgetFilterType->addItem(item);
    
    item = new QListWidgetItemFilterType(ImageFilter::Type::SobelGray,
                                         "Prewitt gray",
                                         ui->listWidgetFilterType);
    ui->listWidgetFilterType->addItem(item);
    item = new QListWidgetItemFilterType(ImageFilter::Type::SobelGray2,
                                         "Sobel gray",
                                         ui->listWidgetFilterType);
    ui->listWidgetFilterType->addItem(item);
    
    item = new QListWidgetItemFilterType(ImageFilter::Type::SobelColor,
                                         "Sobel color",
                                         ui->listWidgetFilterType);
    ui->listWidgetFilterType->addItem(item);
    
    item = new QListWidgetItemFilterType(ImageFilter::Type::Glass,
                                         "Glass",
                                         ui->listWidgetFilterType);
    ui->listWidgetFilterType->addItem(item);
    
        item = new QListWidgetItemFilterType(ImageFilter::Type::EdgeGrayFourMax,
                                         "Emboss",
                                         ui->listWidgetFilterType);
    ui->listWidgetFilterType->addItem(item);
}

/**
 * Nacteni obrazku.
 * 
 * @param imgPath cesta k obrazku
 * @param ok vystupni parametr indikujici spravne nacteni
 * @return nacteny obrazek
 */
cv::Mat MainWindow::loadImage(const std::string& imgPath, bool* ok)
{
    cv::Mat img = cv::imread(imgPath);
    
    if (ok != nullptr)
    {
       *ok = !img.empty();
    }

    return img;
}

/**
 * Zobrazeni dialogu pro vyber souboru.
 * 
 * @param title popisek okna
 * @return cesta k vybranemu souboru
 */
QString MainWindow::openFileName(const std::string& title)
{
    QString fileName;
    if (mImageSource.videoIsRunning())
    {
        QMetaObject::invokeMethod(&mImageSource, "stopVideo");
        fileName = QFileDialog::getOpenFileName(this, tr(title.c_str()));
        QMetaObject::invokeMethod(&mImageSource, "startVideo");
    }
    else
    {
        fileName = QFileDialog::getOpenFileName(this, tr(title.c_str()));
    }
    
    return fileName;
}

/**
 * Reakce na stisk tlacitka pro otevreni noveho obrazku.
 */
void MainWindow::on_pushButtonImage_clicked()
{
    QString fileName = openFileName("Open image");

    if (!fileName.isNull())
    {
        bool ok;

        cv::Mat img = loadImage(fileName.toStdString(), &ok);
        
        if (ok)
        {
            QMetaObject::invokeMethod(&mImageSource, "setImage", Q_ARG(cv::Mat, img));
            ui->listWidgetFilterType->setEnabled(false);
            ui->checkBoxScale->setEnabled(false);
            mLoadingDialog = new LoadingDialog(this);
            mLoadingDialog->show();
            
        }
        else
        {
            QMessageBox::critical(this, "Error", 
                    QString("Could not load image: \"%1\"").arg(fileName));
        }
    }
}

/**
 * Reakce na stisk tlacitka pro otevreni noveho videa.
 */
void MainWindow::on_pushButtonVideo_clicked()
{
    QString fileName = openFileName("Open video");

    if (!fileName.isNull())
    {
        QMetaObject::invokeMethod(&mImageSource, "setVideo", Q_ARG(std::string, fileName.toStdString()));
    }
}

/**
 * Reakce na stisk tlacitka pro otevreni kamery.
 */
void MainWindow::on_pushButtonCam_clicked()
{
    QMetaObject::invokeMethod(&mImageSource, "setCamera");
}

/**
 * Reakce na zmenu filtru.
 */
void MainWindow::on_listWidgetFilterType_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    UNREFERENCED_PARAMETER(previous)
        
    ImageFilter::Type filterType = static_cast<QListWidgetItemFilterType*>(current)->getFilterType();
    
    // Je nutny invokeMethod misto primeho volani, aby byl vypocet proveden
    // v jinem vlakne a nezatezovat GUI.
    QMetaObject::invokeMethod(&mImageSource, 
                             "setFilterType",
                             Qt::QueuedConnection, 
                             Q_ARG(ImageFilter::Type, filterType));
    
    if (mImageSource.getSourceType() == ImageSource::SourceType::IMAGE)
    {
        ui->listWidgetFilterType->setEnabled(false);
        ui->checkBoxScale->setEnabled(false);
        mLoadingDialog = new LoadingDialog(this);
        mLoadingDialog->show();
    }
}

/**
 * Zobrazeni chybove zpravy.
 * 
 * @param title popisek okna
 * @param msg hlaseni zpravy
 */
void MainWindow::errorMessage(std::string title, std::string msg)
{
    QMessageBox::critical(this, QString::fromStdString(title), QString::fromStdString(msg));
}

/**
 * Reakce na dokonceni filtrace obrazku.
 */
void MainWindow::imageFiltred()
{
    ui->listWidgetFilterType->setEnabled(true);
    ui->checkBoxScale->setEnabled(true);
    
    if (mLoadingDialog != nullptr)
    {
        mLoadingDialog->close();
        delete mLoadingDialog;
        mLoadingDialog = nullptr;
    }
}
