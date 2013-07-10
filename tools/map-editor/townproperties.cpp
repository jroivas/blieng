#include "townproperties.h"

using MapEditor::TownProperties;

TownProperties::TownProperties(QWidget *parent) : QWidget(parent)
{
    town = NULL;

    _name = new QLineEdit();
    _size = new QSpinBox();
    _size->setMinimum(1);
    _size->setMaximum(200);

    _size_label = new QLabel("Size:");
    _xlab = new QLabel("X pos:");
    _ylab = new QLabel("Y pos:");
    _save = new QPushButton("Save");

    layout.addWidget(_name);
    size_layout.addWidget(_size_label);
    size_layout.addWidget(_size);
    layout.addLayout(&size_layout);

    layout.addWidget(_xlab);
    layout.addWidget(_ylab);

    layout.addWidget(_save);

    connect(_save, SIGNAL(clicked()), this, SLOT(saveInfo()));

    setLayout(&layout);
}

void TownProperties::setTown(blieng::Town *_town)
{
    town = _town;
    updateInfo();
}

blieng::Town *TownProperties::getTown()
{
    return town;
}

void TownProperties::saveInfo()
{
    if (town == NULL) return;

    town->setName(_name->text().toLatin1().constData());
    town->setSize((unsigned int)_size->value());
    emit updated();

    town = NULL;
    hide();
}

void TownProperties::updatePos()
{
    if (town == NULL) return;

    _xlab->setText("X pos: " + QString::number(town->getPositionX()));
    _ylab->setText("Y pos: " + QString::number(town->getPositionY()));
}

void TownProperties::updateInfo()
{
    if (town == NULL) return;

    _name->setText(town->getName().c_str());
    _size->setValue(town->getSize());

    updatePos();
}
