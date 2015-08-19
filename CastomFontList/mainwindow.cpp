#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFontDatabase>
#include <QComboBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QFontDatabase database;
    QStringList items = database.families();
    ui->comboBox->setStyleSheet("border-radius: 4px;"
                              "background-color: #3b3b3b;"
                              "selection-background-color: #808080;"
                              "color: #ffffff;"
                              "font-size: 12pt;"
                              "down-arrow: {border:none;}");
    ui->comboBox->addItems(items);
    ui->comboBox->setMinimumWidth(200);
    ui->label->setText(ui->comboBox->currentText());
    ui->label->setFont(QFont(ui->comboBox->currentText(),12,10));

    connect(ui->comboBox,(void (QComboBox:: *)(int)) &QComboBox::highlighted,[=](int index){
        ui->label->setText(ui->comboBox->itemText(index));
        ui->label->setFont(QFont(ui->comboBox->itemText(index),12,0));
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
