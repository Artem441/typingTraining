#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVBoxLayout>
#include <QKeyEvent>
#include <QDebug>
#include <QFileDialog>
#include <QFile>
#include <QComboBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    keyboard = new keyboardwidget(this);

    textEdit = new QTextEdit(this);
    textEdit->setPlaceholderText("начните печать...");
    textEdit->installEventFilter(this);
    mainLayout->addWidget(textEdit);
    QComboBox* layoutSelector = new QComboBox(this);
    layoutSelector->addItem("Русский");
    layoutSelector->addItem("English");
    mainLayout->addWidget(layoutSelector);
    connect(layoutSelector,&QComboBox::currentTextChanged,keyboard,&keyboardwidget::setLayoutByName);


    mainLayout->addWidget(keyboard);
    QPushButton* loadbutton = new QPushButton("Загрузить текст",  this);
    mainLayout->addWidget(loadbutton);
    connect(loadbutton,&QPushButton::clicked,this,&MainWindow::loadTextFromFile);

    connect(keyboard, &keyboardwidget::keyPressed, this, [this](const QString& key) {
        textEdit->insertPlainText(key);
    });

    statsLabel = new QLabel("Время: 0 c | Точность 0% | Скорость: 0 зн/мин",this);
    mainLayout->addWidget(statsLabel);

    restartButton = new QPushButton("Перезапустить",this);
    mainLayout->addWidget(restartButton);
    connect(restartButton,&QPushButton::clicked,this,&MainWindow::resetAll);

    setCentralWidget(centralWidget);
    resize(800, 400);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    QString keyText = event->text();

    if (!keyText.isEmpty()) {
        textEdit->insertPlainText(keyText);

        QString upperKey = keyText.toUpper();

        keyboard->highlightKey(upperKey);
    }

    QMainWindow::keyPressEvent(event);
}


void MainWindow::loadTextFromFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,"Выберите текстовый файл","","Text Files (*.txt)");
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            originalText = in.readAll();
            correctness.clear();
            correctness.resize(originalText.size());
            file.close();

            updateDisplayText();
        }
    }
}

void MainWindow::updateDisplayText()
{
    QString display;
    for (int i = 0; i < originalText.size(); ++i) {
        QChar c = originalText[i];
        if (i < currentIndex) {
            if(correctness[i]) {
                display += QString("<span style='color:green;'>%1</span>").arg(c);
            }
            else {
                display += QString("<span style='color:red;'>%1</span>").arg(c);
            }
        }
        else if (i == currentIndex) {
            display += QString("<span style='color:gray;'>%1</span>").arg(c);
        }
        else {
            display += QString("<span style='color:#cccccc;'>%1</span>").arg(c);
        }
    }
    textEdit->setHtml(display);
}

void MainWindow::updateStats()
{
    if(!timerStarted || currentIndex == 0) return;

    int correctCount = std::count(correctness.begin(),correctness.begin()+currentIndex,true);
    int incorrectCount = currentIndex - correctCount;
    int elapsedSec = timer.elapsed() / 1000;

    double accuracy = (double(correctCount) / currentIndex) * 100.0;
    double cpm = elapsedSec > 0 ? (currentIndex*60/elapsedSec) : 0;

    statsLabel->setText(QString("Время: %1 с | Точность: %2% | Скорость: %3 зн/мин").arg(elapsedSec).arg(int(accuracy)).arg(int(cpm)));
}

void MainWindow::resetAll()
{
    currentIndex = 0;
    timerStarted = false;
    correctness.clear();
    correctness.resize(originalText.size());
    statsLabel->setText("Время: 0 с | Точность: 0% | Скорость: 0 зн/мин");
    textEdit->clear();
    updateDisplayText();
}
bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    if (obj == textEdit && event->type() == QEvent::KeyPress) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        QString keyText = keyEvent->text();
        QString upperKey = keyText.toUpper();

        if (!upperKey.isEmpty())
            keyboard->highlightKey(upperKey);

        if (keyEvent->key() == Qt::Key_Backspace) {
            if (currentIndex > 0) {
                currentIndex--;
                updateDisplayText();
            }
            return true;
        }
        if (!timerStarted) {
            timer.start();
            timerStarted = true;
        }

        if (!keyText.isEmpty() && currentIndex < originalText.length()) {
            QChar expectedChar = originalText[currentIndex];
            QChar typedChar = keyText[0];

            if (typedChar == expectedChar) {
                correctness[currentIndex] = true;
            } else {
                correctness[currentIndex] = false;
            }

            currentIndex++;
            updateDisplayText();
            updateStats();
            return true;
        }
    }

    return QMainWindow::eventFilter(obj, event);
}




