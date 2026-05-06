#ifndef CANVAS2DWIDGET_H
#define CANVAS2DWIDGET_H

#include <QWidget>
#include <QMatrix3x3>

class Canvas2DWidget : public QWidget
{
    Q_OBJECT

public:
    explicit Canvas2DWidget(QWidget *parent = nullptr);
    void resetTransform();
    void setTransform(const QMatrix3x3 &mat);

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    QMatrix3x3 multiply(const QMatrix3x3 &A, const QMatrix3x3 &B);

private:
    QMatrix3x3 transform;

    QPointF applyTransform(const QPointF &p);

signals:
    void pointsUpdated(const QVector<QPointF> &pts);
    void matrixUpdated(const QMatrix3x3 &mat);
    void backToMenuRequested();
};

#endif