#pragma once

#include "particle.h"
#include <QVector2D>

class Load
{
public:
    using Ptr = std::unique_ptr<Load>;

    virtual ~Load() {}

    virtual void affectParticle(Particle* particle) const = 0;
};

class Damping : public Load
{
public:
    explicit Damping(double factor = 0.01);

    void affectParticle(Particle* particle) const override;
    void setFactor(double factor);

private:
    double m_factor;
};


class MagneticCursor : public Load
{
public:
    explicit MagneticCursor(double factor);

    void affectParticle(Particle* particle) const override;

    void setCursorPos(const QPointF& cursor_pos);
    void setFactor(double factor);
    void setEnabled(bool enabled);

public:
    QPointF m_cursorPos;
    double m_factor;
    bool m_enabled = false;
};
