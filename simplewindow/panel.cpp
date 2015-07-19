#include "panel.h"
#include "ui_panel.h"
#include <QDesktopWidget>
#include <QDebug>
#include "FullscreenHelper.h"
#include <cmath>
#include <vector>

#define IMAGE_UNLOCK ":/icons/un_lock.png"
#define IMAGE_LOCK ":/icons/lock.png"

Panel::Panel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Panel), m_Frameless(new FramelessHelper)
{
    m_simpleWindow = dynamic_cast<SimpleWindow*>(parent);

    connect(m_simpleWindow, &SimpleWindow::resizeSimpleWindow, [=](QResizeEvent *event){
        ui->widthSpinBox->setValue(event->size().width());
        ui->heightSpinBox->setValue(event->size().height());
    });

    ui->setupUi(this);

    m_Frameless->activateOn(this);
    m_Frameless->setWidgetMovable(true);
    m_Frameless->setWidgetResizable(false);

    FullScreenHelper::MaximizeOnVirtualScreen(this);

    setWindowFlags(Qt::Drawer|Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    settingWidgetPosition();

    initImageSizeComboBox();
    initImageSizeSpinBox();
    initImageSizeLockButton();

    connect(ui->CancelButton,SIGNAL(pressed()),this,SLOT(cancelBtnPressed()));
    connect(ui->CropButton, SIGNAL(pressed()), this, SLOT(cropBtnPressed()));
    connect(ui->comboBox, SIGNAL(activated(int)),this,SLOT(comBoxSelection(int)));

    connect(ui->widthSpinBox, SIGNAL(valueChanged(int)), this, SLOT(chekLockerAndResize()));
    connect(ui->heightSpinBox, SIGNAL(valueChanged(int)), this, SLOT(chekLockerAndResize()));

    connect(ui->lockButton,SIGNAL(clicked(bool)),this,SLOT(setLockButtonIcon(bool)));

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
    this->hide();
    m_simpleWindow->setGrabed();
    this->close();
}

void Panel::cancelBtnPressed()
{
    this->close();
}

void Panel::initImageSizeComboBox()
{
    ui->comboBox->addItem("Custom size...");
    ui->comboBox->addItem("(SD) 480p: 720x480");
    ui->comboBox->addItem("(HD) 720p: 1280x720");
    if(qApp->desktop()->geometry().contains(1920,1080))
    ui->comboBox->addItem("(Full HD) 1080p: 1920x1080");
    if(qApp->desktop()->geometry().contains(3840,2160))
    ui->comboBox->addItem("(Ultra HD) 2160p: 3840x2160");
}

void Panel::comBoxSelection(int activated)
{
    QRect newRect(m_simpleWindow->geometry());
    switch (activated) {
    case 1:
        newRect.setSize(QSize(720,480));
        m_simpleWindow->setGeometry(newRect);
        ui->comboBox->setCurrentIndex(0);
        break;
    case 2:
        newRect.setSize(QSize(1280,720));
        m_simpleWindow->setGeometry(newRect);
        ui->comboBox->setCurrentIndex(0);
        break;
    case 3:
        newRect.setSize(QSize(1920,1080));
        m_simpleWindow->setGeometry(newRect);
        ui->comboBox->setCurrentIndex(0);
        break;
    case 4:
        newRect.setSize(QSize(3840,2160));
        m_simpleWindow->setGeometry(newRect);
        ui->comboBox->setCurrentIndex(0);
        break;
    default:
        break;
    }
}


void Panel::initImageSizeSpinBox()
{
   ui->widthSpinBox->setMinimum(1);
   ui->heightSpinBox->setMinimum(1);

   ui->widthSpinBox->setMaximum(qApp->desktop()->width());
   ui->heightSpinBox->setMaximum(qApp->desktop()->height());

   ui->widthSpinBox->setValue(m_simpleWindow->geometry().width());
   ui->heightSpinBox->setValue(m_simpleWindow->geometry().height());
}

void Panel::initImageSizeLockButton()
{
    ui->lockButton->setFlat(true);
    ui->lockButton->setAutoFillBackground(true);
    ui->lockButton->setIcon(QIcon(IMAGE_UNLOCK));
}

void Panel::setLockButtonIcon(bool state)
{
    if(state){
        ui->lockButton->setIcon(QIcon(IMAGE_LOCK));
    }else{
        ui->lockButton->setIcon(QIcon(IMAGE_UNLOCK));
    }
}

void Panel::chekLockerAndResize()
{
    if (ui->lockButton->isChecked())
    {
        int h = m_simpleWindow->size().height();
        int w = m_simpleWindow->size().width();
        int nW = ui->widthSpinBox->value();
        int nH = ui->heightSpinBox->value();

        if (w!=nW){
            ui->heightSpinBox->setValue(h*nW/w);
        }
        else if (h!=nH)
            ui->widthSpinBox->setValue(w*nH/h);
    }
    m_simpleWindow->resize(ui->widthSpinBox->value(),ui->heightSpinBox->value());
}

void Panel::settingWidgetPosition()
{
    QRect desktopGeometry = QApplication::desktop()->geometry();
    QPoint posOnDesktop((desktopGeometry.bottomRight()-QPoint(this->width()+20,this->height() + 20)));

    this->move(posOnDesktop);
}

void Panel::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape){
        this->close();
    }
}
