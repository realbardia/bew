#include "bewmessagewidget.h"
#include "ui_bewmessagewidget.h"

#include <QTime>
#include <QTimer>

BewMessageWidget::BewMessageWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::BewMessageWidget)
{
    ui->setupUi(this);
    ui->timeoutLabel->setText("01:00");

    mUnlockTimer = new QTimer(this);
    mUnlockTimer->setSingleShot(false);
    mUnlockTimer->setInterval(1000);

    connect(mUnlockTimer, &QTimer::timeout, this, [this](){
        mUnlockCountdown--;
        ui->timeoutLabel->setText( QTime::fromMSecsSinceStartOfDay(mUnlockCountdown * 1000).toString("mm:ss") );
        if (mUnlockCountdown > 0)
            return;

        mUnlockTimer->stop();
        setActivated(false);
    });

    mTimer = new QTimer(this);
    mTimer->setSingleShot(true);

    connect(mTimer, &QTimer::timeout, this, [this](){
        ui->timeoutLabel->setText("01:00");
        setActivated(true);
    });
}

BewMessageWidget::~BewMessageWidget()
{
    delete ui;
}

QString BewMessageWidget::message() const
{
    return mMessage;
}

void BewMessageWidget::setMessage(const QString &newMessage)
{
    mMessage = newMessage;
    ui->messageLabel->setText(mMessage);

    setActivated(!mMessage.isEmpty());
}

qint32 BewMessageWidget::messageIdleMinutes() const
{
    return mMessageIdleMinutes;
}

void BewMessageWidget::setMessageIdleMinutes(qint32 newMessageIdleMinutes)
{
    mMessageIdleMinutes = newMessageIdleMinutes;
}

bool BewMessageWidget::activated() const
{
    return mActivated;
}

void BewMessageWidget::setActivated(bool newActivated)
{
    if (mActivated == newActivated)
        return;

    mActivated = newActivated;
    ui->unlockBtn->setEnabled(true);

    mTimer->stop();
    if (mMessageIdleMinutes)
        mTimer->start(mMessageIdleMinutes * 60000);

    Q_EMIT activatedChanged(mActivated);
}

void BewMessageWidget::on_unlockBtn_clicked()
{
    mUnlockCountdown = 59;

    ui->timeoutLabel->setText("00:59");
    ui->unlockBtn->setEnabled(false);

    mUnlockTimer->stop();
    mUnlockTimer->start();
}

