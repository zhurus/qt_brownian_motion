#pragma once

#include <QGraphicsEllipseItem>
#include <QVector2D>

#include "defs.h"

class Particle
{
public:
    using Ptr = std::unique_ptr<Particle>;

    explicit Particle(const QPointF& center, int particle_size, int speed_limit);

    bool isInfected() const;
    void setInfected(bool is_infected);

    void move();
    void checkContact(Particle* other);
    void setSpeed(const QVector2D& speed);
    int particleSize() const;
    QPointF center() const;

private:
    const int m_particleSize;
    QPointF m_center;
    QVector2D m_speed;
    bool m_isInfected = false;
};


class GraphicsParticle : public Particle,
                         public QGraphicsItem
{
public:
    explicit GraphicsParticle(const QPointF& center, int particle_size, int speed_limit,
                              QGraphicsItem* parent = nullptr);

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
    QRectF boundingRect() const override;
};
