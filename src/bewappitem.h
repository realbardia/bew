#ifndef BEWAPPITEM_H
#define BEWAPPITEM_H

#include <QJsonObject>
#include <QNetworkProxy>
#include <QString>

#include <optional>

class BewAppItem
{
public:
    struct Proxy {
        QString type;
        QString host;
        int port = 0;
        QString user;
        QString pass;
    };

    BewAppItem();
    virtual ~BewAppItem();

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

    std::optional<Proxy> mProxy;
};

typedef QSharedPointer<BewAppItem> BewAppItemPtr;

#endif // BEWAPPITEM_H
