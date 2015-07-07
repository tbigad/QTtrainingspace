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

    connect(m_simpleWindow, &SimpleWindow::resizeSimpleWindow, [=](QResizeEvent *event){
        ui->widthSpinBox->setValue(event->size().width());
        ui->heightSpinBox->setValue(event->size().height());
    });

    //qDebug()<< "ParentWi in Panel" <<parentWi->isEnabled() <<parentWi<<"parent in Panel: "<< parent;
    ui->setupUi(this);

    mFrameless->activateOn(this);
    mFrameless->setWidgetMovable(true);

    setWindowFlags(Qt::Drawer|Qt::CustomizeWindowHint);
    QPoint posOnDesktop = QApplication::desktop()->geometry().topRight();
    posOnDesktop.setX(posOnDesktop.x() - size().width());
    this->move(posOnDesktop);

    fillComboBox();
    settingSpinBox();
    settingLockButton();

    connect(ui->CancelButton,SIGNAL(pressed()),this,SLOT(cancelBtnPressed()));
    connect(ui->CropButton, SIGNAL(pressed()), this, SLOT(cropBtnPressed()));
    connect(ui->comboBox, SIGNAL(activated(int)),this,SLOT(comBoxSelection(int)));

    connect(ui->widthSpinBox,SIGNAL(valueChanged(int)),m_simpleWindow,SLOT(setWidth(int)));
    connect(ui->heightSpinBox, SIGNAL(valueChanged(int)),m_simpleWindow, SLOT(setHeight(int)));

    this->show();
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

}

void Panel::cancelBtnPressed()
{
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
    QRect newRect(m_simpleWindow->geometry());
    switch (activated) {
    case 1:
        newRect.setSize(QSize(720,480));
        m_simpleWindow->setGeometry(newRect);
        break;
    case 2:
        newRect.setSize(QSize(1280,720));
        m_simpleWindow->setGeometry(newRect);
    case 3:
        newRect.setSize(QSize(1920,1080));
        m_simpleWindow->setGeometry(newRect);
    case 4:
        newRect.setSize(QSize(3840,2160));
        m_simpleWindow->setGeometry(newRect);
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
    QIcon icon(":/icons/lock.png");
    ui->lockButton->setIcon(icon);

}
