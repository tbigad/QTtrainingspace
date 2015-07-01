#include "panel.h"
#include "ui_panel.h"
#include <QDesktopWidget>
#include <QDebug>
#include "FullscreenHelper.h"

Panel::Panel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Panel), mFrameless(new FramelessHelper)
{
    ui->setupUi(this);
    FullScreenHelper::MaximizeOnVirtualScreen(this);

    mFrameless->activateOn(this);
    mFrameless->setWidgetMovable(true);

    setWindowFlags(Qt::Drawer|Qt::CustomizeWindowHint);
    QPoint posOnDesktop = QApplication::desktop()->geometry().topRight();
    posOnDesktop.setX(posOnDesktop.x() - size().width());
    this->move(posOnDesktop);


    connect(ui->CancelButton,SIGNAL(pressed()),this,SLOT(cancelBtnPressed()));
    connect(ui->CropButton, SIGNAL(pressed()), this, SLOT(cropBtnPressed()));
}

Panel::~Panel()
{
    delete ui;
}

void Panel::closeEvent(QCloseEvent *event)
{
  QWidget *parentWi = this->parentWidget();
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