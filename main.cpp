#include "adddialog.h"
#include "bew.h"

#include <QApplication>
#include <QCommandLineParser>
#include <QNetworkProxy>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("bew");
    app.setApplicationVersion("0.1");

    QCommandLineParser parser;
    parser.setApplicationDescription("Bew help you to convert Web apps to Desktop apps");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("web-url", "Web site url");

    QCommandLineOption noScrollOption("no-scrollbar", "No scrollbar");
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

    parser.addOption(proxyHostOption);
    parser.addOption(proxyPortOption);
    parser.addOption(proxyTypeOption);
    parser.addOption(proxyUserOption);
    parser.addOption(proxyPassOption);

    parser.process(app);

    const auto args = parser.positionalArguments();
    if (args.isEmpty())
    {
        AddDialog add;
        add.show();

        return app.exec();
    }

    auto web = args.at(0);

    auto noScroll = parser.isSet(noScrollOption);
    auto font = parser.value(fontOption);
    auto title = parser.value(titleOption);
    auto icon = parser.value(iconOption);
    auto agent = parser.value(agentOption);

    auto proxyType = parser.value(proxyTypeOption);
    auto proxyHost = parser.value(proxyHostOption);
    auto proxyPort = parser.value(proxyPortOption).toInt();
    auto proxyUser = parser.value(proxyUserOption);
    auto proxyPass = parser.value(proxyPassOption);

    if (proxyHost.length() || proxyType.length() || proxyPort)
    {
        auto proxyTypeEnum = [proxyType]() -> QNetworkProxy::ProxyType {
            if (proxyType == "noproxy")
                return QNetworkProxy::NoProxy;
            else
            if (proxyType == "socks")
                return QNetworkProxy::Socks5Proxy;
            else
                return QNetworkProxy::HttpProxy;
        }();
        if (proxyHost.isEmpty())
            proxyHost = "127.0.0.1";

        QNetworkProxy proxy;
        proxy.setType(proxyTypeEnum);
        proxy.setHostName(proxyHost);
        proxy.setPort(proxyPort);
        if (proxyUser.length()) proxy.setUser(proxyUser);
        if (proxyPass.length()) proxy.setPassword(proxyPass);

        QNetworkProxy::setApplicationProxy(proxy);
    }

    if (title.count()) {
        app.setApplicationDisplayName(title);
        app.setApplicationName(title);
    }
    if (agent.count())
        Bew::setUserAgent(agent);

    Bew bew;

    if (title.count()) bew.setWindowTitle(title);
    if (icon.count()) bew.setWindowIcon(QIcon(icon));
    if (font.count()) bew.setFonts(font);
    if (noScroll) bew.setScrollBar(false);

    bew.load(web);
    bew.show();

    return app.exec();
}
