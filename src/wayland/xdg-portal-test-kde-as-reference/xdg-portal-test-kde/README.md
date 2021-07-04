# flatpak-portal-test-kde

A simple test application for [Flatpak](http://www.flatpak.org) portals and Qt flatpak platform plugin.

The portal interfaces are defined in [xdg-desktop-portal](https://github.com/flatpak/xdg-desktop-portal).

A Qt/KDE implementation can be found in [xdg-desktop-portal-kde](https://cgit.kde.org/xdg-desktop-portal-kde.git/).

To use this test, use the build script in flatpak-build/ to produce a flatpak of portal-test, then install it with

    flatpak remote-add --user --no-gpg-verify portal-test-kde file:///path/to/repo
    flatpak install --user portal-test-kde org.kde.portal-test-kde

and run it with

    flatpak run org.kde.portal-test-kde

The test expects the xdg-desktop-portal service (and a backend, such as xdg-desktop-portal-kde) to be available on the session bus.
