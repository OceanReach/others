#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow),
    m_status(new QLabel),
    m_console(new Console),
    m_settings(new SettingsDialog),
    m_serial(new QSerialPort(this)) {

    m_settings->setAttribute(Qt::WA_QuitOnClose, false);
    m_ui->setupUi(this);
    m_console->setEnabled(true);
    setCentralWidget(m_console);

    m_ui->actionConnect->setEnabled(true);
    m_ui->actionDisconnect->setEnabled(false);
    m_ui->actionQuit->setEnabled(true);
    m_ui->actionConfigure->setEnabled(true);

    m_ui->statusBar->addWidget(m_status);

    initActionsConnection();

    connect(m_serial, &QSerialPort::errorOccurred, this, &MainWindow::handleError);
    connect(m_serial, &QSerialPort::readyRead, this, &MainWindow::readData);
    connect(m_console, &Console::getData, this, &MainWindow::writeData);

}

MainWindow::~MainWindow() {
    delete m_settings;
    delete m_ui;
}

void MainWindow::openSerialPort() {
    const SettingsDialog::Settings s = m_settings->settings();
    m_serial->setPortName(s.name);
    m_serial->setBaudRate(s.baudRate);
    m_serial->setDataBits(s.databits);
    m_serial->setParity(s.parity);
    m_serial->setStopBits(s.stopBits);
    m_serial->setFlowControl(s.flowControl);

    if(m_serial->open(QIODevice::ReadWrite)) {
        m_ui->actionConnect->setEnabled(false);
        m_ui->actionDisconnect->setEnabled(true);
        m_ui->actionConfigure->setEnabled(false);
        showStatusMessage(tr("正在与串口%1连接，波特率：%2, 奇偶校验位：%3, 停止位：%4，控制流：%5")
                          .arg(s.name).arg(s.stringBaudRate).arg(s.stringParity)
                          .arg(s.stringStopBits).arg(s.stringFlowControl));
    }
    else {
        QMessageBox::critical(this, tr("发生错误"), m_serial->errorString());
        showStatusMessage(tr("串口打开错误"));
    }
}

void MainWindow::closeSerialPort() {
    if (m_serial->isOpen()) {
        m_serial->close();
        m_console->setEnabled(false);
        m_ui->actionConnect->setEnabled(true);
        m_ui->actionDisconnect->setEnabled(false);
        m_ui->actionConfigure->setEnabled(true);

        showStatusMessage(tr("已断开连接"));
    }
}

void MainWindow::about(){
    QMessageBox::about(this, tr("About"), tr("none"));
}

void MainWindow::writeData(const QByteArray &data) {
    m_serial->write(data);
}

void MainWindow::readData() {
    const QByteArray data = m_serial->readAll();
    m_console->putData(data);
}

void MainWindow::handleError(QSerialPort::SerialPortError error) {
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("致命错误"), m_serial->errorString());
        closeSerialPort();
    }
}

void MainWindow::initActionsConnection() {
    connect(m_ui->actionConnect, &QAction::triggered, this, &MainWindow::openSerialPort);
    connect(m_ui->actionDisconnect, &QAction::triggered, this, &MainWindow::closeSerialPort);
    connect(m_ui->actionQuit, &QAction::triggered, this, &MainWindow::close);
    connect(m_ui->actionConfigure, &QAction::triggered, m_settings, &SettingsDialog::show);
    connect(m_ui->actionClear, &QAction::triggered, m_console, &Console::clear);
    connect(m_ui->actionAbout, &QAction::triggered, this, &MainWindow::about);
}

void MainWindow::showStatusMessage(const QString &message) {
    m_status->setText(message);
}
