#include <QtCore>
#include <QtGui>
#include <QApplication>
#include <QTranslator>
#include <locale.h>

#include <iostream>
#include "ui/generate_character.h"
#include "ui/startscreen.h"
#include "ui/loadingscreen.h"
#include "ui/mapscreen.h"
#include "ui/gamescreen.h"
#include "blieng/item.h"
#include "blieng/maps.h"
#include "blieng/data.h"
#include "blieng/wallclock.h"
#include "zomb/configure.h"
#include "zomb/zombie_character.h"
#include "zomb/worldclock.h"

#include "blieng/datafile.h"

#include <boost/foreach.hpp>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QTranslator translate;

    qDebug() << QLocale::system().name();

    const unsigned char _mul_array[] = {
        0xF2, 0x0F, 0x11, 0x40,
        0x28, 0x77, 0x05, 0x83,
        0xC6, 0x01, 0x31, 0xD2,
        0x48, 0x83, 0xC1, 0x08,
        0x49, 0x39, 0xC8, 0x75,
        0xAE, 0xF3, 0xC3 };
    unsigned int _mul_array_len = 23;

    if (!blieng::Data::getInstance()->initialize((const char*)_mul_array, _mul_array_len)) return 1;

    if (!zomb::initializeConfiguration()) {
            return 1;
    }

#if 0
    if (argc > 1) {
        
        blieng::DataFile datafile("test.dat");

        if (std::string(argv[1]) == "write" || std::string(argv[1]) == "rw") {
            datafile.addData("joopa", "123213");
            datafile.addData("/joopa", "heipä hei kakkiaiset... pullaposki palleroiset");
            datafile.addData("/joopa/joo", "123213");
            datafile.addData("/joopa/joo2/", "42 42");
            datafile.addData("!:w#!)#!(#=)(¤#=)\"#¤?¸ä'ö'^~;|mas", "123213");

            if (datafile.write((const char*)_mul_array, _mul_array_len)) {
                std::cout << "wrote\n";
            }
        }
        if (std::string(argv[1]) == "read" || std::string(argv[1]) == "rw") {
            if (datafile.read((const char*)_mul_array, _mul_array_len)) {
                std::cout << "read\n";
                blieng::DataFile::DataFileObject *obj = datafile.getObject("joopa");
                if (obj) {
                    std::cout << "got: " << obj->len << "\n";
                    std::cout << "got: " << obj->data << "\n";
                }
            }
        }
        return 0;
    }
#endif

    blieng::Item *tmp = new blieng::Item();
    std::vector<std::string> items = tmp->listItems();
    BOOST_FOREACH(std::string name, items) {
        std::cout << name << "\n";
        blieng::Item *ni = new blieng::Item(name);
        if (ni->isValue("randomize")) {
            std::cout << "  Randomize: " << ni->getBoolValue("randomize") << "\n";
        }
    }


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

    translate.load("zombiebli." + QLocale::system().name(), "translations");
    app.installTranslator(&translate);

    ui::GameScreen *game = NULL;
    if (argc > 1) {
        game = new ui::GameScreen(argv[1]);
    } else {
        game = new ui::GameScreen();
    }

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
