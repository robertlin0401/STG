#include "include/PlayerBullet.h"

#include <QDebug>

PlayerBullet::PlayerBullet(Player *p, std::vector<Enemy *> &e, int d, int s, int w, int h) : 
  Bullet(-1, 0, d, w, h),
  player(p),
  enemy(e),
  skillProgressOnHit(s)
{
    connect(player->getTick(), SIGNAL(timeout()), this, SLOT(collisionDetection()));
}

PlayerBullet::~PlayerBullet() {}

void PlayerBullet::collisionDetection()
{
    for (Enemy *e : enemy) {
        if (e->damageDetermination(this)) {
            player->addSkillProgress(skillProgressOnHit);
            this->scene()->removeItem(this);
            disconnect(player->getBulletSpeedTimer(), SIGNAL(timeout()), this, SLOT(verticalFly()));
            disconnect(player->getTick(), SIGNAL(timeout()), this, SLOT(collisionDetection()));
            delete this;
            break;
        }
    }
}
