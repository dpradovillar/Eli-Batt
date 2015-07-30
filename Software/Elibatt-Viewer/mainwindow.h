#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "reportgenerator.h"
#include "utils.h"

#include <QtCore>
#include <QMainWindow>

#include "ui_mainwindow.h"

//#define TEST_FOLDER     "C:/Users/rene/Desktop/Archive/work/GPFramework/E-liBatt/Software/Elibatt-Viewer/data"
//#define TEST_OUTPUT_PDF "C:/Users/rene/Desktop/Archive/work/GPFramework/E-liBatt/Software/Elibatt-Viewer/data/doc.pdf"
#define TEMPORARY_FILE "temp.dat"
#define TEMPORARY_PDF "temp.pdf"

class MainWindow : public QMainWindow, UiHandler {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    int lastPercent;
    qint64 lastTotalLines;

public slots:
    void onActionAboutTriggered();
    void onActionSelectFolder();
    void setupInitialDirectory();
    void onProcessButtonClicked();

    void processDirectoryContent(QDate &from, QDate &to);
    QString processDates(const QDate &from, const QDate &to);

    QDate scanDate(const QString &filename, bool &ok);

    qint64 countLines(const QStringList &l);

    virtual void onProgressUpdate(qreal percent);
};

#endif // MAINWINDOW_H
