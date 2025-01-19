#include <cmath>
#include "include/EnemyBoss.h"
#include "include/EnemyBullet1.h"

#include <QDebug>

EnemyBoss::EnemyBoss(QTimer *t, QGraphicsScene *s, Player *p, QGraphicsPixmapItem *e, int x, int y) :
  Enemy(t, s, p, e),
  moveStepCounter(0),
  width(800),
  height(50),
  iniPosX(x),
  iniPosY(y),
  iniHP(15000),
  moveDir(-1),
  respawnENTimer(new QTimer),
  respawnEITimer(new QTimer),
  respawnEN(false),
  respawnEI(false)
{
    for (int i = 0; i < 3; ++i) {
        QGraphicsPixmapItem *e = new QGraphicsPixmapItem(QPixmap(":/img/enemy_scatter.png").scaled(100, 50));
        switch (i) {
            case 1:
                es[i] = new EnemyScatter(t, s, p, e, 230, 35);
                p->getEnemy().push_back(es[i]);
            break;
            case 0:
                es[i] = new EnemyScatter(t, s, p, e, 450, 35);
                p->getEnemy().push_back(es[i]);
            break;
            case 2:
                es[i] = new EnemyScatter(t, s, p, e, 670, 35);
                p->getEnemy().push_back(es[i]);
            break;
        }
    }
    for (int i = 0; i < 4; ++i) en[i] = NULL;
    for (int i = 0; i < 2; ++i) ei[i] = NULL;

    setAttackSpeed(300);
    connect(getAttackSpeedTimer(), SIGNAL(timeout()), this, SLOT(normalAttack()));

    setBulletSpeed(15);

    e->setPos(x, -height - 35);

    setIniHP(iniHP);
    getHPBar()->setFixedHeight(20);
    getHPBar()->setFixedWidth(900);
    getHPBarItem()->setPos(50, 5);
    setExp(0);

    connect(respawnENTimer, SIGNAL(timeout()), this, SLOT(respawn()));
    connect(respawnEITimer, SIGNAL(timeout()), this, SLOT(respawn()));

    connect(getMoveTimer(), SIGNAL(timeout()), this, SLOT(timeCalculation()));
}

EnemyBoss::~EnemyBoss() {}

bool EnemyBoss::damageDetermination(Bullet *b)
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

void EnemyBoss::isDead()
{
    if (!respawnEN && !respawnEI) {
        for (int i = 0; i < 2; ++i) {
            if (en[i] == NULL || en[i]->getHP() <= 0) {
                respawnEN = true;
                respawnENTimer->start(13000);
                return;
            }
        }
    }

    if (!respawnEN && !respawnEI) {
        if (ei[0] == NULL || ei[0]->getHP() <= 0) {
            respawnEI = true;
            respawnENTimer->start(8000);
            return;
        }
    }

    if (!respawnEN && !respawnEI) {
        for (int i = 2; i < 4; ++i) {
            if (en[i] == NULL || en[i]->getHP() <= 0) {
                respawnEN = true;
                respawnENTimer->start(13000);
                return;
            }
        }
    }

    if (!respawnEN && !respawnEI) {
        if (ei[1] == NULL || ei[1]->getHP() <= 0) {
            respawnEI = true;
            respawnENTimer->start(8000);
            return;
        }
    }


    if (getHP() > 0) return;

    getPlayer()->addExp(getExp());

    disconnect(getTick(), SIGNAL(timeout()), this, SLOT(isDead()));
    disconnect(getAttackSpeedTimer(), SIGNAL(timeout()), this, SLOT(normalAttack()));
    disconnect(getMoveTimer(), SIGNAL(timeout()), this, SLOT(timeCalculation()));
    
    // std::vector<Enemy *> &e = getPlayer()->getEnemy();
    // e.erase(std::remove(e.begin(), e.end(), this), e.end());
}

void EnemyBoss::setPosition()
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
                getMoveTimer()->start(10000);
                connect(getTick(), SIGNAL(timeout()), this, SLOT(isDead()));
            }
        break;
        case 1: // attack
        break;
        case 2: // horizontal move
            getEnemy()->setPos(getEnemy()->x() + moveDir, getEnemy()->y());
            for (int i = 0; i < 3; ++i)
                es[i]->getEnemy()->setPos(es[i]->getEnemy()->x() + moveDir, es[i]->getEnemy()->y());
            if (getEnemy()->x() == 0 || getEnemy()->x() == 200)
                moveDir = -moveDir;
            if (int(getEnemy()->x()) % 100 == 0) {
                moveStepCounter--;
                getMoveTimer()->start(10000);
                setDoAttack(true);
                for (int i = 0; i < 3; ++i)
                    es[i]->setDoAttack(true);
                respawnEnemyScatter();
            }
        break;
    }
}

void EnemyBoss::normalAttack()
{
    if (!getDoAttack()) return;

    int bulletSize[2] = { 4, 15 };
    int bulletPos[12] = { 30, 70, 110, 250, 290, 330, 470, 510, 550, 690, 730, 770 };

    for (int i = 0; i < 12; ++i) {
        EnemyBullet1 *temp = new EnemyBullet1(getPlayer(), this, bulletSize[0], bulletSize[1]);
        temp->setPixmap(QPixmap(":/img/enemy_bullet_1.png").scaled(bulletSize[0], bulletSize[1]));
        temp->setPos(getEnemy()->x() + bulletPos[i], height + 10);
        temp->connect(getBulletSpeedTimer(), SIGNAL(timeout()), temp, SLOT(verticalFly()));
        getScene()->addItem(static_cast<QGraphicsPixmapItem *>(temp));
    }
}

void EnemyBoss::timeCalculation()
{    
    switch (moveStepCounter) {
        case 1:
            getMoveTimer()->stop();
            moveStepCounter++;
            setDoAttack(false);
            for (int i = 0; i < 3; ++i)
                es[i]->setDoAttack(false);
        break;
    }
}

void EnemyBoss::respawn()
{
    if (respawnEN) {
        int xPosEN[4] = { 200, 800, 400, 600 };
        int yPosEN[4] = { 120, 120, 240, 240 };
        for (int i = 0; i < 4; ++i) {
            if (en[i] == NULL || en[i]->getHP() <= 0) {
                QGraphicsPixmapItem *e = new QGraphicsPixmapItem(QPixmap(":/img/enemy_normal.png").scaled(84, 90));
                en[i] = new EnemyNormal(getTick(), getScene(), getPlayer(), e, xPosEN[i], yPosEN[i]);
                getPlayer()->getEnemy().push_back(en[i]);
                respawnEN = false;
                respawnENTimer->stop();
                return;
            }
        }
    }
    
    if (respawnEI) {
        int xPosEI[2] = { 250, 750 };
        int yPosEI = rand() % 240 + 120;

        for (int i = 0; i < 2; ++i) {
            if (ei[i] == NULL || ei[i]->getHP() <= 0) {
                QGraphicsPixmapItem *e = new QGraphicsPixmapItem(QPixmap(":/img/enemy_invisible.png").scaled(84, 90));
                ei[i] = new EnemyInvisible(getTick(), getScene(), getPlayer(), e, xPosEI[i], yPosEI);
                getPlayer()->getEnemy().push_back(ei[i]);
                respawnEI = false;
                respawnEITimer->stop();
                return;
            }
        }
    }
}

void EnemyBoss::respawnEnemyScatter()
{
    int xPos[3] = { 350, 130, 570 };
    for (int i = 0; i < 3; ++i) {
        if (es[i]->getHP() <= 0) {
            QGraphicsPixmapItem *e = new QGraphicsPixmapItem(QPixmap(":/img/enemy_scatter.png").scaled(100, 50));
            es[i] = new EnemyScatter(getTick(), getScene(), getPlayer(), e, xPos[i] + getEnemy()->x(), 35);
            getPlayer()->getEnemy().at(i) = es[i];
            break;
        }
    }
}
