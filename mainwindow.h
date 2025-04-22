#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QElapsedTimer>
#include <QLabel>
#include "keyboardwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    Ui::MainWindow *ui;
    keyboardwidget *keyboard;
    QTextEdit *textEdit;
    bool eventFilter(QObject *obj, QEvent *event) override;
    void loadTextFromFile();
    void updateDisplayText();
    QString originalText;
    int currentIndex = 0;
    QVector<bool> correctness;
    QLabel* statsLabel;
    QPushButton* restartButton;
    bool timerStarted = false;
    QElapsedTimer timer;
    void updateStats();
    void resetAll();
};
#endif // MAINWINDOW_H

