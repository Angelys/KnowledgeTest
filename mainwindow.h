#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextCodec>
#include <QFileDialog>
#include <QMessageBox>

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
    void on_actionOpen_triggered();

    void on_action_triggered();

    void on_start_clicked();

    void on_next_clicked();

private:
    Ui::MainWindow *ui;
    QByteArray key;
    void xorIt(QByteArray *buffer);
    QString fileName;
    QList< QStringList > test;
};    

#endif // MAINWINDOW_H
