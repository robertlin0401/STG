#ifndef ENEMYBOSS_H
#define ENEMYBOSS_H

#include "Enemy.h"
#include "Player.h"
#include "EnemyNormal.h"
#include "EnemyInvisible.h"
#include "EnemyScatter.h"

class EnemyBoss : public Enemy {
    public:
        EnemyBoss(QTimer *, QGraphicsScene *, Player *, QGraphicsPixmapItem *, int x, int y);
        ~EnemyBoss();
        bool damageDetermination(Bullet *);
    public slots:
        void isDead();
        void setPosition();
        void normalAttack();
        void specialSkill() {}
        void timeCalculation();
        void respawn();
        void respawnEnemyScatter();
    private:
        EnemyNormal *en[4];
        EnemyInvisible *ei[2];
        EnemyScatter *es[3];
        int moveStepCounter;
        int width, height;
        int iniPosX, iniPosY, iniHP;
        int moveDir;

        QTimer *respawnENTimer, *respawnEITimer;
        bool respawnEN, respawnEI;
};

#endif // ENEMYBOSS_H
