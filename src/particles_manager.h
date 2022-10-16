#pragma once

#include "settings.h"
#include "particle.h"
#include <QGraphicsScene>
#include <QTimer>
#include <QObject>
#include <vector>

class ParticlesManager : public QObject
{
    Q_OBJECT
public:
    explicit ParticlesManager(QGraphicsScene* scene, QObject* parent = nullptr);

    void reset(const Settings& settings);
    Q_SLOT void start();
    Q_SLOT void stop();

private:
    Q_SLOT void timeout();

private:
    QGraphicsScene* const m_scene;
    std::vector<Particle*> m_particles;
    QTimer m_timer;
};
