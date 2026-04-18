#include "gettingalphabet.h"
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    GettingAlphabet dialog;
    if (dialog.exec() == QDialog::Accepted) {
        QString alphabet = dialog.getAlphabet();
        QString extra = dialog.getExtraAlphabet();
        MainWindow w(alphabet, extra);
        w.show();
        return a.exec();
    }
    return 0;
}
