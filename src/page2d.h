#ifndef PAGE2D_H
#define PAGE2D_H

#include <QWidget>
#include <QMatrix3x3>

class Canvas2DWidget;
class MatrixPanel;
 class PointsPanel;

class Page2D : public QWidget
{
    Q_OBJECT

public:
    explicit Page2D(QWidget *parent = nullptr);

private slots:
    void onMatrixChanged(const QMatrix3x3 &mat);

private:
    Canvas2DWidget *canvas;
    MatrixPanel *panel;
signals:
    void backToMenuRequested();
};

#endif