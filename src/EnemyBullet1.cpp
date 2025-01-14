#include <cmath>
#include <vector>
#include "include/EnemyBullet1.h"

#include <QDebug>

EnemyBullet1::EnemyBullet1(Player *p, Enemy *e, int w, int h) : 
  Bullet(1, 0),
  player(p),
  enemy(e),
  width(w),
  height(h)
{
    connect(enemy->getTick(), SIGNAL(timeout()), this, SLOT(collisionDetection()));
}

EnemyBullet1::~EnemyBullet1() {}

void EnemyBullet1::collisionDetection()
{
    bool isCollision = false;
    int x = this->pos().x();
    int y = this->pos().y();

    if (player->getIsInvinsible()) return;

    int centerX = player->getPlayer()->x() + 42;
    int centerY = player->getPlayer()->y() + 67;
    int r = 9;

    auto calCollisionPoint = [=](int px, int py) -> std::vector<double>
    {
        double distance = std::sqrt(std::pow((px - centerX), 2) + std::pow((py - centerY), 2));
        double unitVector[2] = { (px - centerX) / distance, (py - centerY) / distance };
        return std::vector<double> { centerX + unitVector[0] * r, centerY + unitVector[1] * r };
    };

    if (y >= centerY) { // bottom

        if (x >= centerX) { // bottom-right
            std::vector<double> p = calCollisionPoint(x, y);
            if (x <= p[0] && y <= p[1]) {
                player->dropHP();;
                isCollision = true;
            }
        } else if (x < centerX && x + width >= centerX) { // bottom
            std::vector<double> p = calCollisionPoint(x, y);
            if (x >= p[0] && y <= p[1]) {
                player->dropHP();;
                isCollision = true;
            }
        } else { // bottom-left
            std::vector<double> p = calCollisionPoint(x + width, y);
            if (x + width >= p[0] && y <= p[1]) {
                player->dropHP();;
                isCollision = true;
            }
        }

    } else if (y < centerY && y + height >= centerY) { // side

        if (x >= centerX) { // right
            if (x <= centerX + r) {
                player->dropHP();;
                isCollision = true;
            }
        } else { // left
            if (x + width >= centerX - r) {
                player->dropHP();;
                isCollision = true;
            }
        }

    } else { // top

        if (x >= centerX) { // top-right
            std::vector<double> p = calCollisionPoint(x, y + height);
            if (x <= p[0] && y + height >= p[1]) {
                player->dropHP();;
                isCollision = true;
            }
        } else if (x <= centerX && x + width >= centerX) { // top
            std::vector<double> p = calCollisionPoint(x, y + height);
            if (x >= p[0] && y + height >= p[1]) {
                player->dropHP();;
                isCollision = true;
            }
        } else { // top-left
            std::vector<double> p = calCollisionPoint(x + width, y + height);
            if (x + width >= p[0] && y + height >= p[1]) {
                player->dropHP();;
                isCollision = true;
            }
        }
        
    }

    if (isCollision) {
        disconnect(enemy->getBulletSpeedTimer(), SIGNAL(timeout()), this, SLOT(verticalFly()));
        disconnect(enemy->getTick(), SIGNAL(timeout()), this, SLOT(collisionDetection()));
        this->scene()->removeItem(this);
        delete this;
    }
}
