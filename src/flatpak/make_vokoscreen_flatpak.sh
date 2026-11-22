#!/usr/bin/sh

set -e

flatpak remote-add --user --if-not-exists flathub https://flathub.org/repo/flathub.flatpakrepo

flatpak update -y

flatpak-builder --user --install --install-deps-from=flathub --force-clean build-dir com.github.vkohaupt.vokoscreenNG.yml
