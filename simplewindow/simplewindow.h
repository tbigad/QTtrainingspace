#ifndef SIMPLEWINDOW_H
#define SIMPLEWINDOW_H

#include "FramelessHelper.h"
#include "panel.h"
#include "Magnifier.h"

#include <QWidget>
#include <QMouseEvent>
#include <QPainter>
#include <memory>


//class QMouseEvent;

class SimpleWindow: public QWidget
{
public:
    SimpleWindow(QWidget *parent = 0);
   ~SimpleWindow();
    void setChekBoxState(bool chekBoxState);

private:
    FramelessHelper* mFrameless;
    std::shared_ptr<Magnifier> magnifier;
    std::shared_ptr<Panel> panel;
    void setSizeWidget(QPoint moveMousePos);

    QPoint mStartDragPos;

    bool mLeftBtnPressed;
    bool widgetCreated;
    bool isWidgetResizeble;

    bool chekBox;

    void initialConfigurationWidget();
    void secondarySettingWidget(bool setWidgetMovable, bool setWidgetResizable);

    QPixmap desktopPixmap;
    QDesktopWidget *desktop;


protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    void closeEvent(QCloseEvent *event);
};

#endif // SIMPLEWINDOW_H
