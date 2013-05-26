#include <QtCore>
#include <QtGui>
#include <QApplication>

#include <iostream>
#include "ui/generate_character.h"
#include "ui/startscreen.h"
#include "ui/loadingscreen.h"
#include "ui/mapscreen.h"
#include "ui/gamescreen.h"
#include "blieng/item.h"
#include "blieng/maps.h"
#include "blieng/wallclock.h"
#include "zomb/configure.h"
#include "zomb/zombie_character.h"
#include "zomb/worldclock.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    #if 0
    blieng::Item *nail = new blieng::Item("nail");
    blieng::Item *iron = new blieng::Item("iron");
    iron->setUsable();
    std::cout << nail->toString() << "\n";
    std::cout << iron->toString() << "\n";
    nail->consume(iron);
    std::cout << iron->toString() << "\n";
    
    blieng::Item *new_nail = nail->produce();
    if (new_nail != NULL) {
            std::cout << new_nail->toString() << "\n";
    } else {
            std::cout << "Can't produce nail\n";
    }

    new_nail = nail->produce();
    if (new_nail != NULL) {
            std::cout << new_nail->toString() << "\n";
    } else {
            std::cout << "Can't produce nail\n";
    }
    #endif

    #if 0
    for (int j=0; j<10; j++) {
            zomb::ZombieCharacter *z = new zomb::ZombieCharacter();
            std::cout << z->toString() << "\n";
    }
    #endif
    #if 0
    zomb::ZombieCharacter *z1 = new zomb::ZombieCharacter();
    zomb::ZombieCharacter *z2 = new zomb::ZombieCharacter();
    std::cout << "a1\n";
    z1->assignObject(z2);
    std::cout << "a2\n";
    z1->assignObject(nail);
    #endif


    #if 0
    blieng::Wallclock *timer = new blieng::Wallclock(new blieng::Item("time"));
    timer->addProducer(new blieng::Item("ironmine"));
    timer->addProducer(new blieng::Item("worker"));
    timer->addProducer(new blieng::Item("worker"));
    timer->addProducer(new blieng::Item("worker"));
    timer->addProducer(new blieng::Item("worker"));
    timer->addProducer(new blieng::Item("iron"));
    timer->addProducer(new blieng::Item("nail"));
    timer->addProducer(new blieng::Item("wood"));
    timer->addProducer(new blieng::Item("wood"));
    timer->addProducer(new blieng::Item("box"));
    timer->forward();
    timer->forward();
    timer->forward();
    timer->forward();
    timer->forward();
    /*
    */
    #endif

    #if 0
    ui::LoadingScreen *ss = new ui::LoadingScreen();
    ss->setMinimumSize(800, 600);
    ss->show();
    QObject::connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));
    return app.exec();
    #endif

    #if 0
    ui::StartScreen *ss = new ui::StartScreen();
    ss->setMinimumSize(800, 600);
    ss->show();
    QObject::connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));
    return app.exec();
    #endif

    //blieng::Maps *map1 = new blieng::Maps("world1");
    #if 0
    ui::MapScreen *game = new ui::MapScreen("world1");
    game->show();
    QObject::connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));
    
    return app.exec();
    #endif

    if (!zomb::initializeConfiguration()) {
            return 1;
    }

    ui::GameScreen *game = new ui::GameScreen();
    game->show();
    QObject::connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));
    
    return app.exec();

    #if 0
    ui::GenerateCharacter *cg = new ui::GenerateCharacter();
    cg->show();
    QObject::connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));
    
    return app.exec();
    #else
    return 0;
    #endif
}
