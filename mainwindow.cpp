#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);

    QTextEdit *textEdit = new QTextEdit(this);
    textEdit->setPlaceholderText("начните печать...");
    mainLayout->addWidget(textEdit);

    keyboard = new keyboardwidget(this);
    mainLayout->addWidget(keyboard);

    connect(keyboard,&keyboardwidget::keyPressed,[textEdit](const QString& key) {
       textEdit->insertPlainText(key);
    });
    setCentralWidget(centralWidget);
    resize(800,400);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    QString keyText = event->text().toUpper();
    if(!keyText.isEmpty()) {
        keyboard->highlightKey(keyText);
        textEdit->insertPlainText(keyText);
    }
    QMainWindow::keyPressEvent(event);
}




