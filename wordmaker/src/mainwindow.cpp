#include "mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    xlsx = new QXlsx::Document(":/db/db.xlsx");
    cell = xlsx->dimension();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete xlsx;
}
