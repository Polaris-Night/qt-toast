#include <QApplication>
#include <QDebug>

#include "Toast.h"
#include "ToastWindow.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QApplication app(argc, argv);
    Q_INIT_RESOURCE(resources);
    ToastWindow w;
    {
        auto t = ToastManager::success(
            "Success! Confirmation email sent.", "Check your email to complete signup.\n1\n2\n3\n4");
        t->addButton("Button1", []() { qDebug() << "Click Button1"; })
            .addButton("Button2", []() { qDebug() << "Click Button2"; })
            .present();
    }
    w.show();
    return app.exec();
}
