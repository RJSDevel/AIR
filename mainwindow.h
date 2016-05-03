#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QFileDialog>
#include <QListView>
#include <QDesktopServices>

#include "gallary.h"

namespace Ui {
class MainWindow;
}

enum {
    XXHDPI  = 0,
    MDPI    = 3
};

const static float _direction_coeff[] = {
    1,      // XXHDPI
    1.5f,
    2,
    3       // MDPI
};

const static QString _output_dirs[] = {
    "drawable-xxhdpi",
    "drawable-xhdpi",
    "drawable-hdpi",
    "drawable-mdpi"
};

const static QChar _slash = '/';

const static char* PNG = "png";


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QStringList _filenames;
    QString _output_dir;

    Gallary *_gallary;

private:
    QString prepareImages();
    void checkResizeButtonReady();

    void resizeImage(const QPixmap image, const QString name);
    bool createOutputDir(QString pOutputDir);
    QSize getScaleSize(QPixmap pixmap, int direction);

signals:
    void onFilesSelect();
    void onFolderSelect();
    void onResizeImages();
};

#endif // MAINWINDOW_H
