#include <QCoreApplication>
#include <QTimer>
#include <QDebug>
#include "Sensor.hpp"
#include "Analyzer.hpp"
#include "Logger.hpp"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    // Ініціалізація сенсорів
    Sensor sensor1("Sensor_1");
    Sensor sensor2("Sensor_2");

    // Ініціалізація аналітика і логера
    Analyzer analyzer;
    Logger logger;

    // Підключення сигналів до слотів
    QObject::connect(&sensor1, &Sensor::dataReady, &analyzer, &Analyzer::analyzeData);
    QObject::connect(&sensor1, &Sensor::dataReady, &logger, &Logger::writeData);
    QObject::connect(&sensor2, &Sensor::dataReady, &analyzer, &Analyzer::analyzeData);
    QObject::connect(&sensor2, &Sensor::dataReady, &logger, &Logger::writeData);

    // Таймер для генерації даних
    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, [&] {
        sensor1.newData(rand() % 100);
        sensor2.newData(rand() % 100);
    });
    timer.start(1000);

    // Таймер для друку звіту
    QTimer::singleShot(5000, [&] {
        analyzer.reportPrint();
        a.quit();
    });

    return a.exec();
}
