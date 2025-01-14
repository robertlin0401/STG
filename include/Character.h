#ifndef CHARACTER_H
#define CHARACTER_H

#include <QTimer>

class Character {
    public:
        Character()
        {
            attackSpeedTimer = new QTimer;
            bulletSpeedTimer = new QTimer;
        }

        bool getDoAttack() { return doAttack; }
        void setDoAttack(bool b) { doAttack = b; }
        bool getIsInvinsible() { return isInvincible; }
        void setIsInvinsible(bool b) { isInvincible = b; }
        
        QTimer *getAttackSpeedTimer() { return attackSpeedTimer; }
        QTimer *getBulletSpeedTimer() { return bulletSpeedTimer; }
        int getAttackSpeed() { return attackSpeed; }
        void setAttackSpeed(int s) { attackSpeed = s; }
        int getBulletSpeed() { return bulletSpeed; }
        void setBulletSpeed(int s) { bulletSpeed = s; }

    public slots:
        virtual void setPosition() = 0;
        virtual void normalAttack() = 0;
        virtual void specialSkill() = 0;

    private:
        bool doAttack, isInvincible;

        QTimer *attackSpeedTimer, *bulletSpeedTimer;
        int attackSpeed, bulletSpeed;
};

#endif // CHARACTER_H
