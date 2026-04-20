#ifndef TAPEWIDGET_H
#define TAPEWIDGET_H

#include <QWidget>
#include <QVector>

class TapeWidget : public QWidget {
    Q_OBJECT
public:
    explicit TapeWidget(QWidget *parent = nullptr);
    void setTape(const QVector<QString> &left, const QVector<QString> &right, int headPos);
    void setCellSize(int size = 30);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QVector<QString> m_left;
    QVector<QString> m_right;
    int m_headPos;
    int m_cellSize;
    int m_offset;
    void scrollToMakeHeadVisible();
};

#endif // TAPEWIDGET_H
