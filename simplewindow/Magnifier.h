#ifndef GLASS_H
#define GLASS_H

#include <QWidget>
#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>
#include <QPaintEvent>
#include <QPainter>
#include <QPixmap>
#include <simplewindow.h>
#include <memory>

class SimpleWindow;

class Magnifier : public QWidget
{
    Q_OBJECT
public:
    explicit Magnifier(QWidget *parent);
    ~Magnifier();
    void magnifierMove(QPoint *globalMousePos);

private:
    QSize MagnifierSize;
    QSize ParentSize;
    SimpleWindow *c_simpleWindow;
    std::shared_ptr<QPixmap> desktopPixmap;

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *event);
//    void mousePressEvent(QMouseEvent *event);
//    void mouseReleaseEvent(QMouseEvent *event);
//    void mouseMoveEvent(QMouseEvent *event);
//    void paintEvent(QPaintEvent *event);
};

#endif // GLASS_H
