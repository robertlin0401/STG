#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QLabel>
#include <QKeyEvent>
#include <vector>
#include "include/Player.h"
#include "include/Enemy.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();
        void mousePressEvent(QMouseEvent *);
        void keyPressEvent(QKeyEvent *);

    public slots:
        void isWaveClear();
        void isGameOver();

        void setPlayerDes();
        void playerStatRenew();

    private:
        void generateEnemy();
        void pause();
        void resume();
    public slots:
        void resetTick();

    private:
        Ui::MainWindow *ui;
        QGraphicsScene *scene, *board;
        QGraphicsProxyWidget *expBarItem, *skillBarItem;
        QProgressBar *expBar, *skillBar;
        QGraphicsPixmapItem *playerLife[5];

        Player *player;
        std::vector <Enemy *> enemy;
        Enemy *boss;

        QTimer *timerEnemyAttack, *timerIsOver, *timerIsDead;
        QTimer *tick;
        QLabel *levels, *levelNumber, *skills;
        int wave;
        int remainingTime[1];
        bool isGameStart, isPause, nowShield;
};

#endif // MAINWINDOW_H
