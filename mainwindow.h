#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextCodec>
#include <QFileDialog>
#include <QMessageBox>
#include <QTimer>

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
    void testOpen();
    void questionNext();
    void questionSave();
    void questionShow();
    void clearAnswers();
    void setAnswers(int question);
    void testStart();
    void submitResult();
    void timerEvent(QTimerEvent *event);

private:
    Ui::MainWindow *ui;
    QByteArray key;
    void xorIt(QByteArray *buffer);
    QString fileName;
    QList< QStringList > test;
    int timeLeft;
    int currentQuestion;
    int score;
    int timer;
};    

#endif // MAINWINDOW_H
