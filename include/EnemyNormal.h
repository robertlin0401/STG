#ifndef ENEMYNORMAL_H
#define ENEMYNORMAL_H

#include "Enemy.h"
#include "Player.h"

class EnemyNormal : public Enemy {
    public:
        EnemyNormal(QTimer *, QGraphicsScene *, Player *, QGraphicsPixmapItem *, int x, int y);
        ~EnemyNormal();
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
};

#endif // ENEMYNORMAL_H
