#include "WaveformWidget.h"
#include <QPaintEvent>
#include <QResizeEvent>
#include <QPainter>
#include <cmath>
#include <iostream>
#include <limits>
#include <QDebug>

using namespace std;

WaveformWidget::WaveformWidget(QWidget *parent)
    : QWidget(parent),
    mFrequency(-1),
    mChannels(-1),
    mSelectedMarker(-1),
    mEditMode(EDIT_MODE_NONE),
    mMouseButton1Pressed(false),
    mAreaStart(0),
    mAreaEnd(0),

    mWaveformColour(QColor(61, 174, 233)),
    mMarkerColour(QColor(255,165,0)),
    mSelectedMarkerColour(QColor(255,255,0)),
    mCursorColour(QColor(255,255,255)),
    mBackgroundColour(QColor(35,38,41)),
    mLineColour(QColor(192,192,192)),
    mTextColour(QColor(0,0,0)),
    mPlaybackPositionColour(QColor(255,0,0)),
    mRepeatColour(QColor(200,200,200)),

    mZoomSpeed(10.0f),
    mZoomSpeedMin(10.0f),
    mZoomSpeedMax(30.0f),
    mZoomSpeedScale(0.5f),

    mFont(QFont("Sans",12)),
    mFontMetrics(QFontMetrics(mFont)),
    mPadding(2),
    mTableModel(nullptr),
    mCurrentSamplePos(0)
{
    auto log = spdlog::get("WaveformWidget");
    if (log == nullptr)
    {
        auto log = spdlog::stdout_color_mt("WaveformWidget");
    }
    setMinimumHeight(50);
}

SamplePair
WaveformWidget::takeSample
(float* fromHere, int amount)
{
   auto log = spdlog::get("WaveformWidget");
   float min = 0;
   float max = 0;
   for (int i=0; i<amount; i++)
   {
       if (fromHere[i] > max) max = fromHere[i];
       if (fromHere[i] < min) min = fromHere[i];
   }
   return SamplePair(min,max);
}

void
WaveformWidget::paintEvent
(QPaintEvent* event)
{
    auto log = spdlog::get("WaveformWidget");
    QPainter painter(this);

    if (!mData.empty())
    {
        drawWaveform(painter);
        drawZeroCrossingLine(painter);
        drawMarkers(painter);
        drawRepeaters(painter);
    }

    drawPlaybackPosition(painter);
    drawCursor(painter);
    drawTimes(painter);
}

void
WaveformWidget::drawTimes
(QPainter& painter)
{
    auto freqF = static_cast<double>(mFrequency);
    auto startTime = (mAreaStart/freqF)/mChannels;
    auto startTimeSec = QString();
    startTimeSec.sprintf("%02.3f", fmod(startTime,60.0));
    auto startTimeMin = floor(startTime/60.0);
    auto startTimeStr = QString("%1:%2").arg(startTimeMin).arg(startTimeSec);

    auto endTime = (mAreaEnd/freqF)/mChannels;
    auto endTimeSec = QString();
    endTimeSec.sprintf("%02.3f", fmod(endTime,60.0));
    auto endTimeMin = floor(endTime/60.0);
    auto endTimeStr = QString("%1:%2").arg(endTimeMin).arg(endTimeSec);

    painter.setPen(Qt::white);
    painter.setFont(mFont);
    painter.drawText(0,height(),startTimeStr);
    auto endTimeX = width()-mFontMetrics.boundingRect(endTimeStr).width();
    painter.drawText(endTimeX,height(),endTimeStr);
}

void
WaveformWidget::drawPlaybackPosition
(QPainter& painter)
{
    painter.setPen(mPlaybackPositionColour);
    auto x = sampleToWidgetX(mCurrentSamplePos);
    painter.drawLine(x,0,x,height());

}

void
WaveformWidget::drawBackground
(QPainter& painter)
{
    painter.fillRect(rect(), mBackgroundColour);
}

void
WaveformWidget::drawWaveform
(QPainter& painter)
{
    int halfHeight = height()/2;
    int step = (mAreaEnd-mAreaStart) / width();
    //log->trace("Step {}", step);
    auto dataPtr = &mData[static_cast<size_t>(mAreaStart)];

    painter.setPen(mWaveformColour);

    for (int i=0; i<width(); i++)
    {
       auto pair = takeSample(dataPtr,step);
       float minAmplitude = pair.min;
       float maxAmplitude = pair.max;

       float minBarHeight = halfHeight * minAmplitude;
       float maxBarHeight = halfHeight * maxAmplitude;

        QList<QLine> lines;
       lines.push_back(QLine(
           i, static_cast<int>(halfHeight+minBarHeight),
           i, static_cast<int>(halfHeight+maxBarHeight)
        ));
       painter.drawLines(&lines[0],lines.count());

       dataPtr+=step;
    }
}

void
WaveformWidget::drawMarkers
(QPainter& painter)
{
    auto audioDef = mTableModel->getAudioDefinitionHandle();
    for (WWMarker& marker : mMarkers)
    {
        //auto p = marker->getPoint();
        auto p = QPoint(sampleToWidgetX(audioDef->getMarkerSampleIndex(marker.index)),0);

        if (p.x() == -1)
        {
            continue;
        }

         auto text = QString("%1: %2")
            .arg(marker.index)
            .arg(QString::fromStdString(audioDef->getMarkerName(marker.index)));

        auto textBR =  mFontMetrics.boundingRect(text);
        auto textWidth = textBR.width()+(mPadding*2);
        auto textHeight = textBR.height()+mPadding;

        auto x = p.x() - textWidth/2;

        // Bounds Check
        x = (x < 0 ? 0 : x);
        x = (x > width()-textWidth ? width()-textWidth : x);

        marker.rect = QRect(x, 0,textWidth, textHeight);

        while (markerClashes(marker) > -1)
        {
            auto newR = marker.rect;
            newR.setTop(marker.rect.top() + textHeight+mPadding);
            newR.setBottom(marker.rect.bottom() + textHeight+mPadding);
            marker.rect = newR;
        }

        QColor currentColour =  (
            marker.index == mSelectedMarker ?
            mSelectedMarkerColour :
            mMarkerColour
        );

        painter.fillRect(marker.rect, currentColour);

        painter.setPen(currentColour);
        painter.drawLine(p.x(),0,p.x(),height());

        painter.setPen(mTextColour);
        auto markerRect = marker.rect;
        painter.setFont(mFont);
        painter.drawText(
            markerRect.x()+mPadding,
            markerRect.y() + markerRect.height()-mPadding,
            text
        );
    }
}

void
WaveformWidget::drawCursor
(QPainter& painter)
{
    painter.setPen(mCursorColour);
    painter.drawLine(mMousePos.x(),0,mMousePos.x(),height());
}

void
WaveformWidget::drawZeroCrossingLine
(QPainter& painter)
{
    int halfHeight = height() / 2;
    painter.setPen(mLineColour);
    painter.drawLine(0,halfHeight,width(),halfHeight);
}

void
WaveformWidget::drawRepeaters
(QPainter& painter)
{
   if (mSelectedMarker < 0) return;

   auto ad = mTableModel->getAudioDefinitionHandle();
   auto start = ad->getMarkerSampleIndex(mSelectedMarker);
   auto numRepeats = ad->getMarkerRepeat(mSelectedMarker);
   auto period = ad->getMarkerRepeatPeriod(mSelectedMarker);

   painter.setPen(mRepeatColour);
   auto bottom = height();
   for (int i=0; i<numRepeats; i++)
   {
      auto x = start + (period*(i+1));
      auto sx = sampleToWidgetX(x);

      // Off screen
      if (sx < 0 || sx > width()) continue;

      //qDebug() << "Drawind repeater x: " << x << " sx: " << sx;
      if (i == 0)
      {
        mRepeaterFlag = QRect(sx, 0, 20, 20);
        painter.fillRect(mRepeaterFlag, mRepeatColour);
      }
      painter.drawLine(sx,0,sx,bottom);
   }
}

bool
WaveformWidget::mouseOverRepeaterFlag
()
{
    return mRepeaterFlag.contains(mMousePos);
}

void
WaveformWidget::mouseMoveEvent
(QMouseEvent* event)
{
    auto log = spdlog::get("WaveformWidget");
    mMousePos = event->pos();
    if (mSelectedMarker > -1 && mMouseButton1Pressed)
    {
        auto sampleIndex = widgetXToSample(mMousePos.x());
        log->critical("Marker Drag Event, sample {}", sampleIndex);
        auto ad = mTableModel->getAudioDefinitionHandle();

        switch (mEditMode)
        {
            case WaveformWidget::EDIT_MODE_NONE:
                break;
            case WaveformWidget::EDIT_MODE_MOVE_MARKER:
                ad->setMarkerSampleIndex(mSelectedMarker, sampleIndex);
                break;
            case WaveformWidget::EDIT_MODE_MOVE_REPEATER:
                auto markerSample = ad->getMarkerSampleIndex(mSelectedMarker);
                auto offset = sampleIndex - markerSample;
                ad->setMarkerRepeatPeriod(mSelectedMarker, offset);

                break;

        }

        mTableModel->update();
    }
    else if (mMouseButton1Pressed)
    {
        auto sampleIndex = widgetXToSample(mMousePos.x());
        emit notifyScrubToSampleChanged(sampleIndex);
    }
    update();
}

void
WaveformWidget::mouseReleaseEvent
(QMouseEvent* event)
{
    if (event->button() == Qt::MouseButton::LeftButton)
    {
        mMouseButton1Pressed = false;
        mEditMode = EDIT_MODE_NONE;
    }
}

void
WaveformWidget::mousePressEvent
(QMouseEvent* event)
{
    auto log = spdlog::get("WaveformWidget");
    auto pos = event->pos();
    log->critical("Mouse Press Event {},{}",pos.x(),pos.y());

    if (event->button() == Qt::MouseButton::LeftButton)
    {
        mMouseButton1Pressed = true;
        if (mSelectedMarker > -1 && mouseOverRepeaterFlag())
        {
            mEditMode = EDIT_MODE_MOVE_REPEATER;
        }
        else
        {
            auto lastMarker = mSelectedMarker;
            mSelectedMarker = markerAtLocation(pos);
            if (lastMarker != mSelectedMarker)
            {
               emit notifyMarkerSelectionChanged(mSelectedMarker);
            }
            mEditMode = EDIT_MODE_MOVE_MARKER;
        }
    }
    update();
}

void
WaveformWidget::mouseDoubleClickEvent
(QMouseEvent* event)
{
    int markerIndex = markerAtLocation(event->pos());

    if (markerIndex < 0)
    {
        qDebug() << "WW: Creating new marker";
        auto ad = mTableModel->getAudioDefinitionHandle();
        auto newID = ad->createMarker();
        WWMarker newMkr(newID);
        newMkr.point = event->pos();
        ad->setMarkerName(newID, "New Marker");
        ad->setMarkerSampleIndex(newID, widgetXToSample(event->pos().x()));
        mMarkers.push_back(newMkr);
        mTableModel->insertRows(newID-1,1,QModelIndex());
        mTableModel->update();


    }
    else
    {
        if (mSelectedMarker == markerIndex)
        {
            removeMarker(mSelectedMarker);
        }
    }
    update();
}

void
WaveformWidget::removeMarker
(int marker)
{
   auto itr = find_if(mMarkers.begin(),mMarkers.end(), [&](const WWMarker& m)
   {
      return m.index == marker;
   });

   if (itr != mMarkers.end())
   {
        mTableModel->getAudioDefinitionHandle()->removeMarker(marker);
        mMarkers.erase(itr);
        mSelectedMarker = -1;
        mTableModel->update();
        update();
   }
}

int
WaveformWidget::markerAtLocation
(QPoint p)
{
    auto log = spdlog::get("WaveformWidget");
    log->critical("Getting marker");
    for (WWMarker& nextMarker: mMarkers)
    {
        auto rect = nextMarker.rect;
        log->critical(
            "Testing marker {} with rect {},{} {},{}",
            nextMarker.index,
            rect.left(),  rect.top(),
            rect.right(), rect.bottom()
        );
        if (rect.contains(p))
        {
            log->critical("This one matches");
            return nextMarker.index;
        }
    }
    return -1;
}

float
WaveformWidget::scaleSample
(short sample)
{
    return (1.0f/SHRT_MAX)*sample;
}

void
WaveformWidget::setData
(int channels, int frequency,vector<char>& data)
{
    auto log = spdlog::get("WaveformWidget");
    mChannels = channels;
    mFrequency = frequency;
    if (mChannels == 1)
    {
        // 1 short for every 2 chars
        mData.resize(data.size()/2);
    }
    else if (mChannels == 2)
    {
        // 1 short for every 2 chars, 1st channel only
        mData.resize(data.size()/4);
    }

   auto dataPtr = reinterpret_cast<short*>(&data[0]);

   for (size_t i=0;i<mData.size();i++)
   {
       mData[i] = static_cast<float>(scaleSample(*dataPtr));
       dataPtr++;
       // Skip second channel
       if (mChannels == 2)
       {
           dataPtr++;
       }
   }
   mAreaEnd = static_cast<int>(mData.size());
}

int
WaveformWidget::markerClashes
(WWMarker& mkr)
{
    auto ad = mTableModel->getAudioDefinitionHandle();
    QRect thisRect = mkr.rect;
    for (WWMarker& nextMarker: mMarkers)
    {
        if (mkr == nextMarker)
        {
           continue;
        }

        // Off screen
        auto sample = ad->getMarkerSampleIndex(nextMarker.index);
        if (sample < mAreaStart || sample > mAreaEnd)
        {
            continue;
        }

        if (thisRect.intersects(nextMarker.rect))
        {
            return nextMarker.index;
        }
    }
    return -1;

}

void
WaveformWidget::wheelEvent
(QWheelEvent* event)
{
    auto log = spdlog::get("WaveformWidget");
    auto delta = event->pixelDelta();

    auto dy = delta.y();
    auto dx = delta.x();
    auto absDy = abs(dy) * mZoomSpeed;
    auto absDx = abs(dx) * mZoomSpeed;

    //log->critical("dx:{} dy:{}",dx,dy);

    if (absDy > absDx)
    {
        if (dy > 0) // Zoom in
        {
            int step = (mAreaEnd-mAreaStart) / width();
            if (step > 1)
            {
               mAreaStart += absDy;
               mAreaEnd -= absDy;
            }
        }
        else if (dy < 0) // Zoom Out
        {
           mAreaStart -= absDy;
           mAreaEnd += absDy;
        }

        mZoomSpeed += dy*mZoomSpeedScale;
        if (mZoomSpeed < mZoomSpeedMin)
        {
            mZoomSpeed = mZoomSpeedMin;
        }
        else if (mZoomSpeed > mZoomSpeedMax)
        {
            mZoomSpeed = mZoomSpeedMax;
        }

        if (mAreaStart < 0) mAreaStart = 0;

        if (mAreaEnd > static_cast<int>(mData.size()))
        {
            mAreaEnd = static_cast<int>(mData.size());
        }

        if (mAreaEnd < mAreaStart)
        {
            auto tmp = mAreaEnd;
            mAreaEnd = mAreaStart;
            mAreaStart = tmp;
        }
    }
    else if (absDx > absDy)
    {
        if (dx < 0)
        {
            if (mAreaEnd < static_cast<int>(mData.size()))
            {
                mAreaStart += absDx;
                mAreaEnd += absDx;
            }
            else
            {
                mAreaEnd = static_cast<int>(mData.size());
            }
        }
        else if (dx > 0)
        {
            if (mAreaStart > 0)
            {
                mAreaStart -= absDx;
                mAreaEnd -= absDx;
            }
            else
            {
                mAreaStart = 0;
            }
        }
    }
    update();
}

int
WaveformWidget::widgetXToSample
(int x)
{
    if (x < 0)       return mAreaStart;
    if (x > width()) return mAreaEnd;

    auto log = spdlog::get("WaveformWidget");
    float range = mAreaEnd-mAreaStart;
    float percent = static_cast<float>(x)/static_cast<float>(width());
    float sampleOffset = range * percent;
    int retval = mAreaStart + static_cast<int>(trunc(sampleOffset));
    //log->critical("widget X {} to sample {}",x,retval);
    return retval;
}

void WaveformWidget::onSampleOffsetChanged(int pos)
{
   mCurrentSamplePos = pos;
   update();
}

int WaveformWidget::getSelectedMarker() const
{
    return mSelectedMarker;
}

void WaveformWidget::setSelectedMarker(int selectedMarker)
{
    mSelectedMarker = selectedMarker;
}

int WaveformWidget::getCurrentSamplePos() const
{
    return mCurrentSamplePos;
}

void WaveformWidget::setCurrentSamplePos(int currentSamplePos)
{
    mCurrentSamplePos = currentSamplePos;
}

AudioMarkersTableModel*
WaveformWidget::getTableModel
()
const
{
    return mTableModel;
}

void
WaveformWidget::setTableModel
(AudioMarkersTableModel* tableModel)
{
    mTableModel = tableModel;
    auto ad = mTableModel->getAudioDefinitionHandle();
    for (int i=0; i < ad->countMarkers(); i++)
    {
        WWMarker viewMarker(i);
        mMarkers.push_back(viewMarker);
    }
}

int
WaveformWidget::sampleToWidgetX
(int sampleNum)
{
    auto log = spdlog::get("WaveformWidget");
    if (sampleNum<mAreaStart || sampleNum>mAreaEnd)
    {
        return -1;
    }
    //log->critical("Sample is in range");

    int sRange = mAreaEnd-mAreaStart;
    float samplePercent = static_cast<float>(sampleNum-mAreaStart)/sRange;
    int wRange = width();

    int retval = static_cast<int>(wRange*samplePercent);
    //log->critical("sample {} to widget X {}",sampleNum,retval);
    return retval;
}
