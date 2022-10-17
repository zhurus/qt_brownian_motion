#include "load.h"

MagneticCursor::MagneticCursor(double factor) : m_factor(factor) {}

void MagneticCursor::affectParticle(Particle* particle) const
{
    if( m_enabled ) {
        QVector2D particle_center( particle->center() );
        QVector2D diff = QVector2D(m_cursorPos) - particle_center;
        QVector2D particle_speed = particle->speed();
        QVector2D load = diff.normalized() * m_factor;
        particle_speed += load;
        particle->setSpeed( particle_speed );
    }
}

void MagneticCursor::setCursorPos(const QPointF& cursor_pos)
{
    m_cursorPos = cursor_pos;
}

void MagneticCursor::setFactor(double factor)
{
    m_factor = factor;
}

void MagneticCursor::setEnabled(bool enabled)
{
    m_enabled = enabled;
}

Damping::Damping(double factor) : m_factor(factor) {}

void Damping::affectParticle(Particle* particle) const
{
    particle->setSpeed( particle->speed() * (1 - m_factor) );
}

void Damping::setFactor(double factor)
{
    m_factor = factor;
}
