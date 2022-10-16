#pragma once

#include "particles_manager.h"
#include "viewport.h"
#include "settings.h"
#include <QWidget>
#include <QGraphicsScene>
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <vector>


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
