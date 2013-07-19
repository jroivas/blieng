#ifndef __UI_ZOMB_PUSH_BUTTON_H
#define __UI_ZOMB_PUSH_BUTTON_H

#include <QAbstractButton>
#include <QPaintEvent>
#include <QString>
#include <QWidget>
#include <QPaintEvent>
#include <QSize>
#include "ui/zombstyles.h"

namespace ui
{

class ZombPushButton : public QAbstractButton, public ui::ZombStyles
{
    Q_OBJECT

public:
    ZombPushButton(QString label="", QWidget *parent=0); 
    void paintEvent(QPaintEvent *event);

    QSize sizeHint() const;

private:
};

}

#endif
