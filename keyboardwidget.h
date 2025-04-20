#ifndef KEYBOARDWIDGET_H
#define KEYBOARDWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QMap>
#include <QTimer>

class keyboardwidget : public QWidget
{
    Q_OBJECT
public:
    explicit keyboardwidget(QWidget *parent = nullptr);
    void setRussianLayout();
    void highlightKey(const QString& key);
signals:
    void keyPressed(const QString& key);
private:
    QGridLayout* layout;
    QMap<QString,QPushButton*> buttons;
};

#endif // KEYBOARDWIDGET_H
