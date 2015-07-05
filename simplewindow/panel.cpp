#include "panel.h"
#include "ui_panel.h"
#include <QDesktopWidget>
#include <QDebug>
#include "FullscreenHelper.h"

Panel::Panel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Panel), mFrameless(new FramelessHelper)
{
    m_simpleWindow = dynamic_cast<SimpleWindow*>(parent);

    m_simpleWindow->setGeometry(100,100,100,100);
    qDebug() << m_simpleWindow->size();

    connect(m_simpleWindow, &SimpleWindow::myResizeEvent, [=](QResizeEvent *event){
        qDebug() << event->size();
    });

    //qDebug()<< "ParentWi in Panel" <<parentWi->isEnabled() <<parentWi<<"parent in Panel: "<< parent;
    ui->setupUi(this);
    FullScreenHelper::MaximizeOnVirtualScreen(this);

    mFrameless->activateOn(this);
    mFrameless->setWidgetMovable(true);

    setWindowFlags(Qt::Drawer|Qt::CustomizeWindowHint);
    QPoint posOnDesktop = QApplication::desktop()->geometry().topRight();
    posOnDesktop.setX(posOnDesktop.x() - size().width());
    this->move(posOnDesktop);

    fillComboBox();
    settingSpinBox();
    connect(ui->CancelButton,SIGNAL(pressed()),this,SLOT(cancelBtnPressed()));
    connect(ui->CropButton, SIGNAL(pressed()), this, SLOT(cropBtnPressed()));
    connect(ui->comboBox, SIGNAL(activated(int)),this,SLOT(comBoxSelection(int)));

    //connect(ui->widthSpinBox,SIGNAL(valueChanged(int)),this->parentWi,SLOT(setWidth(int)));
}

Panel::~Panel()
{
    delete ui;
}

void Panel::closeEvent(QCloseEvent *event)
{
  m_simpleWindow->close();
}

void Panel::cropBtnPressed()
{
    qDebug()<< "Crop button pressed!!!";
    this->close();

    //qDebug() << m_simpleWindow->size();
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
    ui->comboBox->addItem("(HD) 720p: 1280x720");
    ui->comboBox->addItem("(Full HD) 1080p: 1920x1080");
    ui->comboBox->addItem("(4k) 2160p: 3840x2160");
    ui->comboBox->addItem("Custom size...");

}

void Panel::comBoxSelection(int activated)
{
    QRect newRect;
    switch (activated) {
    case 1:
        qDebug()<<ui->comboBox->currentText();
        newRect=QRect(m_simpleWindow->geometry().topLeft(),QSize(720,480));
        qDebug()<<newRect;
        m_simpleWindow->setGeometry(newRect);
        qDebug()<<"Resized 720x480";
        qDebug()<<m_simpleWindow->geometry();
        break;
    default:
        break;
    }
}

void Panel::settingSpinBox()
{
   ui->widthSpinBox->setMinimum(0);
   ui->heightSpinBox->setMinimum(0);
   ui->widthSpinBox->setMaximum(qApp->desktop()->width());
   ui->heightSpinBox->setMaximum(qApp->desktop()->height());

   ui->widthSpinBox->setValue(m_simpleWindow->geometry().width());
   ui->heightSpinBox->setValue(m_simpleWindow->geometry().height());

}

void Panel::settingLockButton()
{

}
