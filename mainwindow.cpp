#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->selectFilesBtn, &QPushButton::clicked, this, &MainWindow::onFilesSelect);
    connect(ui->selectFolderBtn, &QPushButton::clicked, this, &MainWindow::onFolderSelect);
    connect(ui->imageResizeBtn, &QPushButton::clicked, this, &MainWindow::onResizeImages);

    _gallary = new Gallary(ui->listWidget);
}

MainWindow::~MainWindow()
{
    delete _gallary;
    delete ui;
}

void MainWindow::onFilesSelect()
{
    _gallary->clear();

    _filenames = QFileDialog::getOpenFileNames(
                this,
                tr("Choose Images"),
                QDir::currentPath(),
                tr("Images (*.png)"));

    if (_filenames.isEmpty()) ui->inputPathLE->setText("");
    else{
        QString names = prepareImages();

        ui->inputPathLE->setText(names);
        ui->inputPathLE->setCursorPosition(0);

        _gallary->loadGallery();
    }

    checkResizeButtonReady();
}

QString MainWindow::prepareImages() {
    QString names;
    for (int i = 0; i < _filenames.size(); i++) {
        QString imageName = _filenames[i].section("/", -1, -1);

        names += "\"" + imageName + "\"" + (i == _filenames.size() - 1 ? "" : ", ");

        _gallary->addPicture(_filenames[i], imageName);
    }

    return names;
}

void MainWindow::onFolderSelect()
{
    _output_dir = QFileDialog::getExistingDirectory(this, tr("Choose Folder"));

    if (_output_dir.isEmpty()) ui->outputPathLE->setText("");
    else {
        ui->outputPathLE->setText(_output_dir);
        ui->outputPathLE->setCursorPosition(0);
    }

    checkResizeButtonReady();
}

void MainWindow::checkResizeButtonReady() {
    ui->imageResizeBtn->setEnabled(!(_filenames.isEmpty() || _output_dir.isEmpty()));
}

void MainWindow::onResizeImages() {

    const QMap<QString, QPixmap*> *pixmaps = _gallary->getImages();

    QMap<QString, QPixmap*>::ConstIterator it;
    for(it = pixmaps->constBegin(); it != pixmaps->constEnd(); ++it) {
        resizeImage(*it.value(), it.key());
    }

    QDesktopServices::openUrl(_output_dir);
}

void MainWindow::resizeImage(const QPixmap image, const QString name) {

    QPixmap scaledImage = image.copy();

    for (int direction = XXHDPI; direction <= MDPI; direction++) {

        QString output_dir = _output_dir + _slash + _output_dirs[direction];

        if (createOutputDir(output_dir)) {

            QFile file(output_dir + _slash + name);

            if(file.open(QIODevice::WriteOnly)) {
                scaledImage = scaledImage.scaled(getScaleSize(image, direction), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
                scaledImage.save(&file, PNG, 100);
                file.close();
            }
        }
    }
}

bool MainWindow::createOutputDir(QString outputDir) {

    if (QDir(outputDir).exists()) return true;

    return QDir().mkdir(outputDir);
}

QSize MainWindow::getScaleSize(QPixmap pixmap, int direction) {

    float dirCoeff = _direction_coeff[direction];

    int width = pixmap.width() / dirCoeff;
    int height = pixmap.height() / dirCoeff;

    return QSize(width, height);
}
