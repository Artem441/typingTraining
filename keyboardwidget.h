#ifndef KEYBOARDWIDGET_H
#define KEYBOARDWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QMap>

class keyboardwidget : public QWidget
{
    Q_OBJECT
public:
    explicit keyboardwidget(QWidget *parent = nullptr);
    void highlightKey(const QString &key);
    void setLayoutByName(const QString& layoutName);

signals:
    void keyPressed(const QString &key);

private:
    QGridLayout *layout;
    QMap<QString, QPushButton*> buttons;

    void setEnglishLayout();
    void setRussianLayout();
};

#endif // KEYBOARDWIDGET_H


