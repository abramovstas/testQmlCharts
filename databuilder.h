#ifndef DATABUILDER_H
#define DATABUILDER_H

#include <QObject>
#include <QMap>

#include <QtConcurrent>

typedef QMap<QString, int> WordCount;

class DataBuilder : public QObject
{
    Q_OBJECT
    Q_PROPERTY( int progress READ progress NOTIFY progressChanged)
public:
    explicit DataBuilder(QObject *parent = nullptr);
    ~DataBuilder();

    Q_INVOKABLE void multiThreadedWordCount(const QString &fileName);


    int progress() { return m_progress; }
    void processLines(QStringList lines);
    void finished();

signals:
    void progressChanged(int progress);
    void updateData(const QVector<QPair<QString, int> > data, int maxValue);

private:
    static void reduce(WordCount &result, const WordCount &w);
    static WordCount countWords(const QString &line);
    QString readFromFile(const QString &fileName);

    QFutureWatcher<WordCount> *makeWordsWatcher;
    QFuture<WordCount> futureResult;
    int m_progress;
    int m_step = 1;
    int m_maximum = 0;
};

#endif // DATABUILDER_H
