#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPainter>
#include <QVector3D>
#include <QMatrix4x4>
#include <QKeyEvent>
#include <QTimer>
#include <QLineEdit>
#include <QGridLayout>

class Page3D : public QWidget
{
    Q_OBJECT

public:
    Page3D(QWidget *parent = nullptr);
    ~Page3D() override;

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    QTimer *m_timer;
    QPointF project(const QVector3D &point) const;
    void drawAxes(QPainter &painter);
    // 渲染矩阵数值到屏幕
    void drawMatrixInfo(QPainter &painter, const QMatrix4x4 &matrix);

    QMatrix4x4 m_worldMatrix; // 存储当前的变换矩阵

    // 4x4 的输入框矩阵
    QLineEdit* m_inputs[4][4];
    bool m_isUpdating = false;

private slots:
    // 当输入框内容改变时，更新矩阵
    void updateMatrixFromUI();
    // 当矩阵因按键改变时，更新输入框显示
    void updateUIFromMatrix();




};

#endif