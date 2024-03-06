#include "bew.h"
#include "bewstore.h"
#include "commandparser.h"

#include <QApplication>
#include <QCommandLineParser>
#include <QNetworkProxy>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("bew");
    app.setApplicationVersion("0.1");

    auto parse = CommandParser::parse(app.arguments());
    if (parse.isNull)
    {
        BewStore store;
        store.resize(1200, 700);
        store.refresh();
        store.show();

        return app.exec();
    }

    if (parse.singleInstance && Bew::showInstance())
        return 0;

    const auto proxy = parse.proxy();
    if (proxy.has_value())
        QNetworkProxy::setApplicationProxy(proxy.value());

    if (parse.title.count()) {
        app.setApplicationDisplayName(parse.title);
        app.setApplicationName(parse.title);
    }
    if (parse.agent.count())
        Bew::setUserAgent(parse.agent);

    Bew bew;

    if (parse.title.count()) bew.setWindowTitle(parse.title);
    if (parse.icon.count()) bew.setWindowIcon(QIcon(parse.icon));
    if (parse.font.count()) bew.setFonts(parse.font);
    if (parse.noScroll) bew.setScrollBar(false);
    if (parse.systray) bew.setSystemTray(true);
    if (parse.singleInstance) bew.setSingleInstance(true);

    bew.load(parse.url);
    bew.show();

    return app.exec();
}
