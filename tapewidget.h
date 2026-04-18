#ifndef TAPEWIDGET_H
#define TAPEWIDGET_H

#include <QWidget>
#include <QVector>
#include <QString>

class TapeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TapeWidget(QWidget *parent = nullptr);

    void setTape(const QVector<QString> &tape, int headPos);
    void updateHeadPosition(int headPos);
    void setCellSize(int size = 60);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QVector<QString> m_tape;
    int m_headPos;
    int m_cellSize;
    int m_offset; // для плавного сдвига
};

#endif // TAPEWIDGET_H
