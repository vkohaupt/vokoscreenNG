// include checked
#include <QObject>
#include <QVariantMap>
#include <QDBusInterface>

class QvkMagnifierPortal_wl : public QObject
{
    Q_OBJECT

public:
    typedef struct
    {
        uint node_id;
        QVariantMap map;
    } Stream;
    typedef QList<Stream> Streams;

    explicit QvkMagnifierPortal_wl( QObject* parent = nullptr );
    virtual ~QvkMagnifierPortal_wl();

public slots:
    void slot_startScreenCast(uint,uint);
    void slot_stopScreenCast();

signals:
    void signal_portal_fd_path(QString,QString);
    void signal_portal_aborted();
    void signal_portal_stopped();
    void signal_portal_restoretoken(QString mRestoreToken);


private slots:
    void slot_handleCreateSessionResponse(uint response, const QVariantMap& results);
    void slot_handleSelectSourcesResponse(uint response, const QVariantMap& results);
    void slot_handleStartResponse(uint response, const QVariantMap& results);
    void slot_handleSessionClosed();

private:
    QDBusInterface* screencastPortal();
    QString createSessionToken() const;
    QString createRequestToken() const;

private:
    uint mCursorModes;
    uint mSourcType;
    QString mSession;
    QString mRequestPath;
    QString mRestoreToken;
    QDBusInterface* mScreencastPortal = nullptr;

    enum : uint { MONITOR = 1, WINDOW = 2, VIRTUAL = 4 } SourceType;
    enum : uint { HIDDEN = 1, EMBEDDED = 2, METADATA = 4 } CursorMode;
    enum : uint { TRANSIENT = 0, APPLICATION = 1, PERSISTENT = 2 } PersistMode;
};
