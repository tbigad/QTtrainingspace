#include "simplewindow.h"
#include "fullscreenhelper.h"

#include <QDebug>

SimpleWindow::SimpleWindow(QWidget *parent)
    : QWidget(parent, Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint|Qt::CustomizeWindowHint|Qt::NoDropShadowWindowHint),
      mFrameless(new FramelessHelper)
{
    initialConfigurationWidget();
}

SimpleWindow::~SimpleWindow()
{
}

void SimpleWindow::mousePressEvent(QMouseEvent *event)
{

    mLeftBtnPressed = true;
    switch (event->button()) {
    case Qt::LeftButton:
        if(widgetCreated){
            return;
        }
        mStartDragPos = event->globalPos();
        break;
    case Qt::RightButton:
        this->close();
        break;
    default:
        return;
    }
}

void SimpleWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(widgetCreated)
        return;

    if(mLeftBtnPressed)
    setSizeWidget(event->globalPos());
}

void SimpleWindow::mouseReleaseEvent(QMouseEvent *event)
{
    mLeftBtnPressed = false;
    if(widgetCreated){
        return;
    }

      if((this->size().height()< 10) || (this->size().width()< 10) || (isWidgetResizeble == false) )
    {
        initialConfigurationWidget();
        return;
    }
    if (isWidgetResizeble){
            secondarySettingWidget(true,true);
    }
}

void SimpleWindow::setSizeWidget(QPoint moveMousePos)
{
    if(widgetCreated)
        return;

    setWindowOpacity(0.5);
    QPoint topLeft, bottonRight;
    topLeft = mStartDragPos;
    bottonRight = moveMousePos;
    if(topLeft.x()>bottonRight.x())
    {
         topLeft.setX(bottonRight.x());
         bottonRight.setX(mStartDragPos.x());
    }
    if(topLeft.y()>bottonRight.y())
    {
        topLeft.setY(bottonRight.y());
        bottonRight.setY(mStartDragPos.y());
    }

    isWidgetResizeble = true;
    QRect initRect(topLeft,bottonRight);
    setGeometry(initRect);
    raise();
}

void SimpleWindow::paintEvent(QPaintEvent *event)
{
    const QPen whitePen(Qt::white, 2);
    const QPen blackPen(Qt::black, 2);

    QPainter paint(this);
    paint.setOpacity(1);

    paint.setPen(whitePen);
    QRectF rec(rect().topLeft(),size());
    paint.drawRect(rec);

    QString sizeStr;
    paint.setPen(blackPen);
    if(!widgetCreated)
    {
        QTextStream(&sizeStr)<<size().width()<<" x "<<size().height();
        paint.drawText(rec.center(),sizeStr);
    }
}

void SimpleWindow::resizeEvent(QResizeEvent *event)
{


}

void SimpleWindow::initialConfigurationWidget()
{
    FullScreenHelper::MaximizeOnVirtualScreen(this);
    setWindowOpacity(0.1);
    setCursor(Qt::CrossCursor);
    widgetCreated = false;
    isWidgetResizeble = false;
}

void SimpleWindow::secondarySettingWidget(bool setWidgetMovable, bool setWidgetResizable)
{
    mFrameless->activateOn(this);
    mFrameless->setWidgetMovable(setWidgetMovable);
    mFrameless->setWidgetResizable(setWidgetResizable);
    widgetCreated = true;
    if(chekBox)
    {
        panel = std::make_shared<Panel>();
        panel.get()->show();
    }
}

void SimpleWindow::setChekBoxState(bool chekBoxState)
{
    chekBox = chekBoxState;
}

void SimpleWindow::closeEvent(QCloseEvent *event)
{
    if(chekBox)
    panel.get()->close();
}
