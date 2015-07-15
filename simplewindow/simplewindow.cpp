#include "simplewindow.h"

SimpleWindow::SimpleWindow(QWidget *parent)
    : QWidget(parent, Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint|Qt::CustomizeWindowHint|Qt::NoDropShadowWindowHint),
      m_Frameless(new FramelessHelper)
{
    initialConfigurationWidget();
}

SimpleWindow::~SimpleWindow()
{
}

void SimpleWindow::mousePressEvent(QMouseEvent *event)
{
    m_LeftBtnPressed = true;
    switch (event->button()) {
    case Qt::LeftButton:
        if(m_widgetCreated){
            return;
        }
        m_StartDragPos = event->globalPos();
        m_magnifier = std::make_shared<Magnifier>(this);
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
    if(m_widgetCreated)
        return;

    if(m_LeftBtnPressed) {
        setSizeWidget(event->globalPos());
    }
}

void SimpleWindow::mouseReleaseEvent(QMouseEvent *event)
{
    m_LeftBtnPressed = false;
    if(m_widgetCreated) {
        return;
    }

    if(!m_chekBox)
    {
        m_magnifier->close();
        this->setGrabed();
        this->close();
    }

    if((this->size().height() < 10) || (this->size().width() < 10) || (m_isWidgetResizeble == false) )
    {
        initialConfigurationWidget();
        m_magnifier->close();
        return;
    }
    if (m_isWidgetResizeble){
        m_magnifier->close();
        secondarySettingWidget(true,true);
    }
}

void SimpleWindow::setSizeWidget(QPoint moveMousePos)
{
    if(m_widgetCreated)
        return;

    setWindowOpacity(0.5);
    QPoint topLeft, bottonRight;
    topLeft = m_StartDragPos;
    bottonRight = moveMousePos;
    if(topLeft.x()>bottonRight.x())
    {
         topLeft.setX(bottonRight.x());
         bottonRight.setX(m_StartDragPos.x());
    }
    if(topLeft.y()>bottonRight.y())
    {
        topLeft.setY(bottonRight.y());
        bottonRight.setY(m_StartDragPos.y());
    }

    m_isWidgetResizeble = true;
    QRect initRect(topLeft,bottonRight);
    setGeometry(initRect);

    m_magnifier->magnifierMove(&moveMousePos);
}

void SimpleWindow::paintEvent(QPaintEvent *event)
{
    const QPen whitePen(Qt::white, 2);

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
    m_widgetCreated = false;
    m_isWidgetResizeble = false;
}

void SimpleWindow::secondarySettingWidget(bool setWidgetMovable, bool setWidgetResizable)
{
    m_Frameless->activateOn(this);
    m_Frameless->setWidgetMovable(setWidgetMovable);
    m_Frameless->setWidgetResizable(setWidgetResizable);
    m_widgetCreated = true;

    if(m_chekBox) {
        m_panel = std::make_shared<Panel>(this);
    }
    else {
        setChekBoxState(false);
    }
}

void SimpleWindow::setChekBoxState(bool chekBoxState)
{
    m_chekBox = chekBoxState;
}

void SimpleWindow::closeEvent(QCloseEvent *event)
{
    if(m_chekBox)
    {
        m_panel.reset();
        m_magnifier.reset();
    }
}

void SimpleWindow::setWidth(int w)
{
    this->setGeometry(QRect(this->geometry().topLeft(),QSize(w,this->height())));
}

void SimpleWindow::setHeight(int h)
{
    this->setGeometry(QRect(this->geometry().topLeft(),QSize(this->width(),h)));
}

void SimpleWindow::setGrabed()
{
    emit onGrabed("All good!");
}

void SimpleWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Escape:
        this->close();
        break;
    default:
        break;
    }
}

