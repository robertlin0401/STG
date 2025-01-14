#include <cstdlib>
#include <ctime>
#include <QMessageBox>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "include/DefaultPlayer.h"
#include "include/EnemyBoss.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  scene(new QGraphicsScene(0, 0, 1000, 700)),
  board(new QGraphicsScene(0, 705, 1000, 50)),
  tick(new QTimer),
  wave(0),
  isGameStart(false),
  isPause(false)
{
    ui->setupUi(this);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setSceneRect(
        0, 0,
        ui->graphicsView->contentsRect().width(),
        ui->graphicsView->contentsRect().height()
    );
    ui->graphicsBoard->setScene(board);
    ui->graphicsBoard->setSceneRect(
        0, 0,
        ui->graphicsBoard->contentsRect().width(),
        ui->graphicsBoard->contentsRect().height()
    );

    player = new DefaultPlayer(tick, scene, enemy);
    connect(tick, SIGNAL(timeout()), this, SLOT(setPlayerDes()));

    levels = new QLabel(this);
    levels->setText(tr("  Lv"));
    levels->setGeometry(330, 725, 50, 50);
    levels->setStyleSheet("font: 24px");
    levels->setAlignment(Qt::AlignLeft);
    levelNumber = new QLabel(this);
    levelNumber->setNum(player->getLv());
    levelNumber->setGeometry(380, 725, 30, 50);
    levelNumber->setStyleSheet("font: 24px");
    levelNumber->setAlignment(Qt::AlignLeft);
    expBar = new QProgressBar;
    expBarItem = new QGraphicsProxyWidget;
    expBarItem = board->addWidget(expBar);
    expBarItem->setPos(410, 10);
    skills = new QLabel(this);
    skills->setText(tr("  SKILL"));
    skills->setGeometry(650, 725, 80, 50);
    skills->setStyleSheet("font: 24px");
    skills->setAlignment(Qt::AlignLeft);
    skillBar = new QProgressBar;
    skillBar->setRange(0, 100);
    skillBarItem = new QGraphicsProxyWidget;
    skillBarItem = board->addWidget(skillBar);
    skillBarItem->setPos(750, 10);
    connect(tick, SIGNAL(timeout()), this, SLOT(playerStatRenew()));
    
    for (int i = 0; i < player->getHP(); ++i) {
        playerLife[i] = new QGraphicsPixmapItem(QPixmap(":/img/player_life.png").scaled(64, 36));
        playerLife[i]->setPos(10 + (300 / player->getHP()) * i, 5);
        board->addItem(playerLife[i]);
    }
    connect(tick, SIGNAL(timeout()), this, SLOT(isGameOver()));
    
    srand(time(NULL));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (isGameStart) return;
    
    int x = ui->graphicsView->mapToGlobal(player->getPlayer()->pos()).x();
    int y = ui->graphicsView->mapToGlobal(player->getPlayer()->pos()).y();
    player->setCursorPosOnStart(x, y);

    isGameStart = true;
    tick->start(10);
    player->startGame();
    wave++;
    generateEnemy();
    connect(tick, SIGNAL(timeout()), this, SLOT(isWaveClear()));
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (!isGameStart) return;

    switch(event->key()) {
        case Qt::Key_R:
            if (!isPause)
                player->activeSPSkill();
        break;
        case Qt::Key_Escape:
            if (!isPause)
                pause();
            else
                resume();
            isPause = !isPause;
    }
}

void MainWindow::isWaveClear()
{
    bool isClear = true;
    for (Enemy *e : enemy) {
        if (e->getHP() > 0) {
            isClear = false;
            break;
        }
    }
    if (isClear) {
        player->setDoAttack(false);
        wave++;
        generateEnemy();
    }
}

void MainWindow::isGameOver()
{
    if (player->getHP() <= -10000) {
        pause();
        QMessageBox *finish = new QMessageBox;
        finish->about(finish, "Game Over", "You lose!");
        close();
    }

    if (boss->getHP() <= 0) {
        pause();
        QMessageBox *finish = new QMessageBox;
        finish->about(finish, "Game Over", "You win!");
        close();
    }
}

void MainWindow::setPlayerDes()
{
    int desX = ui->graphicsView->mapFromGlobal(QCursor::pos()).x();
    int desY = ui->graphicsView->mapFromGlobal(QCursor::pos()).y();
    player->setDes(desX, desY);
    // qDebug() << desX << desY;
}

void MainWindow::playerStatRenew()
{
    for (int i = player->getHP(); i < 5; ++i) {
        if (i < 0) break;
        if (playerLife[i] != NULL) {
            board->removeItem(playerLife[i]);
            playerLife[i] = NULL;
        }
    }

    levelNumber->setNum(player->getLv());
    expBar->setValue(player->getExp());
    expBar->setMaximum(player->getLvUpExp());

    skillBar->setValue(player->getSkillProgress());
}

void MainWindow::generateEnemy()
{
    switch (wave) {
        case 1:
            QGraphicsPixmapItem *b = new QGraphicsPixmapItem(QPixmap(":/img/enemy_boss.png").scaled(800, 50));
            enemy.push_back(new EnemyBoss(tick, scene, player, b, 100, 0));
            boss = enemy.at(3);
        break;
    }
}

void MainWindow::pause()
{
    player->pause();
    for (Enemy *e : enemy)
        e->pause();

    remainingTime[0] = tick->remainingTime();
    tick->stop();
}

void MainWindow::resume()
{
    player->resume();
    for (Enemy *e : enemy)
        e->resume();

    if (remainingTime[0] <= 0) {
        tick->start(10);
    } else {
        connect(tick, SIGNAL(timeout()), this, SLOT(resetTick()));
        tick->start(remainingTime[0]);
    }
}

void MainWindow::resetTick()
{
    disconnect(tick, SIGNAL(timeout()), this, SLOT(resetTick()));
    tick->start(10);
}
