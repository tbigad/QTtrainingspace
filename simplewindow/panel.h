#ifndef PANEL_H
#define PANEL_H

#include <QWidget>
#include <FramelessHelper.h>
#include <simplewindow.h>
#include <memory>

namespace Ui {
class Panel;
}

class SimpleWindow;

class Panel : public QWidget
{
    Q_OBJECT

public:
    explicit Panel(QWidget *parent);
    ~Panel();

private:
    Ui::Panel *ui;
    SimpleWindow *m_simpleWindow;
    FramelessHelper* m_Frameless;

    void initImageSizeComboBox();
    void initImageSizeSpinBox();
    void initImageSizeLockButton();
    void settingWidgetPosition();

private slots:
    void cropBtnPressed();
    void cancelBtnPressed();
    void comBoxSelection(int activated);
    void setLockButtonIcon(bool state);
    void chekLockerAndResize();

protected:
    void closeEvent(QCloseEvent *event);
    void keyPressEvent(QKeyEvent *event);
};

#endif // PANEL_H
