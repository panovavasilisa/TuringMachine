#ifndef GETTINGALPHABET_H
#define GETTINGALPHABET_H

#include <QDialog>

namespace Ui {
  class GettingAlphabet;
}

class GettingAlphabet : public QDialog {
    Q_OBJECT

public:
    explicit GettingAlphabet(QWidget *parent = nullptr);
    ~GettingAlphabet();
    QString getAlphabet() const;
    QString getExtraAlphabet() const;

private slots:
    void on_btnSetAlphabet_clicked();

private:
    Ui::GettingAlphabet *ui;
};

#endif // GETTINGALPHABET_H
