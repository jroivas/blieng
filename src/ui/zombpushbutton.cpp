#include "zombpushbutton.h"
#include "blieng/configure.h"
#include <QPainter>
#include <QApplication>
#include <QDebug>

using ui::ZombPushButton;

ZombPushButton::ZombPushButton(QString label, QWidget *parent) : QAbstractButton(parent)
{
    setText(label);

    _none = QColor(0, 0, 0, 0);
    _bg = QColor(255, 255, 255, 255);
    _bg_down = QColor(255, 255, 255, 255);
    _border = QColor(255, 0, 0, 255);
    _border_down = QColor(0, 0, 255, 255);
    _text_color = QColor(0, 0, 0, 255);
}

void ZombPushButton::setTextColor(QColor text)
{
    _text_color = text;
}

void ZombPushButton::setBackground(QColor bg)
{
    _bg = bg;
}

void ZombPushButton::setBackgroundDown(QColor bg)
{
    _bg_down = bg;
}

void ZombPushButton::setBorder(QColor border)
{
    _border = border;
}

void ZombPushButton::setBorderDown(QColor border)
{
    _border_down = border;
}


void ZombPushButton::paintEvent(QPaintEvent *event)
{
    QPainter paint(this);

    QRect geom = geometry();
    static const unsigned int norm_margin = 2;
    static const unsigned int down_margin = 5;

    QPoint topLeft;
    if (isDown()) {
        topLeft = QPoint(5, 5);
    } else {
        topLeft = QPoint(2, 2);
    }
    QPoint bottomRight;
    if (isDown()) {
        bottomRight = QPoint(geom.size().width() - down_margin, geom.size().height() - down_margin);
    } else {
        bottomRight = QPoint(geom.size().width() - norm_margin, geom.size().height() - norm_margin);
    }

    QRect geom2 = QRect(QPoint(2, 2), geom.size());

    if (isDown()) {
        paint.setPen(_bg_down);
        paint.setBrush(_bg_down);
    } else {
        paint.setPen(_bg);
        paint.setBrush(_bg);
    }
    paint.drawRect(geom2);

    paint.setPen(_text_color);
    paint.setBrush(_none);

    QRect namebox = paint.boundingRect(0, 0, geom.width(), geom.height(), Qt::AlignLeft, text());
    
    QPoint textpos = QPoint(geom.width()/2 - namebox.width()/2 + 1, geom.height()/2 + namebox.height()/2 - 1);
    paint.drawText(textpos, text());

    if (isDown()) {
        paint.setPen(_border_down);
    } else {
        paint.setPen(_border);
    }

    paint.drawRect(QRect(topLeft, bottomRight));
}

QSize ZombPushButton::sizeHint() const
{
    ensurePolished();

    int w = 0, h = 0;

    QStyleOptionButton option;
    option.initFrom(this);
    option.features = QStyleOptionButton::None;
    option.features |= QStyleOptionButton::AutoDefaultButton;
    option.features |= QStyleOptionButton::DefaultButton;
    option.text = text();

    QString s(text());
    bool empty = s.isEmpty();
    if (empty) {
        s = QString::fromLatin1("AAAA");
    }
    QFontMetrics fm = fontMetrics();
    QSize sz = fm.size(Qt::TextShowMnemonic, s);

    if (!empty || !w) w += sz.width();
    if (!empty || !h) h = qMax(h, sz.height());

    option.rect.setSize(QSize(w, h));
    QSize sizeHint = (style()->sizeFromContents(QStyle::CT_PushButton, &option, QSize(w, h), this).expandedTo(QApplication::globalStrut()));
    return sizeHint;
}

