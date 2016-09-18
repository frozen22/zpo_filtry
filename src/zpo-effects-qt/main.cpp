/* 
 * Soubor: main.cpp
 * Popis:  VUT Brno FIT - Zpracovani obrazu (ZPO)
 *         Filtrove "efekty" v obrazu
 * Autori: Frantisek Nemec (xnemec61@stud.fit.vutbr.cz)
 *         Jan Opalka (xopalk01@stud.fit.vutbr.cz)
 * Datum:  2015-04-09
 */

#include <QApplication>
#include <QMetaType>
#include <QStyleFactory>
#include <string>
#include <opencv2/core/core.hpp> // cv::Mat

#include "MainWindow.h"
#include "ImageFilter.h"

/**
 * Nastaveni fusion stylu.
 * 
 * @param app QApp
 */
void setFusionStyle(QApplication& app)
{
    app.setStyle(QStyleFactory::create("Fusion"));
}

/**
 * Vypada docela pekne, ale enabled = false u tlacitek neni vubec videt.
 * 
 * @param app QApp
 */
void setFusionDarkStyle(QApplication& app)
{
    QPalette darkPalette;
    
    darkPalette.setColor(QPalette::Window, QColor(53,53,53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(25,25,25));
    darkPalette.setColor(QPalette::AlternateBase, QColor(53,53,53));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(53,53,53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
 
    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);
    
    app.setPalette(darkPalette);
    app.setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }");
}

/**
 * Registrace meta typu pro Qt connect.
 */
void registerMetaTypes()
{
    qRegisterMetaType<cv::Mat>("cv::Mat");
    qRegisterMetaType<std::string>("std::string");
    qRegisterMetaType<ImageFilter::Type>("ImageFilter::Type");
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow w;
    w.show();
    
    registerMetaTypes();
    
    setFusionStyle(app);

    return app.exec();
}
