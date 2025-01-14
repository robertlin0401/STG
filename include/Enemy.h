#ifndef ENEMY_H
#define ENEMY_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsProxyWidget>
#include <QTimer>
#include <QProgressBar>
#include "Character.h"
#include "Bullet.h"
class Player;

class Enemy : public QObject, public Character {
    Q_OBJECT
    public:
        Enemy(QTimer *, QGraphicsScene *, Player *, QGraphicsPixmapItem *);
        ~Enemy();

        QTimer *getTick() { return tick; }
        QGraphicsScene *getScene() { return scene; }
        Player *getPlayer() { return player; }
        QGraphicsPixmapItem *getEnemy() { return enemy; }
        QProgressBar *getHPBar() { return HPBar; }
        QGraphicsProxyWidget *getHPBarItem() { return HPBarItem; }
        QTimer *getMoveTimer() { return moveTimer; }
        void pause();
        void resume();
    public slots:
        void resetAttackSpeedTimer();
        void resetBulletSpeedTimer();

    public:
        int getHP() { return HPBar->value(); }
        void setHP(int HP) { HPBar->setValue((HP > 0)? HP: 0); }
        void setIniHP(int iniHP) { HPBar->setRange(0, iniHP); setHP(iniHP); }
        int getExp() { return exp; }
        void setExp(int e) { exp = e; }

        virtual bool damageDetermination(Bullet *) = 0;

    public slots:
        virtual void isDead() = 0;
        virtual void setPosition() = 0;
        virtual void normalAttack() = 0;
        virtual void specialSkill() = 0;
        virtual void timeCalculation() = 0;
        virtual void respawn() = 0;

    private:
        QTimer *tick;
        QGraphicsScene *scene;
        Player *player;
        QGraphicsPixmapItem *enemy;
        QProgressBar *HPBar;
        QGraphicsProxyWidget *HPBarItem;

        QTimer *moveTimer;
        int remainingTime[3];

        int exp;
};

#endif // ENEMY_H
