#include <cmath>
#include <vector>
#include "include/EnemyBullet2.h"

#include <QDebug>

EnemyBullet2::EnemyBullet2(Player *p, Enemy *e, int r, double xDir, double yDir) : 
  Bullet(yDir, xDir),
  player(p),
  enemy(e),
  radius(r)
{
    connect(enemy->getTick(), SIGNAL(timeout()), this, SLOT(collisionDetection()));
}

EnemyBullet2::~EnemyBullet2() {}

void EnemyBullet2::collisionDetection()
{
    int x = this->pos().x();
    int y = this->pos().y();

    if (player->getIsInvinsible()) return;

    int centerX = player->getPlayer()->x() + 42;
    int centerY = player->getPlayer()->y() + 67;
    int r = 9;

    double distance = std::sqrt(std::pow(((x + radius) - centerX), 2) + std::pow(((y + radius) - centerY), 2));

    if (distance <= r + radius) {
        player->dropHP();;
        disconnect(enemy->getBulletSpeedTimer(), SIGNAL(timeout()), this, SLOT(verticalFly()));
        disconnect(enemy->getTick(), SIGNAL(timeout()), this, SLOT(collisionDetection()));
        this->scene()->removeItem(this);
        delete this;
    }
}
