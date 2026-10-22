#/usr/bin/sh

flatpak remote-add --user --if-not-exists flathub https://flathub.org/repo/flathub.flatpakrepo

# Flatpak-Metadaten aktualisieren
flatpak update

rm -rf com.github.vkohaupt.vokoscreenNG

#flatpak -y --user install org.kde.Sdk//6.10
#flatpak -y --user install org.kde.Platform//6.10

git clone https://github.com/flathub/com.github.vkohaupt.vokoscreenNG
cp com.github.vkohaupt.vokoscreenNG.yml com.github.vkohaupt.vokoscreenNG/com.github.vkohaupt.vokoscreenNG.yml
cd com.github.vkohaupt.vokoscreenNG

flatpak-builder --user --install --install-deps-from=flathub --force-clean build-dir com.github.vkohaupt.vokoscreenNG.yml

