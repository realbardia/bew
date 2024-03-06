#include "commandparser.h"

#include <QCommandLineParser>

CommandParser::CommandParser() {}

CommandParser::Result CommandParser::parse(const QString &command)
{
    QStringList arguments;

    QString cache;
    bool doubleQuotation = false;
    bool quotation = false;
    QChar lastChar;
    for (const auto &c: command)
    {
        if (!doubleQuotation && c == '\'' && lastChar != '\\')
        {
            quotation = !quotation;
            lastChar = c;
            continue;
        }
        if (!quotation && c == '"' && lastChar != '\\')
        {
            doubleQuotation = !doubleQuotation;
            lastChar = c;
            continue;
        }

        if (quotation || doubleQuotation || c != ' ')
        {
            cache += c;
            lastChar = c;
            continue;
        }
        if (cache.isEmpty())
            continue;

        arguments << cache;
        cache.clear();
    }

    if (cache.length())
        arguments << cache;

    return parse(arguments);
}

CommandParser::Result CommandParser::parse(const QStringList &arguments)
{
    QCommandLineParser parser;
    parser.setApplicationDescription("BEW help you to convert Web apps to Desktop apps");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("web-url", "Web site url");

    QCommandLineOption noScrollOption("no-scrollbar", "No scrollbar");
    QCommandLineOption sysTrayOption("system-tray", "Show on system tray");
    QCommandLineOption singleInstanceOption("single-instance", "Only one instance must run");
    QCommandLineOption fontOption("font", "Set all fonts to <fontname>.", "fontname");
    QCommandLineOption titleOption("app-name", "Set app title to <name>.", "name");
    QCommandLineOption iconOption("app-icon", "Set app icon to <icon>.", "icon");
    QCommandLineOption agentOption("user-agent", "Set browser user agent <string>.", "string");
    QCommandLineOption proxyHostOption("proxy-host", "Set proxy host <host>.", "host");
    QCommandLineOption proxyPortOption("proxy-port", "Set proxy port <port>.", "port");
    QCommandLineOption proxyTypeOption("proxy-type", "Set proxy type (http|socks|noproxy).", "type");
    QCommandLineOption proxyUserOption("proxy-username", "Set proxy username <username>.", "username");
    QCommandLineOption proxyPassOption("proxy-password", "Set proxy password <password>.", "password");

    parser.addOption(titleOption);
    parser.addOption(iconOption);
    parser.addOption(fontOption);
    parser.addOption(agentOption);
    parser.addOption(noScrollOption);
    parser.addOption(sysTrayOption);
    parser.addOption(singleInstanceOption);

    parser.addOption(proxyHostOption);
    parser.addOption(proxyPortOption);
    parser.addOption(proxyTypeOption);
    parser.addOption(proxyUserOption);
    parser.addOption(proxyPassOption);

    parser.parse(arguments);

    const auto args = parser.positionalArguments();

    Result res;
    if (args.isEmpty())
        return res;

    res.isNull = false;
    res.url = args.at(0);

    res.noScroll = parser.isSet(noScrollOption);
    res.systray = parser.isSet(sysTrayOption);
    res.singleInstance = parser.isSet(singleInstanceOption);
    res.font = parser.value(fontOption);
    res.title = parser.value(titleOption);
    res.icon = parser.value(iconOption);
    res.agent = parser.value(agentOption);

    res.proxyType = parser.value(proxyTypeOption);
    res.proxyHost = parser.value(proxyHostOption);
    res.proxyPort = parser.value(proxyPortOption).toInt();
    res.proxyUser = parser.value(proxyUserOption);
    res.proxyPass = parser.value(proxyPassOption);

    return res;
}

std::optional<QNetworkProxy> CommandParser::Result::proxy() const
{
    auto host = proxyHost;
    if (host.length() || proxyType.length() || proxyPort)
    {
        auto proxyTypeEnum = [this]() -> QNetworkProxy::ProxyType {
            if (proxyType == "noproxy")
                return QNetworkProxy::NoProxy;
            else
            if (proxyType == "socks")
                return QNetworkProxy::Socks5Proxy;
            else
                return QNetworkProxy::HttpProxy;
        }();
        if (host.isEmpty())
            host = "127.0.0.1";

        QNetworkProxy proxy;
        proxy.setType(proxyTypeEnum);
        proxy.setHostName(host);
        proxy.setPort(proxyPort);
        if (proxyUser.length()) proxy.setUser(proxyUser);
        if (proxyPass.length()) proxy.setPassword(proxyPass);

        return proxy;
    }
    else
        return std::optional<QNetworkProxy>();
}
