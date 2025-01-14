#ifndef ENEMYBULLET1_H
#define ENEMYBULLET1_H

#include "Bullet.h"
#include "Player.h"
#include "EnemyNormal.h"

class EnemyBullet1 : public Bullet {
    public:
        EnemyBullet1(Player *, Enemy *, int w, int h);
        ~EnemyBullet1();
    public slots:
        void collisionDetection();
    private:
        Player *player;
        Enemy *enemy;
        int width, height;
};

#endif // ENEMYBULLET1_H
