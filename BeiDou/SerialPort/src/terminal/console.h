#ifndef CONSOLE_H
#define CONSOLE_H

#include <QPlainTextEdit>
#include <QScrollBar>

class Console : public QPlainTextEdit {
    Q_OBJECT

signals:
    void getData(const QByteArray &);

public:
    explicit Console(QWidget *parent = nullptr);
    void putData(const QByteArray &);
    void setLocalEchoEnabled(bool );

protected:
    void keyPressEvent(QKeyEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
    void mouseDoubleClickEvent(QMouseEvent *) override;
    void contextMenuEvent(QContextMenuEvent *) override;

private:
    bool m_localEchoEnabled = false;
};

#endif // CONSOLE_H
