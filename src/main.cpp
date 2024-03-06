#include "bew.h"
#include "bewstore.h"
#include "commandparser.h"
#include "settingsdialog.h"
#include "bewstoreengine.h"

#include <QApplication>
#include <QCommandLineParser>
#include <QNetworkProxy>
#include <QDebug>

#include <iostream>

int handleCommand(int argc, char *argv[])
{
    if (argc < 2)
        return -1;

    const QString command(argv[1]);

    enum CommandType {
        Install,
        Uninstall,
        Fetch,
        List,
        Search,
        Help
    };

    int type = -1;
    if (command == QStringLiteral("install"))
        type = Install;
    else if (command == QStringLiteral("uninstall"))
        type = Uninstall;
    else if (command == QStringLiteral("fetch"))
        type = Fetch;
    else if (command == QStringLiteral("list"))
        type = List;
    else if (command == QStringLiteral("search"))
        type = Search;
    else if (command == QStringLiteral("--help"))
        type = Help;
    else
        return -1;


    QCoreApplication app(argc, argv);
    app.setApplicationName("bew");
    app.setApplicationVersion("1.0");

    SettingsDialog::configureProxy();

    BewStoreEngine engine;

    switch (type)
    {
    case Install:
        if (argc <= 2)
        {
            qDebug() << "Bad command. Use --help to show help.";
            return 0;
        }
        qDebug() << "Fetching store...";
        engine.refreshStore([&app](const QList<BewAppItemPtr> &list){
            QSet<QString> listToInstall;
            for (const auto &s: app.arguments().mid(2))
                listToInstall.insert(s.toLower());

            for (const auto &ptr: list)
                if (listToInstall.contains(ptr->title().toLower()))
                {
                    std::cout << "Installing " << ptr->title().toStdString();
                    if (ptr->install())
                        std::cout << " [done]" << std::endl;
                    else
                        std::cout << " [failed]" << std::endl;
                }
            app.exit();
        });
        break;

    case Uninstall:
        if (argc <= 2)
        {
            qDebug() << "Bad command. Use --help to show help.";
            return 0;
        }
        qDebug() << "Fetching locally installeds...";
        engine.refreshStore([&app](const QList<BewAppItemPtr> &list){
            QSet<QString> listToInstall;
            for (const auto &s: app.arguments().mid(2))
                listToInstall.insert(s.toLower());

            for (const auto &ptr: list)
                if (listToInstall.contains(ptr->title().toLower()))
                {
                    std::cout << "Uninstalling " << ptr->title().toStdString();
                    if (ptr->uninstall())
                        std::cout << " [done]" << std::endl;
                    else
                        std::cout << " [failed]" << std::endl;
                }
            app.exit();
        });
        break;

    case Search:
        if (argc != 3)
        {
            qDebug() << "Bad command. Use --help to show help.";
            return 0;
        }
        qDebug() << "Searching store...";
        engine.refreshStore([&app](const QList<BewAppItemPtr> &list){
            const auto keyword = app.arguments().last().toLower();
            for (const auto &ptr: list)
                if (ptr->title().toLower().contains(keyword))
                    std::cout << " - " << ptr->title().toStdString() << ": " << ptr->url().toStdString() << std::endl;
            app.exit();
        });
        break;

    case Fetch:
        if (argc != 2)
        {
            qDebug() << "Bad command. Use --help to show help.";
            return 0;
        }
        qDebug() << "Fetching store...";
        engine.refreshStore([&app](const QList<BewAppItemPtr> &list){
            for (const auto &ptr: list)
                std::cout << " - " << ptr->title().toStdString() << ": " << ptr->url().toStdString() << std::endl;
            app.exit();
        });
        break;

    case List:
        if (argc != 2)
        {
            qDebug() << "Bad command. Use --help to show help.";
            return 0;
        }
        qDebug() << "Fetching locally installeds...";
        engine.refreshInstalleds([&app](const QList<BewAppItemPtr> &list){
            for (const auto &ptr: list)
                std::cout << " - " << ptr->title().toStdString() << ": " << ptr->url().toStdString() << std::endl;
            app.exit();
        });
        break;

    case Help:
        std::cout << app.applicationName().toStdString() << " " << app.applicationVersion().toStdString() << std::endl << std::endl;
        std::cout << "    install [app]    Install new bew application." << std::endl;
        std::cout << "    uninstall [app]  Uninstall installed bew application." << std::endl;
        std::cout << "    search [name]    Search for a bew on store." << std::endl;
        std::cout << "    fetch            Fetch and print all store bews." << std::endl;
        std::cout << "    list             List all installed bews." << std::endl;
        std::cout << std::endl;
        return 0;
    }

    return app.exec();
}

int main(int argc, char *argv[])
{
    if (handleCommand(argc, argv) >= 0)
        return 0;

    QApplication app(argc, argv);
    app.setApplicationName("bew");
    app.setApplicationVersion("0.1");
    app.setWindowIcon(QIcon(":/icons/bew.png"));

    SettingsDialog::configureProxy();

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
