#include "bewappitem.h"
#include "commandparser.h"

#include <QDir>
#include <QImage>
#include <QImageReader>
#include <QImageWriter>
#include <QJsonDocument>
#include <QJsonObject>
#include <QProcess>
#include <QRegularExpression>

BewAppItem::BewAppItem()
{
}

BewAppItem::~BewAppItem()
{
}

QString BewAppItem::title() const
{
    return mTitle;
}

void BewAppItem::setTitle(const QString &newTitle)
{
    mTitle = newTitle;
}

QString BewAppItem::iconUrl() const
{
    return mIconUrl;
}

void BewAppItem::setIconUrl(const QString &newIconUrl)
{
    mIconUrl = newIconUrl;
}

bool BewAppItem::isInstalled() const
{
    return QFileInfo::exists(BewAppItem::path());
}

QString BewAppItem::path() const
{
    return applicationsPath() + mTitle + QStringLiteral(".desktop");
}

QString BewAppItem::url() const
{
    return mUrl;
}

void BewAppItem::setUrl(const QString &newUrl)
{
    mUrl = newUrl;
}

QString BewAppItem::userAgent() const
{
    return mUserAgent;
}

void BewAppItem::setUserAgent(const QString &newUserAgent)
{
    mUserAgent = newUserAgent;
}

QString BewAppItem::fontFamily() const
{
    return mFontFamily;
}

void BewAppItem::setFontFamily(const QString &newFontFamily)
{
    mFontFamily = newFontFamily;
}

bool BewAppItem::scrollBar() const
{
    return mScrollBar;
}

void BewAppItem::setScrollBar(bool newScrollBar)
{
    mScrollBar = newScrollBar;
}

bool BewAppItem::singleInstance() const
{
    return mSingleInstance;
}

void BewAppItem::setSingleInstance(bool newSingleInstance)
{
    mSingleInstance = newSingleInstance;
}

bool BewAppItem::systemTray() const
{
    return mSystemTray;
}

void BewAppItem::setSystemTray(bool newSystemTray)
{
    mSystemTray = newSystemTray;
}

std::optional<BewAppItem::Proxy> BewAppItem::proxy() const
{
    return mProxy;
}

void BewAppItem::setProxy(std::optional<BewAppItem::Proxy> newProxy)
{
    mProxy = newProxy;
}

QString BewAppItem::applicationsPath()
{
    static QString res;
    if (res.isEmpty())
    {
        res = QDir::homePath() + QStringLiteral("/.local/share/applications/");
        QDir().mkpath(res);
    }
    return res;
}

QString BewAppItem::iconsPath()
{
    static QString res;
    if (res.isEmpty())
    {
        res = QDir::homePath() + QStringLiteral("/.local/share/icons/");
        QDir().mkpath(res);
    }
    return res;
}

bool BewAppItem::restoreUsingAppName(const QString &appName)
{
    return restoreUsingFile(applicationsPath() + appName + QStringLiteral(".desktop"));
}

bool BewAppItem::restoreUsingFile(const QString &fileName)
{
    QFile f(fileName);
    if (!f.open(QFile::ReadOnly))
        return false;

    const auto data = QString::fromUtf8(f.readAll());
    f.close();

    static const QRegularExpression rx("\nExec\\=(bew\\s+.+)\n");
    const auto match = rx.match(data);
    if (!match.hasMatch())
        return false;

    auto parse = CommandParser::parse(match.captured(1));
    if (parse.isNull)
        return false;

    mTitle = parse.title;
    if (mTitle != QFileInfo(fileName).baseName())
        return false;

    mIconUrl = parse.icon;
    mUrl = parse.url;
    mUserAgent = parse.agent;
    mFontFamily = parse.font;
    mScrollBar = !parse.noScroll;
    mSingleInstance = parse.singleInstance;
    mSystemTray = parse.systray;

    if (!parse.proxyType.isEmpty())
    {
        Proxy proxy;
        proxy.type = parse.proxyType;
        proxy.host = parse.proxyHost;
        proxy.port = parse.proxyPort;
        proxy.user = parse.proxyUser;
        proxy.pass = parse.proxyPass;
        mProxy = proxy;
    }
    else
        mProxy.reset();

    return true;
}

bool BewAppItem::install()
{
    QFile file(":/bew/templates/template.desktop");
    if (!file.open(QFile::ReadOnly))
        return false;

    QString data = QString::fromUtf8(file.readAll());
    file.close();

    QString command = QStringLiteral("bew \"%1\" --app-name \"%2\" --app-icon \"%3\"")
                          .arg(mUrl, mTitle, mIconUrl);

    if (!mScrollBar)
        command += " --no-scrollbar";
    if (mSingleInstance)
        command += " --single-instance";
    if (mSystemTray)
        command += " --system-tray";
    if (mUserAgent.length())
        command += " --user-agent \"" + mUserAgent + "\"";
    if (mFontFamily.length())
        command += " --font \"" + mFontFamily + "\"";

    if (mProxy.has_value())
    {
        const auto proxy = mProxy.value();
        command += " --proxy-type \"" + proxy.type + "\"";
        if (proxy.host.length())
            command += " --proxy-host \"" + proxy.host + "\"";
        if (proxy.port)
            command += " --proxy-port \"" + QString::number(proxy.port) + "\"";
        if (proxy.user.length())
            command += " --proxy-username \"" + proxy.user + "\"";
        if (proxy.pass.length())
            command += " --proxy-password \"" + proxy.pass + "\"";
    }

    data.replace("%command%", command);
    data.replace("%icon%", mIconUrl);
    data.replace("%name%", mTitle);

    file.setFileName(BewAppItem::path());
    file.open(QFile::WriteOnly);
    file.write(data.toUtf8());
    file.close();

    QProcess::startDetached("update-mime-database", QStringList());
    return true;
}

bool BewAppItem::launch()
{
    if (qgetenv("DESKTOP_SESSION").toLower().contains("plasma"))
        QProcess::startDetached("kioclient5", {"exec", BewAppItem::path()});
    else
        QProcess::startDetached("gtk-launch", {mTitle});
    return true;
}

bool BewAppItem::uninstall()
{
    const auto res = QFile::remove(BewAppItem::path());
    QProcess::startDetached("update-mime-database", QStringList());
    return res;
}

QJsonObject BewAppItem::toJson() const
{
    QJsonObject j;
    j["title"] = mTitle;
    j["icon"] = QString::fromLatin1(restoreIcon().toBase64());
    j["url"] = mUrl;
    j["scrollbar"] = mScrollBar;
    j["single-instance"] = mSingleInstance;
    j["systemtray"] = mSystemTray;

    if (!mFontFamily.isEmpty())
        j["font"] = mFontFamily;
    if (!mUserAgent.isEmpty())
        j["user-agent"] = mUserAgent;

    return j;
}

bool BewAppItem::fromJson(const QJsonObject &j)
{

    mTitle = j["title"].toString();
    mUrl = j["url"].toString();
    mUserAgent = j["user-agent"].toString();
    mFontFamily = j["font"].toString();
    mScrollBar = j["scrollbar"].toBool();
    mSingleInstance = j["single-instance"].toBool();
    mSystemTray = j["systemtray"].toBool();

    storeIcon( QByteArray::fromBase64(j["icon"].toString().toLatin1()) );

    return mTitle.length();
}

bool BewAppItem::storeIcon(const QByteArray &iconBytes)
{
    if (mIconUrl.length() && QFileInfo::exists(mIconUrl))
        return true;
    if (iconBytes.length() < 5)
        return false;

    const auto suffixSplitterIdx = iconBytes.left(5).indexOf(':');
    if (suffixSplitterIdx < -1)
        return false;

    const auto suffix = QString::fromUtf8(iconBytes.left(suffixSplitterIdx));
    const auto path = BewAppItem::iconsPath() + mTitle + '.' + suffix;

    QFile f(path);
    if (!f.open(QFile::WriteOnly))
        return false;

    f.write(qUncompress(iconBytes.mid(suffixSplitterIdx+1)));
    f.close();

    mIconUrl = path;
    return true;
}

QByteArray BewAppItem::restoreIcon() const
{
    QByteArray res;

    auto url = mIconUrl;
    QFileInfo info(url);
    if (url.isEmpty() || !info.exists())
        return res;

    bool urlMustRemove = false;
    if (info.suffix().toLower() != "svg" && info.suffix().toLower() != "svgz")
    {
        QImageReader reader(url);
        const auto size = reader.size();
        if (size.width()>64 || size.height()>64)
        {
            const auto img = reader.read().scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation);

            url = QDir::tempPath() + '/' + QUuid::createUuid().toString(QUuid::WithoutBraces) + '.' + info.suffix();
            QImageWriter writer(url);
            writer.write(img);

            urlMustRemove = true;
        }
    }

    QFile f(url);
    if (!f.open(QFile::ReadOnly))
        return res;

    res = QFileInfo(url).suffix().toUtf8() + ':' + qCompress(f.readAll());
    f.close();

    if (urlMustRemove)
        QFile::remove(url);

    return res;
}
