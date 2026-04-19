#include "tapewidget.h"
#include <QPainter>

TapeWidget::TapeWidget(QWidget *parent)
    : QWidget(parent), m_headPos(0), m_cellSize(30), m_offset(0) {
    setMinimumHeight(50);
    setMinimumWidth(100);
}

void TapeWidget::setTape(const QVector<QString> &tape, int headPos) {
    m_tape = tape;
    m_headPos = headPos;
    update();
}

void TapeWidget::updateHeadPosition(int headPos) {
    m_headPos = headPos;
    update();
}

void TapeWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if (m_tape.isEmpty()) {
        painter.drawText(rect(), Qt::AlignCenter, "Лента пуста");
        return;
    }

    int visibleCells = width() / m_cellSize;
    // Центрируем головку
    int startIndex = qMax(0, m_headPos - visibleCells / 2);
    startIndex = qMin(startIndex, qMax(0, m_tape.size() - visibleCells));

    for (int i = 0; i < visibleCells && (startIndex + i) < m_tape.size(); ++i) {
        int x = i * m_cellSize;
        int y = 10;

        // Рисуем ячейку
        painter.drawRect(x, y, m_cellSize, m_cellSize);
        // Рисуем символ
        painter.drawText(QRect(x, y, m_cellSize, m_cellSize),
                         Qt::AlignCenter, m_tape[startIndex + i]);

        // Рисуем головку над текущей ячейкой
        if (startIndex + i == m_headPos) {
            QPolygon triangle;
            triangle << QPoint(x + m_cellSize/2, y - 10)
                     << QPoint(x + m_cellSize/2 - 8, y - 2)
                     << QPoint(x + m_cellSize/2 + 8, y - 2);
            painter.setBrush(QColor(255, 155, 255));
            painter.drawPolygon(triangle);
            painter.setBrush(Qt::NoBrush);
        }
    }
}
