#!/bin/bash

# Testen ob lupdate auf dem System vorhanden ist.
progname=$(which lupdate-qt5)
if [ $? = 0 ]; then
    echo
else
    kdialog --title "preparing.sh" --msgbox "lupdate-qt5 nicht auf System vorhanden. lupdate-qt5 ist bestandteil von Programmpacket libqt5-linguist"
    echo "[preparing.sh] lupdate-qt5 nicht auf System vorhanden"
    echo "[preparing.sh] lupdate-qt5 ist bestandteil von Programmpacket libqt5-linguist"
    exit 1
fi

# Testen ob tx auf dem System vorhanden ist.
progname=$(which tx)
if [ $? = 0 ]; then
    echo
else
    kdialog --title "preparing.sh" --msgbox "tx nicht auf System vorhanden. tx ist bestandteil von Programmpacket transifex-client"
    echo "[preparing.sh] tx nicht auf System vorhanden"
    echo "[preparing.sh] tx ist bestandteil von Programmpacket transifex-client"
    exit 1
fi


echo ============
echo Make main-3.ts
echo ============
lupdate-qt5 -locations none -recursive ./src -ts src/vokoscreen-version-3.ts

echo =====================
echo Remove language packs
echo =====================
rm -r src/language

echo ======================
echo Download language pack
echo ======================
tx pull -a

echo =============================
echo copy language to src/language
echo =============================
cp -r language src/language

echo ===============
echo remove language
echo ===============
rm -r language

echo ====================
echo change in src folder
echo ====================
cd src

echo =========================
echo Remove old screencast.qrc
echo =========================
rm screencast.qrc

echo =====================
echo Create screencast.qrc
echo =====================
##########
# Pictures
##########
echo '<!DOCTYPE RCC><RCC version="1.0">' > screencast.qrc
echo "<qresource>" >> screencast.qrc
array="<file>"$( find ./pictures/* -name *.png | cut -c 3- )"</file>"
echo $array | sed "s/ /<\/file>\n<file>/g" >> screencast.qrc
array="<file>"$( find ./pictures/* -name *.svg | cut -c 3- )"</file>"
echo $array | sed "s/ /<\/file>\n<file>/g" >> screencast.qrc
array="<file>"$( find ./pictures/* -name *.license | cut -c 3- )"</file>"
echo $array | sed "s/ /<\/file>\n<file>/g" >> screencast.qrc

################
# language packs
################
array='<file>'$( ls language/*.ts )'</file>'
echo $array | sed "s/ /<\/file>\n<file>/g" >> screencast.qrc
sed -i 's/\.ts/.qm/g' screencast.qrc

echo '</qresource>' >> screencast.qrc
echo '</RCC>'  >> screencast.qrc

echo ==========
echo Script end
echo ==========
