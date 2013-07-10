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
    bool start;


    QLineEdit *_name;

    QLabel *_size_label;
    QSpinBox  *_size;
    QHBoxLayout size_layout;

    QPushButton *_save;
    QLabel *_xlab;
    QLabel *_ylab;
    QCheckBox *_start;

    QLabel *_zombies_label;
    QSpinBox  *_zombies;
    QHBoxLayout zombies_layout;

    QLabel *_population_label;
    QSpinBox  *_population;
    QHBoxLayout population_layout;
};

}

#endif
