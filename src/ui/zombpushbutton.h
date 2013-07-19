#ifndef __UI_ZOMB_PUSH_BUTTON_H
#define __UI_ZOMB_PUSH_BUTTON_H

#include <QAbstractButton>
#include <QStyle>
#include <QStyleOptionButton>
#include <QEvent>

namespace ui
{

class ZombPushButton : public QAbstractButton
{
    Q_OBJECT

public:
    ZombPushButton(QString label="", QWidget *parent=0); 
    void paintEvent(QPaintEvent *event);

    QSize sizeHint() const;

    void setTextColor(QColor text);
    void setBackground(QColor bg);
    void setBackgroundDown(QColor bg);
    void setBorder(QColor border);
    void setBorderDown(QColor border);

private:
    QColor _text_color;
    QColor _border;
    QColor _border_down;
    QColor _bg;
    QColor _bg_down;
    QColor _none;
};

}

#endif
