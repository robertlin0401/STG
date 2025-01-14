#ifndef DEFAULTPLAYER_H
#define DEFAULTPLAYER_H

#include "Player.h"
#include "PlayerBullet.h"
#include "Enemy.h"

class DefaultPlayer : public Player {
    public:
        DefaultPlayer(QTimer *, QGraphicsScene *, std::vector<Enemy *> &);
        ~DefaultPlayer();
        void startGame();
        void setCursorPosOnStart(int x, int y);
        void activeSPSkill();
        void bodyCollisionDetection();

    public slots:
        void setPosition();
        void normalAttack();
        void specialSkill();
        void addSkillProgressByTime();
        void timeCalculation();

    private:
        void setLSidePlayerPosition(int);
        void setRSidePlayerPosition(int);
        void startSPATK();
        void endSPATK();

        int width, height;
        int sidePwidth, sidePheight;
        QGraphicsPixmapItem *L, *R;
        int skillStepCounter, rotateCounter;
        int LDesX, RDesX, sideDesY;
        double opacity = 1, opacityChange = 0.08;

        int damage[6] = { 0, 2, 3, 4, 6, 8 };
};

#endif // DEFAULTPLAYER_H
