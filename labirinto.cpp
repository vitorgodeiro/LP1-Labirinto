#include "labirinto.h"
#include <string.h>
#include <cstdlib>     /* srand, rand */
#include <ctime>       /* time */
#include <queue>
#include <vector>

#define DEBUG
#include "debug.h"




Labirinto::Labirinto(int rows, int columns) : rows_(rows), columns_(columns)
{
    TRACE(("Labirinto::Labirinto(%d, %d)\n", rows, columns));

    std::srand( (unsigned int)std::time(NULL));

    matrix_ = new CellInfo[rows * columns];
    build();

}



Labirinto::~Labirinto()
{
    TRACE(("Labirinto::~Labirinto()\n"));

    delete[] matrix_;
}



bool Labirinto::build()
{
    std::vector<int> indices;
    int i, indice, walls;
    Wall wall;
    int total = rows() * columns();

    for (i=0; i < total; ++i)
        indices.push_back(i);

    reset();

    while ( !indices.empty() )
    {
        indice = rand() % indices.size();
        i = indices[indice];

        walls = getDestroyableWalls(matrix_[ i ]);

        if ( walls == 0 )
        {
//            TRACE(("Celula [%d,%d] nao tem paredes destruiveis.\n",
//                   toCell(matrix_[i]).row, toCell(matrix_[i]).column));
            indices.erase( indices.begin() +  indice);
            continue;
        }

        wall = pickWallRandomly(walls);
//        TRACE(("pickWallRandomly(%d) retornou %d\n", walls, wall));

        traverse(*getNeighbor(matrix_[i], wall), setIdCallback, &matrix_[i].id);
        destroyWall(matrix_[i], wall);

    }

    destroyWall(*getCellInfo(Cell(0,0)), WestWall);
    destroyWall(*getCellInfo(Cell(rows()-1,columns()-1)), EastWall);

    return true;
}



std::vector<Labirinto::Cell> Labirinto::getPath(const Labirinto::Cell &c1, const Labirinto::Cell &c2) const
{
    std::vector<CellInfo*> path;
    CellInfo *n, *exit = getCellInfo(c2);
    int i;
    const Wall wall[4] = {NorthWall, EastWall, SouthWall, WestWall};

    for (i=0; i < rows_*columns_; ++i)
        matrix_[i].visited = false;

    path.push_back( getCellInfo(c1) );
    path.back()->visited = true;

    while ( path.back() != exit )
    {
        for (i=0; i<4; ++i)
        {
            n = getNeighbor(*path.back(), wall[i]);

            if ( n && (path.back()->destroyed & wall[i]) && !n->visited )
            {
                path.push_back(n);
                n->visited = true;
                break;
            }
        }
        if (i==4)
            path.pop_back();

    }

    std::vector<Cell> ret;

    for (i=0; i < (int)path.size(); ++i)
        ret.push_back(toCell(*path[i]));

    return ret;
}




int Labirinto::getDestroyedWalls(const Labirinto::Cell &cell) const
{
    return getCellInfo(cell)->destroyed;
}



bool Labirinto::isCellValid(const Labirinto::Cell &cell) const
{
    return cell.row >= 0 && cell.column >= 0 &&
            cell.row < rows_ && cell.column < columns_;
}



std::vector<Labirinto::Cell> Labirinto::getNeighbors(
        const Labirinto::Cell &cell, int walls) const
{
    std::vector<Cell> ret;
    CellInfo *c = getCellInfo(cell);
    const Wall wall_list[4] = {NorthWall, EastWall, SouthWall, WestWall};
    int i;

    for (i=0; i<4; ++i)
    {
        if ( walls & wall_list[i] )
        {
            CellInfo *n = getNeighbor(*c, wall_list[i]);
            if (n)
                ret.push_back( toCell(*n) );
        }
    }

    return ret;
}



bool Labirinto::isWallShared(Labirinto::Wall wall, const Labirinto::Cell &cell,
                             Labirinto::Cell *other_cell) const
{
    switch (wall)
    {
        case NorthWall:
            if (cell.row == 0)
                return false;
            else if (other_cell)
            {
                other_cell->row = cell.row - 1;
                other_cell->column = cell.column;
            }
        break;

        case EastWall:
            if ( cell.column == columns_ - 1 )
                return false;
            else if (other_cell)
            {
                other_cell->row = cell.row;
                other_cell->column = cell.column + 1;
            }
        break;

        case SouthWall:
            if (cell.row == rows_ - 1)
                return false;
            else if (other_cell)
            {
                other_cell->row = cell.row + 1;
                other_cell->column = cell.column;
            }
        break;

        case WestWall:
            if ( cell.column == 0 )
                return false;
            else if (other_cell)
            {
                other_cell->row = cell.row;
                other_cell->column = cell.column - 1;
            }
        break;

        default:
            return false;
        break;
    }

    return true;
}


Labirinto::Wall Labirinto::getOppositeWall(Labirinto::Wall wall)
{
    switch (wall)
    {
        case NorthWall: return SouthWall;
        case EastWall:  return WestWall;
        case SouthWall: return NorthWall;
        case WestWall:  return EastWall;
        default:        return None;
    }

    return None;
}



void Labirinto::reset()
{
    TRACE(("Labirinto::reset()\n"));

    int i, num = rows_ * columns_;

    for (i=0; i<num; ++i)
    {
        matrix_[i].destroyed = 0;
        matrix_[i].id = i;
    }
}



Labirinto::CellInfo *Labirinto::getCellInfo(const Labirinto::Cell &cell) const
{
    return &matrix_[ offset(cell.row, cell.column) ];
}



Labirinto::Wall Labirinto::pickWallRandomly(int walls)
{
    int i = 0;
    Wall w[4];

    if (walls & NorthWall)
        w[i++] = NorthWall;

    if (walls & EastWall)
        w[i++] = EastWall;

    if (walls & SouthWall)
        w[i++] = SouthWall;

    if (walls & WestWall)
        w[i++] = WestWall;

    if (i==0)
        return None;
    else
        return w[ rand()%i ];
}



bool Labirinto::setIdCallback(Labirinto::CellInfo &ci, void *param)
{
    ci.id = *(int*)param;

    return false;
}



bool Labirinto::findPathCallback(Labirinto::CellInfo &c, void *param)
{

    if ( &c == (CellInfo*)param )
        return true;


    return false;
}



int Labirinto::traverse(Labirinto::CellInfo &root,
                        bool (*cb)(Labirinto::CellInfo &, void *), void *arg)
{
    std::queue<CellInfo*> fila;
    CellInfo *ci, *n;
    int i, ret = 1;
    const Wall wall[4] = {NorthWall, EastWall, SouthWall, WestWall};

    for (i=0; i < rows_*columns_; ++i)
        matrix_[i].visited = false;

    fila.push(&root);

    while ( !fila.empty() )
    {
        ci = fila.front();

        fila.pop();
        ci->visited = true;

        if ( cb(*ci, arg) )
            break;

        for (i=0; i<4; ++i)
        {
            n = getNeighbor(*ci, wall[i]);

            if ( n && (ci->destroyed & wall[i]) && !n->visited )
            {
                fila.push(n);
            }
        }
    }

    return ret;
}



Labirinto::CellInfo *Labirinto::getNeighbor(const CellInfo &ci,
        Labirinto::Wall wall) const
{
    int ofs = &ci - matrix_;

    switch (wall)
    {
        case NorthWall:
            if (ofs >= columns_)
                return matrix_ + (ofs - columns_);
        break;

        case EastWall:
            if ((ofs+1) % columns_ != 0 )
                return matrix_ + (ofs+1);
        break;

        case SouthWall:
            if (ofs < columns_ * (rows_ - 1) )
                return matrix_ + (ofs + columns_);
        break;

        case WestWall:
            if (ofs % columns_ != 0 )
                return matrix_ + (ofs-1);
        break;

        default:
            return NULL;
        break;
    }

    return NULL;
}



void Labirinto::destroyWall(CellInfo &ci, Labirinto::Wall wall)
{
    ci.destroyed |= wall;

    CellInfo* n = getNeighbor(ci, wall);
    if (n)
        n->destroyed |= getOppositeWall(wall);
}



Labirinto::Cell Labirinto::toCell(const CellInfo &ci) const
{
    return Cell( (&ci - matrix_) / columns(), (&ci - matrix_) % columns() );
}



int Labirinto::getDestroyableWalls(const CellInfo &ci) const
{
    const Wall wall[4] = {NorthWall, EastWall, SouthWall, WestWall};
    int i, ret = 0;
    CellInfo* n;

    for (i=0; i<4; ++i)
    {
        n = getNeighbor(ci, wall[i]);

        if ( !(ci.destroyed & wall[i]) && n && ci.id != n->id )
           ret |= wall[i];
    }

    return ret;
}
