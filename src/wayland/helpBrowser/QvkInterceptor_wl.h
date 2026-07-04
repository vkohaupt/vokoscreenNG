#include <QWebEngineUrlRequestInterceptor>
#include <QWebEngineUrlRequestInfo>

class QvkInterceptor_wl : public QWebEngineUrlRequestInterceptor {
public:
    void interceptRequest(QWebEngineUrlRequestInfo &info) override {
        // Prüfen, ob die Anfrage ein Bild ist
/*
        if (info.resourceType() == QWebEngineUrlRequestInfo::ResourceTypeImage) {
            // Beispiel: Blockiert alle Bilder, die "werbung" in der URL enthalten
            if (info.requestUrl().toString().contains("vcss-blue", Qt::CaseInsensitive)) {
                info.block(true);
            }
            if (info.requestUrl().toString().contains("valid-html401", Qt::CaseInsensitive)) {
                info.block(true);
            }
        }
*/
        QString urlString = info.requestUrl().toString();
        if (urlString.contains("validator.w3.org")) {
            // Die Anfrage blockieren
            info.block(true);
        }
    }
};
