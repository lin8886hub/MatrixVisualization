#include "pointspanel.h"
#include <QVBoxLayout>
#include <QLabel>

PointsPanel::PointsPanel(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout;

    for (int i = 0; i < 4; ++i)
    {
        labels[i] = new QLabel("Point", this);
        layout->addWidget(labels[i]);
    }

    layout->addStretch();
    setLayout(layout);
}

void PointsPanel::setPoints(const QVector<QPointF> &pts)
{
    for (int i = 0; i < pts.size() && i < 4; ++i)
    {
        QString text = QString("P%1: (%2 , %3)")
        .arg(i)
            .arg(pts[i].x(), 0, 'f', 2)
            .arg(pts[i].y(), 0, 'f', 2);

        labels[i]->setText(text);
    }
}