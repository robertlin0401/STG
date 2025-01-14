#include <cmath>
#include "include/EnemyNormal.h"
#include "include/EnemyBullet1.h"

#include <QDebug>

EnemyNormal::EnemyNormal(QTimer *t, QGraphicsScene *s, Player *p, QGraphicsPixmapItem *e, int x, int y) :
  Enemy(t, s, p, e),
  moveStepCounter(0),
  width(84),
  height(90),
  iniPosX(x),
  iniPosY(y),
  iniHP(120)
{
    setAttackSpeed(150);
    connect(getAttackSpeedTimer(), SIGNAL(timeout()), this, SLOT(normalAttack()));

    setBulletSpeed(15);

    e->setPos(x, -height);

    setIniHP(iniHP);
    getHPBar()->setFixedWidth(85);
    setExp(10);

    moveDir = (iniPosX >= 500)? -1: 1;
    rotateCounter = 0;
    connect(getMoveTimer(), SIGNAL(timeout()), this, SLOT(timeCalculation()));

    connect(getTick(), SIGNAL(timeout()), this, SLOT(isDead()));
}

EnemyNormal::~EnemyNormal() {}

bool EnemyNormal::damageDetermination(Bullet *b)
{
    if (getIsInvinsible()) return false;
    int x = b->pos().x();
    int y = b->pos().y();
    int w = b->getWidth();
    int h = b->getHeight();
    int d = b->getDamage();

    int centerX = getEnemy()->x() + 44;
    int centerY = getEnemy()->y() + 21;
    int r = 10;

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
                setHP(getHP() - d);
                return true;
            }
        } else if (x < centerX && x + w >= centerX) { // bottom
            std::vector<double> p = calCollisionPoint(x, y);
            if (x >= p[0] && y <= p[1]) {
                setHP(getHP() - d);
                return true;
            }
        } else { // bottom-left
            std::vector<double> p = calCollisionPoint(x + w, y);
            if (x + w >= p[0] && y <= p[1]) {
                setHP(getHP() - d);
                return true;
            }
        }

    } else if (y < centerY && y + h >= centerY) { // side

        if (x >= centerX) { // right
            if (x <= centerX + r) {
                setHP(getHP() - d);
                return true;
            }
        } else { // left
            if (x + w >= centerX - r) {
                setHP(getHP() - d);
                return true;
            }
        }

    } else { // top

        if (x >= centerX) { // top-right
            std::vector<double> p = calCollisionPoint(x, y + h);
            if (x <= p[0] && y + h >= p[1]) {
                setHP(getHP() - d);
                return true;
            }
        } else if (x <= centerX && x + w >= centerX) { // top
            std::vector<double> p = calCollisionPoint(x, y + h);
            if (x >= p[0] && y + h >= p[1]) {
                setHP(getHP() - d);
                return true;
            }
        } else { // top-left
            std::vector<double> p = calCollisionPoint(x + w, y + h);
            if (x + w >= p[0] && y + h >= p[1]) {
                setHP(getHP() - d);
                return true;
            }
        }
        
    }
    
    return false;
}

void EnemyNormal::isDead()
{
    if (getHP() > 0) return;

    getPlayer()->addExp(getExp());

    moveStepCounter = 3;
    disconnect(getTick(), SIGNAL(timeout()), this, SLOT(isDead()));
    disconnect(getAttackSpeedTimer(), SIGNAL(timeout()), this, SLOT(normalAttack()));
    disconnect(getMoveTimer(), SIGNAL(timeout()), this, SLOT(timeCalculation()));
    getScene()->removeItem(getHPBarItem());
}

void EnemyNormal::setPosition()
{
    switch (moveStepCounter) {
        case 0: // enter
            getEnemy()->setPos(getEnemy()->x(), getEnemy()->y() + 1);
            if (getEnemy()->y() >= iniPosY) {
                moveStepCounter++;
                setIsInvinsible(false);
                getPlayer()->setDoAttack(true);
                setDoAttack(true);
                getAttackSpeedTimer()->start(getAttackSpeed());
                getBulletSpeedTimer()->start(getBulletSpeed());
                getMoveTimer()->start(3000);
            }
        break;
        case 1: // attack
            if (getHP() != iniHP)
                getHPBarItem()->setPos(getEnemy()->x(), getEnemy()->y() - 15);
        break;
        case 2: // horizontal move
            getEnemy()->setPos(getEnemy()->x() + moveDir, getEnemy()->y());
            if (getHP() != iniHP)
                getHPBarItem()->setPos(getEnemy()->x(), getEnemy()->y() - 15);
            if (iniPosX <= 500) {
                if (getEnemy()->x() <= 0 || getEnemy()->x() >= 500 - width)
                    moveDir = -moveDir;
            } else {
                if (getEnemy()->x() <= 500 || getEnemy()->x() >= 1000 - width)
                    moveDir = -moveDir;
            }
        break;
        case 3: // dead
            getEnemy()->setPos(getEnemy()->x() + moveDir, getEnemy()->y() + 2);
            getEnemy()->setRotation(rotateCounter * moveDir);
            rotateCounter += 2;
            if (getEnemy()->y() >= 800) {
                getScene()->removeItem(getEnemy());
                disconnect(getTick(), SIGNAL(timeout()), this, SLOT(setPosition()));
                
                std::vector<Enemy *> &e = getPlayer()->getEnemy();
                e.erase(std::remove(e.begin(), e.end(), this), e.end());
                // delete this;
            }
        break;
    }
}

void EnemyNormal::normalAttack()
{
    if (!getDoAttack()) return;

    int bulletSize[2] = { 4, 15 };
    EnemyBullet1 *temp = new EnemyBullet1(getPlayer(), this, bulletSize[0], bulletSize[1]);
    temp->setPixmap(QPixmap(":/img/enemy_bullet_1.png").scaled(bulletSize[0], bulletSize[1]));
    temp->setPos(getEnemy()->x() + width / 2 - bulletSize[0] / 2 + 1, getEnemy()->y() + height);
    temp->connect(getBulletSpeedTimer(), SIGNAL(timeout()), temp, SLOT(verticalFly()));
    getScene()->addItem(static_cast<QGraphicsPixmapItem *>(temp));

    // // for damage determination
    // for (int i = 1; i <= 10; ++i) {
    //     EnemyBullet1 *tempR = new EnemyBullet1(getPlayer(), this, bulletSize[0], bulletSize[1]);
    //     tempR->setPixmap(QPixmap(":/img/enemy_bullet_1.png").scaled(bulletSize[0], bulletSize[1]));
    //     tempR->setPos(getEnemy()->x() + width / 2 - bulletSize[0] / 2 + 1 + bulletSize[0] * i, getEnemy()->y() + height);
    //     tempR->connect(getBulletSpeedTimer(), SIGNAL(timeout()), tempR, SLOT(verticalFly()));
    //     getScene()->addItem(static_cast<QGraphicsPixmapItem *>(tempR));

    //     EnemyBullet1 *tempL = new EnemyBullet1(getPlayer(), this, bulletSize[0], bulletSize[1]);
    //     tempL->setPixmap(QPixmap(":/img/enemy_bullet_1.png").scaled(bulletSize[0], bulletSize[1]));
    //     tempL->setPos(getEnemy()->x() + width / 2 - bulletSize[0] / 2 + 1 - bulletSize[0] * i, getEnemy()->y() + height);
    //     tempL->connect(getBulletSpeedTimer(), SIGNAL(timeout()), tempL, SLOT(verticalFly()));
    //     getScene()->addItem(static_cast<QGraphicsPixmapItem *>(tempL));
    // }
}

void EnemyNormal::timeCalculation()
{
    switch (moveStepCounter) {
        case 1:
            moveStepCounter++;
            setDoAttack(false);
            getMoveTimer()->start(2000);
        break;
        case 2:
            moveStepCounter--;
            setDoAttack(true);
            getMoveTimer()->start(2000);
        break;
    }
}