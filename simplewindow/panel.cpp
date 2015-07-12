#include "panel.h"
#include "ui_panel.h"
#include <QDesktopWidget>
#include <QDebug>
#include "FullscreenHelper.h"
#include <cmath>
#include <vector>

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
   ui->widthSpinBox->setMinimum(0);
   ui->heightSpinBox->setMinimum(0);
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
        qDebug()<<"in locker";
        int h = m_simpleWindow->size().height();
        int w = m_simpleWindow->size().width();
        int nw = ui->widthSpinBox->value();
        int nh = ui->heightSpinBox->value();
        if (w!=nw && w!=0)
            ui->heightSpinBox->setValue(h*nw/w);
        else if (h!=0)
            ui->widthSpinBox->setValue(w*nh/h);
    }
    m_simpleWindow->resize(ui->widthSpinBox->value(),ui->heightSpinBox->value());
}

void Panel::settingWidgetPosition()
{
    QPoint cursor = QCursor::pos();
    QRect desktopGeometry = qApp->desktop()->geometry();
    QPoint posOnDesktop = desktopGeometry.topLeft();
    this->move(posOnDesktop);

    union angle{
        int i;
        double value;
    } an;

    double win[4];
    win[0] = segmentLenght(qApp->desktop()->geometry().topLeft().x(),qApp->desktop()->geometry().topLeft().y(),
                                 m_simpleWindow->geometry().topLeft().x(), m_simpleWindow->geometry().topLeft().y());
    win[1] = segmentLenght(qApp->desktop()->geometry().topRight().x(),qApp->desktop()->geometry().topRight().y(),
                             m_simpleWindow->geometry().topRight().x(), m_simpleWindow->geometry().topRight().y());
    win[2] = segmentLenght(qApp->desktop()->geometry().bottomRight().x(),qApp->desktop()->geometry().bottomRight().y(),
                                 m_simpleWindow->geometry().bottomRight().x(), m_simpleWindow->geometry().bottomRight().y());
    win[3] = segmentLenght(qApp->desktop()->geometry().bottomLeft().x(),qApp->desktop()->geometry().bottomLeft().y(),
                                 m_simpleWindow->geometry().bottomLeft().x(), m_simpleWindow->geometry().bottomLeft().y());

    double panelDiagonal = segmentLenght(this->geometry().topLeft().x(),this->geometry().topLeft().y(),
                                         this->geometry().bottomRight().x(), this->geometry().bottomRight().y());

    for(int i = 0; i<4;i++)
    {
        qDebug()<< win[i];
        if(win[i]>panelDiagonal)
        {
            switch (i) {
            case 0:
                posOnDesktop = qApp->desktop()->geometry().topLeft();
                this->move(posOnDesktop);
                return;
            case 1:
                posOnDesktop = qApp->desktop()->geometry().topRight();
                posOnDesktop.setX(qApp->desktop()->geometry().topRight().x() - this->width());
                this->move(posOnDesktop);
                return;
            case 2:
                posOnDesktop.setX(desktopGeometry.bottomRight().x() - this->width());
                posOnDesktop.setY(desktopGeometry.bottomRight().y() - this->height());
                this->move(posOnDesktop);
                return;
            case 3:
                posOnDesktop.setX(desktopGeometry.bottomLeft().x());
                posOnDesktop.setY(desktopGeometry.bottomRight().y() - this->height());
                this->move(posOnDesktop);
                return;
            }
        }

    }

    if (QCursor::pos().x()>(desktopGeometry.width()/2)) {
        posOnDesktop.setX(desktopGeometry.right()-this->width());
    }else if (QCursor::pos().x()<(desktopGeometry.width()/2)) {
        posOnDesktop.setX(0);
    }
    if(QCursor::pos().y()> (desktopGeometry.height()/2)){
        posOnDesktop.setY(desktopGeometry.height() - this->height());
    }
  this->move(posOnDesktop);
}

double Panel::segmentLenght(int aX, int aY, int bX, int bY)
{
    return sqrt(pow(static_cast<double>(bX-aX),2)+pow(static_cast<double>(bY-aY),2));
}
