#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class TapeWidget;

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

private:
    Ui::MainWindow *ui;
    TapeWidget *tapeWidget;
    QString m_alphabet;
    QString m_extraAlphabet;
    QVector<QString> m_tape;
    int m_headPos;

    void setupTable();
    void updateTapeDisplay(const QString &inputString);
};

#endif // MAINWINDOW_H
