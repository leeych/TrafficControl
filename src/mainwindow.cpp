#include "mainwindow.h"
#include "macrostring.h"
#include "ui_mainwindow.h"
#include "synccommand.h"
#include "mutility.h"
#include "xmlhelper.h"
#include <QIntValidator>
#include <QKeyEvent>

#define CONN_CHECK_MS       (3000)
#define VER_CHECK_MS        (5000)
#define HEART_BEAT_MS       (8000)

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tip_hlayout->setAlignment(Qt::AlignCenter);
    ui->automanual_button_->setCheckable(true);
    sync_cmd_ = SyncCommand::getInstance();
    conn_timer_ = new QTimer(this);
    ver_timer_ = new QTimer(this);
    heart_beat_timer_ = new QTimer(this);

    is_conn_ = false;
    ui->ip_lineedit_->setInputMask("000.000.000.000; ");
    QIntValidator *validator = new QIntValidator(1025, 65536, this);
    ui->port_lineedit_->setValidator(validator);

    MUtility::getUserDir(cfg_file_);
    cfg_file_ += "signaler.xml";
    if (XmlHelper::GetInstance()->LoadXmlFile(cfg_file_))
    {
        QString ip(XmlHelper::GetInstance()->ParseXmlNodeContent("ip"));
        QString port(XmlHelper::GetInstance()->ParseXmlNodeContent("port"));
        ui->ip_lineedit_->setText(ip);
        ui->port_lineedit_->setText(port);
    }
    else
    {
        ui->ip_lineedit_->setText("127.0.0.1");
        ui->port_lineedit_->setText("12810");
    }

    initSignalSlots();
    onUpdateButtonStatusStatus(false);
    ui->automanual_button_->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
    if (ver_timer_->isActive())
    {
        ver_timer_->stop();
        delete ver_timer_;
        ver_timer_ = NULL;
    }
    if (heart_beat_timer_->isActive())
    {
        heart_beat_timer_->stop();
        delete heart_beat_timer_;
        heart_beat_timer_ = NULL;
    }
    if (conn_timer_->isActive())
    {
        conn_timer_->stop();
        delete conn_timer_;
        conn_timer_ = NULL;
    }
}

void MainWindow::onConnectEstablishedSlot()
{
    conn_timer_->start(CONN_CHECK_MS);
    sync_cmd_->sendVersionCheck(this, SLOT(onCmdParseParam(QByteArray&)));
    ver_timer_->start(VER_CHECK_MS);
    ui->conn_button_->setText(STRING_DISCONNECT);
    ui->conn_button_->setEnabled(true);
    is_conn_ = true;
}

void MainWindow::onConnectErrorStringSlot(const QString &err)
{
    ui->tip_label_->setText(err);
    if (ver_timer_->isActive())
    {
        ver_timer_->stop();
    }
    ui->conn_button_->setText(STRING_CONNECT);
    is_conn_ = false;
    ui->automanual_button_->setEnabled(true);
}

void MainWindow::onDisconnectedSlot()
{
    ui->tip_label_->setText(STRING_DISCONNECT);
    if (ver_timer_->isActive())
    {
        ver_timer_->stop();
    }
    if (heart_beat_timer_->isActive())
    {
        heart_beat_timer_->stop();
    }
    ui->conn_button_->setText(STRING_CONNECT);
    is_conn_ = false;
    ui->automanual_button_->setChecked(false);
    ui->automanual_button_->setEnabled(false);
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Back)
    {
        saveSignaler();
    }
    this->close();
}

void MainWindow::onCmdParseParam(QByteArray &array)
{
    QString recv_str(array.data());
    if (recv_str == "CYT0V100END")
    {
        conn_timer_->stop();
        ver_timer_->stop();
        ui->tip_label_->setText(STRING_CONNECT_ESTABLISHED);
        heart_beat_timer_->start(HEART_BEAT_MS);
        ui->automanual_button_->setEnabled(true);
    }
    else if (recv_str == "SOCKBEGIN")
    {
        ui->tip_label_->setText(STRING_MANUAL_CTRL);
    }
    else if (recv_str == "SOCKEND")
    {
        ui->tip_label_->setText(STRING_AUTO_CTRL);
    }
    else if (recv_str == "STEPBEGIN")
    {
        ui->tip_label_->setText(STRING_STEPPING_CTRL + STRING_BEGIN);
    }
    else if (recv_str == "STEPEND")
    {
        ui->tip_label_->setText(STRING_STEPPING_CTRL + STRING_END);
    }
    else if (recv_str == "YFBEGIN")
    {
        ui->tip_label_->setText(STRING_YELLOW_FLASH_CTRL + STRING_BEGIN);
    }
    else if (recv_str == "YFEND")
    {
        ui->tip_label_->setText(STRING_YELLOW_FLASH_CTRL + STRING_END);
    }
    else if (recv_str == "REDBEGIN")
    {
        ui->tip_label_->setText(STRING_ALL_RED_CTRL + STRING_BEGIN);
    }
    else if (recv_str == "REDEND")
    {
        ui->tip_label_->setText(STRING_ALL_RED_CTRL + STRING_END);
    }
//    else
//    {
//        ui->tip_label_->setText(array.data());
//    }
}

void MainWindow::onParseWLTVersionSlot()
{
}

void MainWindow::onSteppingButtonClicked()
{
    sync_cmd_->sendSteppingCtrl();
}

void MainWindow::onYellowFlashButtonClicked()
{
    sync_cmd_->sendYellowFlashCtrl();
}

void MainWindow::onAllRedButtonClicked()
{
    sync_cmd_->sendAllRedCtrl();
}

void MainWindow::onAutoManualButtonClicked(bool checked)
{
    if (checked)
    {
        sync_cmd_->sendAutoCtrl();
        ui->automanual_button_->setText(STRING_MANUAL_CTRL);
    }
    else
    {
        sync_cmd_->sendManualCtrl();
        ui->automanual_button_->setText(STRING_AUTO_CTRL);
    }
    onUpdateButtonStatusStatus(checked);
}

void MainWindow::onConnButtonClicked()
{
    if (is_conn_)
    {
        sync_cmd_->disconnectFromHost();
    }
    else
    {
        QString ip = ui->ip_lineedit_->text().trimmed();
        unsigned int port = ui->port_lineedit_->text().trimmed().toUInt();
        sync_cmd_->connectToHost(ip, port);
        ui->conn_button_->setEnabled(false);
        ui->tip_label_->setText(STRING_CONNECTING);

//        heart_beat_timer_->start(HEART_BEAT_MS);
    }
}

void MainWindow::onConnectTimerTimeoutSlot()
{
    ver_timer_->stop();
    ui->tip_label_->setText(STRING_HOST_UNREACHABLE);
    sync_cmd_->closeConnection();
    ui->conn_button_->setEnabled(true);
    ui->automanual_button_->setEnabled(false);
}

void MainWindow::onVersionTimerTimeoutSlot()
{
    sync_cmd_->sendVersionCheck();
}

void MainWindow::onHeartbeatTimerTimeoutSlot()
{
    sync_cmd_->sendHeartBeat();
}

void MainWindow::initSignalSlots()
{
    connect(ui->conn_button_, SIGNAL(clicked()), this, SLOT(onConnButtonClicked()));
    connect(ui->stepping_button_, SIGNAL(clicked()), this, SLOT(onSteppingButtonClicked()));
    connect(ui->yellow_flash_button_, SIGNAL(clicked()), this, SLOT(onYellowFlashButtonClicked()));
    connect(ui->all_red_button_, SIGNAL(clicked()), this, SLOT(onAllRedButtonClicked()));
//    connect(ui->automanual_button_, SIGNAL(clicked()), this, SLOT(on_automanual_button__clicked()));
    connect(ui->automanual_button_, SIGNAL(clicked(bool)), this, SLOT(onAutoManualButtonClicked(bool)));

    connect(sync_cmd_, SIGNAL(connectedSignal()), this, SLOT(onConnectEstablishedSlot()));
    connect(sync_cmd_, SIGNAL(connectErrorStrString(QString)), this, SLOT(onConnectErrorStringSlot(QString)));
    connect(sync_cmd_, SIGNAL(disconnectedSignal()), this, SLOT(onDisconnectedSlot()));

    connect(conn_timer_, SIGNAL(timeout()), this, SLOT(onConnectTimerTimeoutSlot()));
    connect(ver_timer_, SIGNAL(timeout()), this, SLOT(onVersionTimerTimeoutSlot()));
    connect(heart_beat_timer_, SIGNAL(timeout()), this, SLOT(onHeartbeatTimerTimeoutSlot()));
    connect(ui->automanual_button_, SIGNAL(clicked(bool)), this, SLOT(onUpdateButtonStatusStatus(bool)));
}

void MainWindow::onUpdateButtonStatusStatus(bool enable)
{
    ui->all_red_button_->setEnabled(enable);
    ui->stepping_button_->setEnabled(enable);
    ui->yellow_flash_button_->setEnabled(enable);
}

void MainWindow::updateUI()
{
    XmlHelper *xmlhelper = XmlHelper::GetInstance();
    if (!xmlhelper->LoadXmlFile(cfg_file_))
    {
        ui->ip_lineedit_->setText("127.0.0.1");
        ui->port_lineedit_->setText("12810");
    }
    else
    {
        ui->ip_lineedit_->setText(xmlhelper->ParseXmlNodeContent("ip"));
        ui->port_lineedit_->setText(xmlhelper->ParseXmlNodeContent("port"));
    }
}

void MainWindow::saveSignaler()
{
    QString ip(MUtility::trimmedAll(ui->ip_lineedit_->text()));
    QString port(MUtility::trimmedAll(ui->port_lineedit_->text()));

    XmlHelper *helper = XmlHelper::GetInstance();
    if (!QFile::exists(cfg_file_))
    {
label:  bool status = helper->CreateXmlDocument(cfg_file_, ip, port);
        if (!status)
        {
            return;
        }
    }
    else
    {
        if (!helper->LoadXmlFile(cfg_file_))
        {
            goto label;
        }
    }
}
