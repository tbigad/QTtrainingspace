#ifndef PANEL_H
#define PANEL_H

#include <QWidget>
#include <FramelessHelper.h>
#include <simplewindow.h>

namespace Ui {
class Panel;
}

class SimpleWindow;

class Panel : public QWidget
{
    Q_OBJECT

public:
    explicit Panel(QWidget *parent = 0);
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
protected:
    void closeEvent(QCloseEvent *event);
};

#endif // PANEL_H
