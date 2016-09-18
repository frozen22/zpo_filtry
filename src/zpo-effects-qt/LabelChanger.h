/* 
 * Soubor: LabelChanger.h
 * Autor: Frantisek Nemec (xnemec61@stud.fit.vutbr.cz)
 * Datum: 2015-05-04
 */

#ifndef BUTTONLABELCHANGER_H
#define	BUTTONLABELCHANGER_H

#include <string>
#include <vector>

#include <QTimer>
#include <QLabel>

/**
 * Trida pro automatickou zmenu popisku tlacitka. Interval zmeny udava 
 * mInterval a seznam popisku je v mLabels. Kazdy casovy interval se zmeni
 * popis tracitka na dalsi retezec v seznamu popisku.
 */
class LabelChanger : QObject
{
    Q_OBJECT
    
    private:

        QTimer mTimer;
        
        QLabel* mLabel;
        std::vector<std::string> mLabels;
        int mInterval;
        
        size_t mActiveLabelIndex;
        
    public:

        LabelChanger(QLabel* label, std::vector<std::string> labels, int interval);

    public slots:

        void start();
        void stop();

    private slots:

        void changeLabel();

};

#endif	/* BUTTONLABELCHANGER_H */
