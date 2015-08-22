#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFontDatabase>
#include <QComboBox>
#include <QDebug>
#include <QAction>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QFontDatabase database;
    QStringList fonts;
    fonts <<"Arial"<<"Comic Sans MS"<<"Courier New"<<"Georgia"<<"Impact"<<"Times New Roman"
                            <<"Trebuchet MS"<<"Verdana";
    ui->comboBox->setStyleSheet("border-radius: 4px;"
                              "background-color: #3b3b3b;"
                              "selection-background-color: #808080;"
                              "color: #ffffff;"
                              "font-size: 12pt;"
                              "QComboBox::drop-down {border-width: 0px;} QComboBox::down-arrow {image: url(noimg); border-width: 0px;}");
    ui->comboBox->addItems(fonts);
    ui->comboBox->setMinimumWidth(200);
    ui->label->setText(ui->comboBox->currentText());
    ui->label->setFont(QFont(ui->comboBox->currentText(),12,10));

    connect(ui->comboBox,(void (QComboBox:: *)(int)) &QComboBox::highlighted,[=](int index){
        ui->label->setText(ui->comboBox->itemText(index));
        ui->label->setFont(QFont(ui->comboBox->itemText(index),12,0));
    });
//-----------------------------------------------------------------------------------------------------------
    QMenu *menu = new QMenu();
    ui->pushButton->setMenu(menu);
    menu->setStyleSheet("border-radius: 4px;"
                        "background-color: #3b3b3b;"
                        "selection-background-color: #808080;"
                        "color: #ffffff;"
                        "font-size: 12pt;");
    QStringList list;
    list <<"Arial"<<"Comic Sans MS"<<"Courier New"<<"Georgia"<<"Impact"<<"Times New Roman"
            <<"Trebuchet MS"<<"Verdana";
    foreach (QString fontFamily, list) {
        if(database.hasFamily(fontFamily)){
            QAction *item = new QAction(menu);
            item->setText(fontFamily);
            item->setFont(QFont(fontFamily));
            menu->addAction(item);
        }
    }
    connect(menu, &QMenu::hovered, [=](QAction *item){
        ui->label->setText(item->text());
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
