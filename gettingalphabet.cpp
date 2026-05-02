#include "gettingalphabet.h"
#include "ui_gettingalphabet.h"
#include <QMessageBox>

GettingAlphabet::GettingAlphabet(QWidget *parent)
    : QDialog(parent), ui(new Ui::GettingAlphabet) {
    ui->setupUi(this);
    this->setStyleSheet("background-color: #c0e0ff;");
}

GettingAlphabet::~GettingAlphabet() {
    delete ui;
}

QString GettingAlphabet::getAlphabet() const {
    return ui->lineEditAlphabet->text();
}

QString GettingAlphabet::getExtraAlphabet() const {
    return ui->lineEditExtra->text();
}
void GettingAlphabet::on_btnSetAlphabet_clicked() {
    QString alphabet = ui->lineEditAlphabet->text().trimmed();
    QString extra = ui->lineEditExtra->text().trimmed();

    // Проверяем пересечение символов
    for (QChar ch : extra) {
        if (alphabet.contains(ch)) {
            QMessageBox::warning(this, "Ошибка",
                                 QString("Дополнительные символы не должны совпадать с символами основного алфавита!\n"
                                         "Символ '%1' уже есть в основном алфавите.").arg(ch));
            return;
        }
    }
    accept();
}
