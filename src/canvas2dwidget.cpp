#include "canvas2dwidget.h"
#include <QPainter>
#include <QKeyEvent>
#include <cmath>

Canvas2DWidget::Canvas2DWidget(QWidget *parent)
    : QWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);

    transform.setToIdentity();
}

void Canvas2DWidget::setTransform(const QMatrix3x3 &mat)
{
    transform = mat;
    emit matrixUpdated(transform);
    update();
}

QPointF Canvas2DWidget::applyTransform(const QPointF &p)
{
    float x = p.x();
    float y = p.y();

    float nx = transform(0,0)*x + transform(0,1)*y + transform(0,2);
    float ny = transform(1,0)*x + transform(1,1)*y + transform(1,2);

    return QPointF(nx, ny);
}
void Canvas2DWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.translate(width()/2, height()/2);

    painter.drawLine(-200, 0, 200, 0);
    painter.drawLine(0, -200, 0, 200);

    QVector<QPointF> rect = {
        {-50, -30},
        {50, -30},
        {50, 30},
        {-50, 30}
    };

    QPolygonF poly;
    QVector<QPointF> transformed;

    for (auto &p : rect)
    {
        QPointF tp = applyTransform(p);
        poly << tp;
        transformed << tp;
    }

    painter.setBrush(Qt::cyan);
    painter.drawPolygon(poly);

    //
    emit pointsUpdated(transformed);
}

void Canvas2DWidget::keyPressEvent(QKeyEvent *event)
{
    QMatrix3x3 T;
    T.setToIdentity();

    float step = 5.0f;
    float angle = 5.0f * M_PI / 180.0f;
    float scale = 1.1f;

    switch (event->key())
    {
    //平移
    case Qt::Key_W:
        T(1,2) = -step;
        break;
    case Qt::Key_S:
        T(1,2) = step;
        break;
    case Qt::Key_A:
        T(0,2) = -step;
        break;
    case Qt::Key_D:
        T(0,2) = step;
        break;

    //旋转
    case Qt::Key_Q:
        T(0,0) = cos(angle);
        T(0,1) = -sin(angle);
        T(1,0) = sin(angle);
        T(1,1) = cos(angle);
        break;

    case Qt::Key_E:
        T(0,0) = cos(-angle);
        T(0,1) = -sin(-angle);
        T(1,0) = sin(-angle);
        T(1,1) = cos(-angle);
        break;

    //缩放
    case Qt::Key_Z:
        T(0,0) = scale;
        T(1,1) = scale;
        break;

    case Qt::Key_X:
        T(0,0) = 1.0f / scale;
        T(1,1) = 1.0f / scale;
        break;

    case Qt::Key_R:
        resetTransform();
        break;

    case Qt::Key_O:
        emit backToMenuRequested();
        break;

    default:
        return;
    }

    //核心：矩阵组合
    transform = multiply(T, transform);
    emit matrixUpdated(transform);
    update();
}
QMatrix3x3 Canvas2DWidget::multiply(const QMatrix3x3 &A, const QMatrix3x3 &B)
{
    QMatrix3x3 R;

    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
        {
            R(i,j) = 0;
            for (int k = 0; k < 3; ++k)
                R(i,j) += A(i,k) * B(k,j);
        }

    return R;
}

void Canvas2DWidget::resetTransform()
{
    QMatrix3x3 m;
    m.setToIdentity();

    setTransform(m);
}