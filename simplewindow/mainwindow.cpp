#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

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
    win->setAutoClose(!ui->checkBox->isChecked());
    win->show();

    connect(win.get(), &SimpleWindow::onGrabed, [=](QSize rect)
    {
        QMessageBox msgBox;
        win->close();
        msgBox.setText("done!");
        msgBox.exec();
        msgBox.show();
    });
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if(!win)
    return;

    win->close();
}
