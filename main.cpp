#include "adddialog.h"
#include "bew.h"

#include <QApplication>
#include <QCommandLineParser>

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

    parser.addOption(titleOption);
    parser.addOption(iconOption);
    parser.addOption(fontOption);
    parser.addOption(agentOption);
    parser.addOption(noScrollOption);

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
