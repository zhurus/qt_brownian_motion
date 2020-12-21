#include "widget.h"

Widget::Widget(QWidget *parent):
    QWidget(parent),
    _scene(new QGraphicsScene(0,0,SCENE_WIDTH,SCENE_HEIGHT,this)),
    _view(new Viewport(_scene,this)),
    _nParticlesEdit(new QSpinBox(this)),
    _particleSizeEdit(new QSpinBox(this)),
    _speedLimitEdit(new QSpinBox(this)),
    _dtEdit(new QSpinBox(this)),
    _startBtn(new QPushButton(tr("Start"),this)),
    _pauseBtn(new QPushButton(tr("Pause"),this)),
    _stopBtn(new QPushButton(tr("Stop"),this)),
    _timeOutput(new QLabel("0",this))
{
    _view->scale(1,-1);
    QGridLayout* layout = new QGridLayout(this);
    layout->addWidget(_view,0,0);

    QGridLayout* sideLayout = new QGridLayout();
    layout->addLayout(sideLayout,0,1);

    _nParticlesEdit->setRange(2,10000);
    _nParticlesEdit->setValue(200);

    _particleSizeEdit->setValue(10);
    _particleSizeEdit->setMinimum(1);

    _speedLimitEdit->setValue(5);
    _speedLimitEdit->setMinimum(1);

    _dtEdit->setValue(30);
    _dtEdit->setRange(1,10000);

    sideLayout->addWidget( new QLabel(tr("Number of particles"),this),0,0 );
    sideLayout->addWidget(_nParticlesEdit,0,1);

    sideLayout->addWidget( new QLabel(tr("Size of particle"),this),1,0 );
    sideLayout->addWidget(_particleSizeEdit,1,1);

    sideLayout->addWidget( new QLabel(tr("Speed limit"),this),2,0 );
    sideLayout->addWidget(_speedLimitEdit,2,1);

    sideLayout->addWidget( new QLabel(tr("Time step, msec"),this),3,0 );
    sideLayout->addWidget(_dtEdit,3,1);


    connect( _startBtn, SIGNAL(clicked()),
             this, SLOT(start()) );
    sideLayout->addWidget( _startBtn, 4,1 );

    connect( _pauseBtn, SIGNAL(clicked()),
             this, SLOT(pause()) );
    sideLayout->addWidget( _pauseBtn, 5,1 );

    connect( _stopBtn, SIGNAL(clicked()),
             this, SLOT(stop()) );
    sideLayout->addWidget( _stopBtn, 6,1 );

    sideLayout->addWidget( new QLabel(tr("Time elapsed")), 7,0);
    sideLayout->addWidget(_timeOutput, 7, 1);

    connect( &_timer, SIGNAL(timeout()),
             this, SLOT(timeout()) );
}

Widget::~Widget()
{
}

void Widget::init()
{
    int particleSize = _particleSizeEdit->value();
    int nParticles = _nParticlesEdit->value();
    int speedLimit = _speedLimitEdit->value();
    int dt = _dtEdit->value();

    for( int i = 0; i < nParticles; ++i ) {
        auto particle = make_shared<Particle>(particleSize,speedLimit);
        _particle_array.push_back( particle );
        _scene->addItem( _particle_array.back().get() );
    }
    _particle_array.front()->setInfected( true );

    _timer.setInterval(dt);
    _initialized = true;
}

void Widget::run()
{
    _timer.start();
}

void Widget::timeout()
{
    for( auto particle : _particle_array )
        particle->move();
    _scene->update(_scene->sceneRect());
    checkHits();
    increaseTime();
}

void Widget::start()
{
    if( !_initialized ) {
        _particleSizeEdit->setDisabled(true);
        _speedLimitEdit->setDisabled(true);
        _dtEdit->setDisabled(true);
        _nParticlesEdit->setDisabled(true);
        init();
        _initialized = true;
    }
    run();
}

void Widget::pause()
{
    _timer.stop();
}

void Widget::stop()
{
    _timer.stop();
    _particleSizeEdit->setDisabled(false);
    _speedLimitEdit->setDisabled(false);
    _dtEdit->setDisabled(false);
    _nParticlesEdit->setDisabled(false);
    _particle_array.clear();
    _initialized = false;
    _timeOutput->setText("0");
}

void Widget::checkHits()
{
    for( size_t i = 0; i < _particle_array.size(); ++i ) {
        for( size_t j = i+1; j < _particle_array.size(); ++j ) {
            auto p1 = _particle_array[i];
            auto p2 = _particle_array[j];
            if( p1->getCenter().x() - p2->getCenter().x() > p1->getParticleSize() )
                continue;
            if( p1->getCenter().y() - p2->getCenter().y() > p1->getParticleSize() )
                continue;
            if( isHit(i,j) )
                hit(i,j);
        }
    }
}

bool Widget::isHit(int id1, int id2)
{
    double length = (QVector2D(_particle_array[id1]->getCenter()) -
            QVector2D(_particle_array[id2]->getCenter())).length();
    if( length < _particle_array.front()->getParticleSize() )
        return true;
    return false;
}

void Widget::hit(int id1, int id2)
{
    QVector2D speed1 = _particle_array[id1]->getSpeed();
    QVector2D speed2 = _particle_array[id2]->getSpeed();
    QPointF p1 = _particle_array[id1]->getCenter();
    QPointF p2 = _particle_array[id2]->getCenter();

    QVector2D n = QVector2D( p2.rx() - p1.rx(),
                             p2.ry() - p1.ry() ).normalized();
    QVector2D tau = { n.y(), -n.x() };

    double vn1 = QVector2D::dotProduct(speed1, n);
    double vt1 = QVector2D::dotProduct(speed1,tau);
    double vn2 = QVector2D::dotProduct(speed2, n);
    double vt2 = QVector2D::dotProduct(speed2,tau);
    if( vn1 < 0 && vn2 > 0 )
        return;

    double vn2_ = vn1;
    double vn1_ = vn1 + vn2 - vn2_;

    _particle_array[id1]->setSpeed( vt1 * tau + vn1_ * n );
    _particle_array[id2]->setSpeed( vt2 * tau + vn2_ * n );
    if( _particle_array[id1]->isInfected() )
        _particle_array[id2]->setInfected(true);
    if( _particle_array[id2]->isInfected() )
        _particle_array[id1]->setInfected(true);
}

void Widget::increaseTime()
{
    QString newText = QString::number(_timeOutput->text().toDouble() + double(_timer.interval()) / 1000,'f',3);
    _timeOutput->setText( newText );
}

