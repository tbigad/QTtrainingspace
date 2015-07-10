#include "Magnifier.h"


Magnifier::Magnifier(QWidget *parent)
    : QWidget(parent, Qt::Window|Qt::FramelessWindowHint|Qt::CustomizeWindowHint|Qt::NoDropShadowWindowHint)
{
    MagnifierSize = QSize(100,100);
    setFixedSize(MagnifierSize);
    setWindowOpacity(1);
    c_simpleWindow = dynamic_cast<SimpleWindow*>(parent);

    settingSizeLabel();

    QDesktopWidget *desktop = QApplication::desktop();
    desktopPixmap = std::make_shared<QPixmap>(qApp->primaryScreen()->grabWindow(desktop->winId(), desktop->geometry().x(),
                                                      desktop->geometry().y(), desktop->geometry().width(),
                                                      desktop->geometry().height()));

    connect(c_simpleWindow, &SimpleWindow::resizeSimpleWindow, [=](QResizeEvent *event){

        QString sizeStr;
        QTextStream(&sizeStr)<<event->size().width()<<" x "<<event->size().height();
        labelSize->setText(sizeStr);
    });

    this->show();
}

Magnifier::~Magnifier()
{
}

void Magnifier::settingSizeLabel()
{
    labelSize = new QLabel(this);
    labelSize->setLineWidth(0);
    labelSize->setStyleSheet("background-color: gray;""color: white;");

    labelLayout = new QVBoxLayout;
    labelLayout->addWidget(labelSize);
    this->setLayout(labelLayout);
    labelLayout->setAlignment(Qt::AlignHCenter|Qt::AlignBottom);
    labelLayout->setAlignment(Qt::AlignHCenter|Qt::AlignBottom);

}

void Magnifier::magnifierMove(QPoint *globalMousePos)
{
    QRect desktopRect = QApplication::desktop()->geometry();
    auto newPos = globalMousePos;

    auto newX = newPos->x();
    auto newY = newPos->y();

    auto maxX = desktopRect.width() - MagnifierSize.width();
    auto maxY = desktopRect.height() - MagnifierSize.height();

    if(c_simpleWindow->geometry().left() == newX) newX -= this->width();
    if(c_simpleWindow->geometry().top() == newY) newY -= this->height();

    if(newX > maxX) newX = maxX;
    if(newY > maxY) newY = maxY;

    if(newX < 0) newX = 0;
    if(newY < 0) newY = 0;

    this->move(newX, newY);
    this->repaint();
    this->raise();
}

void Magnifier::paintEvent(QPaintEvent *event)
{

    QPoint scaledCoefficient(this->size().width()/5,this->size().height()/5);
    QRect scaledRect(QCursor::pos()-scaledCoefficient,QCursor::pos()+scaledCoefficient);
    QPixmap zoomPixmap = desktopPixmap->copy(scaledRect).scaled(this->size(), Qt::IgnoreAspectRatio);

    QPainter paint(this);
    QPoint drawPoint = QPoint(0,0);
    paint.drawPixmap(drawPoint, zoomPixmap);

    paint.setPen( QPen(QBrush( QColor(Qt::black) ), 2) );
    paint.drawRect(this->rect());

    drawPoint = this->rect().center();
    paint.drawText(drawPoint,"+");
}

