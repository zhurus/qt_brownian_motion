#include "viewport.h"

Viewport::Viewport(QGraphicsScene* scene, QWidget* parent):
    QGraphicsView(scene, parent)
{
}

void Viewport::resizeEvent(QResizeEvent* event)
{
    Q_UNUSED(event)
    fitInView(sceneRect(),Qt::AspectRatioMode::KeepAspectRatio);
}

void Viewport::mousePressEvent(QMouseEvent* event)
{
    if(event->button() == Qt::MouseButton::LeftButton) {
        emit mouseStateChanged( true, event->pos() );
    }
}

void Viewport::mouseReleaseEvent(QMouseEvent* event)
{
    if(event->button() == Qt::MouseButton::LeftButton) {
        emit mouseStateChanged( false, event->pos() );
    }
}

void Viewport::mouseMoveEvent(QMouseEvent* event)
{
    if(event->buttons().testFlag(Qt::MouseButton::LeftButton)) {
        emit mouseStateChanged(true, event->pos());
    }
}
