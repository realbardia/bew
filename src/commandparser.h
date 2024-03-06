#ifndef COMMANDPARSER_H
#define COMMANDPARSER_H

#include <QStringList>
#include <QNetworkProxy>

#include <optional>

class CommandParser
{
    CommandParser();

public:
    struct Result {
        bool isNull = true;

        QString url;

        bool noScroll = false;
        bool systray = false;
        bool singleInstance = false;

        QString font;
        QString title;
        QString icon;
        QString agent;

        QString proxyType;
        QString proxyHost;
        int proxyPort = 0;
        QString proxyUser;
        QString proxyPass;

        std::optional<QNetworkProxy> proxy() const;
    };

    static Result parse(const QString &command);
    static Result parse(const QStringList &args);
};

#endif // COMMANDPARSER_H
