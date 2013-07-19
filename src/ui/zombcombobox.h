#ifndef __UI_ZOMB_COMBO_BOX_H
#define __UI_ZOMB_COMBO_BOX_H

#include <QWidget>
#include <QSize>
#include <QStandardItemModel>
#include <QPaintEvent>
#include <QString>
#include <QVariant>
#include "ui/zombstyles.h"

namespace ui
{

class ZombComboBox : public QWidget, public ui::ZombStyles
{
    Q_OBJECT

public:
    typedef enum ComboBoxStyle { ComboBox_Horizontal, ComboBox_Vertical, ComboBox_Popup };
    ZombComboBox(QWidget *parent=0); 
    void paintEvent(QPaintEvent *event);

    QSize sizeHint() const;
    QString currentText() const;
    int count() const;
    int currentIndex() const;

    void insertItem(int index, const QString &text, const QVariant &userData = QVariant());
    inline void addItem(const QString &text, const QVariant &userData = QVariant());
    QString itemText(int index) const;
    void setCurrentIndex(int index);
    void setCurrentIndex(const QModelIndex &mi);
    void setStyle();

private:
    QStandardItemModel *model;
    QPersistentModelIndex _root;
    QPersistentModelIndex _currentIndex;
    int modelColumn;
    QString itemText(const QModelIndex &index) const;
    int itemRole() const;
    ComboBoxStyle _style;

};

inline void ZombComboBox::addItem(const QString &text, const QVariant &userData)
{
    insertItem(count(), text, userData);
}


}

#endif
