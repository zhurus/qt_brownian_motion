#pragma once

#include <QGraphicsView>
#include <QResizeEvent>
#include <QMouseEvent>

class Viewport: public QGraphicsView
{
    Q_OBJECT
public:
    Viewport(QGraphicsScene* scene,QWidget* parent = nullptr);

    void resizeEvent(QResizeEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

    Q_SIGNAL void mouseStateChanged(bool pressed, const QPointF& pos);
};

