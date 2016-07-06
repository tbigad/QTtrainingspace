#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QStringList>

#include "ui_mainwindow.h"
#include "xlsxdocument.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btnFind_pressed();

private:
    int findRow(QString& word);
    QStringList getWordsList(int row);

    QXlsx::Document* m_xlsx;
    QXlsx::CellRange m_cell;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
