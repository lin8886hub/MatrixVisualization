#ifndef MATRIXPANEL_H
#define MATRIXPANEL_H

#include <QWidget>
#include <QMatrix3x3>

class QLineEdit;

class MatrixPanel : public QWidget
{
    Q_OBJECT

public:
    explicit MatrixPanel(QWidget *parent = nullptr);
    void setMatrix(const QMatrix3x3 &mat);

signals:
    void matrixChanged(const QMatrix3x3 &mat);

private slots:
    void onEditChanged();

private:
    QLineEdit *edits[3][3];

    QMatrix3x3 getMatrix();
};

#endif