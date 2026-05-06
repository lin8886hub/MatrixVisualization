#include "page3d.h"

Page3D::Page3D(QWidget *parent) : QWidget(parent)
{
    resize(1200, 800);
    setWindowTitle("Matrix Transformation Lab (Qt 6)");
    m_worldMatrix.setToIdentity(); // 初始为单位矩阵
    setFocusPolicy(Qt::StrongFocus); // 确保窗口能接收键盘事件
    m_timer = new QTimer(this);

    // 连接信号：每当时间到了，就执行 update()
    connect(m_timer, &QTimer::timeout, this, QOverload<>::of(&Page3D::update));

    // 启动定时器：16毫秒刷新一次 (1000ms / 60fps ≈ 16.67ms)
    m_timer->start(16);

    // 创建一个半透明的面板放在左侧存放输入框
    QWidget *controlPanel = new QWidget(this);
    controlPanel->setStyleSheet("background-color: rgba(50, 50, 50, 200); color: white;");
    QGridLayout *gridLayout = new QGridLayout(controlPanel);

    for (int r = 0; r < 4; ++r) {
        for (int c = 0; c < 4; ++c) {
            m_inputs[r][c] = new QLineEdit(controlPanel);
            m_inputs[r][c]->setFixedWidth(60);
            m_inputs[r][c]->setAlignment(Qt::AlignCenter);
            gridLayout->addWidget(m_inputs[r][c], r, c);

            // 连接信号：文本改变就更新矩阵
            connect(m_inputs[r][c], &QLineEdit::textChanged, this, &Page3D::updateMatrixFromUI);
        }
    }

    // 放置面板位置
    controlPanel->move(20, 220);
    updateUIFromMatrix(); // 初始化显示

}

Page3D::~Page3D() = default;

void Page3D::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), QColor(30, 30, 30));

    // 1. 绘制矩阵信息
    drawMatrixInfo(painter, m_worldMatrix);

    // 2. 绘制左侧坐标列表
    painter.setPen(Qt::cyan);
    int startX = 20;
    int startY = 420;
    painter.drawText(startX, startY, "Transformed Vertices (World Space):");

    float w = 100, h = 60, d = 80;
    QVector3D localV[8] = {
        {0, 0, 0}, {w, 0, 0}, {w, 0, d}, {0, 0, d},
        {0, -h, 0}, {w, -h, 0}, {w, -h, d}, {0, -h, d}
    };

        QPointF screenP[8];
    QVector3D transformedV[8];
    for(int i = 0; i < 8; ++i) {
        transformedV[i] = m_worldMatrix.map(localV[i]);
            screenP[i] = project(transformedV[i]);

            // 绘制左侧详细列表
            QString vInfo = QString("V%1: (%2, %3, %4)")
                  .arg(i)
                  .arg(transformedV[i].x(), 6, 'f', 1)
                  .arg(transformedV[i].y(), 6, 'f', 1)
                  .arg(transformedV[i].z(), 6, 'f', 1);
        painter.drawText(startX, startY + 25 + (i * 20), vInfo);
    }

    // --- 开始绘制 3D 图形 ---
    painter.save();
    painter.translate(width() / 2.0, height() / 2.0);

        // 3. 绘制参考坐标轴[cite: 4]
        drawAxes(painter);

    // 4. 渲染长方体连线
    painter.setPen(QPen(Qt::yellow, 2));
    for(int i = 0; i < 4; ++i) {
        painter.drawLine(screenP[i], screenP[(i + 1) % 4]);
        painter.drawLine(screenP[i+4], screenP[(i + 1) % 4 + 4]);
        painter.drawLine(screenP[i], screenP[i+4]);
    }

    // 5. 在长方体上标注简易编号 (V0, V1...)
    painter.setPen(Qt::white);
    QFont smallFont = painter.font();
    smallFont.setPointSize(10);
    smallFont.setBold(true);
    painter.setFont(smallFont);

    for(int i = 0; i < 8; ++i) {
        // 在顶点位置稍微偏移一点（比如 x+5, y-5），避免压线
        painter.drawText(screenP[i] + QPointF(5, -5), QString("V%1").arg(i));
    }

    painter.restore();
}

void Page3D::keyPressEvent(QKeyEvent *event)
{
    // 根据按键修改矩阵（这里直接对当前矩阵进行累加变换）
    switch (event->key()) {
    case Qt::Key_W: m_worldMatrix.translate(0, -5, 0); break; // 向上平移
    case Qt::Key_S: m_worldMatrix.translate(0, 5, 0);  break; // 向下平移
    case Qt::Key_A: m_worldMatrix.rotate(5, 0, 1, 0);  break; // 绕Y轴旋转
    case Qt::Key_D: m_worldMatrix.rotate(-5, 0, 1, 0); break;
    case Qt::Key_Q: m_worldMatrix.scale(1.1f); break;         // 缩放
    case Qt::Key_E: m_worldMatrix.scale(0.9f); break;
    case Qt::Key_R: m_worldMatrix.setToIdentity(); break;     // 重置矩阵
    }

    updateUIFromMatrix(); // 同步更新输入框里的数字
    update();

}

void Page3D::drawMatrixInfo(QPainter &painter, const QMatrix4x4 &matrix)
{
    painter.setPen(Qt::white);
    QFont font = painter.font();
    font.setFamily("Courier New"); // 等宽字体更美观
    font.setPointSize(12);
    painter.setFont(font);

    QString title = "Current Transformation Matrix:";
    painter.drawText(20, 40, title);

    // 提取矩阵 4x4 的数据
    for (int row = 0; row < 4; ++row) {
        QString rowStr = "| ";
        for (int col = 0; col < 4; ++col) {
            rowStr += QString("%1 ").arg((double)matrix(row, col), 6, 'f', 2);
        }
        rowStr += "|";
        painter.drawText(20, 70 + row * 25, rowStr);
    }

    painter.drawText(20, 180, "Controls: W/S:Move, A/D:Rotate, Q/E:Scale, R:Reset");
}

void Page3D::drawAxes(QPainter &painter)
{
    // 设置字体
    QFont font = painter.font();
    font.setBold(true);
    font.setPointSize(10);
    painter.setFont(font);

    // 绘制 X 轴 (红色)
    painter.setPen(QPen(Qt::red, 2));
    QPointF xEnd = project({200, 0, 0});
    painter.drawLine(project({0, 0, 0}), xEnd);
    painter.drawText(xEnd + QPointF(5, 5), "X"); // 在轴末端标注

    // 绘制 Y 轴 (绿色) - 注意：Qt 的屏幕坐标 Y 是向下的，这里我们向上画
    painter.setPen(QPen(Qt::green, 2));
    QPointF yEnd = project({0, -200, 0});
    painter.drawLine(project({0, 0, 0}), yEnd);
    painter.drawText(yEnd + QPointF(5, -5), "Y");

    // 绘制 Z 轴 (蓝色)
    painter.setPen(QPen(Qt::blue, 2));
    QPointF zEnd = project({0, 0, 200});
    painter.drawLine(project({0, 0, 0}), zEnd);
    painter.drawText(zEnd + QPointF(5, 5), "Z");
}

QPointF Page3D::project(const QVector3D &point) const
{
    static const float radX = qDegreesToRadians(30.0f);
    static const float radY = qDegreesToRadians(-45.0f);
    float x1 = point.x() * cos(radY) + point.z() * sin(radY);
    float z1 = -point.x() * sin(radY) + point.z() * cos(radY);
    float y2 = point.y() * cos(radX) - z1 * sin(radX);
    return QPointF(x1, y2);
}

// 从界面输入更新到矩阵对象
void Page3D::updateMatrixFromUI()
{
    if (m_isUpdating) return;

    m_isUpdating = true;
    for (int r = 0; r < 4; ++r) {
        for (int c = 0; c < 4; ++c) {
            float val = m_inputs[r][c]->text().toFloat();
            m_worldMatrix(r, c) = val;
        }
    }
    m_isUpdating = false;
    update(); // 触发重绘[cite: 4]
}

// 从矩阵对象更新到界面显示
void Page3D::updateUIFromMatrix()
{
    m_isUpdating = true;
    for (int r = 0; r < 4; ++r) {
        for (int c = 0; c < 4; ++c) {
            m_inputs[r][c]->setText(QString::number(m_worldMatrix(r, c), 'f', 2));
        }
    }
    m_isUpdating = false;
}
