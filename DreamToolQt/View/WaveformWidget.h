#pragma once

#include <QWidget>
#include <vector>
#include <QColor>
#include <DreamCore.h>
#include "../Model/AudioMarkersTableModel.h"

using namespace std;

class SamplePair
{
public:
    SamplePair(float min, float max) : min(min), max(max) {}
    float min;
    float max;
};

using Dream::AudioEventMarker;

class WWMarker
{
public:
    // 0Args Const
    WWMarker(int idx) : index(idx)
    {}

    // Copy Const
    WWMarker(const WWMarker& other)
      : index(other.index),
        point(other.point),
        rect(other.rect)
    {}

    bool operator==(const WWMarker& other)
    {
        return other.index == this->index;
    }

    int index;
    QPoint point;
    QRect rect;
};

class WaveformWidget : public QWidget
{
    Q_OBJECT

    enum EditMode
    {
        EDIT_MODE_NONE,
        EDIT_MODE_MOVE_MARKER,
        EDIT_MODE_MOVE_REPEATER
    };

    void removeMarker(int marker);
public:
    explicit WaveformWidget(QWidget *parent = nullptr);

    int markerClashes(WWMarker& mkr);
    int markerAtLocation(QPoint p);
    void setData(int channels, int frequency,vector<char>&);
    void setModel(AudioMarkersTableModel* tableModel);
    AudioMarkersTableModel* getTableModel() const;
    void setTableModel(AudioMarkersTableModel* tableModel);
    int getCurrentSamplePos() const;
    void setCurrentSamplePos(int currentSamplePos);

    int getSelectedMarker() const;
    void setSelectedMarker(int selectedMarker);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

    SamplePair takeSample(float* fromHere, int amount);
    float scaleSample(short sample);
    int sampleToWidgetX(int sampleNum);
    int widgetXToSample(int x);

    void drawZeroCrossingLine(QPainter& painter);
    void drawCursor(QPainter& painter);
    void drawMarkers(QPainter& painter);
    void drawBackground(QPainter& painter);
    void drawWaveform(QPainter& painter);
    void drawTimes(QPainter& painter);
    void drawPlaybackPosition(QPainter& painter);
    void drawRepeaters(QPainter& painter);


public slots:
    void onSampleOffsetChanged(int);

signals:
    void notifyScrubToSampleChanged(int);
    void notifyMarkerSelectionChanged(int);

protected:
    QPoint mMousePos;
    vector<float> mData;
    int mFrequency;
    int mChannels;
    vector<WWMarker> mMarkers;
    int  mSelectedMarker;
    QRect mRepeaterFlag;
    EditMode mEditMode;

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
    QColor mPlaybackPositionColour;
    QColor mRepeatColour;

    float mZoomSpeed;
    float mZoomSpeedMin;
    float mZoomSpeedMax;
    float mZoomSpeedScale;

    QFont mFont;
    QFontMetrics mFontMetrics;
    int mPadding;
    AudioMarkersTableModel* mTableModel;
    int mCurrentSamplePos;
    bool mouseOverRepeaterFlag();
};
