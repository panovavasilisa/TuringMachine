#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tapewidget.h"
#include "gettingalphabet.h"
#include <QMessageBox>
#include <QDebug>
#include <QStyle>
#include <utility>

MainWindow::MainWindow(const QString &alphabet, const QString &extraAlphabet, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), m_alphabet(alphabet)
    , m_extraAlphabet(extraAlphabet) {
    ui->setupUi(this);

    this->setStyleSheet("QMainWindow { background-color: #c0e0ff; }");

    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &MainWindow::step);
    m_delayMs = 500;   // задержка 500 мс
    if (ui->Speed) {
        ui->Speed->setValue(m_delayMs);
        connect(ui->Speed, &QSlider::valueChanged, this, &MainWindow::on_Speed_valueChanged);
    }
    connect(ui->tableProgram, &QTableWidget::cellChanged, this, &MainWindow::on_tableProgram_cellChanged);


    tapeWidget = ui->tapeWidget;

    setupTable();

    m_currentState = "q0";
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &MainWindow::step);
    m_delayMs = 500;
    if (ui->Speed) {
        ui->Speed->setValue(m_delayMs);
        connect(ui->Speed, &QSlider::valueChanged, this, &MainWindow::on_Speed_valueChanged);
    }

    m_left.clear();
    m_right.clear();
    tapeWidget->setTape(m_left, m_right, m_left.size());

    // Белая таблица
    ui->tableProgram->setAlternatingRowColors(false);
    ui->tableProgram->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableProgram->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableProgram->setStyleSheet(
        "QTableWidget { background-color: white; }"
        "QTableWidget::item { background-color: white; }"
        "QTableWidget::item:selected { background-color: white; }"
        "QTableWidget::item:hover { background-color: white; }"
        "QHeaderView::section { background-color: white; }"
        );
    QPalette pal = ui->tableProgram->palette();
    pal.setColor(QPalette::Base, Qt::white);
    pal.setColor(QPalette::AlternateBase, Qt::white);
    pal.setColor(QPalette::Highlight, Qt::white);
    ui->tableProgram->setPalette(pal);

    ui->tableProgram->setStyleSheet(
        "QTableWidget { background-color: white; }"
        "QTableWidget::item { background-color: white; color: black; }"
        "QTableWidget::item:selected { background-color: #d0d0d0; color: black; }"
        "QTableWidget::item:hover { background-color: #f0f0f0; }"
        "QHeaderView::section { background-color: white; }"
        );

    // Иконки на кнопках
    ui->btnRun->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    ui->btnStop->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
    ui->btnStep->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));
    ui->btnReset->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));
    ui->btnRun->setIconSize(QSize(24,24));
    ui->btnStop->setIconSize(QSize(24,24));
    ui->btnStep->setIconSize(QSize(24,24));
    ui->btnReset->setIconSize(QSize(24,24));
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setupTable() {
    // 1. Сохраняем старые данные, если таблица уже была
    QVector<QVector<QString>> oldData;
    int oldRows = ui->tableProgram->rowCount();
    int oldCols = ui->tableProgram->columnCount();

    for (int r = 0; r < oldRows; ++r) {
        QVector<QString> row;
        for (int c = 0; c < oldCols; ++c) {
            QTableWidgetItem *item = ui->tableProgram->item(r, c);
            row.append(item ? item->text() : "");
        }
        oldData.append(row);
    }

    // 2. Формируем список уникальных символов алфавита + пустой символ "Λ"
    QString emptySymbol = "Λ";
    QString allSymbols = m_alphabet + emptySymbol + m_extraAlphabet;

    QVector<QChar> unique;
    for (QChar ch : std::as_const(allSymbols)) {
        if (!unique.contains(ch)) unique.append(ch);
    }

    // 3. Настраиваем новую таблицу
    int newCols = unique.size() + 1; // +1 для столбца "Состояние"
    ui->tableProgram->setColumnCount(newCols);
    ui->tableProgram->verticalHeader()->setDefaultSectionSize(35);   // высота строк
    ui->tableProgram->horizontalHeader()->setDefaultSectionSize(100); // ширина столбцов

    QStringList headers;
    headers << "Состояние";
    for (QChar ch : std::as_const(unique)) headers << QString(ch);
    ui->tableProgram->setHorizontalHeaderLabels(headers);

    // 4. Определяем количество строк (минимум 1)
    int newRows = qMax(1, oldRows);
    ui->tableProgram->setRowCount(newRows);

    // 5. Восстанавливаем данные
    for (int r = 0; r < newRows; ++r) {
        // Столбец "Состояние"
        QString state;
        if (r < oldRows && !oldData[r][0].isEmpty()) {
            state = oldData[r][0];
        } else {
            state = QString("q%1").arg(r);
        }
        ui->tableProgram->setItem(r, 0, new QTableWidgetItem(state));

        // Столбцы символов
        for (int c = 1; c < newCols; ++c) {
            QString symbol = ui->tableProgram->horizontalHeaderItem(c)->text();
            QString oldValue = "";

            // Ищем старый столбец с таким же символом
            if (r < oldRows) {
                for (int oc = 1; oc < oldCols; ++oc) {
                    QString oldSymbol = ui->tableProgram->horizontalHeaderItem(oc)->text();
                    if (oldSymbol == symbol) {
                        oldValue = oldData[r][oc];
                        break;
                    }
                }
            }
            ui->tableProgram->setItem(r, c, new QTableWidgetItem(oldValue));
        }
    }

    // 6. Настройки внешнего вида таблицы
    ui->tableProgram->verticalHeader()->setDefaultSectionSize(30); // высота строк 30 пикселей
    ui->tableProgram->horizontalHeader()->setDefaultSectionSize(100); // ширина столбцов 100

    ui->tableProgram->resizeColumnsToContents();   // чтобы содержимое было видно
    ui->tableProgram->resizeRowsToContents();
    ui->tableProgram->setWordWrap(true);           // перенос длинных команд
    //ui->tableProgram->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    ui->tableProgram->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

}

void MainWindow::updateTapeDisplay(const QString &inputString) {
    m_initialString = inputString;
    // Проверка символов
    for (QChar ch : std::as_const(inputString)) {
        if (!m_alphabet.contains(ch)) {
            QMessageBox::warning(this, "Ошибка", "Символ '" + QString(ch) + "' не входит в алфавит");
            return;
        }
    }
    m_left.clear();
    m_right.clear();
    // Заполняем правую часть: входная строка
    for (QChar ch : std::as_const(inputString))
        m_right.append(QString(ch));
    // Добавляем запас пустых символов справа (для движения)
    for (int i = 0; i < 5; ++i)
        m_right.append("Λ");
    // Головка в начале (m_left пуст, m_right[0] – первый символ строки)
    tapeWidget->setTape(m_left, m_right, m_left.size());
}

void MainWindow::moveLeft() {
    if (!m_left.isEmpty()) {
        m_right.prepend(m_left.last());
        m_left.removeLast();
    } else {
        m_right.prepend("Λ");
    }
    tapeWidget->setTape(m_left, m_right, m_left.size());
}

void MainWindow::moveRight() {
    if (m_right.isEmpty()) return;
    // переносим текущий символ в левую часть
    m_left.append(m_right[0]);
    m_right.removeFirst();
    if (m_right.isEmpty())
        m_right.append("Λ");
    tapeWidget->setTape(m_left, m_right, m_left.size());
}

void MainWindow::loadProgramFromTable() {
    m_program.clear();
    int rows = ui->tableProgram->rowCount();
    int cols = ui->tableProgram->columnCount();
    for (int r = 0; r < rows; ++r) {
        QTableWidgetItem *stateItem = ui->tableProgram->item(r, 0);
        if (!stateItem) continue;
        QString state = stateItem->text().trimmed();
        if (state.isEmpty()) continue;

        for (int c = 1; c < cols; ++c) {
            QTableWidgetItem *headerItem = ui->tableProgram->horizontalHeaderItem(c);
            if (!headerItem) continue;
            QString readSymbol = headerItem->text().trimmed(); // символ, который видит головка

            QTableWidgetItem *cmdItem = ui->tableProgram->item(r, c);
            if (!cmdItem) continue;
            QString cmd = cmdItem->text().trimmed();
            if (cmd.isEmpty()) continue;

            // Разбор команды: write, direction, nextState
            QStringList parts = cmd.split(',');
            if (parts.size() != 3) {
                cmdItem->setBackground(Qt::red);
                continue;
            }
            QString writeSymbol = parts[0].trimmed();
            QString direction = parts[1].trimmed().toUpper();
            QString nextState = parts[2].trimmed();

            // Проверка: writeSymbol должен быть в алфавите или быть "Λ"
            if (!m_alphabet.contains(writeSymbol) && writeSymbol != "Λ") {
                cmdItem->setBackground(Qt::red);
                continue;
            }
            if (direction != "L" && direction != "R") {
                cmdItem->setBackground(Qt::red);
                continue;
            }
            // Проверка существования следующего состояния
            bool stateExists = false;
            for (int i = 0; i < rows; ++i) {
                QTableWidgetItem *item = ui->tableProgram->item(i, 0);
                if (item && item->text().trimmed() == nextState) {
                    stateExists = true;
                    break;
                }
            }
            if (!stateExists && nextState != "!") { // "!" – терминальное состояние
                cmdItem->setBackground(Qt::red);
                continue;
            }
            // Всё хорошо
            cmdItem->setBackground(Qt::white);
            m_program[state][readSymbol] = {writeSymbol, direction, nextState};
        }
    }
}

void MainWindow::step() {
    QString currentSymbol = readSymbol();
    if (!m_program.contains(m_currentState) ||
        !m_program[m_currentState].contains(currentSymbol)) {
        m_timer->stop();
        QMessageBox::information(this, "Машина Тьюринга", "Выполнение завершено (нет правила)");
        return;
    }
    Transition t = m_program[m_currentState][currentSymbol];
    // Запись символа
    writeSymbol(t.writeSymbol);
    // Движение
    if (t.move == "L")
        moveLeft();
    else if (t.move == "R")
        moveRight();
    // Смена состояния
    if (t.nextState == "!") {
        m_timer->stop();
        QMessageBox::information(this, "Машина Тьюринга", "Программа завершена (!)");
        return;
    }
    m_currentState = t.nextState;
    highlightCurrentState();
}

void MainWindow::highlightCurrentState() {
    for (int i = 0; i < ui->tableProgram->rowCount(); ++i) {
        ui->tableProgram->item(i, 0)->setBackground(Qt::white);
    }
    for (int i = 0; i < ui->tableProgram->rowCount(); ++i) {
        if (ui->tableProgram->item(i, 0)->text() == m_currentState) {
            ui->tableProgram->item(i, 0)->setBackground(QColor(200, 200, 200));
            break;
        }
    }
}

void MainWindow::writeSymbol(const QString &sym) {
    if (!m_right.isEmpty())
        m_right[0] = sym;
    tapeWidget->setTape(m_left, m_right, m_left.size());
}

QString MainWindow::readSymbol() const {
    return m_right.isEmpty() ? "Λ" : m_right[0];
}

// Слоты
void MainWindow::on_btnSetString_clicked() {
    QString input = ui->lineEditString->text();
    if (input.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите строку");
        return;
    }
    updateTapeDisplay(input);
}

void MainWindow::on_btnChangeAlphabet_clicked() {
    GettingAlphabet dialog;
    if (dialog.exec() == QDialog::Accepted) {
        m_alphabet = dialog.getAlphabet();
        m_extraAlphabet = dialog.getExtraAlphabet();
        setupTable();
        ui->lineEditString->clear();
        m_left.clear();
        m_right.clear();
        tapeWidget->setTape(m_left, m_right, m_left.size());
    }
    m_initialString.clear();
}

void MainWindow::on_btnRun_clicked() {
    if (m_timer->isActive()) return;
    // Перед запуском перезагружаем программу из таблицы
    loadProgramFromTable();
    // Проверяем, есть ли вообще правила
    if (m_program.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Нет ни одного правила");
        return;
    }
    m_timer->start(m_delayMs);
}

void MainWindow::on_btnStop_clicked() {
    m_timer->stop();
}

void MainWindow::on_btnStep_clicked() {
    if (m_timer->isActive()) return;
    loadProgramFromTable();
    step();
}

void MainWindow::on_btnReset_clicked() {
    m_timer->stop();
    if (!m_initialString.isEmpty())
        updateTapeDisplay(m_initialString);
    else {
        m_left.clear();
        m_right.clear();
        tapeWidget->setTape(m_left, m_right, 0);
    }
    m_currentState = "q0";
    highlightCurrentState();
}

void MainWindow::on_btnAddState_clicked() {
    int row = ui->tableProgram->rowCount();
    ui->tableProgram->insertRow(row);
    ui->tableProgram->setItem(row, 0, new QTableWidgetItem(QString("q%1").arg(row)));
}

void MainWindow::on_btnRemoveState_clicked() {
    int row = ui->tableProgram->rowCount();
    if (row > 1) ui->tableProgram->removeRow(row - 1);
}

void MainWindow::on_Speed_valueChanged(int value) {
    m_delayMs = value;
    if (m_timer->isActive()) {
        m_timer->setInterval(m_delayMs);
    }
}

void MainWindow::on_tableProgram_cellChanged(int /*row*/, int /*column*/) {
    loadProgramFromTable();
}

void MainWindow::on_btnInsertEmpty_clicked() {
    QTableWidgetItem *item = ui->tableProgram->currentItem();
    if (!item || item->column() == 0) return;
    QString old = item->text();
    item->setText(old + "Λ");
    // Устанавливаем фокус на ячейку
    ui->tableProgram->setCurrentItem(item);
    // Если нужно поставить курсор в конец, можно запустить таймером:
    QTimer::singleShot(10, [this, item]() {
        ui->tableProgram->editItem(item);
    });
}
