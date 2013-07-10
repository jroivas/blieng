#ifndef __MAPEDITOR_TOWN_PROPERTIES_H
#define __MAPEDITOR_TOWN_PROPERTIES_H

#include <QtGui>
#include <vector>
#include <blieng/town.h>

namespace MapEditor
{

class TownProperties : public QWidget
{
    Q_OBJECT

public:
    TownProperties(QWidget *parent = 0);
    void setTown(blieng::Town *town);
    blieng::Town *getTown();

signals:
    void updated();

public slots:
    void updatePos();

private slots:
    void saveInfo();

private:
    void updateInfo();

    blieng::Town *town;
    QVBoxLayout layout;
    QHBoxLayout size_layout;


    QLineEdit *_name;
    QLabel *_size_label;
    QSpinBox  *_size;
    QPushButton *_save;
    QLabel *_xlab;
    QLabel *_ylab;
};

}

#endif
