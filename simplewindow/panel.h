#ifndef PANEL_H
#define PANEL_H

#include <QWidget>
#include <FramelessHelper.h>

namespace Ui {
class Panel;
}

class Panel : public QWidget
{
    Q_OBJECT

public:
    explicit Panel(QWidget *parent = 0);
    ~Panel();

private:
    Ui::Panel *ui;
    FramelessHelper* mFrameless;
private slots:
    void cropBtnPressed();
    void cancelBtnPressed();
protected:
    void closeEvent(QCloseEvent *event);
};

#endif // PANEL_H
