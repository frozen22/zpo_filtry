/* 
 * Soubor: MainWindow.h
 * Popis:  VUT Brno FIT - Zpracovani obrazu (ZPO)
 *         Filtrove "efekty" v obrazu
 * Autori: Frantisek Nemec (xnemec61@stud.fit.vutbr.cz)
 *         Jan Opalka (xopalk01@stud.fit.vutbr.cz)
 * Datum:  2015-04-09
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <string>

#include <QMainWindow>
#include <QListWidgetItem>

#include "ImageSource.h"
#include "LoadingDialog.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
    private:
        
        Ui::MainWindow *ui;
        ImageSource mImageSource;
        LoadingDialog* mLoadingDialog;
        
    public:
        
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

    private slots:
        
        void on_pushButtonImage_clicked();
        void on_pushButtonVideo_clicked();
        void on_pushButtonCam_clicked();

        void on_listWidgetFilterType_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
        
        void errorMessage(std::string title, std::string msg);
        void imageFiltred();
        
    private:
        
        void initImageViewer();
        void initImageSource();
        void initImage();
        void initListFilterType();
        
        void center();
        
        cv::Mat loadImage(const std::string& imgPath, bool* ok = nullptr);
        QString openFileName(const std::string& title);
        

};

#endif // MAINWINDOW_H
