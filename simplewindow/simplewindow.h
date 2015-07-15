#ifndef SIMPLEWINDOW_H
#define SIMPLEWINDOW_H

#include "FramelessHelper.h"
#include "panel.h"
#include "Magnifier.h"
#include "fullscreenhelper.h"

#include <QWidget>
#include <QMouseEvent>
#include <QPainter>
#include <memory>
#include <QDebug>
#include <QScreen>

class Panel;
class Magnifier;

class SimpleWindow: public QWidget
{
    Q_OBJECT
public:
    explicit SimpleWindow(QWidget *parent = 0);
   ~SimpleWindow();
    void setChekBoxState(bool chekBoxState);
private:
    FramelessHelper* m_Frameless;
    std::shared_ptr<Magnifier> m_magnifier;
    std::shared_ptr<Panel> m_panel;
    void setSizeWidget(QPoint moveMousePos);

    QPoint m_StartDragPos;

    bool m_LeftBtnPressed;
    bool m_widgetCreated;
    bool m_isWidgetResizeble;

    bool m_chekBox;

    void initialConfigurationWidget();
    void secondarySettingWidget(bool setWidgetMovable, bool setWidgetResizable);

signals:
    void resizeSimpleWindow(QResizeEvent *event);
    void onGrabed(QString text);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    void closeEvent(QCloseEvent *event);
    void keyPressEvent(QKeyEvent *event);

public slots:
     void setWidth(int w);
     void setHeight(int h);
     void setGrabed();
};

#endif // SIMPLEWINDOW_H
