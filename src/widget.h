#pragma once

#include "particle.h"
#include "viewport.h"
#include <QWidget>
#include <QGraphicsScene>
#include <QTimer>
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <vector>


//---------------------------------------------------------------------------//
//  Settings                                                                 //
//---------------------------------------------------------------------------//

struct Settings
{
    int timestep_msec;
    int particles_count;
    int particle_size;
    int particle_speed_limit;
};

//---------------------------------------------------------------------------//
//  ParticlesManager                                                         //
//---------------------------------------------------------------------------//

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

//---------------------------------------------------------------------------//
//  Widget                                                                   //
//---------------------------------------------------------------------------//

class Widget : public QWidget
{
    Q_OBJECT
public:
    enum State {
        Idle,
        Started,
        Paused
    };
public:
    Widget(QWidget *parent = nullptr);

private:
    Q_SLOT void start();
    Q_SLOT void pause();
    Q_SLOT void stop();

private:
    State m_state = Idle;

    ParticlesManager* m_particlesManager;

    QGraphicsScene* m_scene;
    QGraphicsView* m_view;

    QSpinBox* m_particlesCountEdit;
    QSpinBox* m_particleSizeEdit;
    QSpinBox* m_speedLimitEdit;
    QSpinBox* m_timestepEdit;

    QPushButton* m_startBtn;
    QPushButton* m_pauseBtn;
    QPushButton* m_stopBtn;
};
