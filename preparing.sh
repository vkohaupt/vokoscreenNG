#!/bin/bash

# This file is only for the main developer, so it contains a mixture of
# German and English. It does two things:
#
# 1. Looks for new words to be translated and writes them to the file
# vokoscreen-version-3.ts for later translation on Transifex.
# 2. Downloads completed translations from Transifex and writes them to
# screencast.qrc.


# Testen ob lupdate auf dem System vorhanden ist.
progname=$(which lupdate6)
if [ $? = 0 ]; then
    echo
else
    kdialog --title "preparing.sh" --msgbox "lupdate6 nicht auf System vorhanden. lupdate6 ist bestandteil von Programmpacket qt6-tools-linguist"
    echo "[preparing.sh] lupdate6 nicht auf System vorhanden"
    echo "[preparing.sh] lupdate6 ist bestandteil von Programmpacket qt6-tools-linguist"
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


echo ==============================
echo Create vokoscreen-version-3.ts
echo ==============================
lupdate6 -locations none -recursive ./src -ts src/vokoscreen-version-3.ts

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
array="<file>"$( find ./pictures/* -name *.license | cut -c 3- )"</file>"
echo $array | sed "s/ /<\/file>\n<file>/g" >> screencast.qrc
array="<file>"$( find ./pictures/* -name *.qss | cut -c 3- )"</file>"
echo $array | sed "s/ /<\/file>\n<file>/g" >> screencast.qrc

#######
# Sound
#######
array="<file>"$( find ./sound/* -name *.wav | cut -c 3- )"</file>"
echo $array | sed "s/ /<\/file>\n<file>/g" >> screencast.qrc

###############################
# cisco openh264 binary license
###############################
array="<file>"$( find ./ciscoOpenh264/* -name *.txt | cut -c 3- )"</file>"
echo $array | sed "s/ /<\/file>\n<file>/g" >> screencast.qrc

################
# language packs
################
array='<file>'$( ls language/*.ts )'</file>'
echo $array | sed "s/ /<\/file>\n<file>/g" >> screencast.qrc
sed -i 's/\.ts/.qm/g' screencast.qrc

echo '</qresource>' >> screencast.qrc
echo '</RCC>'  >> screencast.qrc


echo =======================
echo Create translations.pri
echo =======================

############################################
# language packs write into translations.pri
############################################
array='TRANSLATIONS += '$( ls language/*.ts )
echo $array >> translations.pri


echo ==========
echo Script end
echo ==========

