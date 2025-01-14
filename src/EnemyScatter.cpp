#include <cmath>
#include "include/EnemyScatter.h"
#include "include/EnemyBullet2.h"

#include <QDebug>
#define PI 3.1415926

EnemyScatter::EnemyScatter(QTimer *t, QGraphicsScene *s, Player *p, QGraphicsPixmapItem *e, int x, int y) :
  Enemy(t, s, p, e),
  moveStepCounter(0),
  width(100),
  height(50),
  iniPosX(x),
  iniPosY(y),
  iniHP(300),
  moveDir(-1)
{
    setAttackSpeed(250);
    connect(getAttackSpeedTimer(), SIGNAL(timeout()), this, SLOT(normalAttack()));

    setBulletSpeed(10);

    e->setPos(x, -height);

    setIniHP(iniHP);
    getHPBar()->setFixedWidth(100);
    setExp(5);

    connect(getMoveTimer(), SIGNAL(timeout()), this, SLOT(timeCalculation()));

    connect(getTick(), SIGNAL(timeout()), this, SLOT(isDead()));
}

EnemyScatter::~EnemyScatter() {}

bool EnemyScatter::damageDetermination(Bullet *b)
{
    if (getIsInvinsible()) return false;
    
    int d = b->getDamage();

    for (QGraphicsItem *i : getScene()->collidingItems(b)) {
        if (i == this->getEnemy()) {
            setHP(getHP() - d);
            return true;
        }
    }
    return false;
}

void EnemyScatter::isDead()
{
    if (getHP() > 0) return;

    getPlayer()->addExp(getExp());

    moveStepCounter = 3;
    disconnect(getTick(), SIGNAL(timeout()), this, SLOT(isDead()));
    disconnect(getAttackSpeedTimer(), SIGNAL(timeout()), this, SLOT(normalAttack()));
    disconnect(getMoveTimer(), SIGNAL(timeout()), this, SLOT(timeCalculation()));
    getScene()->removeItem(getHPBarItem());
}

void EnemyScatter::setPosition()
{
    switch (moveStepCounter) {
        case 0: // enter
            getEnemy()->setPos(getEnemy()->x(), getEnemy()->y() + 0.5);
            if (getEnemy()->y() >= iniPosY) {
                moveStepCounter++;
                setIsInvinsible(false);
                getPlayer()->setDoAttack(true);
                setDoAttack(true);
                getAttackSpeedTimer()->start(getAttackSpeed());
                getBulletSpeedTimer()->start(getBulletSpeed());
                // getMoveTimer()->start(10000);
            }
        break;
        case 1: // attack
            if (getHP() != iniHP)
                getHPBarItem()->setPos(getEnemy()->x(), getEnemy()->y());
        break;
        case 2: // horizontal move
            // getEnemy()->setPos(getEnemy()->x() + moveDir, getEnemy()->y());
            // if (getHP() != iniHP)
            //     getHPBarItem()->setPos(getEnemy()->x(), getEnemy()->y());
            // if (getEnemy()->x() == iniPosX - 100 || getEnemy()->x() == iniPosX + 100)
            //     moveDir = -moveDir;
            // if (getEnemy()->x() == iniPosX - 100 || getEnemy()->x() == iniPosX + 100 || getEnemy()->x() == iniPosX) {
            //     moveStepCounter--;
            //     getMoveTimer()->start(10000);
            //     setDoAttack(true);
            // }
        break;
        case 3: // dead
            getEnemy()->setPos(getEnemy()->x(), getEnemy()->y() - 2);
            if (getEnemy()->y() <= -height) {
                getScene()->removeItem(getEnemy());
                disconnect(getTick(), SIGNAL(timeout()), this, SLOT(setPosition()));
                
                // std::vector<Enemy *> &e = getPlayer()->getEnemy();
                // e.erase(std::remove(e.begin(), e.end(), this), e.end());
                // delete this;
            }
        break;
    }
}

void EnemyScatter::normalAttack()
{
    if (!getDoAttack()) return;

    int R = 10;
    int angle[5] = { -60, -30, 0, 30, 60 };

    for (int i = 0; i < 5; ++i) {
        double xDir = std::sin(angle[i] * PI / 180.0);
        double yDir = std::cos(angle[i] * PI / 180.0);
        EnemyBullet2 *temp = new EnemyBullet2(getPlayer(), this, R / 2, xDir, yDir);
        temp->setPixmap(QPixmap(":/img/enemy_bullet_2.png").scaled(R, R));
        temp->setPos(getEnemy()->x() + width / 2 + (width / 2) * xDir - R / 2, getEnemy()->y() + height * yDir);
        temp->connect(getBulletSpeedTimer(), SIGNAL(timeout()), temp, SLOT(horizontalFly()));
        temp->connect(getBulletSpeedTimer(), SIGNAL(timeout()), temp, SLOT(verticalFly()));
        getScene()->addItem(static_cast<QGraphicsPixmapItem *>(temp));
    }

    // // for damage determination
    // int angleDD[5] = { -45, -15, 0, 15, 45 };
    // for (int i = 0; i < 5; ++i) {
    //     double xDir = std::sin(angleDD[i] * PI / 180.0);
    //     double yDir = std::cos(angleDD[i] * PI / 180.0);
    //     EnemyBullet2 *temp = new EnemyBullet2(getPlayer(), this, R / 2, xDir, yDir);
    //     temp->setPixmap(QPixmap(":/img/enemy_bullet_2.png").scaled(R, R));
    //     temp->setPos(getEnemy()->x() + width / 2 + (width / 2) * xDir - R / 2, getEnemy()->y() + height * yDir);
    //     temp->connect(getBulletSpeedTimer(), SIGNAL(timeout()), temp, SLOT(horizontalFly()));
    //     temp->connect(getBulletSpeedTimer(), SIGNAL(timeout()), temp, SLOT(verticalFly()));
    //     getScene()->addItem(static_cast<QGraphicsPixmapItem *>(temp));
    // }
}

void EnemyScatter::timeCalculation()
{
    // switch (moveStepCounter) {
    //     case 1:
    //         getMoveTimer()->stop();
    //         moveStepCounter++;
    //         setDoAttack(false);
    //     break;
    // }
}