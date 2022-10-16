#include "widget.h"

#include <QFormLayout>
#include <QHBoxLayout>
#include <QRandomGenerator>

//---------------------------------------------------------------------------//
//  ParticlesManager                                                         //
//---------------------------------------------------------------------------//

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

//---------------------------------------------------------------------------//
//  Widget                                                                   //
//---------------------------------------------------------------------------//

Widget::Widget(QWidget *parent):
    QWidget(parent),
    m_scene(new QGraphicsScene(0, 0, SCENE_WIDTH, SCENE_HEIGHT, this)),
    m_view(new Viewport(m_scene, this)),
    m_particlesCountEdit(new QSpinBox(this)),
    m_particleSizeEdit(new QSpinBox(this)),
    m_speedLimitEdit(new QSpinBox(this)),
    m_timestepEdit(new QSpinBox(this)),
    m_startBtn(new QPushButton(tr("Start"), this)),
    m_pauseBtn(new QPushButton(tr("Pause"), this)),
    m_stopBtn(new QPushButton(tr("Stop"), this))
{
    m_particlesManager = new ParticlesManager( m_scene, this );

    auto main_layout = new QHBoxLayout(this);
    main_layout->addWidget( m_view );

    auto side_layout = new QFormLayout();
    main_layout->addLayout( side_layout );

    side_layout->addRow( tr("Number of particles"), m_particlesCountEdit );
    side_layout->addRow( tr("Size of particle"), m_particleSizeEdit );
    side_layout->addRow( tr("Speed limit"), m_speedLimitEdit );
    side_layout->addRow( tr("Time step, msec"), m_timestepEdit );

    m_particlesCountEdit->setRange(2, 10000);
    m_particlesCountEdit->setValue(10);

    m_particleSizeEdit->setValue(10);
    m_particleSizeEdit->setMinimum(1);

    m_speedLimitEdit->setValue(5);
    m_speedLimitEdit->setMinimum(1);

    m_timestepEdit->setValue(30);
    m_timestepEdit->setRange(1, 10000);

    connect( m_startBtn, &QPushButton::clicked,
             this, &Widget::start );
    side_layout->addWidget( m_startBtn );

    connect( m_pauseBtn, &QPushButton::clicked,
             this, &Widget::pause );
    side_layout->addWidget( m_pauseBtn );

    connect( m_stopBtn, &QPushButton::clicked,
             this, &Widget::stop );
    side_layout->addWidget( m_stopBtn );
}

void Widget::start()
{
    Settings settings;
    settings.particle_size = m_particleSizeEdit->value();
    settings.particle_speed_limit = m_speedLimitEdit->value();
    settings.particles_count = m_particlesCountEdit->value();
    settings.timestep_msec = m_timestepEdit->value();
    if( m_state == Started ) {
        stop();
    }
    if( m_state == Idle ) {
        m_particlesManager->reset(settings);
    }
    m_particlesManager->start();
}

void Widget::pause()
{
    m_particlesManager->stop();
    m_state = Paused;
}

void Widget::stop()
{
    m_particlesManager->stop();
    m_state = Idle;
}
