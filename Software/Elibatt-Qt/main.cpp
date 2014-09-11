#include <QApplication>

#include "uidialog.h"
//#include "dialog.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    //Dialog dialog;
    //dialog.show();

    UiDialog uiDialog;
    uiDialog.show();
    return app.exec();
}
