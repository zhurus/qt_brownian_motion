#include "particles_manager.h"

#include <QRandomGenerator>

ParticlesManager::ParticlesManager(QGraphicsScene* scene, QObject* parent)
    : QObject(parent),
      m_scene(scene)
{
    connect(&m_timer, &QTimer::timeout, this, &ParticlesManager::timeout);
}

void ParticlesManager::reset(const Settings& settings)
{
    m_timer.stop();
    m_timer.setInterval(settings.timestep_msec);

    m_scene->clear();
    m_particles.clear();

    m_particles.reserve(settings.particles_count);
    int min_x = settings.particle_size / 2;
    int max_x = SCENE_WIDTH - settings.particle_size / 2;
    int min_y = min_x;
    int max_y = SCENE_HEIGHT - settings.particle_size / 2;
    for( int i = 0; i < settings.particles_count; ++i ) {
        QPointF center( QRandomGenerator::global()->bounded( min_x, max_x ),
                        QRandomGenerator::global()->bounded( min_y, max_y ) );
        auto particle = new GraphicsParticle( center, settings.particle_size, settings.particle_speed_limit );
        if( i == 0 ) {
            particle->setInfected(true);
        }
        m_particles.push_back(particle);
        m_scene->addItem(particle);
    }
}

void ParticlesManager::start()
{
    m_timer.start();
}

void ParticlesManager::stop()
{
    m_timer.stop();
}

void ParticlesManager::timeout()
{
    for( size_t i = 0; i < m_particles.size(); ++i ) {
        for( size_t j = i+1; j < m_particles.size(); ++j ) {
            m_particles[i]->checkContact( m_particles[j] );
        }
    }

    for( auto particle : m_particles ) {
        particle->move();
    }
    m_scene->update(m_scene->sceneRect());
}
