#ifndef GALLARY_H
#define GALLARY_H

#include <QObject>
#include <QListView>
#include <QListWidget>
#include <QThread>
#include <QMap>

class Gallary : public QObject
{
    Q_OBJECT
public:
    explicit Gallary(QListWidget *lw);
    virtual ~Gallary();

    void addPicture(QString path, QString name);
    void loadGallery();
    void clear();

    const QMap<QString, QPixmap*> *getImages() const;

private:
    QListWidget *_lw;
    QThread *_load_thread;

    QMap<QString, QString>  *_image_names;
    QMap<QString, QImage>  *_loaded_images;

    QMap<QString, QPixmap*> *_pixmaps;

signals:
    void process();
};

#endif // GALLARY_H
