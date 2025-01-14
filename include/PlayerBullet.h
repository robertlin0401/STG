#ifndef PLAYERBULLET_H
#define PLAYERBULLET_H

#include "Bullet.h"
#include "Player.h"
#include "EnemyNormal.h"

class PlayerBullet : public Bullet {
    public:
        PlayerBullet(Player *, std::vector<Enemy *> &, int damage, int skillProgressOnHit, int w, int h);
        ~PlayerBullet();
    public slots:
        void collisionDetection();
    private:
        Player *player;
        std::vector<Enemy *> &enemy;
        int skillProgressOnHit;
};

#endif // PLAYERBULLET_H
