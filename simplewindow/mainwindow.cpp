#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    if(win.use_count())
        win.reset();

    win = std::make_shared<SimpleWindow>();
    win->setChekBoxState(ui->checkBox->isChecked());
    win->show();
}

void MainWindow::closeEvent(QCloseEvent *event) {
    win.get()->close();
}
