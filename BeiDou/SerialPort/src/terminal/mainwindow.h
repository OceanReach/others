#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "console.h"
#include "settingsdialog.h"
#include <QMainWindow>
#include <QLabel>
#include <QMessageBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void openSerialPort();
    void closeSerialPort();
    void about();
    void writeData(const QByteArray &data);
    void readData();

    void handleError(QSerialPort::SerialPortError error);

private:
    void initActionsConnection();


private:
    void showStatusMessage(const QString &message);

    Ui::MainWindow *m_ui = nullptr;
    QLabel *m_status = nullptr;
    Console *m_console = nullptr;
    SettingsDialog *m_settings= nullptr;
    QSerialPort *m_serial = nullptr;
};

#endif // MAINWINDOW_H
