//#include <QtWidgets>
#include "digitalclock.h"
#include <QTimer>
#include <QTime>


DigitalClock::DigitalClock(QWidget *parent)
    : QLCDNumber(parent), stop_time_(0)
{
    setSegmentStyle(Filled);

    timer_.setSingleShot(false);
    connect(&timer_, SIGNAL(timeout()), this, SLOT(showTime()));
    start();
}



DigitalClock::~DigitalClock()
{
    timer_.stop();
}



void DigitalClock::start(int time)
{
    stop_time_ = time;

    time_.start();
    showTime();
    timer_.start(1000);
}



void DigitalClock::stop()
{
    timer_.stop();
    if (stop_time_ == 0)
        stop_time_ = time_.elapsed();
}

int DigitalClock::getTime()
{
    return time_.elapsed();
}



void DigitalClock::showTime()
{
    QTime t(0, 0);

    int diff = abs(stop_time_ - time_.elapsed());

    if ( diff <= 0 && stop_time_ != 0)
    {
        stop();
    }

    t = t.addMSecs( diff );

    QString text = t.toString("mm:ss");

    display(text);
}

