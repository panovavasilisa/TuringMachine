#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QString>
#include <QMap>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class TapeWidget;

struct Transition {
    QString writeSymbol;
    QString move;      // "L" или "R"
    QString nextState;
};

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(const QString &alphabet, const QString &extraAlphabet, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnSetString_clicked();
    void on_btnChangeAlphabet_clicked();
    void on_btnRun_clicked();
    void on_btnStop_clicked();
    void on_btnStep_clicked();
    void on_btnReset_clicked();
    void on_btnAddState_clicked();
    void on_btnRemoveState_clicked();
    void on_Speed_valueChanged(int value);
    void on_tableProgram_cellChanged(int row, int column);
    void on_btnInsertEmpty_clicked();

private:
    Ui::MainWindow *ui;
    TapeWidget *tapeWidget;
    QString m_alphabet;
    QString m_extraAlphabet;

    QVector<QString> m_left;
    QVector<QString> m_right;
     int m_headPos;

    // Программа
    QMap<QString, QMap<QString, Transition>> m_program;
    QString m_currentState;
    QTimer *m_timer;
    int m_delayMs;

    void setupTable();
    void updateTapeDisplay(const QString &inputString);
    void loadProgramFromTable();
    void step();                         // один шаг машины
    void stopAndReset();                 // остановка таймера и сброс флагов
    void highlightCurrentState();        // подсветка строки состояния в таблице

    // Работа с лентой
    QString readSymbol() const;
    void writeSymbol(const QString &sym);
    void moveLeft();
    void moveRight();

    QString m_initialString;

};

#endif // MAINWINDOW_H
