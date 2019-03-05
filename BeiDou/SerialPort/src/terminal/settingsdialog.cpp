#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::SettingsDialog),
    m_intvalidator(new QIntValidator(0,4000000, this)){
    m_ui->setupUi(this);

    m_ui->baudRateBox->setInsertPolicy(QComboBox::NoInsert);

    connect(m_ui->applyButton, &QPushButton::clicked, this, &SettingsDialog::apply);
    connect(m_ui->serialPortInfoListBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SettingsDialog::showPortInfo);
    connect(m_ui->baudRateBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SettingsDialog::checkBaudRatePolicy);
    connect(m_ui->serialPortInfoListBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SettingsDialog::checkCustomDevicePathPolicy);

    fillPortParameters();
    fillPortInfo();
    updateSettings();
}

SettingsDialog::~SettingsDialog() {
    delete m_ui;
}

const SettingsDialog::Settings SettingsDialog::settings() const {
    return m_currentSettings;
}

void SettingsDialog::showPortInfo(int idx) {
    if (idx < 0)
        return;

    const QString list = m_ui->serialPortInfoListBox->itemData(idx).toString();

    m_ui->descriptionLabel->setText(tr("描述：%1").arg(list.count() > 1 ? list.at(1) : tr("")));
    m_ui->manufactureLabel->setText(tr("制造信息：%1").arg(list.count() > 2 ? list.at(2) : tr("")));
    m_ui->serialNumLabel->setText(tr("串口：%1").arg(list.count() > 3 ? list.at(3) : tr("")));
    m_ui->locationLabel->setText(tr("定位：%1").arg(list.count() > 4 ? list.at(4) : tr("")));
    m_ui->vidLabel->setText(tr("厂家信息：%1").arg(list.count() > 5 ? list.at(5) : tr("")));
    m_ui->pidLabel->setText(tr("产品信息：%1").arg(list.count() >6 ? list.at(6) :tr("")));

}

void SettingsDialog::apply() {
    updateSettings();
    hide();
}

void SettingsDialog::checkBaudRatePolicy(int idx) {
    bool isCustomBaudRate = !m_ui->baudRateBox->itemData(idx).isValid();
    m_ui->baudRateBox->setEditable(isCustomBaudRate);
    if(isCustomBaudRate) {
        m_ui->baudRateBox->clearEditText();
        QLineEdit *edit = m_ui->baudRateBox->lineEdit();
        edit->setValidator(m_intvalidator);
    }
}

void SettingsDialog::checkCustomDevicePathPolicy(int idx) {
    bool isCustomPath = !m_ui->serialPortInfoListBox->itemData(idx).isValid();
    m_ui->serialPortInfoListBox->setEditable(isCustomPath);
    if (isCustomPath) {
        m_ui->serialPortInfoListBox->clearEditText();
    }
}

void SettingsDialog::fillPortParameters() {
    m_ui->baudRateBox->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
    m_ui->baudRateBox->addItem(QStringLiteral("19200"), QSerialPort::Baud19200);
    m_ui->baudRateBox->addItem(QStringLiteral("38400"), QSerialPort::Baud38400);
    m_ui->baudRateBox->addItem(tr("Custom"));\

    m_ui->dataBitsBox->addItem(QStringLiteral("5"), QSerialPort::Data5);
    m_ui->dataBitsBox->addItem(QStringLiteral("6"), QSerialPort::Data6);
    m_ui->dataBitsBox->addItem(QStringLiteral("7"), QSerialPort::Data7);
    m_ui->dataBitsBox->addItem(QStringLiteral("8"), QSerialPort::Data8);
    m_ui->dataBitsBox->setCurrentText("3");

    m_ui->parityBox->addItem(tr("None"), QSerialPort::NoParity);
    m_ui->parityBox->addItem(tr("Even"), QSerialPort::EvenParity);
    m_ui->parityBox->addItem(tr("Odd"), QSerialPort::OddParity);
    m_ui->parityBox->addItem(tr("Mark"), QSerialPort::MarkParity);
    m_ui->parityBox->addItem(tr("Space"), QSerialPort::SpaceParity);

    m_ui->stopBitBox->addItem(QStringLiteral("1"), QSerialPort::OneStop);
#ifdef Q_OS_WIN
    m_ui->stopBitBox->addItem(tr("1.5"), QSerialPort::OneAndHalfStop);
#endif
    m_ui->stopBitBox->addItem(QStringLiteral("2"), QSerialPort::TwoStop);

    m_ui->flowControlBox->addItem(tr("None"), QSerialPort::NoFlowControl);
    m_ui->flowControlBox->addItem(tr("RTS/CTS"), QSerialPort::HardwareControl);
    m_ui->flowControlBox->addItem(tr("XON/XOFF"), QSerialPort::SoftwareControl);


}


void SettingsDialog::fillPortInfo(){
    m_ui->serialPortInfoListBox->clearEditText();
    QString description, manufacture, serialNumber;

    const auto infos = QSerialPortInfo::availablePorts();
    for(const QSerialPortInfo &info : infos) {
        description = info.description();
        manufacture = info.manufacturer();
        serialNumber = info.serialNumber();

        QStringList list;
        list << info.portName()
             << (!description.isEmpty() ? description : "")
             << (!manufacture.isEmpty() ? manufacture : "")
             << (!serialNumber.isEmpty()? serialNumber :"")
             << info.systemLocation()
             << (info.vendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : "")
             << (info.productIdentifier() ? QString::number(info.productIdentifier(), 16): "");
        m_ui->serialPortInfoListBox->addItem(list.first(), list);
    }
    m_ui->serialPortInfoListBox->addItem(tr("Custom"));
}

void SettingsDialog::updateSettings() {
    m_currentSettings.name = m_ui->serialPortInfoListBox->currentText();

    if(m_ui->baudRateBox->currentIndex() == 4) {
        m_currentSettings.baudRate = m_ui->baudRateBox->currentText().toInt();
    }
    else {
        m_currentSettings.baudRate = static_cast<QSerialPort::BaudRate>(
                    m_ui->baudRateBox->itemData(m_ui->baudRateBox->currentIndex()).toInt());
    }

    m_currentSettings.stringBaudRate = QString::number(m_currentSettings.baudRate);

    m_currentSettings.databits = static_cast<QSerialPort::DataBits>(
                m_ui->dataBitsBox->itemData(m_ui->dataBitsBox->currentIndex()).toInt());

    m_currentSettings.stringDataBits = m_ui->dataBitsBox->currentText();

    m_currentSettings.parity = static_cast<QSerialPort::Parity>(
                m_ui->parityBox->itemData(m_ui->parityBox->currentIndex()).toInt());
    m_currentSettings.stringParity = m_ui->parityBox->currentText();

    m_currentSettings.stopBits = static_cast<QSerialPort::StopBits>(
                m_ui->stopBitBox->itemData(m_ui->stopBitBox->currentIndex()).toInt());
    m_currentSettings.stringStopBits = m_ui->stopBitBox->currentText();

    m_currentSettings.flowControl = static_cast<QSerialPort::FlowControl>(
                m_ui->flowControlBox->itemData(m_ui->flowControlBox->currentIndex()).toInt());
    m_currentSettings.stringFlowControl = m_ui->flowControlBox->currentText();

    m_currentSettings.localEchoEnabled = m_ui->localEchoCheckBox->isChecked();



}
