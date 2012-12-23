#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForTr(codec);

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

    int time = test[0][1].toInt();
    int minutes = time / 60;

    ui->test_time->setText(QString::number(minutes) + tr(" хвилин"));

    if(ui->stackedWidget->currentIndex() == 1)
    {
        killTimer(timer);
        ui->stackedWidget->setCurrentIndex(0);
    }
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
    timer = startTimer(1000);

    score = 0;
    currentQuestion = 1;
    MainWindow::questionShow();

    ui->stackedWidget->setCurrentIndex(1);
}



void MainWindow::questionNext()
{
    questionSave();

    if( (test.count() - 1) > currentQuestion)
    {
        currentQuestion++;
        questionShow();
    } else
    {
        submitResult();
    }
}

void MainWindow::questionSave()
{
    QObjectList children = ui->scrollAreaWidgetContents->children();

    QString label;

    while(!children.isEmpty())
    {
        QObject* child = children.takeFirst();
        QRadioButton* rbutton = qobject_cast<QRadioButton*>(child);

        if( (rbutton != NULL) && (rbutton->isChecked() == true) )
        {
            label = rbutton->text();
        }
    }


    for(int i = 2; i < test[currentQuestion].count(); i+=2)
    {
        if(test[currentQuestion][i] == label)
        {
            score += test[currentQuestion][i+1].toInt();
        }
    }

}

void MainWindow::questionShow()
{
    ui->question->setText(test[currentQuestion][0]);

    clearAnswers();

    setAnswers(currentQuestion);

}

void MainWindow::clearAnswers()
{
    QLayoutItem *child;

    QObjectList children = ui->scrollAreaWidgetContents->children();

    //Delete all
    while(!children.isEmpty())
    {
        QObject* child = children.takeFirst();
        if(qobject_cast<QRadioButton*>(child) != NULL)
        {
            delete child;
        }
    }

}

void MainWindow::setAnswers(int question)
{
    for(int i = 2; i < test[currentQuestion].count(); i+=2)
    {
        ui->scrollAreaWidgetContents->layout()->addWidget(new QRadioButton(test[currentQuestion][i] , ui->scrollAreaWidgetContents));
    }
}

void MainWindow::submitResult()
{
    ui->stackedWidget->setCurrentIndex(2);

    ui->resultArea->setText("Фінальний результат : " + QString::number(score));
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    int minutes = timeLeft / 60;

    int seconds = timeLeft - minutes*60;

    ui->Timer->display(QString::number(minutes) + ":" + QString::number(seconds) );

    if(timeLeft <=0)
    {
        MainWindow::submitResult();
    }

    timeLeft --;
}
