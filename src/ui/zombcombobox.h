#ifndef __UI_ZOMB_COMBO_BOX_H
#define __UI_ZOMB_COMBO_BOX_H

#include <QWidget>
#include <QSize>
#include <QStandardItemModel>
#include <QPaintEvent>
#include <QPolygon>
#include <QString>
#include <QVariant>
#include <QMouseEvent>
#include "ui/zombstyles.h"

namespace ui
{

class ZombComboBox : public QWidget, public ui::ZombStyles
{
    Q_OBJECT

public:
    enum ComboBoxStyle { ComboBox_Horizontal, ComboBox_Vertical, ComboBox_Popup };
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

public slots:
    void prevItem();
    void nextItem();

signals:
    void clicked();

protected:
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    bool event(QEvent *event);

private:
    enum ComboBoxButtons { ComboBox_None, ComboBox_Left, ComboBox_Right, ComboBox_Item };
    QStandardItemModel *model;
    QPersistentModelIndex _root;
    QPersistentModelIndex _currentIndex;
    int modelColumn;
    QString itemText(const QModelIndex &index) const;
    int itemRole() const;
    ComboBoxStyle _style;

    void updateGeometries();

    QPoint _topLeft;
    QPoint _bottomRight;
    QPoint _leftBoxPoint;
    QPoint _rightBoxPoint;
    QPoint _topRight;
    QRect _leftBox;
    QRect _rightBox;
    QRect _box;
    QRect _smaller_box;
    QRect _full_box;
    QPolygon _polyLeft;
    QPolygon _polyRight;
    QRect sub_geom;
    ComboBoxButtons _buttons;
};

inline void ZombComboBox::addItem(const QString &text, const QVariant &userData)
{
    insertItem(count(), text, userData);
}

}

#endif
