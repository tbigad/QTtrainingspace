#include "panel.h"
#include "ui_panel.h"
#include <QDesktopWidget>

Panel::Panel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Panel)
{
    ui->setupUi(this);
    QPoint posOnDesktop = QApplication::desktop()->geometry().topRight();
    posOnDesktop.setX(posOnDesktop.x() - 100);
    this->move(posOnDesktop);

    connect(ui->CancelButton,SIGNAL(pressed()),this,SLOT(close()));
}

Panel::~Panel()
{
    delete ui;
}
