#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_triggered()
{
    ui->test_name->setText("triggered OLOLO");
}

void MainWindow::on_actionOpen_triggered()
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
}

void MainWindow::xorIt(QByteArray *buffer)
{
    int keyLength = key.length();

    for(int i = 0; i < buffer->length(); i++) {
        (*buffer)[i] = (*buffer)[i] ^ key[ i % keyLength ];
    }
}

void MainWindow::on_start_clicked()
{

}

void MainWindow::on_next_clicked()
{

}
