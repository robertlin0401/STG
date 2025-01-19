#include <cmath>
#include "include/EnemyInvisible.h"
#include "include/EnemyBullet1.h"

#include <QDebug>

EnemyInvisible::EnemyInvisible(QTimer *t, QGraphicsScene *s, Player *p, QGraphicsPixmapItem *e, int x, int y) :
  Enemy(t, s, p, e),
  moveStepCounter(0),
  width(84),
  height(90),
  iniPosX(x),
  iniPosY(y),
  iniHP(60),
  opacity(0)
{
    setAttackSpeed(150);
    connect(getAttackSpeedTimer(), SIGNAL(timeout()), this, SLOT(normalAttack()));

    setBulletSpeed(15);

    e->setPos(x, -height);
    e->setOpacity(0);

    setIniHP(iniHP);
    getHPBar()->setFixedWidth(85);
    getHPBarItem()->setOpacity(0);
    setExp(15);

    moveDir = (iniPosX >= 500)? -1: 1;
    rotateCounter = 0;
    connect(getMoveTimer(), SIGNAL(timeout()), this, SLOT(timeCalculation()));

    connect(getTick(), SIGNAL(timeout()), this, SLOT(isDead()));
}

EnemyInvisible::~EnemyInvisible() {}

bool EnemyInvisible::damageDetermination(Bullet *b)
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

void EnemyInvisible::isDead()
{
    if (getHP() > 0) return;

    getPlayer()->addExp(getExp());
    getEnemy()->setOpacity(1);

    moveStepCounter = 4;
    disconnect(getTick(), SIGNAL(timeout()), this, SLOT(isDead()));
    disconnect(getAttackSpeedTimer(), SIGNAL(timeout()), this, SLOT(normalAttack()));
    disconnect(getMoveTimer(), SIGNAL(timeout()), this, SLOT(timeCalculation()));
    getScene()->removeItem(getHPBarItem());
}

void EnemyInvisible::setPosition()
{
    double distance;
    switch (moveStepCounter) {
        case 0: // enter
            getEnemy()->setPos(getEnemy()->x(), getEnemy()->y() + 1);
            if (getEnemy()->y() >= iniPosY || getHP() != iniHP) {
                moveStepCounter++;
                setIsInvinsible(false);
            }
        break;
        case 1: // show up
            if (getHP() != iniHP)
                getHPBarItem()->setPos(getEnemy()->x(), getEnemy()->y() - 15);

            opacity += 0.05;
            getEnemy()->setOpacity(opacity);
            getHPBarItem()->setOpacity(opacity);
            if (opacity >= 1) {
                moveStepCounter++;
                setDoAttack(true);
                getAttackSpeedTimer()->start(getAttackSpeed());
                getBulletSpeedTimer()->start(getBulletSpeed());
                getMoveTimer()->start(1000);
            }
        break;
        case 2: // attack
            if (getHP() != iniHP)
                getHPBarItem()->setPos(getEnemy()->x(), getEnemy()->y() - 15);
        break;
        case 3: // vanish and random move
            if (opacity > 0) {
                opacity -= 0.01;
                getEnemy()->setOpacity(opacity);
                getHPBarItem()->setOpacity(opacity);
            } else {
                setIsInvinsible(false);
            }
            if (getHP() != tempHP)
                moveStepCounter = 1;

            getEnemy()->setPos(getEnemy()->x() + moveDirX, getEnemy()->y() + moveDirY);
            if (getHP() != iniHP)
                getHPBarItem()->setPos(getEnemy()->x(), getEnemy()->y() - 15);

            distance = std::sqrt(std::pow((getEnemy()->x() - moveDesX), 2) + std::pow((getEnemy()->y() - moveDesY), 2));
            if (distance < 1)
                moveStepCounter = 1;
        break;
        case 4: // dead
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

void EnemyInvisible::normalAttack()
{
    if (!getDoAttack()) return;

    int bulletSize[2] = { 4, 15 };
    EnemyBullet1 *temp = new EnemyBullet1(getPlayer(), this, bulletSize[0], bulletSize[1]);
    temp->setPixmap(QPixmap(":/img/enemy_bullet_1.png").scaled(bulletSize[0], bulletSize[1]));
    temp->setPos(getEnemy()->x() + width / 2 - bulletSize[0] / 2 + 1, getEnemy()->y() + height);
    temp->connect(getBulletSpeedTimer(), SIGNAL(timeout()), temp, SLOT(verticalFly()));
    getScene()->addItem(static_cast<QGraphicsPixmapItem *>(temp));
}

void EnemyInvisible::timeCalculation()
{
    double distance;
    switch (moveStepCounter) {
        case 2:
            getMoveTimer()->stop();
            setDoAttack(false);
            setIsInvinsible(true);
            tempHP = getHP();

            while (1) {
                moveDesX = rand() % 400 + 10;
                if (iniPosX >= 500) moveDesX += 500;
                moveDesY = rand() % 120 + 120;

                moveDirX = moveDesX - getEnemy()->x();
                moveDirY = moveDesY - getEnemy()->y();
                distance = std::sqrt(std::pow(moveDirX, 2) + std::pow(moveDirY, 2));
                if (distance > 100) break;
            }
            moveDirX /= distance;
            moveDirY /= distance;
            moveStepCounter = 3;
        break;
        case 3:
            moveStepCounter = 1;
        break;
    }
}
