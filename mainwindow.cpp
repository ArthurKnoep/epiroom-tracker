#include <QProcess>
#include <QDebug>
#include <QTimer>
#include <QLabel>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QSslError>
#include <QNetworkConfiguration>
#include <QNetworkConfigurationManager>
#include <QNetworkSession>
#include "mainwindow.h"
#include "ui_mainwindow.h"

bool printErrorMsg = false;

MainWindow::MainWindow()
{
    timer = new QTimer();
    timer->setInterval(10 * 1000);
    timer->start();
    text_e = new QTextEdit(this);
    text_e->resize(500, 500);
    resize(500, 500);

    sticon = new QSystemTrayIcon(this);

    QMenu *stmenu = new QMenu(this);
    QAction *track = new QAction("Tracking", this);
    track->setCheckable(true);
    track->setChecked(true);
    QAction *quit = new QAction("Quitter", this);
    connect(track, SIGNAL(triggered(bool)), this, SLOT(update_track(bool)));
    connect(quit, SIGNAL(triggered(bool)), this, SLOT(close()));
    stmenu->addAction(track);
    stmenu->addAction(quit);

    sticon->setContextMenu(stmenu);
    QIcon icon("icon.png");
    sticon->setIcon(icon);
    sticon->show();
    connect(timer, SIGNAL(timeout()), this, SLOT(launch_process()));
    launch_process();
}

void MainWindow::update_track(bool val)
{
    if (val) {
        timer->start();
    } else {
        timer->stop();
    }
}

void MainWindow::launch_process()
{
//    qDebug() << "Launch";
    process = new QProcess(this);
    process->start("/sbin/iw wlp2s0 scan");
//    connect(process, SIGNAL(readyReadStandardOutput()), this, SLOT(read_wifi()));
    connect(process, SIGNAL(finished(int)), this, SLOT(read_wifi(int)));
}

QString get_arg(QString str)
{
    QStringList list = str.split(":");
    if (list.size() != 2)
        return "";
    return list[1].trimmed();
}

void MainWindow::read_request(QNetworkReply *reply)
{
    (void) reply;
//    QByteArray ret;

//    if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 200)
//    {
//        ret = reply->readAll();
//    }
}

int compForSort(wifi a, wifi b)
{
    return a.signal > b.signal;
}

QVector<wifi> filterWifi(QVector<wifi> wifis)
{
    QVector<wifi> ret;

    for (wifi net : wifis) {
        if (net.name == NETWORK_TARGET) {
            ret.push_back(net);
        }
    }
    std::sort(ret.begin(), ret.end(), compForSort);
    return ret;
}

wifi MainWindow::parse_wifi(QStringList const &lines, int &i)
{
    wifi ret;
    QString line = lines.at(i);

    ret.bssid = line.right(line.size() - 4).left(17);
    i += 1;
    line = lines.at(i);
    while (!line.startsWith("BSS") && i< lines.size()) {
        line = lines.at(i);
        if (line.contains("SSID:")) {
            ret.name = get_arg(line);
        }
        if (line.contains("signal:")) {
            QString signal = get_arg(line);
            ret.signal = signal.left(signal.size() - 4).toFloat();
        }
        i += 1;
    }
    return ret;
}

void MainWindow::read_wifi(int exitCode)
{
    if (exitCode != 0) {
        qDebug() << exitCode;
        if (!printErrorMsg && exitCode == 255) {
            sticon->showMessage("EpiRoom", "Permission insuffisante");
            printErrorMsg = true;
        }
        return;
    }
    printErrorMsg = false;
    QByteArray byteArray = process->readAllStandardOutput();
    QStringList strLines = QString(byteArray).split("\n");
    QVector<wifi> wifis;

    for (auto i = 0; i < strLines.size(); i++) {
        QString line = strLines.at(i);
        if (line.startsWith("BSS")) {
            wifis.append(parse_wifi(strLines, i));
        }
    }
    wifis = filterWifi(wifis);
    for (auto i : wifis) {
        qDebug() << i.name << " : " << i.bssid << " : " << i.signal;
    }
    if (wifis.size() != 0)
        qDebug() << wifis[0].bssid;
}

void send_pos(QString const &bssid)
{
    (void) bssid;
//    const QUrl url = QUrl("http://epiroom.com/transmit.php");
//    QNetworkRequest req(url);
//    QNetworkAccessManager *m = new QNetworkAccessManager;
//    const QString val = QString("user_key=") + QString(USER_KEY) + QString("&access_point=") + bssid;
//    text_e->append(val);
//    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
//    m->post(req, val.toLatin1());
//    connect(m, SIGNAL(finished(QNetworkReply *)), this, SLOT(read_request(QNetworkReply *)));
//    text_e->append(str);
}

MainWindow::~MainWindow()
{
}
