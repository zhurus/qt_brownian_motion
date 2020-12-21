#pragma once


#include "Particle.h"

#include <QWidget>
#include "Viewport.h"
#include <QGraphicsScene>
#include <QGridLayout>
#include <QTimer>

#include <QSpinBox>
#include <QLabel>
#include <QPushButton>

#include <vector>


using namespace std;

class Widget : public QWidget
{
    Q_OBJECT
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void init();
    void run();

private slots:
    void timeout();
    void start();
    void pause();
    void stop();

private:
    void checkHits();
    bool isHit(int id1, int id2);
    void hit(int id1, int id2);
    void increaseTime();

    vector<PtrParticle> _particle_array;
    QTimer _timer;
    bool _initialized = {false};

    QGraphicsScene* _scene;
    QGraphicsView* _view;

    QSpinBox* _nParticlesEdit;
    QSpinBox* _particleSizeEdit;
    QSpinBox* _speedLimitEdit;
    QSpinBox* _dtEdit;

    QPushButton* _startBtn;
    QPushButton* _pauseBtn;
    QPushButton* _stopBtn;

    QLabel* _timeOutput;
};
