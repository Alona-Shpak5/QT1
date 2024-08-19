#ifndef SENSOR_HPP
#define SENSOR_HPP
#include <QObject>
#include "SensorMetric.hpp"

class Sensor : public QObject {
    Q_OBJECT
public:
    Sensor(const QString& name) : name(name) {}

    void newData(int value) {
        SensorMetric metric{name, value};
        emit dataReady(metric);
    }

signals:
    void dataReady(const SensorMetric& aSensorMetric);

private:
    QString name;
};
#endif // SENSOR_HPP
