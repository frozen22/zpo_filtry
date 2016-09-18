/* 
 * Soubor: LoadingDialog.cpp
 * Popis:  VUT Brno FIT - Zpracovani obrazu (ZPO)
 *         Filtrove "efekty" v obrazu
 * Autori: Frantisek Nemec (xnemec61@stud.fit.vutbr.cz)
 *         Jan Opalka (xopalk01@stud.fit.vutbr.cz)
 * Datum:  2015-04-09
 */

#include "LoadingDialog.h"
#include "ui_LoadingDialog.h"

#include <QDesktopWidget>

LoadingDialog::LoadingDialog(QWidget *parent) 
    : QDialog(parent)
    , ui(new Ui::LoadingDialog)
{
    ui->setupUi(this);
    
    ui->label->setText("Calculating filter...");
    
    setModal(true);
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    
    std::vector<std::string> labels = {"Calculating", "Calculating.", 
                                       "Calculating..","Calculating..."};
     
    mLabelChanger = new LabelChanger(ui->label, labels, 500);
    mLabelChanger->start();
    
    center();
}

LoadingDialog::~LoadingDialog()
{
    mLabelChanger->stop();
    delete mLabelChanger;
    
    delete ui;
}

/**
 * Posunuti okna na stred obrozovky.
 */
void LoadingDialog::center()
{
    this->move(QApplication::desktop()->screen()->rect().center() 
                - this->rect().center() - QPoint(0,40));
}
