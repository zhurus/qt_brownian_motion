#include "particle.h"

#include <cstdlib>
#include <QPainter>
#include <cmath>
#include <random>

#include <QRandomGenerator>

Particle::Particle(const QPointF& center, int particle_size, int speed_limit)
    : m_particleSize(particle_size),
      m_center(center)
{
    m_speed.setX( QRandomGenerator::global()->bounded(-speed_limit, speed_limit) );
    m_speed.setY( QRandomGenerator::global()->bounded(-speed_limit, speed_limit) );
}

void Particle::move()
{
    if( m_center.x() + m_particleSize / 2 > SCENE_WIDTH || m_center.x() - m_particleSize / 2 < 0 )
    {
        m_speed.setX( -m_speed.x() );
    }
    if( m_center.y() + m_particleSize / 2 > SCENE_HEIGHT || m_center.y() - m_particleSize / 2 < 0 )
    {
        m_speed.setY( -m_speed.y() );
    }
    m_center.setX( m_center.x() + m_speed.x() );
    m_center.setY( m_center.y() + m_speed.y() );
}

void Particle::checkContact(Particle* other)
{
    qreal distance = ( QVector2D( m_center ) - QVector2D( other->m_center ) ).length();
    bool is_contact = distance < ( m_particleSize + other->m_particleSize ) / 2;
    // TODO additional check
    if( is_contact ) {
        QVector2D n = QVector2D( other->m_center.x() - m_center.x(),
                                 other->m_center.y() - m_center.y() ).normalized();
        QVector2D tau = QVector2D( n.y(), -n.x() );

        double vn1 = QVector2D::dotProduct( m_speed, n );
        double vt1 = QVector2D::dotProduct( m_speed,tau );
        double vn2 = QVector2D::dotProduct( other->m_speed, n );
        double vt2 = QVector2D::dotProduct( other->m_speed, tau );
        if( vn1 < 0 && vn2 > 0 ) {
            return;
        }

        double vn2_ = vn1;
        double vn1_ = vn1 + vn2 - vn2_;

        m_speed = vt1 * tau + vn1_ * n;
        other->m_speed = vt2 * tau + vn2_ * n;
        if( m_isInfected ) {
            other->m_isInfected = true;
        }
        if( other->m_isInfected ) {
            m_isInfected = true;
        }
    }
}

void Particle::setSpeed(const QVector2D& speed)
{
    m_speed = speed;
}

bool Particle::isInfected() const
{
    return m_isInfected;
}

void Particle::setInfected(bool is_infected)
{
    m_isInfected = is_infected;
}

int Particle::particleSize() const
{
    return m_particleSize;
}

QPointF Particle::center() const
{
    return m_center;
}

GraphicsParticle::GraphicsParticle(const QPointF& center, int particle_size, int speed_limit, QGraphicsItem* parent)
    : Particle(center, particle_size, speed_limit),
      QGraphicsItem(parent)
{}

void GraphicsParticle::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    painter->setBrush( isInfected()? Qt::red : Qt::blue );
    painter->drawEllipse( boundingRect().adjusted( 1, 1, -1, -1 ) );
    Q_UNUSED(option)
    Q_UNUSED(widget)
}

QRectF GraphicsParticle::boundingRect() const
{
    QPointF c = center();
    int particle_size = particleSize();
    return QRectF( c.x() - particle_size/2,
                   c.y() - particle_size/2,
                   particle_size,
                   particle_size );
}
