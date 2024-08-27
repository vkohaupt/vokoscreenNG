
#include <QObject>
#include <QRect>
#include <QVariantMap>

// forward
class QDBusInterface;
class QScreen;

class Portal_wl : public QObject
{
    Q_OBJECT

public:
    typedef struct
    {
        uint node_id;
        QVariantMap map;
    } Stream;
    typedef QList<Stream> Streams;

    explicit Portal_wl(QObject* parent = nullptr);
    virtual ~Portal_wl();

public slots:
    void startScreenCast(bool withCursor);
    void stopScreenCast();

signals:
    void signal_portal_fd_path(QString,QString);
    void screenCastAborted();

private slots:
    void handleCreateSessionResponse(uint response, const QVariantMap& results);
    void handleSelectSourcesResponse(uint response, const QVariantMap& results);
    void handleStartResponse(uint response, const QVariantMap& results);

private:
    QDBusInterface* screencastPortal();
    QString createSessionToken() const;
    QString createRequestToken() const;

private:
    QRect mScreenRect;
    bool mWithCursor{false};
    QString mSession;
    QString mRequestPath;
    QString mRestoreToken;
    QDBusInterface* mScreencastPortal{nullptr};
};
