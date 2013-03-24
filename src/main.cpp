#include <QtCore>
#include <QtGui>

#include <iostream>
#include "ui/generate_character.h"
#include "blieng/item.h"
#include "blieng/zombie_character.h"
#include "blieng/wallclock.h"

int main(int argc, char **argv)
{
	QApplication app(argc, argv);

	/*
	for (int j=0; j<5; j++) {
		blieng::Item *i = new blieng::Item();
		std::cout << i->toString() << "\n";
	}
	*/
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

	for (int j=0; j<10; j++) {
		blieng::ZombieCharacter *z = new blieng::ZombieCharacter();
		std::cout << z->toString() << "\n";
	}


	blieng::Wallclock *timer = new blieng::Wallclock(new blieng::Item("time"));
	timer->addProducer(new blieng::Item("ironmine"));
	timer->addProducer(new blieng::Item("worker"));
	timer->addProducer(new blieng::Item("worker"));
	timer->addProducer(new blieng::Item("iron"));
	timer->addProducer(new blieng::Item("nail"));
	timer->addProducer(new blieng::Item("wood"));
	timer->addProducer(new blieng::Item("box"));
	timer->forward();

	#if 0
	GenerateCharacter *cg = new GenerateCharacter();
	cg->show();
	QObject::connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));
	
	return app.exec();
	#else
	return 0;
	#endif
}
