#ifndef POINTSPANEL_H
#define POINTSPANEL_H

#include <QWidget>
#include <QPointF>
#include <QVector>

class QLabel;

class PointsPanel : public QWidget
{
    Q_OBJECT

public:
    explicit PointsPanel(QWidget *parent = nullptr);

    void setPoints(const QVector<QPointF> &pts);

private:
    QLabel *labels[4];
};

#endif