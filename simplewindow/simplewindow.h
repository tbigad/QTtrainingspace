#ifndef SIMPLEWINDOW_H
#define SIMPLEWINDOW_H

#include "FramelessHelper.h"
#include "sizepanel.h"
#include "Magnifier.h"
#include "fullscreenhelper.h"

#include <QWidget>
#include <QMouseEvent>
#include <QPainter>
#include <memory>
#include <QDebug>
#include <QScreen>

class SizePanel;
class Magnifier;

class SimpleWindow: public QWidget
{
    Q_OBJECT
public:
    explicit SimpleWindow(QWidget *parent = 0);
   ~SimpleWindow();
    void setAutoClose(bool chekBoxState);

private:
    FramelessHelper* m_Frameless;
    std::shared_ptr<Magnifier> m_magnifier;
    std::shared_ptr<SizePanel> m_panel;

    QPoint m_StartDragPos;

    bool m_LeftBtnPressed;
    bool m_widgetCreated;
    bool m_isWidgetResizeble;
    bool m_autoClose;

    void setSizeWidget(QPoint moveMousePos);
    void initialConfigurationWidget();
    void secondarySettingWidget(bool setWidgetMovable, bool setWidgetResizable);

signals:
    void resizeSimpleWindow(QResizeEvent *event);
    void onGrabed(QSize rect);

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
