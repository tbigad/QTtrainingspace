#include "panel.h"
#include "ui_panel.h"
#include <QDesktopWidget>
#include <QDebug>
#include "FullscreenHelper.h"

Panel::Panel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Panel), mFrameless(new FramelessHelper)
{

    parentWi = this->parentWidget();

    ui->setupUi(this);
    FullScreenHelper::MaximizeOnVirtualScreen(this);

    mFrameless->activateOn(this);
    mFrameless->setWidgetMovable(true);

    setWindowFlags(Qt::Drawer|Qt::CustomizeWindowHint);
    QPoint posOnDesktop = QApplication::desktop()->geometry().topRight();
    posOnDesktop.setX(posOnDesktop.x() - size().width());
    this->move(posOnDesktop);

    fillComboBox();

    connect(ui->CancelButton,SIGNAL(pressed()),this,SLOT(cancelBtnPressed()));
    connect(ui->CropButton, SIGNAL(pressed()), this, SLOT(cropBtnPressed()));
    connect(ui->comboBox, SIGNAL(activated(int)),this,SLOT(comBoxSelection(int)));
}

Panel::~Panel()
{
    delete ui;
}

void Panel::closeEvent(QCloseEvent *event)
{
  parentWi->close();
}

void Panel::cropBtnPressed()
{
    qDebug()<< "Crop button pressed!!!";
    this->close();
}

void Panel::cancelBtnPressed()
{
    qDebug()<< "Cancel button pressed!!!";
    this->close();
}

void Panel::fillComboBox()
{
    ui->comboBox->addItem("Capture Fragment");
    ui->comboBox->addItem("(SD) 480p: 720x480");
}

void Panel::comBoxSelection(int activated)
{
    QRect newRect;
    switch (activated) {
    case 1:
        qDebug()<<ui->comboBox->currentText();
        newRect=QRect(parentWi->geometry().topLeft(),QSize(720,480));
        qDebug()<<newRect;
        parentWi->setGeometry(newRect);
        qDebug()<<"Resized 720x480";
        break;
    default:
        break;
    }
}
