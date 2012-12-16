#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    key =  "Швидка руда лисиця стрибає навколо ледачої собаки";

    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(testOpen()));
    connect(ui->next, SIGNAL(clicked()), this, SLOT(questionNext()));
    connect(ui->start, SIGNAL(clicked()), this, SLOT(testStart()));

    ui->stackedWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::testOpen()
{
    fileName = QFileDialog::getOpenFileName(this, tr("Файл тесту"), "./", tr("Файли тестів") + " (*.tst)", 0, 0);

    if( fileName.isEmpty() ) {
        return;
    }

    QFile file(fileName);

    if( ! file.open(QIODevice::ReadOnly) ) {
        QMessageBox::critical(this, tr("Помилка"), tr("Не вдалося відкрити файл для читання"));
        return;
    }

    QByteArray buffer = file.readAll();

    file.close();

    if( ! buffer.length() ) {
        test.empty();
        test.append( QStringList("") );
        ui->test_name->setText("");
        return;
    }

    xorIt(&buffer);

    QDataStream dataStream(buffer);
    dataStream >> test;

    ui->test_name->setText( test[0][0] );
    ui->test_time->setText(test[0][1]);
}

void MainWindow::xorIt(QByteArray *buffer)
{
    int keyLength = key.length();

    for(int i = 0; i < buffer->length(); i++) {
        (*buffer)[i] = (*buffer)[i] ^ key[ i % keyLength ];
    }
}

void MainWindow::testStart()
{
    if(test.empty())
    {
        QMessageBox::information(this, tr("Помилка"), tr("Виберіть файл тесту"));
        return;
    }

    timeLeft = test[0][1].toInt();
    startTimer(1000);

    currentQuestion = 1;
    MainWindow::questionShow();

    ui->stackedWidget->setCurrentIndex(1);
}



void MainWindow::questionNext()
{

}

void MainWindow::questionSave()
{
}

void MainWindow::questionShow()
{
    ui->question->setText("question");

    QObjectList answers =  ui->scrollAreaWidgetContents->layout()->children();

    qDebug() << answers;

    for ( int i = 0 ; i < answers.count() ; i++ )
    {
        delete answers[i];
    }

}

void MainWindow::submitResult()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    ui->Timer->display(timeLeft);

    if(timeLeft <=0)
    {
        MainWindow::submitResult();
    }

    timeLeft --;
}
