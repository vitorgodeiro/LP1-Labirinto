#include "player.h"
#include "color.h"
#include <string>
#include "labirinto.h"


Player::Player(const std::string &name, const Color& color) :
    name_(name), cell_(0,0), color_(color)
{

}



Player::~Player()
{

}



bool Player::setCell(const Labirinto::Cell &cell)
{
    cell_ = cell;

    return true;
}

