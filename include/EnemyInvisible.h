#ifndef ENEMYINVISIBLE_H
#define ENEMYINVISIBLE_H

#include "Enemy.h"
#include "Player.h"

class EnemyInvisible : public Enemy {
    public:
        EnemyInvisible(QTimer *, QGraphicsScene *, Player *, QGraphicsPixmapItem *, int x, int y);
        ~EnemyInvisible();
        bool damageDetermination(Bullet *);
    public slots:
        void isDead();
        void setPosition();
        void normalAttack();
        void specialSkill() {}
        void timeCalculation();
        void respawn() {}
    private:
        int moveStepCounter;
        int width, height;
        int iniPosX, iniPosY, iniHP;
        int moveDir, rotateCounter;
        int moveDesX, moveDesY;
        double moveDirX, moveDirY;
        double opacity;
        int tempHP;
};

#endif // ENEMYINVISIBLE_H
