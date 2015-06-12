#ifndef SIMPLEWINDOW_H
#define SIMPLEWINDOW_H
#include <QWidget>
#include <QtGui/QMouseEvent>

class QMouseEvent;

class SimpleWindow: public QWidget
{
public:
    SimpleWindow(QWidget *parent = 0);
   ~SimpleWindow();
private:
    QPoint _gmp;//Global Mouse Point

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
};

#endif // SIMPLEWINDOW_H
