#include "Particle.h"

#include <cstdlib>
#include <QPainter>
#include <cmath>
#include <random>


inline int genRandomNumber(int limit)
{
    std::random_device rd;
    std::mt19937 mt(rd());
    return round(double(limit) * (mt() - mt.min())/(mt.max() - mt.min()));
}


inline QPointF genRandomCenter(int particleSize)
{
    return QPointF( particleSize/2 + genRandomNumber(SCENE_WIDTH - particleSize),
                    particleSize/2 + genRandomNumber(SCENE_HEIGHT - particleSize) );
}



Particle::Particle(int particleSize, int speedLimit):
    _center(genRandomCenter(particleSize)),
    _particleSize(particleSize),
    _speed( genRandomNumber(speedLimit) * (genRandomNumber(1)*2 - 1),
            genRandomNumber(speedLimit) * (genRandomNumber(1)*2 - 1) )
{
}

void Particle::move()
{
    if( _center.x() + _particleSize / 2 > SCENE_WIDTH ||
            _center.x() - _particleSize / 2 < 0)
        _speed.setX( -_speed.x() );
    if( _center.y() + _particleSize / 2 > SCENE_HEIGHT ||
            _center.y() - _particleSize / 2 < 0)
        _speed.setY( -_speed.y() );
    _center.setX( _center.x() + _speed.x() );
    _center.setY( _center.y() + _speed.y() );
}

void Particle::setCenter(int x, int y)
{
    _center = QPoint(x,y);
}

QPointF Particle::getCenter() const
{
    return _center;
}

void Particle::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    painter->setBrush( _isInfected? Qt::red : Qt::cyan );
    painter->drawEllipse( boundingRect().adjusted(1,1,-1,-1) );
    Q_UNUSED(option)
    Q_UNUSED(widget)
}

QRectF Particle::boundingRect() const
{
    return QRectF( _center.x() - _particleSize/2,
                   _center.y() - _particleSize/2,
                   _particleSize,
                   _particleSize );
}

QVector2D Particle::getSpeed() const
{
    return _speed;
}

void Particle::setSpeed(const QVector2D& speed)
{
    _speed = speed;
}

void Particle::setInfected(bool value)
{
    _isInfected = value;
}

bool Particle::isInfected() const
{
    return _isInfected;
}

int Particle::getParticleSize() const
{
    return _particleSize;
}

