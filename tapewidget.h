#ifndef TAPEWIDGET_H
#define TAPEWIDGET_H

#include <QWidget>
#include <QVector>

class TapeWidget : public QWidget {
    Q_OBJECT
public:
    explicit TapeWidget(QWidget *parent = nullptr);
    void setTape(const QVector<QString> &left, const QVector<QString> &right, int headPos);
    void setCellSize(int size = 60);
    void updateHeadPosition(int headPos); // если нужно обновить только позицию
    void scrollToMakeHeadVisible();       // принудительно сдвинуть, чтобы головка была видна

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QVector<QString> m_left;   // слева от головки (обратный порядок)
    QVector<QString> m_right;  // справа, включая головку
    int m_headAbsPos;          // абсолютная позиция головки (количество символов слева)
    int m_cellSize;
    int m_offset;              // индекс первого отображаемого символа в полной ленте
};

#endif // TAPEWIDGET_H
