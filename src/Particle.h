#pragma once


#include <QGraphicsEllipseItem>
#include <QVector2D>

#include "defs.h"



class Particle: public QGraphicsItem
{
public:
    Particle(int particleSize, int speedLimit);
    ~Particle() = default;

    void move();

    void setCenter(int x, int y);
    QPointF getCenter() const;

    void paint(QPainter* painter,
               const QStyleOptionGraphicsItem* option,
               QWidget* widget = nullptr) override;
    QRectF boundingRect() const override;

    QVector2D getSpeed() const;
    void setSpeed(const QVector2D& speed);

    void setInfected(bool value);
    bool isInfected() const;

    int getParticleSize() const;

private:
    QPointF _center;
    QVector2D _speed;
    bool _isInfected = {false};
    int _particleSize;
};
typedef std::shared_ptr<Particle> PtrParticle;
