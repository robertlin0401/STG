#-------------------------------------------------
#
# Project created by QtCreator 2018-06-02T01:36:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = STG
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += CORE_LIBRARY

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    main.cpp \
    mainwindow.cpp \
    src/Player.cpp \
    src/DefaultPlayer.cpp \
    src/Enemy.cpp \
    src/EnemyNormal.cpp \
    src/EnemyInvisible.cpp \
    src/EnemyScatter.cpp \
    src/EnemyBoss.cpp \
    src/PlayerBullet.cpp \
    src/EnemyBullet1.cpp \
    src/EnemyBullet2.cpp \

HEADERS += \
    mainwindow.h \
    include/Character.h \
    include/Player.h \
    include/DefaultPlayer.h \
    include/Enemy.h \
    include/EnemyNormal.h \
    include/EnemyInvisible.h \
    include/EnemyScatter.h \
    include/EnemyBoss.h \
    include/Bullet.h \
    include/PlayerBullet.h \
    include/EnemyBullet1.h \
    include/EnemyBullet2.h \

FORMS += \
    mainwindow.ui

RESOURCES += \
    res.qrc
