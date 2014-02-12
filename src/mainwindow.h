#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class SyncCommand;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void onConnectEstablishedSlot();
    void onConnectErrorStringSlot(const QString &);
    void onDisconnectedSlot();

protected:
    void keyPressEvent(QKeyEvent *e);

private slots:
    void onCmdParseParam(QByteArray &array);
    void onParseWLTVersionSlot();

    void onSteppingButtonClicked();
    void onYellowFlashButtonClicked();
    void onAllRedButtonClicked();
    void onAutoManualButtonClicked(bool checked);

    void onUpdateButtonStatusStatus(bool state);

    void onConnButtonClicked();

    void onConnectTimerTimeoutSlot();
    void onVersionTimerTimeoutSlot();
    void onHeartbeatTimerTimeoutSlot();

private:
    void initSignalSlots();
    void updateUI();
    void saveSignaler();

private:
    Ui::MainWindow *ui;
    SyncCommand *sync_cmd_;
    QTimer *conn_timer_;
    QTimer *ver_timer_;
    QTimer *heart_beat_timer_;
    QByteArray recv_array_;

    bool is_conn_;
    QString cfg_file_;
};

#endif // MAINWINDOW_H
