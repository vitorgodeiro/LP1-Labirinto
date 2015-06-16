/**
 * Projeto: Maze
 * Versao:  1.0b
 * Autores: Leno
 *          Raimundo
 */

#ifndef DIGITALCLOCK_H
#define DIGITALCLOCK_H


#include <QLCDNumber>
#include <QTimer>
#include <QTime>

class DigitalClock : public QLCDNumber
{
    Q_OBJECT

public:
    DigitalClock(QWidget *parent = 0);
    ~DigitalClock();

    void start(int time = 0);
    void stop();
    int getTime();


private slots:
    void showTime();

private:
    QTimer timer_;
    QTime time_;
    int stop_time_;
};


#endif // DIGITALCLOCK_H
