#include <QAbstractTextDocumentLayout>
#include <QKeyEvent>
#include <QPainter>
#include <QPaintEvent>
#include <QScrollBar>
#include <QTextBlock>
#include <QToolTip>
#include <TLogger/TLogger>
#include "codetextedit.h"

#include <QDebug>

//===================================================================================================================//
//                                             CODE TEXT EDIT
//===================================================================================================================//

//-----Конструкторы--------------------------------------------------------------------------------------------------//

CodeTextEdit::CodeTextEdit(QWidget *parent) :
    QTextEdit(parent),
    m_lineNumberVisible(true)
{
    initInterface();
    initConnections();
}

CodeTextEdit::~CodeTextEdit()
{

}

//-----Методы--------------------------------------------------------------------------------------------------------//

void CodeTextEdit::initInterface()
{
    // Инициализация
    m_lineNumberArea = new LineNumberArea(this);
    // Кастомизация
    QFont font;
    font.setFamily("Сonsolas");
    font.setFixedPitch(true);
    setFont(font);
    setWordWrapMode(QTextOption::NoWrap);
    updateLineNumberAreaWidth(0);
}

void CodeTextEdit::initConnections()
{
    connect(this->document(), SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(updateLineNumberArea()));
    connect(this, SIGNAL(textChanged()), this, SLOT(updateLineNumberArea()));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(updateLineNumberArea()));
}

int CodeTextEdit::getFirstVisibleBlockId()
{
    QTextCursor curs = QTextCursor(document());
    curs.movePosition(QTextCursor::Start);
    for(int i=0; i < this->document()->blockCount(); ++i)
    {
        QTextBlock block = curs.block();

        QRect r1 = viewport()->geometry();
        QRect r2 = document()->documentLayout()->blockBoundingRect(block).
                   translated(viewport()->geometry().x(),
                              viewport()->geometry().y() -
                                (verticalScrollBar()->sliderPosition())
                              ).toRect();

        if (r1.contains(r2.bottomLeft(), true))
        {
            return i;
        }
        curs.movePosition(QTextCursor::NextBlock);
    }
    return 0;
}

void CodeTextEdit::resizeEvent(QResizeEvent *e)
{
    QTextEdit::resizeEvent(e);

    QRect cr = this->contentsRect();
    m_lineNumberArea->setGeometry(QRect(cr.left(), cr.top(),
                                        lineNumberAreaWidth(), cr.height()));
}

/**
 * @return состояние отображение нумерации строк. true в случае если отображение включено,
 * в противном случае вернет false.
 */
bool CodeTextEdit::lineNumberVisible() const
{
    return m_lineNumberVisible;
}

void CodeTextEdit::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    if(m_lineNumberVisible == false)
    {
        return;
    }

    verticalScrollBar()->setSliderPosition(verticalScrollBar()->sliderPosition());

    QPainter painter(m_lineNumberArea);
    painter.fillRect(event->rect(), backgroundColor());

    int blockNumber = getFirstVisibleBlockId();

    QTextBlock block = document()->findBlockByNumber(blockNumber+1);
    QTextBlock prev_block = document()->findBlockByNumber(blockNumber);
    int translate_y = (blockNumber > 0) ? -verticalScrollBar()->sliderPosition() : 0;

    int top = viewport()->geometry().top();

    // Настройка положения текста в соответствии с предыдущим не полностью
    // отображаемым блоков.
    int additional_margin;
    if (blockNumber == 0)
    {
        // Край документа
        additional_margin = document()->documentMargin() -1 -
                            verticalScrollBar()->sliderPosition() +
                            document()->documentLayout()->blockBoundingRect(prev_block).height();
    }
    else
    {
        // Получение высоты предыдущей не полностью видимой части блока
        additional_margin = document()->documentLayout()->blockBoundingRect(prev_block).
                            translated(0, translate_y).intersected(viewport()->geometry()).height();
    }

    // Shift the starting point
    top -= document()->documentLayout()->blockBoundingRect(prev_block).height() - additional_margin;

    int bottom = top + document()->documentLayout()->blockBoundingRect(block).height();


    painter.setPen(borderColor());
    QPoint p1(m_lineNumberArea->width()-1,0);
    QPoint p2(m_lineNumberArea->width()-1,m_lineNumberArea->height());
    painter.drawLine(p1,p2);

    // Отрисовка цифр (текущая строка цветом currentColor)
    while (block.isValid() && top <= event->rect().bottom())
    {
        if (block.isVisible() && bottom >= event->rect().top())
        {
            QString number = QString::number(blockNumber+1);
            painter.setPen((this->textCursor().blockNumber() == blockNumber) ?
                            currentColor() :
                            color());
            painter.drawText(-5, top, m_lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + document()->documentLayout()->blockBoundingRect(block).height();
        ++blockNumber;
    }
    // Отрисовываем первую строку даже если нет текста
    QString number = QString::number(blockNumber+1);
    painter.setPen((this->textCursor().blockNumber() == blockNumber) ?
                    currentColor() :
                    color());
    painter.drawText(-5, top, m_lineNumberArea->width(), fontMetrics().height(),
                     Qt::AlignRight, number);
}

/**
 * @param lineNumberVisible состояние отображения нумерации строк
 * @details Устанавливает состояние отображения нумерации строк.
 */
void CodeTextEdit::setLineNumberVisible(bool lineNumberVisible)
{
    m_lineNumberVisible = lineNumberVisible;
}

/**
 * @details Обновляет ширину поля отображения нумерации строк в зависимости
 * числа строк.
 */
void CodeTextEdit::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

/**
 * @details Обновляет отображение поля нумерации строк перерисовывая виджет.
 */
void CodeTextEdit::updateLineNumberArea()
{
    QRect rect =  this->contentsRect();
    m_lineNumberArea->update(0, rect.y(), m_lineNumberArea->width(), rect.height());
    updateLineNumberAreaWidth(0);
    //----------
    int dy = this->verticalScrollBar()->sliderPosition();
    if (dy > -1)
    {
        m_lineNumberArea->scroll(0, dy);
    }
}

/**
 * @details В случае если режим отображения нумерации строк включен возвращает значение
 * ширины поля нумерации, в противном случае вернет 0.
 * @return ширину поля нумерации строк в пикселях
 */
int CodeTextEdit::lineNumberAreaWidth()
{
    if(m_lineNumberVisible == false)
    {
        return 0;
    }
    int digits = 1;
    int max = qMax(1, this->document()->blockCount());
    while (max >= 10)
    {
        max /= 10;
        ++digits;
    }
    int space = 13 + fontMetrics().width(QLatin1Char('9')) * digits;
    return space;
}

//===================================================================

QColor CodeTextEdit::limitColor() const
{
    return m_limitColor;
}

void CodeTextEdit::setLimitColor(const QColor &limitColor)
{
    m_limitColor = limitColor;
}

void CodeTextEdit::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Colon)
    {
        QTextCursor tc = textCursor();
        tc.select(QTextCursor::WordUnderCursor);
        QString strWord = tc.selectedText();
        QString toolTipText;
        if( strWord == "find" )
        {
            toolTipText = "find: where, what";
        }
        else if( strWord == "findprev" )
        {
            toolTipText = "find: where, what";
        }
        else if( strWord == "regexp" )
        {
            toolTipText = "regexp: where, regexp, cap, count, var";
        }
        else if( strWord == "set" )
        {
            toolTipText = "set: var, value";
        }
        else if( strWord == "concat" )
        {
            toolTipText = "concat: var, str1, str2, ..., strN";
        }
        else if( strWord == "eventlog" )
        {
            toolTipText = "eventlog: msg";
        }
        else
        {
            return QTextEdit::keyPressEvent(e);
        }
        QToolTip::showText(mapToGlobal(cursorRect().topLeft()), toolTipText);
    }
    else if(e->key() == Qt::Key_Bar)
    {
        QToolTip::hideText();
    }
    QTextEdit::keyPressEvent(e);
}



QColor CodeTextEdit::findColor() const
{
    return m_findColor;
}

void CodeTextEdit::setFindColor(const QColor &findColor)
{
    m_findColor = findColor;
}

/**
 * @return заданный в стиле цвет границы поля нумерации строк.
 */
QColor CodeTextEdit::borderColor() const
{
    return m_borderColor;
}

/**
 * @param borderColor цвет границы
 * @details Задает цвет границы поля нумерации строк
 */
void CodeTextEdit::setBorderColor(const QColor &borderColor)
{
    m_borderColor = borderColor;
}

/**
 * @return заданный в стиле цвет фона поля нумерации строк.
 */
QColor CodeTextEdit::backgroundColor() const
{
    return m_backgroundColor;
}

/**
 * @param backgroundColor цвет фона
 * @details Задает цвет фона поля нумерации строк
 */
void CodeTextEdit::setBackgroundColor(const QColor &backgroundColor)
{
    m_backgroundColor = backgroundColor;
}

/**
 * @return заданный в стиле цвет текста поля нумерации строк.
 */
QColor CodeTextEdit::color() const
{
    return m_color;
}

/**
 * @param color цвет текста
 * @details Задает цвет текста поля нумерации строк
 */
void CodeTextEdit::setColor(const QColor &color)
{
    m_color = color;
}

/**
 * @return заданный в стиле цвет текущей строки поля нумерации строк.
 */
QColor CodeTextEdit::currentColor() const
{
    return m_currentColor;
}

/**
 * @param currentColor цвет текущей строки
 * @details Задает цвет текущей строки поля нумерации строк
 */
void CodeTextEdit::setCurrentColor(const QColor &currentColor)
{
    m_currentColor = currentColor;
}
