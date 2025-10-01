#ifndef BEWMESSAGEWIDGET_H
#define BEWMESSAGEWIDGET_H

#include <QWidget>

namespace Ui {
class BewMessageWidget;
}

class BewMessageWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(bool activated READ activated WRITE setActivated NOTIFY activatedChanged FINAL)

public:
    explicit BewMessageWidget(QWidget *parent = nullptr);
    ~BewMessageWidget();

    QString message() const;
    void setMessage(const QString &newMessage);

    qint32 messageIdleMinutes() const;
    void setMessageIdleMinutes(qint32 newMessageIdleMinutes);

    bool activated() const;

Q_SIGNALS:
    void activatedChanged(bool activated);

protected:
    void setActivated(bool newActivated);

private slots:
    void on_unlockBtn_clicked();

private:
    Ui::BewMessageWidget *ui;

    QString mMessage;
    qint32 mMessageIdleMinutes = 0;
    bool mActivated = false;

    QTimer *mTimer = nullptr;
    QTimer *mUnlockTimer = nullptr;

    qint32 mUnlockCountdown = 0;
};

#endif // BEWMESSAGEWIDGET_H
