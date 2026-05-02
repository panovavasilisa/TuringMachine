#include "tapewidget.h"
#include <QPainter>
#include <algorithm>

TapeWidget::TapeWidget(QWidget *parent)
    : QWidget(parent), m_headPos(0), m_cellSize(30), m_offset(0) {
    setMinimumHeight(50);
}

void TapeWidget::setTape(const QVector<QString> &left, const QVector<QString> &right, int headPos) {
    m_left = left;
    m_right = right;
    m_headPos = headPos;
    scrollToMakeHeadVisible();
    update();
}

void TapeWidget::scrollToMakeHeadVisible() {
    int fullSize = m_left.size() + m_right.size();
    if (fullSize == 0) return;
    int headAbsPos = m_left.size() + m_headPos;
    int visibleCells = width() / m_cellSize;
    if (visibleCells <= 0) visibleCells = 1;

    if (headAbsPos < m_offset) {
        m_offset = headAbsPos - visibleCells / 3;
    } else if (headAbsPos >= m_offset + visibleCells) {
        m_offset = headAbsPos - visibleCells / 3;
    }
    m_offset = std::max(0, std::min(m_offset, fullSize - visibleCells));
    if (m_offset < 0) m_offset = 0;
}

void TapeWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    QVector<QString> full = m_left;
    full.append(m_right);

    if (full.isEmpty()) {
        painter.drawText(rect(), Qt::AlignCenter, "Лента пуста");
        return;
    }

    int headAbsPos = m_left.size() + m_headPos;
    int visibleCells = width() / m_cellSize;
    if (visibleCells <= 0) visibleCells = 1;
    scrollToMakeHeadVisible();

    for (int i = 0; i < visibleCells; ++i) {
        int idx = m_offset + i;
        if (idx >= full.size()) break;
        int x = i * m_cellSize;
        int y = 20;
        painter.drawRect(x, y, m_cellSize, m_cellSize);
        painter.drawText(QRect(x, y, m_cellSize, m_cellSize), Qt::AlignCenter, full[idx]);
        if (idx == headAbsPos) {
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
