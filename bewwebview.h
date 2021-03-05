#ifndef BEWWEBVIEW_H
#define BEWWEBVIEW_H

#include <QWebEngineView>

class BewWebView : public QWebEngineView
{
    Q_OBJECT
public:
    BewWebView(QWidget *parent = nullptr);
    virtual ~BewWebView();

private:
    QString featureToString(QWebEnginePage::Feature);
    QString featureToKey(QWebEnginePage::Feature);
    void checkPermission(QWebEngineView *view, QWebEnginePage *page, const QUrl &securityOrigin, QWebEnginePage::Feature feature);

protected:
    virtual QWebEngineView *createWindow(QWebEnginePage::WebWindowType type);
};

#endif // BEWWEBVIEW_H
