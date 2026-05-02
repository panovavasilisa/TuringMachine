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
    m_delayMs = 500;
    if (ui->Speed) {
        ui->Speed->setValue(m_delayMs);
        connect(ui->Speed, &QSlider::valueChanged, this, &MainWindow::on_Speed_valueChanged);
    }


    tapeWidget = ui->tapeWidget;

    setupTable();
    m_currentState = "q0";
    highlightCurrentState();

    m_left.clear();
    m_right.clear();
    tapeWidget->setTape(m_left, m_right, m_left.size());

    // Белая таблица
    ui->tableProgram->setAlternatingRowColors(false);
    ui->tableProgram->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableProgram->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableProgram->setStyleSheet(
        "QTableWidget { background-color: white; }"
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
    //highlightCurrentState();
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
    int newCols = unique.size() + 1;
    ui->tableProgram->setColumnCount(newCols);
    ui->tableProgram->verticalHeader()->setDefaultSectionSize(50);
    ui->tableProgram->horizontalHeader()->setDefaultSectionSize(200);

    QStringList headers;
    headers << "Состояние";
    for (QChar ch : std::as_const(unique)) headers << QString(ch);
    ui->tableProgram->setHorizontalHeaderLabels(headers);

    // 4. Определяем количество строк
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
    ui->tableProgram->verticalHeader()->setDefaultSectionSize(50);
    ui->tableProgram->horizontalHeader()->setDefaultSectionSize(200);

    ui->tableProgram->resizeColumnsToContents();   // чтобы содержимое было видно
    ui->tableProgram->resizeRowsToContents();
    ui->tableProgram->setWordWrap(true);           // перенос длинных команд
    ui->tableProgram->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    ui->tableProgram->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

void MainWindow::updateTapeDisplay(const QString &inputString) {
    m_initialString = inputString;
    for (QChar ch : std::as_const(inputString)) {
        if (!m_alphabet.contains(ch)) {
            QMessageBox::warning(this, "Ошибка", "Символ '" + QString(ch) + "' не входит в алфавит");
            return;
        }
    }
    m_left.clear();
    m_right.clear();
    for (QChar ch : std::as_const(inputString))
        m_right.append(QString(ch));
    //запас пустых символов
    for (int i = 0; i < 5; ++i)
        m_right.append("Λ");
    m_headPos = 0;
    tapeWidget->setTape(m_left, m_right, m_headPos);
}

void MainWindow::moveLeft() {
    m_headPos--;
    if (m_headPos < -m_left.size()) {
        m_left.prepend("Λ");
    }
    tapeWidget->setTape(m_left, m_right, m_headPos);
}

void MainWindow::moveRight() {
    m_headPos++;
    if (m_headPos >= m_right.size()) {
        m_right.append("Λ");
    }
    tapeWidget->setTape(m_left, m_right, m_headPos);
}

void MainWindow::loadProgramFromTable() {
    m_program.clear();
    m_tableHasErrors = false;
    m_lastErrors.clear();

    if (!ui || !ui->tableProgram) return;

    int rows = ui->tableProgram->rowCount();
    int cols = ui->tableProgram->columnCount();
    if (rows <= 0 || cols <= 0) return;

    int firstErrorRow = -1, firstErrorCol = -1;

    for (int r = 0; r < rows; ++r) {
        QTableWidgetItem *stateItem = ui->tableProgram->item(r, 0);
        if (!stateItem) continue;
        QString state = stateItem->text().trimmed();
        if (state.isEmpty()) continue;

        for (int c = 1; c < cols; ++c) {
            QTableWidgetItem *headerItem = ui->tableProgram->horizontalHeaderItem(c);
            if (!headerItem) continue;
            QString readSymbol = headerItem->text().trimmed();
            if (readSymbol.isEmpty()) continue;

            QTableWidgetItem *cmdItem = ui->tableProgram->item(r, c);
            if (!cmdItem) continue;
            QString cmd = cmdItem->text().trimmed();

            // Пустые ячейки – не ошибка
            if (cmd.isEmpty()) {
                cmdItem->setBackground(Qt::white);
                continue;
            }

            // Сброс фона
            cmdItem->setBackground(Qt::white);

            // 1. Проверка формата
            QStringList parts = cmd.split(',');
            if (parts.size() != 3) {
                cmdItem->setBackground(Qt::red);
                m_lastErrors << QString("Строка %1, столбец %2: не хватает запятых (нужно 2 запятые)").arg(r+1).arg(c+1);
                m_tableHasErrors = true;
                if (firstErrorRow == -1) { firstErrorRow = r; firstErrorCol = c; }
                continue;
            }

            QString writeSymbol = parts[0].trimmed();
            QString direction = parts[1].trimmed().toUpper();
            QString nextState = parts[2].trimmed();

            bool cellError = false;

            // 2. Проверка writeSymbol (если не пусто)
            if (!writeSymbol.isEmpty()) {
                if (!m_alphabet.contains(writeSymbol) && writeSymbol != "Λ" && !m_extraAlphabet.contains(writeSymbol)) {
                    cmdItem->setBackground(Qt::red);
                    m_lastErrors << QString("Строка %1, столбец %2: символ '%3' не входит в алфавит").arg(r+1).arg(c+1).arg(writeSymbol);
                    m_tableHasErrors = true;
                    cellError = true;
                    if (firstErrorRow == -1) { firstErrorRow = r; firstErrorCol = c; }
                }
            }

            // 3. Проверка direction (если не пусто и ещё нет ошибки в этой ячейке)
            if (!direction.isEmpty() && !cellError) {
                if (direction != "L" && direction != "R") {
                    cmdItem->setBackground(Qt::red);
                    m_lastErrors << QString("Строка %1, столбец %2: направление '%3' должно быть L или R").arg(r+1).arg(c+1).arg(direction);
                    m_tableHasErrors = true;
                    cellError = true;
                    if (firstErrorRow == -1) { firstErrorRow = r; firstErrorCol = c; }
                }
            }

            // 4. Проверка nextState (если не пусто, не "!" и ещё нет ошибки)
            if (!nextState.isEmpty() && nextState != "!" && !cellError) {
                bool stateExists = false;
                for (int i = 0; i < rows; ++i) {
                    QTableWidgetItem *item = ui->tableProgram->item(i, 0);
                    if (item && item->text().trimmed() == nextState) {
                        stateExists = true;
                        break;
                    }
                }
                if (!stateExists) {
                    cmdItem->setBackground(Qt::red);
                    m_lastErrors << QString("Строка %1, столбец %2: состояние '%3' не существует").arg(r+1).arg(c+1).arg(nextState);
                    m_tableHasErrors = true;
                    cellError = true;
                    if (firstErrorRow == -1) { firstErrorRow = r; firstErrorCol = c; }
                }
            }

            // Если ошибок в ячейке нет – добавляем правило
            if (!cellError) {
                cmdItem->setBackground(Qt::white);
                m_program[state][readSymbol] = {writeSymbol, direction, nextState};
            }
        }
    }

    // Если есть ошибки, выделяем первую неправильную ячейку
    if (m_tableHasErrors && firstErrorRow != -1 && firstErrorCol != -1) {
        QTableWidgetItem *firstErrorItem = ui->tableProgram->item(firstErrorRow, firstErrorCol);
        if (firstErrorItem) {
            ui->tableProgram->setCurrentItem(firstErrorItem);
            ui->tableProgram->scrollToItem(firstErrorItem, QAbstractItemView::EnsureVisible);
        }
    }

    if (m_tableHasErrors) {
        qDebug() << "Ошибки в таблице:" << m_lastErrors;
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

    // Запись символа, только если он указан
    if (!t.writeSymbol.isEmpty()) {
        writeSymbol(t.writeSymbol);
    }
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
    if (!t.nextState.isEmpty()) {
        m_currentState = t.nextState;
    }
    highlightCurrentState();
}


void MainWindow::highlightCurrentState() {
    for (int r = 0; r < ui->tableProgram->rowCount(); ++r) {
        for (int c = 0; c < ui->tableProgram->columnCount(); ++c) {
            QTableWidgetItem *item = ui->tableProgram->item(r, c);
            if (item) item->setBackground(Qt::white);
        }
    }

    QString currentSymbol = readSymbol();
    bool foundState = false;

    for (int r = 0; r < ui->tableProgram->rowCount(); ++r) {
        QTableWidgetItem *stateItem = ui->tableProgram->item(r, 0);
        if (stateItem && stateItem->text().trimmed() == m_currentState) {
            //stateItem->setBackground(QColor(255, 155, 255));
            foundState = true;

            // Ищем столбец, соответствующий читаемому символу
            for (int c = 1; c < ui->tableProgram->columnCount(); ++c) {
                QTableWidgetItem *headerItem = ui->tableProgram->horizontalHeaderItem(c);
                if (headerItem && headerItem->text().trimmed() == currentSymbol) {
                    QTableWidgetItem *cellItem = ui->tableProgram->item(r, c);
                    if (cellItem) {
                        cellItem->setBackground(QColor(255, 155, 255));
                    }
                    break;
                }
            }
            break;
        }
    }

    if (!foundState) {
        qDebug() << "Состояние" << m_currentState << "не найдено в таблице";
    }

    ui->tableProgram->viewport()->update();
}

void MainWindow::writeSymbol(const QString &sym) {
    if (sym.isEmpty()) return; // нечего писать
    if (m_headPos >= 0) {
        while (m_headPos >= m_right.size()) {
            m_right.append("Λ");
        }
        m_right[m_headPos] = sym;
    } else {
        int idx = -m_headPos - 1;
        while (idx >= m_left.size()) {
            m_left.append("Λ");
        }
        m_left[idx] = sym;
    }
    tapeWidget->setTape(m_left, m_right, m_headPos);
}

QString MainWindow::readSymbol() const {
    if (m_headPos >= 0) {
        if (m_headPos < m_right.size())
            return m_right[m_headPos];
    } else {
        int idx = -m_headPos - 1;
        if (idx < m_left.size())
            return m_left[idx];
    }
    return "Λ";
}

// Слоты
void MainWindow::on_btnSetString_clicked() {
    QString input = ui->lineEditString->text();
    if (input.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите строку");
        return;
    }
    updateTapeDisplay(input);
    highlightCurrentState();
}

void MainWindow::on_btnChangeAlphabet_clicked() {
    GettingAlphabet dialog;
    if (dialog.exec() == QDialog::Accepted) {
        m_alphabet = dialog.getAlphabet();
        m_extraAlphabet = dialog.getExtraAlphabet();
        setupTable();
        // Проверка существования текущего состояния
        bool exists = false;
        for (int i = 0; i < ui->tableProgram->rowCount(); ++i) {
            if (ui->tableProgram->item(i, 0)->text().trimmed() == m_currentState) {
                exists = true;
                break;
            }
        }
        if (!exists) m_currentState = "q0";
        highlightCurrentState();
        ui->lineEditString->clear();
        m_left.clear();
        m_right.clear();
        tapeWidget->setTape(m_left, m_right, 0);
    }
    m_initialString.clear();
}

void MainWindow::on_btnRun_clicked() {
    if (m_timer->isActive()) return;

    if (m_right.isEmpty() && m_left.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Сначала задайте строку (кнопка «Задать строку»)");
        return;
    }

    loadProgramFromTable();

    if (m_tableHasErrors) {
        QMessageBox::warning(this, "Ошибка", "В таблице есть ошибки.\n" + m_lastErrors.join("\n"));
        return;
    }
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
    if (m_tableHasErrors) {
        QMessageBox::warning(this, "Ошибка", "В таблице есть ошибки.\nИсправьте их перед выполнением шага.");
        return;
    }
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

void MainWindow::on_btnInsertEmpty_clicked() {
    QTableWidgetItem *item = ui->tableProgram->currentItem();
    if (!item || item->column() == 0) return;
    QString old = item->text();
    item->setText(old + "Λ");
    ui->tableProgram->setCurrentItem(item);
    QTimer::singleShot(10, [this, item]() {
        ui->tableProgram->editItem(item);
    });
}
