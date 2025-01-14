#include "include/Enemy.h"
#include "include/DefaultPlayer.h"

#include <QDebug>

Enemy::Enemy(QTimer *t, QGraphicsScene *s, Player *p, QGraphicsPixmapItem *e) :
  tick(t),
  scene(s),
  player(p),
  enemy(e),
  HPBar(new QProgressBar),
  HPBarItem(new QGraphicsProxyWidget),
  moveTimer(new QTimer)
{
    connect(tick, SIGNAL(timeout()), this, SLOT(setPosition()));
    scene->addItem(enemy);

    HPBar->setTextVisible(false);
    HPBar->setStyleSheet("\
        QProgressBar {\
            height: 5px;\
            background: transparent;\
        }\
        QProgressBar::chunk {\
            background: red;\
            width: 1px;\
        }\
    ");
    HPBarItem = scene->addWidget(HPBar);
    HPBarItem->setPos(0, -100);
    HPBarItem->setZValue(100);
    setIsInvinsible(true);
}

Enemy::~Enemy() {}

void Enemy::pause()
{
    remainingTime[0] = getAttackSpeedTimer()->remainingTime();
    getAttackSpeedTimer()->stop();

    remainingTime[1] = getBulletSpeedTimer()->remainingTime();
    getBulletSpeedTimer()->stop();

    remainingTime[2] = moveTimer->remainingTime();
    moveTimer->stop();
}

void Enemy::resume()
{
    if (getHP() <= 0) return;
    
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
        moveTimer->start(remainingTime[2]);
}

void Enemy::resetAttackSpeedTimer()
{
    disconnect(getAttackSpeedTimer(), SIGNAL(timeout()), this, SLOT(resetAttackSpeedTimer()));
    getAttackSpeedTimer()->start(getAttackSpeed());
}

void Enemy::resetBulletSpeedTimer()
{
    disconnect(getBulletSpeedTimer(), SIGNAL(timeout()), this, SLOT(resetBulletSpeedTimer()));
    getBulletSpeedTimer()->start(getBulletSpeed());
}
