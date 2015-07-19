#ifndef SIZEPANEL_H
#define SIZEPANEL_H

#include <QWidget>
#include <FramelessHelper.h>
#include <simplewindow.h>
#include <memory>

namespace Ui {
class SizePanel;
}

class SimpleWindow;

class SizePanel : public QWidget
{
    Q_OBJECT

public:
    explicit SizePanel(QWidget *parent);
    ~SizePanel();

private:
    Ui::SizePanel *ui;
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

#endif // SIZEPANEL_H
