/* 
 * Soubor: LabelChanger.cpp
 * Autor: Frantisek Nemec (xnemec61@stud.fit.vutbr.cz)
 * Datum: 2015-05-04
 */

#include "LabelChanger.h"

/**
 * Konstruktor.
 * 
 * @param button tlacitko, ktere bude meneno
 * @param labels seznam popisku
 * @param interval interval mezi vymenou popisku
 */
LabelChanger::LabelChanger(QLabel* label,
                           std::vector<std::string> labels,
                           int interval)
    : mTimer(this)
    , mLabel(label)
    , mLabels(labels)
    , mInterval(interval)
    , mActiveLabelIndex(0)
{
    mTimer.setInterval(interval);
    
    connect(&mTimer, SIGNAL(timeout()), this, SLOT(changeLabel()));
}

/**
 * Start automaticke zmeny popisku.
 */
void LabelChanger::start()
{
    if (mLabel != nullptr)
    {
        changeLabel(); // Prvni zmenu chceme provest ihned
        mTimer.start();
    }
}

/**
 * Zastaveni automaticke zmeny popisku.
 */
void LabelChanger::stop()
{
    mTimer.stop();
    mActiveLabelIndex = 0;
}

/**
 * Zmena popisku na dalsi popisek ze seznamu.
 */
void LabelChanger::changeLabel()
{
    mLabel->setText(QString::fromStdString(mLabels[mActiveLabelIndex]));
    mActiveLabelIndex = (mActiveLabelIndex + 1) % mLabels.size();
}
