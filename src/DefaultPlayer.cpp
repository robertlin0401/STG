#include <QCursor>
#include "include/DefaultPlayer.h"
#include "include/PlayerBullet.h"

#include <QDebug>

DefaultPlayer::DefaultPlayer(QTimer *t, QGraphicsScene *s, std::vector<Enemy *> &e) : 
  Player(t, s, e, new QGraphicsPixmapItem(QPixmap(":/img/player.png").scaled(84, 90)), 458, 500), 
  width(84),
  height(90),
  sidePwidth(20),
  sidePheight(20),
  skillStepCounter(-1)
{
    setHP(5);
    setAttackSpeed(100);
    connect(getAttackSpeedTimer(), SIGNAL(timeout()), this, SLOT(normalAttack()));

    setBulletSpeed(10);
    setSkillAutoProgressInterval(2000);
    setSkillProgressOnHit(2);

    L = new QGraphicsPixmapItem(QPixmap(":/img/side_player.png").scaled(sidePwidth, sidePheight));
    R = new QGraphicsPixmapItem(QPixmap(":/img/side_player.png").scaled(sidePwidth, sidePheight));

    connect(getSkillTimer(), SIGNAL(timeout()), this, SLOT(timeCalculation()));
    connect(getSkillTimer(), SIGNAL(timeout()), this, SLOT(addSkillProgressByTime()));
}

DefaultPlayer::~DefaultPlayer() {}

void DefaultPlayer::startGame()
{
    setIsInvinsible(false);
    getAttackSpeedTimer()->start(getAttackSpeed());
    getBulletSpeedTimer()->start(getBulletSpeed());
    getSkillTimer()->start(getSkillAutoProgressInterval());
}

void DefaultPlayer::setCursorPosOnStart(int x, int y)
{
    QCursor cursor;
    cursor.setPos(x + width / 2, y + height * 5 / 6);
}


void DefaultPlayer::activeSPSkill()
{
    if (skillStepCounter != -1 || getSkillProgress() < 100 || !getDoAttack()) return;

    if (!getIsInvinsible()) {
        opacity = 1;
        opacityChange = 0.08;
        L->setOpacity(opacity);
        R->setOpacity(opacity);
    }

    setSkillProgressOnHit(0);

    sideDesY = 700;
    skillStepCounter = rotateCounter = 0;
    L->setRotation(rotateCounter);
    R->setRotation(rotateCounter);
    getScene()->addItem(L);
    getScene()->addItem(R);
}

void DefaultPlayer::startSPATK()
{
    getSkillTimer()->stop();

    skillStepCounter++;
    setAttackSpeed(getAttackSpeed() / 2);
    setBulletSpeed(getBulletSpeed() / 2);
    connect(getAttackSpeedTimer(), SIGNAL(timeout()), this, SLOT(specialSkill()));

    getAttackSpeedTimer()->start(getAttackSpeed());
    getBulletSpeedTimer()->start(getBulletSpeed());
    getSkillTimer()->start(8000);
}

void DefaultPlayer::endSPATK()
{
    if (!getIsInvinsible()) {
        opacity = 1;
        opacityChange = 0.08;
        L->setOpacity(opacity);
        R->setOpacity(opacity);
    }
    
    skillStepCounter = 3;
    resetSkillProgress();
    setSkillProgressOnHit(2);
    setAttackSpeed(getAttackSpeed() * 2);
    setBulletSpeed(getBulletSpeed() * 2);

    getAttackSpeedTimer()->start(getAttackSpeed());
    getBulletSpeedTimer()->start(getBulletSpeed());

    getSkillTimer()->start(getSkillAutoProgressInterval());
    disconnect(getAttackSpeedTimer(), SIGNAL(timeout()), this, SLOT(specialSkill()));
}

void DefaultPlayer::setPosition()
{
    int desX = getDesX();
    int desY = getDesY();
    
    if (desX < width / 2 + sidePwidth / 2) {
        desX = width / 2 + sidePwidth / 2;
    } else if (desX > 1000 - width / 2 - sidePwidth / 2) {
        desX = 1000 - width / 2 - sidePwidth / 2;
    }
    desX = desX - width / 2;

    float centerY = 5.0 / 6;
    if (desY < centerY * height) {
        desY = centerY * height;
    } else if(desY > 700 - (1 - centerY) * height) {
        desY = 700 - (1 - centerY) * height;
    }
    desY = desY - centerY * height;

    getPlayer()->setPos(desX, desY);

    switch (skillStepCounter) {
        case 0: // enter
            setLSidePlayerPosition(desX);
            setRSidePlayerPosition(desX);
            sideDesY -= 3;
            if (sideDesY <= desY + height - sidePheight)
                startSPATK();
        break;
        case 1: // attack
            sideDesY = desY + height - sidePheight;
            setLSidePlayerPosition(desX);
            setRSidePlayerPosition(desX);
        break;
        case 2: // leave with momentum
            // TODO
        break;
        case 3: // leave 
            setLSidePlayerPosition(LDesX--);
            setRSidePlayerPosition(RDesX++);
            sideDesY += 2;
            L->setRotation(-rotateCounter);
            R->setRotation(rotateCounter);
            rotateCounter += 2;
            if (sideDesY > 700) {
                getScene()->removeItem(L);
                getScene()->removeItem(R);
                skillStepCounter = -1;
            }
        break;
    }

    if (getDoAttack() && getIsInvinsible()) {
        if (opacity >= 1 || opacity <= 0.2) opacityChange = -opacityChange;
        opacity += opacityChange;
        getPlayer()->setOpacity(opacity);
        L->setOpacity(opacity);
        R->setOpacity(opacity);
    }
}

void DefaultPlayer::setLSidePlayerPosition(int desX)
{
    if (skillStepCounter == 1) LDesX = desX;
    desX -= (sidePwidth - 5);
    L->setPos(desX, sideDesY);
}

void DefaultPlayer::setRSidePlayerPosition(int desX)
{
    if (skillStepCounter == 1) RDesX = desX;
    desX += (width - 7);
    R->setPos(desX, sideDesY);
}

void DefaultPlayer::normalAttack()
{
    bool isClear = true;
    for (Enemy *e : getEnemy()) {
        if (e->getHP() > 0) {
            isClear = false;
            break;
        }
    }
    if (isClear) setDoAttack(false);
    if (!getDoAttack()) return;
    
    int bulletSize[2] = { 4, 15 };
    PlayerBullet *temp = new PlayerBullet(this, getEnemy(), damage[getLv()], getSkillProgressOnHit(), bulletSize[0], bulletSize[1]);
    temp->setPixmap(QPixmap(":/img/player_bullet.png").scaled(bulletSize[0], bulletSize[1]));
    temp->setPos(getPlayer()->x() + width / 2 - bulletSize[0] / 2, getPlayer()->y() - bulletSize[1]);
    temp->connect(getBulletSpeedTimer(), SIGNAL(timeout()), temp, SLOT(verticalFly()));
    getScene()->addItem(static_cast<QGraphicsPixmapItem *>(temp));

    // // for damage determination
    // for (int i = 1; i <= 10; ++i) {
    //     PlayerBullet *tempR = new PlayerBullet(this, getEnemy(), damage[getLv()], getSkillProgressOnHit(), bulletSize[0], bulletSize[1]);
    //     tempR->setPixmap(QPixmap(":/img/player_bullet.png").scaled(bulletSize[0], bulletSize[1]));
    //     tempR->setPos(getPlayer()->x() + width / 2 - bulletSize[0] / 2 + bulletSize[0] * i, getPlayer()->y() - bulletSize[1]);
    //     tempR->connect(getBulletSpeedTimer(), SIGNAL(timeout()), tempR, SLOT(verticalFly()));
    //     getScene()->addItem(static_cast<QGraphicsPixmapItem *>(tempR));

    //     PlayerBullet *tempL = new PlayerBullet(this, getEnemy(), damage[getLv()], getSkillProgressOnHit(), bulletSize[0], bulletSize[1]);
    //     tempL->setPixmap(QPixmap(":/img/player_bullet.png").scaled(bulletSize[0], bulletSize[1]));
    //     tempL->setPos(getPlayer()->x() + width / 2 - bulletSize[0] / 2 - bulletSize[0] * i, getPlayer()->y() - bulletSize[1]);
    //     tempL->connect(getBulletSpeedTimer(), SIGNAL(timeout()), tempL, SLOT(verticalFly()));
    //     getScene()->addItem(static_cast<QGraphicsPixmapItem *>(tempL));
    // }
}

void DefaultPlayer::specialSkill()
{
    if (!getDoAttack()) return;
    if (!getIsInvinsible()) {
        opacity = 1;
        opacityChange = 0.08;
        L->setOpacity(opacity);
        R->setOpacity(opacity);
    }

    float bulletSize[2] = { 3, 8 };

    int posX = getPlayer()->x();
    int posY = getPlayer()->y() + height - sidePheight - bulletSize[1];

    PlayerBullet *tempL = new PlayerBullet(this, getEnemy(), damage[getLv()] / 2, 0, bulletSize[0], bulletSize[1]);
    tempL->setPixmap(QPixmap(":/img/player_bullet.png").scaled(bulletSize[0], bulletSize[1]));
    tempL->setPos(posX - sidePwidth / 4 - bulletSize[0] / 2, posY);
    tempL->connect(getBulletSpeedTimer(), SIGNAL(timeout()), tempL, SLOT(verticalFly()));
    getScene()->addItem(static_cast<QGraphicsPixmapItem *>(tempL));
    
    PlayerBullet *tempR = new PlayerBullet(this, getEnemy(), damage[getLv()] / 2, 0, bulletSize[0], bulletSize[1]);
    tempR->setPixmap(QPixmap(":/img/player_bullet.png").scaled(bulletSize[0], bulletSize[1]));
    tempR->setPos(posX + width + bulletSize[0] / 2, posY);
    tempR->connect(getBulletSpeedTimer(), SIGNAL(timeout()), tempR, SLOT(verticalFly()));
    getScene()->addItem(static_cast<QGraphicsPixmapItem *>(tempR));
}

void DefaultPlayer::addSkillProgressByTime()
{
    if (skillStepCounter != -1) return;

    addSkillProgress(1);
    getSkillTimer()->start(getSkillAutoProgressInterval());
}

void DefaultPlayer::timeCalculation()
{
    switch (skillStepCounter) {
        case 1:
            endSPATK();
        break;
    }
}
