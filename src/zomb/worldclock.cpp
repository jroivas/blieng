#include "worldclock.h"
#include "blieng/bliobject.h"

using zomb::WorldClock;

WorldClock::WorldClock()
{
	timer.setInterval(1000);
	timer_background.setInterval(10);
	connect(&timer, SIGNAL(timeout()), this, SLOT(tick()));
	connect(&timer_background, SIGNAL(timeout()), this, SLOT(tickBackground()));
	last_time = QDateTime::currentDateTime();

	start();
}

void WorldClock::pause()
{
	timer.stop();
	timer_background.stop();
}

void WorldClock::start()
{
	timer.start();
	timer_background.start();
}

void WorldClock::tick()
{
	emit clockTick();
}

void WorldClock::tickBackground()
{
	// This should not generate massive amount of ticks
	// TODO Configuration
	int r1 = blieng::BliObject::getRandomInt(0, 250);
	int r2 = blieng::BliObject::getRandomInt(0, 250);
	if (r1 == r2) {
		QDateTime now_time = QDateTime::currentDateTime();
		qDebug() << "rand" << last_time.msecsTo(now_time);
		last_time = now_time;
		emit randomTick();
	}
	emit backgroundTick();
}
