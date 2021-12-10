#include "databuilder.h"

#include <QFile>
#include <QTextStream>
#include <QPair>

extern int topWordCount;

DataBuilder::DataBuilder(QObject *parent) : QObject(parent)
{
    makeWordsWatcher = new QFutureWatcher<WordCount>(this);
    connect(makeWordsWatcher, &QFutureWatcher<WordCount>::finished, this, &DataBuilder::finished);
    connect(makeWordsWatcher, &QFutureWatcher<WordCount>::progressValueChanged, this, [this](int progress)
    {
        if (progress == m_maximum)
            m_progress = 100;
        else
            m_progress = progress * m_step;

        emit progressChanged(m_progress);
    });

    QObject::connect(makeWordsWatcher,  &QFutureWatcher<WordCount>::progressRangeChanged, this, [this](int , int maximum)
    {
        if (maximum > 0)
        {
            m_maximum = maximum;
            m_step = 100/maximum;
        }
    });
}

DataBuilder::~DataBuilder()
{
    makeWordsWatcher->cancel();
    makeWordsWatcher->waitForFinished();
}

void DataBuilder::multiThreadedWordCount(const QString &fileName)
{
    QFuture<QString> future = QtConcurrent::run(this, &DataBuilder::readFromFile, fileName);
    QStringList lines = future.result().split("\n");
    processLines(lines);
}

void DataBuilder::reduce(WordCount &result, const WordCount &w)
{
    QMapIterator<QString, int> i(w);
    while (i.hasNext())
    {
        i.next();
        result[i.key()] += i.value();
    }
}

WordCount DataBuilder::countWords(const QString &line)
{
    WordCount wordCount;

    auto words =  line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
    for (QString &word : words)
        wordCount[word.remove(",").remove(".").remove(":").remove("!").remove("?")] += 1;

    return wordCount;
}

QString DataBuilder::readFromFile(const QString &fileName)
{
    QString text;
    QFile file(QString(fileName).remove("file://"));
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        text = in.readAll();
        file.close();
    }
    return text;
}

void DataBuilder::finished()
{
    QtConcurrent::run([this] ()
    {
        WordCount total = makeWordsWatcher->result();
        QVector<QPair<QString, int> > sortedByFrequencyVector;
        for(auto word : total.keys())
            sortedByFrequencyVector.push_back(QPair<QString, int> (word, total.value(word)));

        std::sort(sortedByFrequencyVector.begin(), sortedByFrequencyVector.end(), [](QPair<QString, int> l, QPair<QString, int> r)
        {
            return l.second < r.second;
        });

        QVector<QPair<QString, int> > sortedByWordsVector(sortedByFrequencyVector.mid(sortedByFrequencyVector.size()-topWordCount));

        std::sort(sortedByWordsVector.begin(), sortedByWordsVector.end(), [](QPair<QString, int> l, QPair<QString, int> r)
        {
            return l.first.toLower() < r.first.toLower();
        });

        emit updateData(sortedByWordsVector, sortedByFrequencyVector.last().second);
    });
}

void DataBuilder::processLines(QStringList lines)
{
    if (makeWordsWatcher->isRunning())
    {
        makeWordsWatcher->cancel();
        makeWordsWatcher->waitForFinished();
    }

    makeWordsWatcher->setFuture( QtConcurrent::mappedReduced(lines, &DataBuilder::countWords, &DataBuilder::reduce) );
}
