#include "mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_xlsx = new QXlsx::Document(":/db/db.xlsx");
    m_cell = m_xlsx->dimension();

}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_xlsx;
}

int MainWindow::findRow(QString &word)
{
    for(int i=1;i!=m_cell.rowCount();i++){
        for(int j=1;j!=m_cell.columnCount();j++){
            QString find = m_xlsx->read(i,j).toString();
            if(QString::compare(find,word,Qt::CaseInsensitive)==0){
                return i;
            }
        }
    }
    return -1;
}

QStringList MainWindow::getWordsList(int row)
{
    int i=1;
    QStringList list;
    while(i!=m_cell.columnCount())
    {
       QString word = m_xlsx->read(row,i).toString();
       if(!word.isEmpty())
           list<<word;
       i++;
    }
    return list;
}

void MainWindow::on_btnFind_pressed()
{
    //qDebug()<< getWordsList(findRow(ui->lineEdit->text()));
    //qDebug()<< ui->lineEdit->text().split(" ");
    qDebug()<<"Input:"<<ui->lineEdit->text();
    QStringList list = ui->lineEdit->text().split(" ");
    foreach (QString word, list) {
        if(!word.isEmpty()){
            int row = findRow(word);
            if(row != -1){
                QStringList choisen = getWordsList(row);
                foreach (QString choise, choisen) {
                    QString out = ui->lineEdit->text();
                    out.replace(word,choise,Qt::CaseInsensitive);
                    qDebug()<<out;
                }
            }
        }
    }
}
