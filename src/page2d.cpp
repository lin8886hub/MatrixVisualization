#include "page2d.h"
#include "canvas2dwidget.h"
#include "matrixpanel.h"
#include "pointspanel.h"
#include <QHBoxLayout>
Page2D::Page2D(QWidget *parent)
    : QWidget(parent)
{
    canvas = new Canvas2DWidget(this);
    panel = new MatrixPanel(this);
    PointsPanel *points = new PointsPanel(this);

    QHBoxLayout *layout = new QHBoxLayout;

    layout->addWidget(points, 1);   // 左
    layout->addWidget(canvas, 3);   // 中
    layout->addWidget(panel, 1);    // 右

    setLayout(layout);

    connect(panel, &MatrixPanel::matrixChanged,
            this, &Page2D::onMatrixChanged);
    connect(canvas, &Canvas2DWidget::matrixUpdated,
            panel, &MatrixPanel::setMatrix);
    //
    connect(canvas, &Canvas2DWidget::pointsUpdated,
            points, &PointsPanel::setPoints);
    connect(canvas, &Canvas2DWidget::backToMenuRequested,
            this, &Page2D::backToMenuRequested);
}

void Page2D::onMatrixChanged(const QMatrix3x3 &mat)
{
    canvas->setTransform(mat);
}