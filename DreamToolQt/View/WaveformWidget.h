#pragma once

#include <QWidget>
#include <vector>
#include <QColor>

using namespace std;

class SamplePair
{
public:
    SamplePair(float min, float max) : min(min), max(max) {}
    float min;
    float max;
};

class WWMarker
{

public:

    WWMarker()
        : mIndex(0),
          mName("NewMarker"),
          mSampleIndex(0)
    {

    }

    QString getName() const
    {
        return mName;
    }

    void setName(const QString& name)
    {
        mName = name;
    }

    size_t getIndex() const
    {
        return mIndex;
    }

    void setIndex(size_t index)
    {
        mIndex = index;
    }

    QPoint& getPoint()
    {
        return mPoint;
    }

    void setPoint(const QPoint& point)
    {
        mPoint = point;
    }

    QRect& getRect()
    {
        return mRect;
    }

    void setRect(const QRect& rect)
    {
        mRect = rect;
    }

    size_t getSampleIndex() const
    {
        return mSampleIndex;
    }

    void setSampleIndex(const size_t& sampleIndex)
    {
        mSampleIndex = sampleIndex;
    }

private:
    QPoint mPoint;
    QRect mRect;
    size_t mIndex;
    QString mName;
    size_t mSampleIndex;
};

class WaveformWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WaveformWidget(QWidget *parent = nullptr);

    WWMarker* markerClashes(shared_ptr<WWMarker> mkr);
    WWMarker* markerAtLocation(QPoint p);
    void setData(int channels, int frequency,vector<char>&);


protected:
    void paintEvent(QPaintEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    SamplePair takeSample(float* fromHere, int amount);
    float scaleSample(short sample);
    void drawZeroCrossingLine(QPainter& painter);
    void drawCursor(QPainter& painter);
    void drawMarkers(QPainter& painter);
    void drawBackground(QPainter& painter);
    void drawWaveform(QPainter& painter);
    void drawTimes(QPainter& painter);
    int sampleToWidgetX(size_t sampleNum);
    size_t widgetXToSample(int x);

protected:
    QPoint mMousePos;
    vector<float> mData;
    int mFrequency;
    int mChannels;
    vector<shared_ptr<WWMarker>> mMarkers;
    WWMarker* mSelectedMarker;
    bool mMouseButton1Pressed;
    int mAreaStart;
    int mAreaEnd;

    QColor mWaveformColour;
    QColor mMarkerColour;
    QColor mSelectedMarkerColour;
    QColor mCursorColour;
    QColor mBackgroundColour;
    QColor mLineColour;
    QColor mTextColour;

    float mZoomSpeed;

    QFont mFont;
    QFontMetrics mFontMetrics;
    int mPadding;
};
