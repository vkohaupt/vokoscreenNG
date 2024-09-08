#/usr/bin/sh

rm -rf com.github.vkohaupt.vokoscreenNG

flatpak -y --user install org.kde.Sdk//6.6
flatpak -y --user install org.kde.Platform//6.6

#git clone https://github.com/flathub/com.github.vkohaupt.vokoscreenNG
mkdir com.github.vkohaupt.vokoscreenNG
cp com.github.vkohaupt.vokoscreenNG.yml com.github.vkohaupt.vokoscreenNG/com.github.vkohaupt.vokoscreenNG.yml
cp vokoscreenNG.appdata.xml com.github.vkohaupt.vokoscreenNG/vokoscreenNG.appdata.xml
cd com.github.vkohaupt.vokoscreenNG

flatpak-builder build-dir com.github.vkohaupt.vokoscreenNG.yml
flatpak-builder --user --install --force-clean build-dir com.github.vkohaupt.vokoscreenNG.yml
flatpak run com.github.vkohaupt.vokoscreenNG

