#pragma once

#include <QGraphicsView>
#include <QResizeEvent>

class Viewport: public QGraphicsView
{
public:
    Viewport(QGraphicsScene* scene,QWidget* parent = nullptr);
    ~Viewport() = default;

    void resizeEvent(QResizeEvent* event) override;
};

