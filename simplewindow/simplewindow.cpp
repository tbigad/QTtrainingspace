#include "simplewindow.h"

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
        magnifier = std::make_shared<Magnifier>(this);
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
    {
    setSizeWidget(event->globalPos());
    }
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
        magnifier->close();
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

    magnifier->magnifierMove(&moveMousePos);

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
}

void SimpleWindow::resizeEvent(QResizeEvent *event)
{
    emit resizeSimpleWindow(event);
}

void SimpleWindow::initialConfigurationWidget()
{

    FullScreenHelper::MaximizeOnVirtualScreen(this);
    setWindowOpacity(0.1);
    setCursor(Qt::CrossCursor);
    widgetCreated = false;
    isWidgetResizeble = false;
    getDesktopPixmap();
}

void SimpleWindow::secondarySettingWidget(bool setWidgetMovable, bool setWidgetResizable)
{
    mFrameless->activateOn(this);
    mFrameless->setWidgetMovable(setWidgetMovable);
    mFrameless->setWidgetResizable(setWidgetResizable);
    widgetCreated = true;
    if(chekBox)
    {
        panel = std::make_shared<Panel>(this);
    }
}

void SimpleWindow::setChekBoxState(bool chekBoxState)
{
    chekBox = chekBoxState;
}

void SimpleWindow::closeEvent(QCloseEvent *event)
{
    if(chekBox)
    panel.reset();
    magnifier.reset();
}

void SimpleWindow::getDesktopPixmap()
{
    QDesktopWidget *desktop = QApplication::desktop();
    desktopPixmap = qApp->primaryScreen()->grabWindow(desktop->winId(), desktop->geometry().x(),
                                                      desktop->geometry().y(), desktop->geometry().width(),
                                                      desktop->geometry().height());
}

void SimpleWindow::setWidth(int w)
{
    this->setGeometry(QRect(this->geometry().topLeft(),QSize(w,this->height())));
}

void SimpleWindow::setHeight(int h)
{
    this->setGeometry(QRect(this->geometry().topLeft(),QSize(this->width(),h)));
}




