#ifndef BEWAPPITEM_H
#define BEWAPPITEM_H

#include <QJsonObject>
#include <QNetworkProxy>
#include <QString>

#include <optional>

class BEWAppItem
{
public:
    struct Proxy {
        QString type;
        QString host;
        int port = 0;
        QString user;
        QString pass;
    };

    BEWAppItem();
    virtual ~BEWAppItem();

    QString title() const;
    void setTitle(const QString &newTitle);

    QString url() const;
    void setUrl(const QString &newUrl);

    QString userAgent() const;
    void setUserAgent(const QString &newUserAgent);

    QString fontFamily() const;
    void setFontFamily(const QString &newFontFamily);

    bool scrollBar() const;
    void setScrollBar(bool newScrollBar);

    bool singleInstance() const;
    void setSingleInstance(bool newSingleInstance);

    bool systemTray() const;
    void setSystemTray(bool newSystemTray);

    std::optional<Proxy> proxy() const;
    void setProxy(std::optional<Proxy> newProxy);

    static QString applicationsPath();
    static QString iconsPath();

    QString iconUrl() const;
    void setIconUrl(const QString &newIconUrl);

    bool isInstalled() const;
    QString path() const;

    bool restoreUsingAppName(const QString &appName);
    bool restoreUsingFile(const QString &filePath);

    bool install();
    bool launch();
    bool uninstall();

    QJsonObject toJson() const;
    bool fromJson(const QJsonObject &json);

    QString message() const;
    void setMessage(const QString &newMessage);

    qint32 messageIdleMinutes() const;
    void setMessageIdleMinutes(qint32 newMessageIdleMinutes);

    bool messageEnabled() const;
    void setMessageEnabled(bool newMessageEnabled);

protected:
    bool storeIcon(const QByteArray &iconBytes);
    QByteArray restoreIcon() const;

private:
    QString mTitle;
    QString mIconUrl;
    QString mUrl;
    QString mUserAgent;
    QString mFontFamily;
    bool mScrollBar = true;
    bool mSingleInstance = false;
    bool mSystemTray = false;

    QString mMessage;
    qint32 mMessageIdleMinutes = 0;
    bool mMessageEnabled = false;

    std::optional<Proxy> mProxy;
};

typedef QSharedPointer<BEWAppItem> BEWAppItemPtr;

#endif // BEWAPPITEM_H
