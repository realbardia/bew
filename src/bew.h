#ifndef BEW_H
#define BEW_H

#include "bewwebview.h"
#include "bewmessagewidget.h"

#include <QLocalServer>
#include <QMainWindow>
#include <QWebEngineContextMenuData>
#include <QWebEngineProfile>
#include <QSystemTrayIcon>
#include <QLabel>

class BEW : public QMainWindow
{
    Q_OBJECT
public:
    BEW(QWidget *parent = nullptr);
    ~BEW();

    void setFonts(const QString &font);
    void downloadRequested(QWebEngineDownloadItem *download);
    void setScrollBar(bool state);
    void setSystemTray(bool state);
    void setSingleInstance(bool state);

    static QString homePath();

    static QString userAgent();
    static QString instancePath();
    static void setUserAgent(const QString &userAgent);
    static bool showInstance();

    QString message() const;
    void setMessage(const QString &newMessage);

    qint32 messageIdleMinutes() const;
    void setMessageIdleMinutes(qint32 newMessageIdleMinutes);

public Q_SLOTS:
    void load(const QString &url);

private:
    void webViewUrlChanged(const QUrl &arg1);
    void webViewIconChanged(const QIcon &arg1);
    void webViewCustomContextMenuRequested(const QPoint &pos);

protected:
    void restore();
    void save();
    void keyPressEvent(QKeyEvent *event);

private:
    BEWWebView *mWeb;
    BewMessageWidget *mMessage;
    QLocalServer *mInstanceServer;
    QSystemTrayIcon *mSysTray;

    static QString mUserAgent;
};
#endif // BEW_H
