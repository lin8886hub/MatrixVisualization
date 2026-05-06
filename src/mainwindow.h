#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QPushButton;
class Page2D;
class Page3D;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void enter2D();
    void enter3D();
    void backToMenu();
private:
    QWidget *menuWidget;
    QPushButton *btn2D;
    QPushButton *btn3D;

    Page2D *page2D;
    Page3D *page3D;
};

#endif