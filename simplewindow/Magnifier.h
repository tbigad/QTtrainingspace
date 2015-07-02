#ifndef GLASS_H
#define GLASS_H

#include <QWidget>
#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>
#include <QPaintEvent>
#include <QPainter>
#include <QScreen>

class Magnifier : public QWidget
{
    Q_OBJECT
public:
    explicit Magnifier(QWidget *parent);
    ~Magnifier();
    void magnifierMove(QPoint *globalMousePos);
    void SetParentSize(QSize SizeP);
    void SetParentDesktopScreen(QPixmap *primaryScreenPixmap);

private:
    QSize MagnifierSize;
    QSize ParentSize;
    QWidget *parentWidg;


    QPixmap *desktopPixmap;
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
