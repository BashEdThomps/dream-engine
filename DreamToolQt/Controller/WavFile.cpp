#include <qendian.h>
#include <QVector>
#include <QDebug>
#include "WavFile.h"
#include <DreamCore.h>

using Dream::Constants;

struct chunk
{
    char        id[4];
    quint32     size;
};

struct RIFFHeader
{
    chunk       descriptor;     // "RIFF"
    char        type[4];        // "WAVE"
};

struct WAVEHeader
{
    chunk       descriptor;
    quint16     audioFormat;
    quint16     numChannels;
    quint32     sampleRate;
    quint32     byteRate;
    quint16     blockAlign;
    quint16     bitsPerSample;
};

struct DATAHeader
{
    chunk       descriptor;
};

struct CombinedHeader
{
    RIFFHeader  riff;
    WAVEHeader  wave;
};

WavFile::WavFile(QObject *parent)
    : QFile(parent)
    , mHeaderLength(0)
{

}

bool WavFile::open(const QString &fileName)
{
    qDebug() << "WF: Opening" << fileName;
    close();
    setFileName(fileName);
    return QFile::open(QIODevice::ReadOnly) && readHeader() && readAudioData();
}

const QAudioFormat &WavFile::fileFormat() const
{
    return mFileFormat;
}

qint64 WavFile::headerLength() const
{
    return mHeaderLength;
}

bool WavFile::readHeader()
{
    seek(0);
    CombinedHeader header;
    bool result = read(reinterpret_cast<char *>(&header), sizeof(CombinedHeader)) == sizeof(CombinedHeader);
    if (result)
    {
        if ((memcmp(&header.riff.descriptor.id, "RIFF", 4) == 0
             || memcmp(&header.riff.descriptor.id, "RIFX", 4) == 0)
            && memcmp(&header.riff.type, "WAVE", 4) == 0
            && memcmp(&header.wave.descriptor.id, "fmt ", 4) == 0
            && (header.wave.audioFormat == 1 || header.wave.audioFormat == 0))
        {

            // Read off remaining header information
            DATAHeader dataHeader;

            if (qFromLittleEndian<quint32>(header.wave.descriptor.size) > sizeof(WAVEHeader))
            {
                // Extended data available
                quint16 extraFormatBytes;
                if (peek((char*)&extraFormatBytes, sizeof(quint16)) != sizeof(quint16))
                {
                    return false;
                }
                const qint64 throwAwayBytes = sizeof(quint16) + qFromLittleEndian<quint16>(extraFormatBytes);
                if (read(throwAwayBytes).size() != throwAwayBytes)
                {
                    return false;
                }
            }

            if (read((char*)&dataHeader, sizeof(DATAHeader)) != sizeof(DATAHeader))
            {
                return false;
            }

            // Establish format
            if (memcmp(&header.riff.descriptor.id, "RIFF", 4) == 0)
            {
                mFileFormat.setByteOrder(QAudioFormat::LittleEndian);
            }
            else
            {
                mFileFormat.setByteOrder(QAudioFormat::BigEndian);
            }

            int bps = qFromLittleEndian<quint16>(header.wave.bitsPerSample);
            mFileFormat.setChannelCount(qFromLittleEndian<quint16>(header.wave.numChannels));
            mFileFormat.setCodec("audio/pcm");
            mFileFormat.setSampleRate(qFromLittleEndian<quint32>(header.wave.sampleRate));
            mFileFormat.setSampleSize(qFromLittleEndian<quint16>(header.wave.bitsPerSample));
            mFileFormat.setSampleType(bps == 8 ? QAudioFormat::UnSignedInt : QAudioFormat::SignedInt);
        }
        else
        {
            result = false;
        }
    }
    mHeaderLength = pos();
    return result;
}

char*
WavFile::getAudioData
()
{
    return &mAudioBuffer[0];
}

ALsizei
WavFile::getAudioDataSize
()
{
    qDebug() << "WF: Audio is " << mAudioBuffer.size() << " in size";
    return static_cast<ALsizei>(mAudioBuffer.size());
}

ALint
WavFile::getFrequency
()
{
    qDebug() << "WF: Freq/SampleRate: " << mFileFormat.sampleRate();
    return mFileFormat.sampleRate();
}

ALenum
WavFile::getFormat
()
{
    switch (mFileFormat.channelCount())
    {
        case 1:
            switch (mFileFormat.sampleSize())
            {
                case 8:
                    qDebug() << "WF: Format M8";
                    return AL_FORMAT_MONO8;
                case 16:
                    qDebug() << "WF: Format M16";
                    return AL_FORMAT_MONO16;
            }
        case 2:
            switch (mFileFormat.sampleSize())
            {
                case 8:
                    qDebug() << "WF: Format S8";
                    return AL_FORMAT_STEREO8;
                case 16:
                    qDebug() << "WF: Format S16";
                    return AL_FORMAT_STEREO16;
            }
    }
    qDebug() << "WF: IDK???? Format S16";
    return AL_FORMAT_STEREO16;
}

bool
WavFile::readAudioData
()
{
    if (mHeaderLength == 0)
    {
        return false;
    }

    seek(mHeaderLength);
    qint64 bytesRead = 0;
    char* buffer = new char[Constants::AUDIO_BUFFER_SIZE];
    while ((bytesRead = read(buffer, static_cast<qint64>(Constants::AUDIO_BUFFER_SIZE))) > 0)
    {
        mAudioBuffer.insert(mAudioBuffer.end(), buffer, buffer + bytesRead);
    }
    return mAudioBuffer.size() > 0;
}
