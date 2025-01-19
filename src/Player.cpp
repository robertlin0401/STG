#include "include/Player.h"
#include "include/Enemy.h"

#include <QDebug>

Player::Player(QTimer *t, QGraphicsScene *s, std::vector<Enemy *> &e, QGraphicsPixmapItem *p, int x, int y) :
  tick(t),
  scene(s),
  enemy(e),
  player(p),
  skillTimer(new QTimer),
  invinsibleTimer(new QTimer),
  skillProgress(0),
  lv(1), exp(0)
{
    isPause = false;
    setDoAttack(false);
    setIsInvinsible(false);
    scene->addItem(player);
    player->setPos(x, y);
    connect(tick, SIGNAL(timeout()), this, SLOT(setPosition()));
    connect(tick, SIGNAL(timeout()), this, SLOT(bodyCollisionDetection()));
    connect(invinsibleTimer, SIGNAL(timeout()), this, SLOT(endInvinsibleAnimation()));
}

Player::~Player() {}

void Player::pause()
{
    remainingTime[0] = getAttackSpeedTimer()->remainingTime();
    getAttackSpeedTimer()->stop();

    remainingTime[1] = getBulletSpeedTimer()->remainingTime();
    getBulletSpeedTimer()->stop();

    remainingTime[2] = skillTimer->remainingTime();
    skillTimer->stop();

    remainingTime[3] = invinsibleTimer->remainingTime();
    invinsibleTimer->stop();
}

void Player::resume()
{
    startGame();

    if (remainingTime[0] <= 0) {
        getAttackSpeedTimer()->start(getAttackSpeed());
    } else {
        connect(getAttackSpeedTimer(), SIGNAL(timeout()), this, SLOT(resetAttackSpeedTimer()));
        getAttackSpeedTimer()->start(remainingTime[0]);
    }

    if (remainingTime[1] <= 0) {
        getBulletSpeedTimer()->start(getBulletSpeed());
    } else {
        connect(getBulletSpeedTimer(), SIGNAL(timeout()), this, SLOT(resetBulletSpeedTimer()));
        getBulletSpeedTimer()->start(remainingTime[1]);
    }

    if (remainingTime[2] > 0)
        skillTimer->start(remainingTime[2]);

    if (remainingTime[3] > 0)
        invinsibleTimer->start(remainingTime[3]);
}

void Player::resetAttackSpeedTimer()
{
    disconnect(getAttackSpeedTimer(), SIGNAL(timeout()), this, SLOT(resetAttackSpeedTimer()));
    getAttackSpeedTimer()->start(getAttackSpeed());
}

void Player::resetBulletSpeedTimer()
{
    disconnect(getBulletSpeedTimer(), SIGNAL(timeout()), this, SLOT(resetBulletSpeedTimer()));
    getBulletSpeedTimer()->start(getBulletSpeed());
}

void Player::dropHP()
{
    HP--;
    setIsInvinsible(true);
    invinsibleTimer->start(3000);
}

void Player::addSkillProgress(int p)
{
    skillProgress = (skillProgress + p >= 100)? 100: skillProgress + p;
}

void Player::addExp(int e)
{
    exp += e;
    if (lv < 10 && exp >= lvUpExp[lv]) {
        exp -= lvUpExp[lv];
        lv++;
    }
}

void Player::bodyCollisionDetection()
{
    if (getIsInvinsible()) return;

    for (QGraphicsItem *i : scene->collidingItems(player))
        for (Enemy *e : enemy)
            if (i == e->getEnemy()) {
                dropHP();
                e->setHP(e->getHP() - 50);
            }
}

void Player::endInvinsibleAnimation()
{
    player->setOpacity(1);
    setIsInvinsible(false);
    invinsibleTimer->stop();
}
