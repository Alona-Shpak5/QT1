#ifndef LOGGER_HPP
#define LOGGER_HPP
#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include "SensorMetric.hpp"
#include "Config.hpp"

class Logger : public QObject {
    Q_OBJECT
public:
    Logger() {
        QString fileName = Config::LOG_FILE_PREFIX + QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss") + Config::LOG_FILE_EXTENSION;
        logFile.setFileName(fileName);
        if (!logFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qWarning() << "Cannot open log file for writing";
        }
    }

    ~Logger() {
        logFile.close();
    }

public slots:
    void writeData(const SensorMetric& aSensorMetric) {
        if (!logFile.isOpen()) return;

        QTextStream out(&logFile);
        out << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss") << " | name="
            << aSensorMetric.name << " | value=" << aSensorMetric.value << "\n";
        out.flush();
    }

private:
    QFile logFile;
};
#endif // LOGGER_HPP
