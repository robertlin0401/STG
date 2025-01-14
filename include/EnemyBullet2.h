#ifndef ENEMYBULLET2_H
#define ENEMYBULLET2_H

#include "Bullet.h"
#include "Player.h"
#include "EnemyNormal.h"

class EnemyBullet2 : public Bullet {
    public:
        EnemyBullet2(Player *, Enemy *, int r, double xDir, double yDir);
        ~EnemyBullet2();
    public slots:
        void collisionDetection();
    private:
        Player *player;
        Enemy *enemy;
        int radius;
};

#endif // ENEMYBULLET2_H
