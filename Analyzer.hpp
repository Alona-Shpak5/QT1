
#ifndef ANALYZER_HPP
#define ANALYZER_HPP
#include <QObject>

#include <QMap>
#include <QVector>
#include <algorithm>
#include <numeric>
#include <qdebug.h>
#include "Config.hpp"
#include "SensorMetric.hpp"

class Analyzer : public QObject {
    Q_OBJECT
public slots:
    void analyzeData(const SensorMetric& aSensorMetric) {
        // Оновлюємо дані для аналітики
        QString sensorName = aSensorMetric.name;
        int value = aSensorMetric.value;

        if (!data.contains(sensorName)) {
            data[sensorName] = QVector<int>();
        }
        if (data[sensorName].size() >= Config::MAX_RECORDS) {
            data[sensorName].removeFirst();
        }
        data[sensorName].append(value);
    }

    void reportPrint() const {
        QMap<QString, QVector<int>> allData;

        for (const auto& key : data.keys()) {
            QVector<int> values = data[key];
            if (values.isEmpty()) continue;

            int max = *std::max_element(values.begin(), values.end());
            int min = *std::min_element(values.begin(), values.end());
            double avg = std::accumulate(values.begin(), values.end(), 0) / static_cast<double>(values.size());

            QVector<int> sortedValues = values;
            std::sort(sortedValues.begin(), sortedValues.end());
            double median;
            int size = sortedValues.size();
            if (size % 2 == 0) {
                median = (sortedValues[size / 2 - 1] + sortedValues[size / 2]) / 2.0;
            } else {
                median = sortedValues[size / 2];
            }

            qDebug() << QString("Sensor: %1").arg(key);
            qDebug() << QString("Max: %1").arg(max);
            qDebug() << QString("Min: %1").arg(min);
            qDebug() << QString("Average: %1").arg(avg);
            qDebug() << QString("Median: %1").arg(median);
            qDebug() << "-----------------------------------";

            allData[key] = values;
        }

        // Аналіз всіх сенсорів
        QVector<int> allValues;
        for (const auto& values : allData.values()) {
            allValues.append(values);
        }

        if (!allValues.isEmpty()) {
            int maxAll = *std::max_element(allValues.begin(), allValues.end());
            int minAll = *std::min_element(allValues.begin(), allValues.end());
            double avgAll = std::accumulate(allValues.begin(), allValues.end(), 0) / static_cast<double>(allValues.size());

            QVector<int> sortedAllValues = allValues;
            std::sort(sortedAllValues.begin(), sortedAllValues.end());
            double medianAll;
            int sizeAll = sortedAllValues.size();
            if (sizeAll % 2 == 0) {
                medianAll = (sortedAllValues[sizeAll / 2 - 1] + sortedAllValues[sizeAll / 2]) / 2.0;
            } else {
                medianAll = sortedAllValues[sizeAll / 2];
            }

            qDebug() << "Overall Analysis";
            qDebug() << QString("Max: %1").arg(maxAll);
            qDebug() << QString("Min: %1").arg(minAll);
            qDebug() << QString("Average: %1").arg(avgAll);
            qDebug() << QString("Median: %1").arg(medianAll);
        }
    }

private:
    QMap<QString, QVector<int>> data;
};

#endif // ANALYZER_HPP
