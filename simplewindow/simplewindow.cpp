#include "simplewindow.h"

SimpleWindow::SimpleWindow(QWidget *parent)
    : QWidget(parent, Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint|Qt::CustomizeWindowHint|Qt::NoDropShadowWindowHint),
      mFrameless(new FramelessHelper)
{
    initialConfigurationWidget();
    qDebug()<< this;
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
        magnifier = std::make_shared<Magnifier>(new Magnifier(this));
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
        magnifier.get()->hide();
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

    magnifier.get()->SetParentSize(size());
    magnifier.get()->SetParentDesktopScreen(&desktopPixmap);
    magnifier.get()->magnifierMove(&moveMousePos);

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

}

void SimpleWindow::initialConfigurationWidget()
{
    desktop = QApplication::desktop();
    QScreen *screen = QApplication::primaryScreen();
    desktopPixmap = screen->grabWindow(desktop->winId(), desktop->geometry().x(),
                                                      desktop->geometry().y(), desktop->geometry().width(),
                                                      desktop->geometry().height());

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
        panel = std::make_shared<Panel>(new Panel(this));
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
    magnifier.reset();
}

void SimpleWindow::setWidth(int w)
{
    this->size().setWidth(w);
}


