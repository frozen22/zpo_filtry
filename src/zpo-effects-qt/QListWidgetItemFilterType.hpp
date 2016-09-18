/* 
 * Soubor: QListWidgetItemFilterType.h
 * Popis:  VUT Brno FIT - Zpracovani obrazu (ZPO)
 *         Filtrove "efekty" v obrazu
 * Autori: Frantisek Nemec (xnemec61@stud.fit.vutbr.cz)
 *         Jan Opalka (xopalk01@stud.fit.vutbr.cz)
 * Datum:  2015-04-13
 */

#ifndef QLISTWIDGETITEMFILTERTYPE_HPP
#define	QLISTWIDGETITEMFILTERTYPE_HPP

class QListWidgetItemFilterType : public QListWidgetItem
{
    private:
        
        ImageFilter::Type mFilterType;
        
    public:

        explicit QListWidgetItemFilterType(ImageFilter::Type filterType, 
                                           const QString & text, 
                                           QListWidget * parent = 0, 
                                           int type = Type)
            : QListWidgetItem(text, parent, type)
            , mFilterType(filterType)
        { }

        ImageFilter::Type getFilterType() const
        {
            return mFilterType;
        }
        
};

#endif	/* QLISTWIDGETITEMFILTERTYPE_HPP */

