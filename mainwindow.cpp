#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tapewidget.h"
#include "gettingalphabet.h"
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(const QString &alphabet, const QString &extraAlphabet, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_alphabet(alphabet)
    , m_extraAlphabet(extraAlphabet)
    , m_headPos(0) {
    ui->setupUi(this);
    tapeWidget = ui->tapeWidget;
    setupTable();
    m_tape.clear();
    tapeWidget->setTape(m_tape, m_headPos);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setupTable() {
    QString emptySymbol = "Λ";
    QString allSymbols = m_alphabet + emptySymbol + m_extraAlphabet;
    // Убираем дубликаты
    QVector<QChar> unique;
    for (QChar ch : allSymbols) {
        if (!unique.contains(ch)) unique.append(ch);
    }
    int colCount = unique.size() + 1;
    ui->tableProgram->setColumnCount(colCount);
    QStringList headers;
    headers << "Состояние";
    for (QChar ch : unique) headers << QString(ch);
    ui->tableProgram->setHorizontalHeaderLabels(headers);
    ui->tableProgram->setRowCount(1);
    ui->tableProgram->setItem(0, 0, new QTableWidgetItem("q0"));
    ui->tableProgram->setEnabled(true);
}

void MainWindow::updateTapeDisplay(const QString &inputString) {
    for (QChar ch : inputString) {
        if (!m_alphabet.contains(ch)) {
            QMessageBox::warning(this, "Ошибка", "Символ '" + QString(ch) + "' не входит в алфавит");
            return;
        }
    }
    m_tape.clear();
    for (int i = 0; i < 5; ++i) m_tape.append("Λ");
    for (QChar ch : inputString) m_tape.append(QString(ch));
    for (int i = 0; i < 5; ++i) m_tape.append("Λ");
    m_headPos = 5;
    tapeWidget->setTape(m_tape, m_headPos);
}

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
        m_tape.clear();
        tapeWidget->setTape(m_tape, 0);
    }
}

void MainWindow::on_btnRun_clicked() { qDebug() << "Run"; }
void MainWindow::on_btnStop_clicked() { qDebug() << "Stop"; }
void MainWindow::on_btnStep_clicked() { qDebug() << "Step"; }
void MainWindow::on_btnReset_clicked() { qDebug() << "Reset"; }
void MainWindow::on_Speed_valueChanged(int value) { qDebug() << "Change speed";}

void MainWindow::on_btnAddState_clicked() {
    int row = ui->tableProgram->rowCount();
    ui->tableProgram->insertRow(row);
    ui->tableProgram->setItem(row, 0, new QTableWidgetItem(QString("q%1").arg(row)));
}

void MainWindow::on_btnRemoveState_clicked() {
    int row = ui->tableProgram->rowCount();
    if (row > 1) ui->tableProgram->removeRow(row - 1);
}
