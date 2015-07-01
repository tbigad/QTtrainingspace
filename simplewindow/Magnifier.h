#ifndef GLASS_H
#define GLASS_H

#include <QWidget>

class Magnifier : public QWidget
{
    Q_OBJECT
public:
    explicit Magnifier(QWidget *parent);
    ~Magnifier();
    void magnifierMove(QPoint *globalMousePos);
    void SetParentSize(QSize SizeP);

private:
    QSize MagnifierSize;
    QSize ParentSize;
    QWidget *parentWidg;
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
