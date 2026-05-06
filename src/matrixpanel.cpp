#include "matrixpanel.h"
#include <QGridLayout>
#include <QLineEdit>

MatrixPanel::MatrixPanel(QWidget *parent)
    : QWidget(parent)
{
    QGridLayout *layout = new QGridLayout;

    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            edits[i][j] = new QLineEdit(this);
            edits[i][j]->setText(i == j ? "1" : "0");

            layout->addWidget(edits[i][j], i, j);

            connect(edits[i][j], &QLineEdit::editingFinished,
                    this, &MatrixPanel::onEditChanged);
        }
    }

    setLayout(layout);
}

QMatrix3x3 MatrixPanel::getMatrix()
{
    QMatrix3x3 m;

    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            m(i,j) = edits[i][j]->text().toFloat();

    return m;
}

void MatrixPanel::onEditChanged()
{
    emit matrixChanged(getMatrix());
}

void MatrixPanel::setMatrix(const QMatrix3x3 &m)
{
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            edits[i][j]->blockSignals(true);
            edits[i][j]->setText(QString::number(m(i,j)));
            edits[i][j]->blockSignals(false);
        }
    }
}