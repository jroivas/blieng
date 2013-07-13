#include "generate_character.h"
#include <list>
#include <QLabel>

using ui::GenerateCharacter;

GenerateCharacter::GenerateCharacter(QWidget *parent) : QWidget(parent)
{
    character = new zomb::PlayerCharacter();
    setLayout(&layout);
    improve_points = 5;
    update();
}

void GenerateCharacter::reroll()
{
    improve_points = 5;
    character->roll();
    update();
}

void GenerateCharacter::clearLayout(QLayout *del_layout)
{
    while (del_layout->count() > 0) {
        QLayoutItem *item = del_layout->itemAt(0);
        if (item != NULL) {
            QLayout *lay = item->layout();
            QWidget *wid = item->widget();
            QSpacerItem *space = item->spacerItem();

            if (lay != NULL) {
                del_layout->removeItem(item);
                clearLayout(lay);
                delete lay;
            }
            else if (wid != NULL) {
                del_layout->removeWidget(wid);
                delete wid;
            }
            else if (space != NULL) {
                del_layout->removeItem(item);
                delete space;
            }
            else {
                printf("else\n");
                break;
            }
        } else {
            printf("null\n");
        }
    }
}
void GenerateCharacter::clear()
{
    clearLayout(&layout);
}

void GenerateCharacter::improve()
{
    if (improve_points > 0) {
        std::string key = improve_map[(QPushButton*)sender()];
        if (character->increase(key)) {
            --improve_points;
            update();
        }
    }
}

void GenerateCharacter::done()
{
    emit generated(character);
    character = new zomb::PlayerCharacter();
    reroll();
}

void GenerateCharacter::update()
{
    clear();

    //: Re-roll a character
    QPushButton *reroll_button = new QPushButton(tr("Re-roll"));
    reroll_button->setMinimumHeight(20);
    connect(reroll_button, SIGNAL(clicked()), this, SLOT(reroll()));

    //: Done selecting character
    QPushButton *done_button = new QPushButton(tr("Done"));
    done_button->setMinimumHeight(20);
    connect(done_button, SIGNAL(clicked()), this, SLOT(done()));

    layout.addWidget(done_button);
    layout.addWidget(reroll_button);

    //: Improve points for a character left
    QLabel *improve = new QLabel(tr("Improve points left:") + " " + QString::number(improve_points));
    layout.addWidget(improve);

    std::list<std::string> keys = character->getKeys();
    std::list<std::string>::iterator key_iter = keys.begin();
    while (key_iter != keys.end()) {
        QLabel *l = new QLabel(QString(key_iter->c_str()));
        QLabel *l2 = NULL;
        bool ok = true;
        bool can_improve = false;

        const std::type_info *valtype = character->getValueType(*key_iter);
        if (*valtype == typeid(unsigned int)) {
            unsigned int val = character->getIntValue(*key_iter);
            l2 = new QLabel(QString::number(val));
            can_improve = (key_iter->find("max") == std::string::npos);
        } else if (*valtype == typeid(int)) {
            int val = character->getIntValue(*key_iter);
            l2 = new QLabel(QString::number(val));
            can_improve = (key_iter->find("max") == std::string::npos);
        } else if (*valtype == typeid(double)) {
            int val = character->getDoubleValue(*key_iter);
            l2 = new QLabel(QString::number(val));
            can_improve = false;
        } else if (*valtype == typeid(bool)) {
            bool val = character->getBoolValue(*key_iter);
            //: For boolean values show Yes/No as verbosal output
            l2 = new QLabel(val ? tr("Yes") : tr("No"));
            can_improve = false;
        } else if (*valtype == typeid(std::string)) {
            std::string val = character->getStringValue(*key_iter);
            l2 = new QLabel(QString(val.c_str()));
            can_improve = false;
        } else {
            ok = false;
        }
        QHBoxLayout *h = new QHBoxLayout();
        h->addWidget(l);
        h->addSpacing(100);
        if (improve_points <= 0) {
            can_improve = false;
        }
        if (l2 != NULL) {
            h->addWidget(l2);
            h->setAlignment(l2, Qt::AlignRight);
            if (can_improve) {
                //: Increase points of a feature of the character
                QPushButton *imp = new QPushButton(tr("+"));
                imp->setMaximumWidth(30);
                connect(imp, SIGNAL(clicked()), this, SLOT(improve()));
                improve_map[imp] = *key_iter;
                h->addWidget(imp);
            }
        }
        layout.addLayout(h);
        key_iter++;
    }
}
