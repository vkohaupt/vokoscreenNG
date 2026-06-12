#include "QvkAudioPipewireSingle_wl.h"
#include "ui_QvkAudioPipewireSingle_wl.h"

QvkAudioPipewireSingle_wl::QvkAudioPipewireSingle_wl(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::QvkAudioPipewireSingle_wl)
{
    ui->setupUi(this);
}

QvkAudioPipewireSingle_wl::~QvkAudioPipewireSingle_wl()
{
    delete ui;
}
