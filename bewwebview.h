#ifndef BEWWEBVIEW_H
#define BEWWEBVIEW_H

#include <QWebEngineView>

class BewWebView : public QWebEngineView
{
    Q_OBJECT
public:
    BewWebView(QWidget *parent = nullptr);
    virtual ~BewWebView();

private Q_SLOTS:
    void on_webEngineView_urlChanged(const QString &arg1);

protected:
    virtual QWebEngineView *createWindow(QWebEnginePage::WebWindowType type);
};

#endif // BEWWEBVIEW_H
