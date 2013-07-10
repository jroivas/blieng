#include "townproperties.h"

using MapEditor::TownProperties;

TownProperties::TownProperties(QWidget *parent) : QWidget(parent)
{
    town = NULL;

    _name = new QLineEdit();

    _size = new QSpinBox();
    _size->setMinimum(1);
    _size->setMaximum(200);

    _zombies = new QSpinBox();
    _zombies->setMinimum(0);
    _zombies->setMaximum(100);

    _population = new QSpinBox();
    _population->setMinimum(0);
    _population->setMaximum(100);

    _size_label = new QLabel("Size:");
    _xlab = new QLabel("X pos:");
    _ylab = new QLabel("Y pos:");
    _zombies_label = new QLabel("Zombies:");
    _population_label = new QLabel("Population index:");
    _save = new QPushButton("Save town");
    _start = new QCheckBox("Hometown");

    layout.addWidget(_name);

    size_layout.addWidget(_size_label);
    size_layout.addWidget(_size);
    layout.addLayout(&size_layout);

    layout.addWidget(_start);

    zombies_layout.addWidget(_zombies_label);
    zombies_layout.addWidget(_zombies);
    layout.addLayout(&zombies_layout);

    population_layout.addWidget(_population_label);
    population_layout.addWidget(_population);
    layout.addLayout(&population_layout);

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
    if (_start->checkState() == Qt::Checked) {
        town->setValue("start", true);
    } else {
        town->setValue("start", false);
    }
    town->setValue("zombies", (unsigned int)_zombies->value());
    town->setValue("population-index", (double)_population->value());
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
    if (town->isValue("start") && town->getBoolValue("start")) {
        _start->setCheckState(Qt::Checked);
    } else {
        _start->setCheckState(Qt::Unchecked);
    }
    if (town->isValue("zombies")) {
        _zombies->setValue(town->getUIntValue("zombies"));
    } else {
        _zombies->setValue(0);
    }
    if (town->isValue("population-index")) {
        _population->setValue(town->getDoubleValue("population-index"));
    } else {
        _population->setValue(0);
    }

    updatePos();
}
