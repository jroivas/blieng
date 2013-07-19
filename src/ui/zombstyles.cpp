#include "ui/zombstyles.h"

using ui::ZombStyles;

ZombStyles::ZombStyles()
{
    _none = QColor(0, 0, 0, 0);
    _bg = QColor(255, 255, 255, 255);
    _bg_down = QColor(255, 255, 255, 255);
    _border = QColor(255, 0, 0, 255);
    _border_down = QColor(0, 0, 255, 255);
    _text_color = QColor(0, 0, 0, 255);
}

void ZombStyles::setTextColor(QColor text)
{
    _text_color = text;
}

void ZombStyles::setBackground(QColor bg)
{
    _bg = bg;
}

void ZombStyles::setBackgroundDown(QColor bg)
{
    _bg_down = bg;
}

void ZombStyles::setBorder(QColor border)
{
    _border = border;
}

void ZombStyles::setBorderDown(QColor border)
{
    _border_down = border;
}


