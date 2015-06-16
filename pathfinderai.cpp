#include "pathfinderai.h"
#include "labirinto.h"
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <QMutex>
#define DEBUG
#include "debug.h"

PathFinderAI::PathFinderAI() : mutex_(QMutex::Recursive)
{
    srand( (unsigned int)time(NULL) );
    connect( &timer_, SIGNAL(timeout()), this, SLOT(update()) );
}



bool PathFinderAI::stop()
{
    timer_.stop();
    return true;
}



bool PathFinderAI::setIQ(int iq)
{
    if (iq < 1 || iq > 200)
        return false;

    iq_ = iq;
    memory_size_ = (iq+2) ;
    return true;
}



Labirinto::Cell PathFinderAI::getCurrentCell()
{
    Labirinto::Cell ret;

    mutex_.lock();
    ret = current_cell_;
    mutex_.unlock();

    return ret;
}



PathFinderAI::~PathFinderAI()
{
    stop();
}



bool PathFinderAI::start()
{
    stop();
    setCurrentCell(initial_cell_);
    memory_.clear();
    timer_.start(100);

    return true;
}



void PathFinderAI::update()
{
    std::vector<Labirinto::Cell> path;
    int i, j;
    Labirinto::Cell current_cell;
    std::vector<Labirinto::Cell> cells;

    path = labirinto_->getPath(current_cell_, final_cell_);

    if ( path.size() < 2 )
        return;

    // a proxima celula sera uma das que estao em cells
    cells = labirinto_->getNeighbors( path[0],
                                      labirinto_->getDestroyedWalls(path[0]) );

    // remove a celula certa e as que ja estao na memoria
    for (i=0; i < (int)cells.size(); ++i)
    {
        if ( cells[i] == path[1] )
        {
            cells.erase( cells.begin() + i );
            --i;
            continue;
        }

        for (j=0; j < (int)memory_.size(); ++j)
        {
            if ( cells[i] == memory_[j] )
            {
                cells.erase( cells.begin() + i );
                --i;
                break;
            }
        }
    }

    if ( cells.size() == 0 )
        current_cell = path[1];
    else
    {
        int p = (int)( iq_ - path.size() );

        if (p < 1)
            p = 1;
        else if (p>100)
            p = 100;

        if ( p >= (rand()%100 + 1) )
            current_cell = path[1];
        else
        {
            i = rand() % cells.size();
            current_cell = cells[ i ];
        }
    }

    mutex_.lock();
    current_cell_ = current_cell;
    mutex_.unlock();

    if ( (int)memory_.size() == memory_size_ )
    {
        memory_.erase( memory_.begin() );
        TRACE(("memory limite alcancado %d\n", memory_.size() ));
    }
    memory_.push_back( path[0] );

    i = rand()%600 + path.size();

    if (i < 400)
    i = 400;

    timer_.start(i);
}

