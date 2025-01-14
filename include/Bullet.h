#ifndef BULLET_H
#define BULLET_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>

class Bullet : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
    public:
        Bullet(double y, double x) { dirX = x, dirY = y; }
        Bullet(double y, double x, int d, int w, int h)
            { dirX = x, dirY = y, damage = d, width = w, height = h; }
        ~Bullet() {}
        int getWidth() { return width; }
        int getHeight() { return height; }
        int getDamage() { return damage; }
        
    public slots:
        void verticalFly()
        {
            setPos(x(), y() + dirY * 5);
            if (y() < 0 || y() > 700) {
                this->scene()->removeItem(this);
                delete this;
            }
        }
        void horizontalFly()
        {
            setPos(x() + dirX * 5, y());
            if (x() < 0 || x() > 1000) {
                this->scene()->removeItem(this);
                delete this;
            }
        }
        virtual void collisionDetection() = 0;

    private:
        double dirX, dirY;
        int damage, width, height;
};

#endif // BULLET_H
