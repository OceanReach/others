#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QIntValidator>
#include <QLineEdit>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    struct Settings{
        QString name;
        qint32 baudRate;
        QString stringBaudRate;
        QSerialPort::DataBits databits;
        QString stringDataBits;
        QSerialPort::Parity parity;
        QString stringParity;
        QSerialPort::StopBits stopBits;
        QString stringStopBits;
        QSerialPort::FlowControl flowControl;
        QString stringFlowControl;
        bool localEchoEnabled;
    };
    explicit SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog();

    const Settings settings() const;

private slots:
    void showPortInfo(int);
    void apply();
    void checkBaudRatePolicy(int);
    void checkCustomDevicePathPolicy(int );

private:
    void fillPortParameters();
    void fillPortInfo();
    void updateSettings();

private:
    Ui::SettingsDialog *m_ui = nullptr;
    Settings m_currentSettings;
    QIntValidator *m_intvalidator = nullptr;
};

#endif // SETTINGSDIALOG_H
