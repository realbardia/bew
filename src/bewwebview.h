#ifndef BEWWEBVIEW_H
#define BEWWEBVIEW_H

#include <QWebEngineView>

class BEWWebView : public QWebEngineView
{
    Q_OBJECT
public:
    BEWWebView(QWidget *parent = nullptr);
    virtual ~BEWWebView();

private:
    QString featureToString(QWebEnginePage::Feature);
    QString featureToKey(QWebEnginePage::Feature);
    void checkPermission(QWebEngineView *view, QWebEnginePage *page, const QUrl &securityOrigin, QWebEnginePage::Feature feature);

protected:
    virtual QWebEngineView *createWindow(QWebEnginePage::WebWindowType type);
};

#endif // BEWWEBVIEW_H
