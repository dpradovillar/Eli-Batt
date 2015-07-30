#include "mainwindow.h"

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>

#include "ui_aboutdialog.h"

#define DATE_REGEXP_FILENAME "(\\d\\d\\d\\d)(\\d\\d)(\\d\\d).*"
#define SETTINGS_FILE "settings.ini"
#define DATA_FOLDER_KEY "data_folder"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(onActionAboutTriggered()));
    connect(ui->actionSelectFolder, SIGNAL(triggered()), this, SLOT(onActionSelectFolder()));
    connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));

    connect(ui->process, SIGNAL(clicked()), this, SLOT(onProcessButtonClicked()));

    QSettings settings(SETTINGS_FILE, QSettings::IniFormat);
    ui->folder->setText(settings.value(DATA_FOLDER_KEY, "").toString());
    setupInitialDirectory();

#ifdef TEST_FOLDER
#ifdef TEST_OUTPUT_PDF
    onActionSelectFolder();
#endif
#endif
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onActionAboutTriggered() {
    QDialog d;
    Ui::AboutDialog ui;
    ui.setupUi(&d);
    d.exec();
}

void MainWindow::onActionSelectFolder() {
    QString filename =
#ifdef TEST_FOLDER
        TEST_FOLDER
#else
        QFileDialog::getExistingDirectory(this, "Seleccionar una carpeta", "", QFileDialog::ShowDirsOnly)
#endif
    ;
    ui->folder->setText(filename);
    setupInitialDirectory();
}
void MainWindow::setupInitialDirectory() {
    QString filename = ui->folder->text();
    if (!filename.isEmpty()) {
        QSettings settings(SETTINGS_FILE, QSettings::IniFormat);
        settings.setValue(DATA_FOLDER_KEY, filename);

        QDir dir(filename);
        dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
        int countFiles = dir.count();
        ui->total_files->setText(QString("%1 archivos.").arg(countFiles));

        if (countFiles > 0) {
            ui->to_date->setEnabled(true);
            ui->from_date->setEnabled(true);
            ui->process->setEnabled(true);

            QDate f, t;
            processDirectoryContent(f, t);

            ui->from_date->setDateRange(f, t);
            ui->to_date->setDateRange(f, t);

            ui->from_date->setSelectedDate(f);
            ui->to_date->setSelectedDate(t);

#ifdef TEST_FOLDER
#ifdef TEST_OUTPUT_PDF
            onProcessButtonClicked();
#endif
#endif
        }
    }
}

void MainWindow::onProcessButtonClicked() {
    lastPercent = -1;
    onProgressUpdate(0.0);
    QString msg = processDates(
        qMin(ui->from_date->selectedDate(), ui->to_date->selectedDate()),
        qMax(ui->from_date->selectedDate(), ui->to_date->selectedDate())
    );
    if (msg.isEmpty()) {
        ReportGenerator rg(TEMPORARY_FILE, TEMPORARY_PDF, this);

        QString outputFilename =
#ifdef TEST_OUTPUT_PDF
            TEST_OUTPUT_PDF
#else
            QFileDialog::getSaveFileName(0, "Archivo PDF de salida")
#endif
        ;
        if (outputFilename.isNull()) {
            QMessageBox::warning(0, "Error", "Procesamiento cancelado.");
            return;
        }
        if (!outputFilename.toLower().endsWith(".pdf")) {
            outputFilename += ".pdf";
        }

        msg = rg.create(lastTotalLines);

        if (QFile(TEMPORARY_PDF).rename(outputFilename)) {
            QMessageBox::information(0, "Informacion", "Procesamiento OK\n" + outputFilename + ".");
            return;
        }
        QMessageBox::warning(0, "Error", "No se puede generar el archivo\n" + outputFilename);
    } else {
        QMessageBox::warning(0, "Error", msg);
    }
}

void MainWindow::processDirectoryContent(QDate &from, QDate &to) {
    QStringList files = QDir(ui->folder->text()).entryList();
    bool seenDates = false;
    foreach(QString s, files) {
        bool ok = false;
        QDate aDate = scanDate(s, ok);
        if (ok) {
            if (seenDates) {
                if (aDate < from) {
                    from = aDate;
                }
                if (aDate > to) {
                    to = aDate;
                }
            } else {
                from = to = aDate;
                seenDates = true;
            }
        }
    }
}

QString MainWindow::processDates(const QDate &from, const QDate &to) {
    QDir path(ui->folder->text());
    QStringList files = path.entryList();
    QFile temporary(TEMPORARY_FILE);
    temporary.remove();
    if (!temporary.open(QIODevice::WriteOnly)) {
        return "No se puede crear archivo temporal.";
    }
    QTextStream out(&temporary);
    QStringList cleanedFilenames;

    foreach(QString s, files) {
        bool ok = false;
        QDate aDate = scanDate(s, ok);
        if (ok && from <= aDate && aDate <= to) {
            QString w = path.absoluteFilePath(s);
            cleanedFilenames.append(w);
        }
    }

    lastTotalLines = countLines(cleanedFilenames);
    qint64 currentLines = 0;

    foreach(QString w, cleanedFilenames) {
        QFile inputFile(w);
        if (!inputFile.open(QIODevice::ReadOnly)) {
            temporary.close();
            return "El archivo " + w + " no puede ser leido.";
        }
        QTextStream in(&inputFile);
        QString temp;
        qint64 last_t = -1;
        while(!(temp = in.readLine()).isNull()) {
            currentLines++;
            onProgressUpdate(((double)currentLines) / lastTotalLines);
            QStringList parts = temp.split(";");
            QDateTime dt = QDateTime::fromString(parts[0], "yyyyMMdd'T'hhmmss");
            // Save only one datum every 5 minutes
            qint64 slot = dt.toMSecsSinceEpoch() / 300000;
            if (slot != last_t) {
                out <<
                    parts[0] << ';' <<
                    parts[1] << ';' <<
                    parts[2] << ';' <<
                    parts[3] <<
                endl;
                last_t = slot;
            }
        }
        inputFile.close();
    }

    temporary.close();

    return "";
}

QDate MainWindow::scanDate(const QString &filename, bool &ok) {
    QRegExp r(DATE_REGEXP_FILENAME);
    if (r.indexIn(filename) != -1) {
        QStringList captured = r.capturedTexts();

        ok = false;
        int y = captured[1].toInt(&ok);
        if (!ok) {
            return QDate();
        }
        int m = captured[2].toInt(&ok);
        if (!ok) {
            return QDate();
        }
        int d = captured[3].toInt(&ok);
        if (!ok) {
            return QDate();
        }
        ok = true;
        return QDate(y, m, d);
    }
    ok = false;
    return QDate();
}

qint64 MainWindow::countLines(const QStringList &l) {
    qint64 t = 0;
    foreach(QString s, l) {
        QFile f(s);
        if(!f.open(QIODevice::ReadOnly)) {
            return -1;
        }
        QTextStream in(&f);
        QString temp;
        while(!(temp = in.readLine()).isNull()) {
            t++;
        }
        f.close();
    }
    return t;
}

void MainWindow::onProgressUpdate(qreal percent) {
    int p = (int)(100*percent);
    if (p != lastPercent) {
        ui->progressBar->setValue(p);
    }
    qApp->processEvents();
}
