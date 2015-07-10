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
    FramelessHelper* mFrameless;

    void fillComboBox();
    void settingSpinBox();
    void settingLockButton();
private slots:
    void cropBtnPressed();
    void cancelBtnPressed();
    void comBoxSelection(int activated);
    void setLockButtonIcon(bool state);
protected:
    void closeEvent(QCloseEvent *event);
};

#endif // PANEL_H
