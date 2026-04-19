#include "tapewidget.h"
#include <QPainter>
#include <algorithm>

TapeWidget::TapeWidget(QWidget *parent)
    : QWidget(parent), m_headAbsPos(0), m_cellSize(30), m_offset(0) {
    setMinimumHeight(50);
}

void TapeWidget::setTape(const QVector<QString> &left, const QVector<QString> &right, int headPos) {
    m_left = left;
    m_right = right;
    m_headAbsPos = headPos;  // headPos = m_left.size()
    scrollToMakeHeadVisible();
    update();
}

void TapeWidget::updateHeadPosition(int headPos) {
    m_headAbsPos = headPos;
    scrollToMakeHeadVisible();
    update();
}

void TapeWidget::scrollToMakeHeadVisible() {
    // Строим полную ленту для определения размера
    int fullSize = m_left.size() + m_right.size();
    if (fullSize == 0) return;

    int visibleCells = width() / m_cellSize;
    if (visibleCells <= 0) visibleCells = 1;

    // Если головка левее видимой области
    if (m_headAbsPos < m_offset) {
        m_offset = m_headAbsPos;   // сдвигаем влево до головки
    }
    // Если головка правее видимой области (последний видимый индекс = m_offset + visibleCells - 1)
    else if (m_headAbsPos >= m_offset + visibleCells) {
        // Сдвигаем так, чтобы головка оказалась на 1/3 от левого края (или на 2/3? По ТЗ – на треть длины)
        m_offset = m_headAbsPos - visibleCells / 3;
    }
    // Ограничиваем m_offset
    m_offset = std::max(0, std::min(m_offset, fullSize - visibleCells));
}

void TapeWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    // Собираем полную ленту: левая часть в правильном порядке + правая
    QVector<QString> full;
    for (int i = m_left.size() - 1; i >= 0; --i) full.append(m_left[i]);
    full.append(m_right);

    if (full.isEmpty()) {
        painter.drawText(rect(), Qt::AlignCenter, "Лента пуста");
        return;
    }

    int visibleCells = width() / m_cellSize;
    if (visibleCells <= 0) visibleCells = 1;
    // Корректируем m_offset, если full изменился
    scrollToMakeHeadVisible();

    for (int i = 0; i < visibleCells; ++i) {
        int idx = m_offset + i;
        if (idx >= full.size()) break;
        int x = i * m_cellSize;
        int y = 20;
        painter.drawRect(x, y, m_cellSize, m_cellSize);
        painter.drawText(QRect(x, y, m_cellSize, m_cellSize), Qt::AlignCenter, full[idx]);
        if (idx == m_headAbsPos) {
            QPolygon triangle;
            triangle << QPoint(x + m_cellSize/2, y - 10)
                     << QPoint(x + m_cellSize/2 - 8, y - 2)
                     << QPoint(x + m_cellSize/2 + 8, y - 2);
            painter.setBrush(Qt::red);
            painter.drawPolygon(triangle);
            painter.setBrush(Qt::NoBrush);
        }
    }
}
