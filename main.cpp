#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QSplashScreen>
#include <QProgressBar>
#include <QTimer>
#include "mainwindow.h" // Include your MainWindow header

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "EmbeddedLabProject_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            app.installTranslator(&translator);
            break;
        }
    }

    QPixmap pixmap("C:/Users/ahmedabdel-ghany/Documents/EmbeddedLabProject/splash.png"); // Correct path to your splash image
    // Calculate the new width and height based on a percentage (e.g., 50%)
    int scaledWidth = pixmap.width() * 50 / 100; // Adjust the percentage as needed
    int scaledHeight = pixmap.height() * 50 / 100; // Adjust the percentage as needed

    // Create a QSplashScreen with the scaled size
    QSplashScreen splash(pixmap.scaled(scaledWidth, scaledHeight));


    splash.show();

    // Loading some items
    splash.showMessage("Loaded modules");
    QCoreApplication::processEvents(); // Process events to display the splash screen

    // Establishing connections
    splash.showMessage("Established connections", Qt::AlignCenter, Qt::black);
    QCoreApplication::processEvents();

    MainWindow w;
    // Using a single-shot timer to delay the execution of the lambda function
    QTimer::singleShot(4000, [&]() { // 4000 milliseconds = 4 seconds
        w.show();
        splash.finish(&w);
    });
    return app.exec();
}
