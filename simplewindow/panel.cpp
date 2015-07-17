#include "panel.h"
#include "ui_panel.h"
#include <QDesktopWidget>
#include <QDebug>
#include "FullscreenHelper.h"
#include <cmath>
#include <vector>

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

    fillComboBox();
    settingSpinBox();
    settingLockButton();

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
    qDebug()<< "Crop button pressed!!!";
    this->hide();
    m_simpleWindow->setGrabed();
    this->close();
}

void Panel::cancelBtnPressed()
{
    this->close();
}

void Panel::fillComboBox()
{
    ui->comboBox->addItem("Custom size...");
    ui->comboBox->addItem("(SD) 480p: 720x480");
    ui->comboBox->addItem("(HD) 720p: 1280x720");
    ui->comboBox->addItem("(Full HD) 1080p: 1920x1080");
    ui->comboBox->addItem("(4k) 2160p: 3840x2160");
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

void Panel::settingSpinBox()
{
   ui->widthSpinBox->setMinimum(1);
   ui->heightSpinBox->setMinimum(1);
   ui->widthSpinBox->setMaximum(qApp->desktop()->width());
   ui->heightSpinBox->setMaximum(qApp->desktop()->height());

   ui->widthSpinBox->setValue(m_simpleWindow->geometry().width());
   ui->heightSpinBox->setValue(m_simpleWindow->geometry().height());

}

void Panel::settingLockButton()
{
    ui->lockButton->setFlat(true);
    ui->lockButton->setAutoFillBackground(true);
    ui->lockButton->setIcon(QIcon(":/icons/un_lock.png"));
}

void Panel::setLockButtonIcon(bool state)
{
    if(state){
        ui->lockButton->setIcon(QIcon(":/icons/lock.png"));
    }else{
        ui->lockButton->setIcon(QIcon(":/icons/un_lock.png"));
    }
}

void Panel::chekLockerAndResize()
{
    if (ui->lockButton->isChecked())
    {
        int h = m_simpleWindow->size().height();
        int w = m_simpleWindow->size().width();
        int nw = ui->widthSpinBox->value();
        int nh = ui->heightSpinBox->value();

        qDebug()<<" h:"<<h<<" w:"<<w<<" nw:"<< nw << " nh:"<<nh <<" new Height"<< (h*nw/w) << " new Weight:"<< (w*nh/h);

        if (w!=nw){
            ui->heightSpinBox->setValue(h*nw/w);
        }
        else if (h!=nh)
            ui->widthSpinBox->setValue(w*nh/h);
    }
    m_simpleWindow->resize(ui->widthSpinBox->value(),ui->heightSpinBox->value());
}

void Panel::settingWidgetPosition()
{
    QRect desktopGeometry = QApplication::desktop()->geometry();
    QPoint posOnDesktop = desktopGeometry.topLeft();

    if (QCursor::pos().x()>(desktopGeometry.width()/2)) {
        if((desktopGeometry.right()-this->width())<((m_simpleWindow->geometry().right()+(desktopGeometry.right()-this->width()))/2))
           posOnDesktop.setX(desktopGeometry.right()-this->width());
        else
            posOnDesktop.setX((m_simpleWindow->geometry().right()+(desktopGeometry.right()-this->width()))/2);
    }else if (QCursor::pos().x()<(desktopGeometry.width()/2)) {
        if(desktopGeometry.left()>(m_simpleWindow->geometry().left()-this->width()))
            posOnDesktop.setX(desktopGeometry.left());
        else
            posOnDesktop.setX((m_simpleWindow->geometry().left()-this->width())/2);
    }
    if(QCursor::pos().y()> (desktopGeometry.height()/2)){
        if(desktopGeometry.bottom()>(m_simpleWindow->geometry().bottom()+this->height()))
            posOnDesktop.setY((m_simpleWindow->geometry().bottom()+(desktopGeometry.bottom()-this->height()))/2);
        else
            posOnDesktop.setY(desktopGeometry.height() - this->height());
    }else if(QCursor::pos().y()<(desktopGeometry.height()/2))
    {
        if(desktopGeometry.top()<(m_simpleWindow->geometry().top()-this->height()))
            posOnDesktop.setY((m_simpleWindow->geometry().top()-this->height())/2);
        else
            posOnDesktop.setY(desktopGeometry.top());
    }
  this->move(posOnDesktop);
}

double Panel::segmentLenght(int aX, int aY, int bX, int bY)
{
    return sqrt(pow(static_cast<double>(bX-aX),2)+pow(static_cast<double>(bY-aY),2));
}

void Panel::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Escape:
        this->close();
        break;
    default:
        break;
    }
}
