#ifndef BEW_GLOBALS_H
#define BEW_GLOBALS_H

#include <QString>
#include <QSettings>
#include <QSharedPointer>
#include <QDir>

#define BEW_DEFAULT_USER_AGENT QStringLiteral("Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/122.0.0.0 Safari/537.36 ")
#define BEW_STORE_URL QStringLiteral("https://raw.githubusercontent.com/realbardia/bew/main/store.json")

#define BEW_SETTINGS QSharedPointer<QSettings>(new QSettings(QDir::homePath() + "/.config/bew.ini", QSettings::IniFormat))

#endif // BEW_GLOBALS_H
