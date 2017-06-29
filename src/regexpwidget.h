#ifndef REGEXPWIDGET_H
#define REGEXPWIDGET_H

#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QTextEdit>
#include <QWidget>

class RegexpWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RegexpWidget(QWidget *parent = 0);
private:
    QLineEdit* m_regexpLineEdit;
    QTextEdit* m_textEdit;
    QTextEdit* m_resultTextEdit;
    QPushButton* m_execButton;
    QSpinBox* m_capSpinBox;
    QSpinBox* m_numberSpinBox;

    void initInterface();
    void initConnections();
signals:

public slots:

private slots:
    void exec();
};

#endif // REGEXPWIDGET_H
