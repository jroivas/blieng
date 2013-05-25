#include "worldclock.h"
#include "blieng/bliobject.h"
#include "blieng/configure.h"

using zomb::WorldClock;

WorldClock::WorldClock() : QObject()
{
	timer.setInterval(blieng::Configure::getInstance()->getUIntValue("clock"));
	timer_background.setInterval(blieng::Configure::getInstance()->getUIntValue("clock_background"));
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
	unsigned int freq = blieng::Configure::getInstance()->getUIntValue("random_freq");
	unsigned int prob = blieng::Configure::getInstance()->getUIntValue("random_prob");
	
	// This should not generate massive amount of ticks
	int r1 = blieng::BliObject::getRandomInt(0, freq);
	int r2 = blieng::BliObject::getRandomInt(0, prob);
	if (r1 == r2) {
		if (blieng::Configure::getInstance()->getBoolValue("debug"))  {
			QDateTime now_time = QDateTime::currentDateTime();
			qDebug() << "rand" << last_time.msecsTo(now_time);
			last_time = now_time;
		}
		emit randomTick();
	}
	emit backgroundTick();
}
