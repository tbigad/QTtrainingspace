#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    test = std::make_shared<TestWidget>(new TestWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}
