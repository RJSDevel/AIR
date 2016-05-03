#include "gallary.h"

Gallary::Gallary(QListWidget *lw) : _lw(lw), QObject()
{
   _load_thread = new QThread();

   _image_names = new QMap<QString, QString>();
   _loaded_images = new QMap<QString, QImage>();
   _pixmaps = new QMap<QString, QPixmap*>();

   moveToThread(_load_thread);
   connect(_load_thread, &QThread::started, this, &Gallary::process);

   _lw->setViewMode(QListWidget::IconMode);
   _lw->setIconSize(QSize(200, 250));
   _lw->setResizeMode(QListWidget::Fixed);
}

Gallary::~Gallary()
{
    delete _pixmaps;
    delete _loaded_images;
    delete _image_names;
    delete _load_thread;
}

void Gallary::addPicture(QString path, QString name)
{
   _image_names->insert(path, name);
}

void Gallary::loadGallery()
{
    _load_thread->start();
}

void Gallary::clear()
{
    _lw->clear();
    _image_names->clear();

    QMap<QString, QPixmap*>::ConstIterator it;
    for(it = _pixmaps->constBegin(); it != _pixmaps->constEnd(); ++it) {
        delete it.value();
    }

    _pixmaps->clear();
}

const QMap<QString, QPixmap*> *Gallary::getImages() const
{
    return _pixmaps;
}

void Gallary::process()
{
    QMap<QString, QString>::ConstIterator it;
    for(it = _image_names->constBegin(); it != _image_names->constEnd(); ++it) {

          QImage image = QImage(it.key());
          _loaded_images->insert(it.key(), image);

          QPixmap *pixmap = new QPixmap();

          _pixmaps->insert(it.value(), pixmap);

          if (pixmap->convertFromImage(image)) _lw->addItem(new QListWidgetItem(*pixmap, it.value()));
     }

    _load_thread->exit();
}
