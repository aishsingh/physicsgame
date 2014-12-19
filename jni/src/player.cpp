#include "player.h"

Player::Player(float x, float y, float width, float height, Rend_player* rend) : Object(x,y,width,height) {
    _rend = rend;
}

void Player::setBasePoint(Point2D point) {
    _base = point;
}
