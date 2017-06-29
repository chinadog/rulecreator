#include "linenumberarea.h"
#include "codetextedit.h"

LineNumberArea::LineNumberArea(CodeTextEdit *editor) :
    QWidget(editor)
{
    m_codeEditor = editor;
    QFont font;
    font.setFamily("Сonsolas");
    font.setFixedPitch(true);
    setFont(font);
}

/**
 * @param event событие отрисовки
 * @details Отрисовка поля нумерации строк. Вызывает у текстового поля
 * TelegramTextEdit соответствующий метод отрисовки.
 */
void LineNumberArea::paintEvent(QPaintEvent *event)
{
    m_codeEditor->lineNumberAreaPaintEvent(event);
}

/**
 * @return рекомендуемый размер поля нумерации строк на основе значения
 * ширины вычисленного в методе lineNumberAreaWidth.
 */
QSize LineNumberArea::sizeHint() const
{
    return QSize(m_codeEditor->lineNumberAreaWidth(), 0);
}
