#include "mainwindow.h"
#include "page2d.h"
#include "page3d.h"

#include <QPushButton>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    page2D = nullptr;
    page3D = nullptr;
    menuWidget = new QWidget(this);
    setCentralWidget(menuWidget);

    btn2D = new QPushButton("2D", menuWidget);
    btn3D = new QPushButton("3D", menuWidget);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(btn2D);
    layout->addWidget(btn3D);

    menuWidget->setLayout(layout);

    connect(btn2D, &QPushButton::clicked, this, &MainWindow::enter2D);
    connect(btn3D, &QPushButton::clicked, this, &MainWindow::enter3D);
    resize(640,480);
}

void MainWindow::enter2D()
{
    if (!page2D)
    {
        page2D = new Page2D(this);

        connect(page2D, &Page2D::backToMenuRequested,
                this, &MainWindow::backToMenu);
    }

    setCentralWidget(page2D);
}

void MainWindow::enter3D()
{
    if(page3D == nullptr)
    {
        page3D = new Page3D(this);
    }
    setCentralWidget(page3D);
}

void MainWindow::backToMenu()
{
    if(menuWidget != nullptr)
        setCentralWidget(menuWidget);
}

