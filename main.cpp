#include <QCoreApplication>
#include <QCommandLineParser>
#include <QProcessEnvironment>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QDateTime>
#include <QResource>

// Функція для копіювання конфігураційного файлу з ресурсів
void copyDefaultConfigFile(const QString &destinationPath) {
    QResource resource(":/config.json");
    if (!resource.isValid()) {
        qWarning() << "Resource not found.";
        return;
    }

    QFile file(destinationPath);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Couldn't open file for writing:" << destinationPath;
        return;
    }

    // Перетворюємо дані з ресурсу в QByteArray
    QByteArray resourceData(reinterpret_cast<const char*>(resource.data()), resource.size());
    file.write(resourceData);
    file.close();
    qDebug() << "Copied default config to" << destinationPath;
}

// Функція для читання JSON з файлу
void readJsonFile(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Couldn't open file:" << filePath;
        return;
    }

    QByteArray jsonData = file.readAll();
    QJsonParseError parseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "Error parsing JSON:" << parseError.errorString();
        return;
    }

    QJsonObject jsonObject = jsonDoc.object();
    qDebug() << "JSON content from file:" << jsonObject;

    // Приклад роботи з даними JSON (перевірка наявності ключа)
    if (jsonObject.contains("exampleKey")) {
        qDebug() << "exampleKey:" << jsonObject["exampleKey"].toString();
    }
}

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    // Параметри командного рядка
    QCommandLineParser parser;
    parser.setApplicationDescription("Configuration File Example");
    parser.addHelpOption();
    parser.addOption(QCommandLineOption("config", "Path to configuration file.", "file"));
    parser.process(app);

    // Отримання шляху до конфігураційного файлу з параметрів командного рядка
    QString configPath = parser.value("config");
    if (configPath.isEmpty()) {
        configPath = "config.json";  // Дефолтний шлях
    }

    // Перевірка наявності конфігураційного файлу
    QFile configFile(configPath);
    if (!configFile.exists()) {
        qDebug() << "Config file not found. Copying default configuration.";
        copyDefaultConfigFile(configPath);
    }

    // Читання конфігураційного файлу
    readJsonFile(configPath);

    // Перевірка змінних середовища
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    if (env.contains("WINDOW_TITLE")) {
        QString windowTitle = env.value("WINDOW_TITLE");
        qDebug() << "Window Title from environment:" << windowTitle;
    }

    return app.exec();
}
