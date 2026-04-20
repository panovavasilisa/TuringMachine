#include "gettingalphabet.h"
#include "ui_gettingalphabet.h"

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
    accept();  // закрывает диалог с результатом QDialog::Accepted
}
