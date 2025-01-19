#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QTimer>
#include <QProgressBar>
#include <vector>
#include "Character.h"
class Enemy;

class Player : public QObject, public Character {
    Q_OBJECT
    public:
        Player(QTimer *, QGraphicsScene *, std::vector<Enemy *> &, QGraphicsPixmapItem *, int x, int y);
        ~Player();

        QTimer *getTick() { return tick; }
        QGraphicsScene *getScene() { return scene; }
        std::vector<Enemy *> &getEnemy() { return enemy; }
        QGraphicsPixmapItem *getPlayer() { return player; }
        QTimer *getSkillTimer() { return skillTimer; }
        void pause();
        void resume();
    public slots:
        void resetAttackSpeedTimer();
        void resetBulletSpeedTimer();

    public:
        virtual void startGame() = 0;
        virtual void setCursorPosOnStart(int x, int y) = 0;

        void dropHP();
        void setHP(int hp) { HP = hp; }
        int getHP() { return HP; }
        int getSkillProgress() { return skillProgress; }
        void resetSkillProgress() { skillProgress = 0; }
        void addSkillProgress(int);
        int getSkillAutoProgressInterval() { return skillAutoProgressInterval; }
        void setSkillAutoProgressInterval(int p) { skillAutoProgressInterval = p; }
        int getSkillProgressOnHit() { return skillProgressOnHit; }
        void setSkillProgressOnHit(int p) { skillProgressOnHit = p; }

        int getLv() { return lv; }
        void setLv(int l) { lv = l; }
        double getExp() { return exp; }
        double getLvUpExp() { return lvUpExp[lv]; }
        void addExp(int);
        
        void setDes(int x, int y) { desX = x, desY = y; }
        int getDesX() { return desX; }
        int getDesY() { return desY; }
        
        virtual void activeSPSkill() = 0;

    public slots:
        void bodyCollisionDetection();
        void endInvinsibleAnimation();
        virtual void setPosition() = 0;
        virtual void normalAttack() = 0;
        virtual void specialSkill() = 0;
        virtual void addSkillProgressByTime() = 0;
        virtual void timeCalculation() = 0;

    private:
        QTimer *tick;
        QGraphicsScene *scene;
        std::vector<Enemy *> &enemy;
        QGraphicsPixmapItem *player;

        bool isPause;
        int remainingTime[4];
        QTimer *skillTimer, *invinsibleTimer;

        int desX, desY;
        int HP, skillProgress, skillAutoProgressInterval, skillProgressOnHit;
        int lv, exp;
        int lvUpExp[10] = { 0, 20, 40, 60, 80, 100, 120, 140, 160, 180 };
};

#endif // PLAYER_H
