#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define USER_KEY "<%KEY%>"
#define NETWORK_TARGET "IONIS"

#include <QProcess>
#include <QTextEdit>
#include <QSystemTrayIcon>
#include <QNetworkAccessManager>
#include <QWidget>
#include <QVector>

class QLabel;
class QSystemTrayIcon;

struct wifi {
    QString name;
    QString bssid;
    float signal;
};

class MainWindow : public QWidget
{
    Q_OBJECT

    public:
        MainWindow();
        ~MainWindow();

    private slots:
        void update_track(bool);
        void launch_process();
        void read_wifi(int);
        void read_request(QNetworkReply *reply);

    private:
        void getNetwork();
        QStringList WiFisList;
        void send_pos(QString const &bssid);
        wifi parse_wifi(QStringList const &line, int &i);
        QSystemTrayIcon *sticon;
        QProcess *process;
        QLabel *text;
        QTextEdit *text_e;
        QTimer *timer;
        QAction *reload10;
        QAction *reload30;
        QAction *reload60;
        QAction *reloadno;
};

#endif // MAINWINDOW_H
