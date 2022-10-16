#include "viewport.h"

Viewport::Viewport(QGraphicsScene* scene, QWidget* parent):
    QGraphicsView(scene, parent)
{
}

void Viewport::resizeEvent(QResizeEvent* event)
{
    fitInView(sceneRect(),Qt::AspectRatioMode::KeepAspectRatio);
}
