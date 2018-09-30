#pragma once

#ifdef __APPLE__
    #include <OpenAL/al.h>
    #include <OpenAL/alc.h>
#else
    #include <AL/al.h>
    #include <AL/alc.h>
#endif

#include <QObject>
#include <QFile>
#include <QAudioFormat>
#include <vector>

using std::vector;

class WavFile : public QFile
{
public:
    WavFile(QObject *parent = nullptr);

    using QFile::open;
    bool open(const QString &fileName);
    const QAudioFormat &fileFormat() const;
    qint64 headerLength() const;

    bool readAudioData();
    bool readHeader();
    char* getAudioData();
    ALsizei getAudioDataSize();
    ALint getFrequency();
    ALenum getFormat();

private:
    QAudioFormat mFileFormat;
    qint64 mHeaderLength;
    vector<char> mAudioBuffer;
};
