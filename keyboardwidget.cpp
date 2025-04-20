#include "keyboardwidget.h"
#include <QTimer>
#include <QSpacerItem>

keyboardwidget::keyboardwidget(QWidget *parent)
    : QWidget{parent}
{
    layout = new QGridLayout(this);
    layout->setSpacing(3);
    setRussianLayout();
}

void keyboardwidget::setRussianLayout()
{
    qDeleteAll(buttons);
    buttons.clear();

    // Стиль с фиксированными размерами
    const QString defaultStyle =
        "QPushButton {"
        "   font-size: 16px;"
        "   background-color: #e0e0e0;"
        "   color: #000000;"
        "   border: 1px solid #a0a0a0;"
        "   min-width: 40px;"
        "   min-height: 40px;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #c0c0c0;" // Слегка темнее при нажатии
        "}";

    QStringList russianKeys = {
        "Й", "Ц", "У", "К", "Е", "Н", "Г", "Ш", "Щ", "З", "Х", "Ъ",
        "Ф", "Ы", "В", "А", "П", "Р", "О", "Л", "Д", "Ж", "Э",
        "Я", "Ч", "С", "М", "И", "Т", "Ь", "Б", "Ю", ".",
    };

    // Настройка растяжения колонок
    for(int i = 0; i < 12; i++) {
        layout->setColumnStretch(i, (i == 0 || i == 11) ? 1 : 0);
    }

    int row = 0, col = 0;
    for (const QString& key : russianKeys) {
        QPushButton* btn = new QPushButton(key, this);
        btn->setStyleSheet(defaultStyle);
        btn->setProperty("defaultStyle", defaultStyle);

        // Подключаем сигнал pressed вместо clicked
        connect(btn, &QPushButton::pressed, this, [this, key]() {
            emit keyPressed(key);
            highlightKey(key);
        });

        // Логика размещения
        if (row == 0 && col >= 12) {
            row++;
            col = 0;
        }
        else if (row == 1 && col >= 11) {
            row++;
            col = 0;
        }

        layout->addWidget(btn, row, col, 1, 1, Qt::AlignCenter);
        buttons[key] = btn;
        col++;
    }

    // Кнопка пробела
    QPushButton *spaceBtn = new QPushButton("Пробел", this);
    spaceBtn->setStyleSheet(defaultStyle +
                            " QPushButton {"
                            "   min-width: 480px;"
                            "   max-width: 480px;"
                            "}");
    spaceBtn->setProperty("defaultStyle", spaceBtn->styleSheet());

    connect(spaceBtn, &QPushButton::pressed, this, [this]() {
        emit keyPressed(" ");
        highlightKey(" ");
    });

    layout->addWidget(spaceBtn, 3, 0, 1, 12, Qt::AlignCenter);
    buttons[" "] = spaceBtn;
}

void keyboardwidget::highlightKey(const QString &key)
{
    if (buttons.contains(key)) {
        QPushButton* btn = buttons[key];
        const QString originalStyle = btn->property("defaultStyle").toString();

        // Подсветка с анимацией
        btn->setStyleSheet("background-color: #ff9900; color: #000000;");

        QTimer::singleShot(100, btn, [btn, originalStyle]() {
            btn->setStyleSheet(originalStyle);
        });
    }
}
