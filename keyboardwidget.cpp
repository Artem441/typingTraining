#include "keyboardwidget.h"
#include <QTimer>
#include <QSpacerItem>
#include <QDebug>

keyboardwidget::keyboardwidget(QWidget *parent)
    : QWidget{parent}
{
    layout = new QGridLayout(this);
    layout->setSpacing(6);  // Чуть больше отступов между кнопками
    layout->setAlignment(Qt::AlignCenter);
    setRussianLayout();
}

void keyboardwidget::setRussianLayout()
{
    qDeleteAll(buttons);
    buttons.clear();

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
        "   background-color: #c0c0c0;"
        "}";

    QStringList russianKeys = {
        "Й", "Ц", "У", "К", "Е", "Н", "Г", "Ш", "Щ", "З", "Х", "Ъ",
        "Ф", "Ы", "В", "А", "П", "Р", "О", "Л", "Д", "Ж", "Э",
        "Я", "Ч", "С", "М", "И", "Т", "Ь", "Б", "Ю", "."
    };

    QList<int> rowBreaks = {12, 11, 10}; // кол-во кнопок на каждой строке

    int row = 0, col = 0, keyIndex = 0;
    for (int lineKeys : rowBreaks) {
        col = 0;
        int offset = (12 - lineKeys) / 2; // центрируем строку
        for (int i = 0; i < offset; ++i)
            layout->addItem(new QSpacerItem(40, 40, QSizePolicy::Fixed, QSizePolicy::Fixed), row, col++);

        for (int i = 0; i < lineKeys; ++i, ++keyIndex) {
            const QString& key = russianKeys[keyIndex];
            QPushButton* btn = new QPushButton(key, this);
            btn->setStyleSheet(defaultStyle);
            btn->setProperty("defaultStyle", defaultStyle);
            connect(btn, &QPushButton::pressed, this, [this, key]() {
                emit keyPressed(key);
                highlightKey(key);
            });
            layout->addWidget(btn, row, col++, Qt::AlignCenter);
            buttons[key] = btn;
        }
        ++row;
    }

    QPushButton *spaceBtn = new QPushButton("Пробел", this);
    spaceBtn->setStyleSheet(defaultStyle +
                            " QPushButton { min-width: 480px; max-width: 480px; }");
    spaceBtn->setProperty("defaultStyle", spaceBtn->styleSheet());

    connect(spaceBtn, &QPushButton::pressed, this, [this]() {
        emit keyPressed(" ");
        highlightKey(" ");
    });

    layout->addWidget(spaceBtn, row, 0, 1, 12, Qt::AlignCenter);
    buttons[" "] = spaceBtn;
}

void keyboardwidget::highlightKey(const QString &key)
{

    if (buttons.contains(key)) {
        QPushButton* btn = buttons[key];
        const QString originalStyle = btn->property("defaultStyle").toString();

        btn->setStyleSheet(originalStyle + " QPushButton { background-color: #ffcc66; }");

        QTimer::singleShot(150, btn, [btn, originalStyle]() {
            btn->setStyleSheet(originalStyle);
        });
    }
}

void keyboardwidget::setLayoutByName(const QString &layoutName)
{
    if (layoutName == "Русский") {
        setRussianLayout();
    }
    else if (layoutName == "English") {
        setEnglishLayout();
    }
}

void keyboardwidget::setEnglishLayout()
{
    qDeleteAll(buttons);
    buttons.clear();

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
        "   background-color: #c0c0c0;"
        "}";
    QStringList englishKeys = {
        "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P",
        "A", "S", "D", "F", "G", "H", "J", "K", "L",
        "Z", "X", "C", "V", "B", "N", "M", "."
    };
    QList<int> rowBreaks = {10,9,8};
    int row = 0, col = 0, keyIndex = 0;
    for (int lineKeys : rowBreaks) {
        col = 0;
        int offset = (12 - lineKeys) / 2;
        for (int i = 0; i < offset; ++i)
            layout->addItem(new QSpacerItem(40,40, QSizePolicy::Fixed),row,col++);
        for (int i = 0; i < lineKeys;++i,++keyIndex) {
            const QString& key = englishKeys[keyIndex];
            QPushButton* btn = new QPushButton(key,this);
            btn->setStyleSheet(defaultStyle);
            btn->setProperty("defaultStyle",defaultStyle);
            connect(btn,&QPushButton::pressed,this,[this,key]() {
                emit keyPressed(key);
                highlightKey(key);
            });
            layout->addWidget(btn,row,col++,Qt::AlignCenter);
            buttons[key] = btn;
        }
        ++row;
    }
    QPushButton *spaceBtn = new QPushButton("Space",this);
    spaceBtn->setStyleSheet(defaultStyle + " QPushButton { min-width: 480px; max-width: 480px; }");
    spaceBtn->setProperty("defaultStyle", spaceBtn->styleSheet());
    connect(spaceBtn,&QPushButton::pressed,this,[this]() {
        emit keyPressed(" ");
        highlightKey(" ");
    });
    layout->addWidget(spaceBtn,row,0,1,12,Qt::AlignCenter);
    buttons[" "] = spaceBtn;
}

