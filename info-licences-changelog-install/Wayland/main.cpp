#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <LayerShellQt/Window>

int main(int argc, char *argv[]) {

    QApplication app(argc, argv);

    QWidget window;
    window.setWindowTitle("Wayland LayerShell Test");
    window.resize(350, 150);
    window.setAttribute(Qt::WA_TranslucentBackground, true);

    // Einfaches Layout und ein Schließen-Button
    auto *layout = new QVBoxLayout(&window);
    auto *closeBtn = new QPushButton("Beenden", &window);
    QObject::connect(closeBtn, &QPushButton::clicked, &app, &QApplication::quit);
    layout->addWidget(closeBtn);

    // 2. Erzwingt die Erstellung des QWindow-Handles vor dem Anzeigen
    window.winId();

    // 3. LayerShellQt-Einstellungen anwenden
    if (auto *layerWindow = LayerShellQt::Window::get(window.windowHandle())) {
        // Legt das Fenster dauerhaft ÜBER alle normalen Anwendungsfenster
        layerWindow->setLayer(LayerShellQt::Window::LayerTop);

        // Verhindert, dass das Fenster Tastaturfokus bekommt
        layerWindow->setKeyboardInteractivity(LayerShellQt::Window::KeyboardInteractivityNone);

        // Optional: Positionierung am Bildschirm (Standard ist zentriert)
        // layerWindow->setAnchors(LayerShellQt::Window::AnchorTop | LayerShellQt::Window::AnchorRight);
        // layerWindow->setMargins(QMargins(0, 20, 20, 0));
    } else {
        qWarning("LayerShellQt konnte nicht initialisiert werden.");
    }

    window.show();

    return app.exec();
}
