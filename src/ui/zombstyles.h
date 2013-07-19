#ifndef __UI_ZOMB_STYLES_H
#define __UI_ZOMB_STYLES_H

#include <QColor>

namespace ui
{

class ZombStyles
{
public:
    ZombStyles();
    void setTextColor(QColor text);
    void setBackground(QColor bg);
    void setBackgroundDown(QColor bg);
    void setBorder(QColor border);
    void setBorderDown(QColor border);

protected:
    QColor _text_color;
    QColor _border;
    QColor _border_down;
    QColor _bg;
    QColor _bg_down;
    QColor _none;
};

}

#endif
