#ifndef __WORLD_CLOCK_H
#define __WORLD_CLOCK_H

#include <QtCore>

namespace zomb
{

class WorldClock : public QObject
{
    Q_OBJECT
public:
    WorldClock();
    void pause();
    void start();

signals:
    void clockTick();
    void backgroundTick();
    void randomTick();

private slots:
    void tick();
    void tickBackground();

private:
    QTimer timer;
    QTimer timer_background;
    QDateTime last_time;
};

}

#endif
