/* 
 * Soubor: LoadingDialog.h
 * Popis:  VUT Brno FIT - Zpracovani obrazu (ZPO)
 *         Filtrove "efekty" v obrazu
 * Autori: Frantisek Nemec (xnemec61@stud.fit.vutbr.cz)
 *         Jan Opalka (xopalk01@stud.fit.vutbr.cz)
 * Datum:  2015-04-09
 */

#ifndef LOADINGDIALOG_H
#define LOADINGDIALOG_H

#include <QDialog>
#include "LabelChanger.h"

namespace Ui {
    class LoadingDialog;
}

class LoadingDialog : public QDialog
{
    Q_OBJECT

    public:
        
        explicit LoadingDialog(QWidget *parent = 0);
        ~LoadingDialog();

    private:
        
        Ui::LoadingDialog *ui;
        
        LabelChanger* mLabelChanger;
        
        void center();
        
};

#endif // LOADINGDIALOG_H
