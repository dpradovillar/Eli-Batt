#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "reportgenerator.h"

#include "ui_mainwindow.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

public slots:
    void onActionAboutTriggered();
    void onActionSelectFolder();
    void onProcessButtonClicked();

    void processDirectoryContent(QDate &from, QDate &to);
    QString processDates(const QDate &from, const QDate &to);

    QDate scanDate(const QString &filename, bool &ok);
};

#endif // MAINWINDOW_H
