#ifndef CODETEXTEDIT_H
#define CODETEXTEDIT_H

#include <QTextEdit>
#include "linenumberarea.h"

class CodeTextEdit : public QTextEdit
{
    Q_OBJECT
    Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor DESIGNABLE true)
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor DESIGNABLE true)
    Q_PROPERTY(QColor color READ color WRITE setColor DESIGNABLE true)
    Q_PROPERTY(QColor currentColor READ currentColor WRITE setCurrentColor DESIGNABLE true)
    Q_PROPERTY(QColor findColor READ findColor WRITE setFindColor DESIGNABLE true)
    Q_PROPERTY(QColor limitColor READ limitColor WRITE setLimitColor DESIGNABLE true)
public:
    explicit CodeTextEdit(QWidget *parent = 0);
    ~CodeTextEdit();

    void setLineNumberVisible(bool lineNumberVisible);
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();
private:
    LineNumberArea *m_lineNumberArea;
    bool m_lineNumberVisible;

    // Members style property
    QColor m_backgroundColor;
    QColor m_borderColor;
    QColor m_color;
    QColor m_currentColor;
    QColor m_findColor;
    QColor m_limitColor;

    // Methods
    void initInterface();
    void initConnections();

    int getFirstVisibleBlockId();
    bool lineNumberVisible() const;

    // Methods style property
    QColor borderColor() const;
    void setBorderColor(const QColor &borderColor);
    QColor backgroundColor() const;
    void setBackgroundColor(const QColor &backgroundColor);
    QColor color() const;
    void setColor(const QColor &color);
    QColor currentColor() const;
    void setCurrentColor(const QColor &currentColor);
    QColor findColor() const;
    void setFindColor(const QColor &findColor);
    QColor limitColor() const;
    void setLimitColor(const QColor &limitColor);

    void keyPressEvent(QKeyEvent* e) override;

signals:

private slots:
    void resizeEvent(QResizeEvent *e) override;
    void updateLineNumberArea();
    void updateLineNumberAreaWidth(int newBlockCount);
};

#endif // CODETEXTEDIT_H
