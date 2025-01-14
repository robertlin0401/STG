#ifndef ENEMYSCATTER_H
#define ENEMYSCATTER_H

#include "Enemy.h"
#include "Player.h"

class EnemyScatter : public Enemy {
    public:
        EnemyScatter(QTimer *, QGraphicsScene *, Player *, QGraphicsPixmapItem *, int x, int y);
        ~EnemyScatter();
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
        int moveDir;
};

#endif // ENEMYSCATTER_H
