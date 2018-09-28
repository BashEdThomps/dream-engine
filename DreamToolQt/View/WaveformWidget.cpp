#include "WaveformWidget.h"
#include <QPaintEvent>
#include <QResizeEvent>
#include <QPainter>
#include <cmath>
#include <iostream>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <limits>

using namespace std;

WaveformWidget::WaveformWidget(QWidget *parent) :

    QWidget(parent),
    mFrequency(-1),
    mChannels(-1),
    mSelectedMarker(nullptr),
    mMouseButton1Pressed(false),
    mAreaStart(0),
    mAreaEnd(0),
    mZoomSpeed(5.0f),

    mWaveformColour(QColor(61, 174, 233)),
    mMarkerColour(QColor(255,165,0)),
    mSelectedMarkerColour(QColor(255,255,0)),
    mCursorColour(QColor(255,255,255)),
    mBackgroundColour(QColor(35,38,41)),
    mLineColour(QColor(192,192,192)),
    mTextColour(QColor(0,0,0)),
    mFont(QFont("Sans",12)),
    mFontMetrics(QFontMetrics(mFont)),
    mPadding(2)
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
        drawMarkers(painter);
    }

    drawCursor(painter);
    drawZeroCrossingLine(painter);
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

       painter.drawLine(
           i, static_cast<int>(halfHeight+minBarHeight),
           i, static_cast<int>(halfHeight+maxBarHeight)
        );

       dataPtr+=step;
    }
}

void
WaveformWidget::drawMarkers
(QPainter& painter)
{
    for (auto marker : mMarkers)
    {
        //auto p = marker->getPoint();
        auto p = QPoint(sampleToWidgetX(marker->getSampleIndex()),0);

        if (p.x() == -1)
        {
            continue;
        }

         auto text = QString("%1: %2")
            .arg(marker->getIndex())
            .arg(marker->getName());

        auto textBR =  mFontMetrics.boundingRect(text);
        auto textWidth = textBR.width()+(mPadding*2);
        auto textHeight = textBR.height()+mPadding;

        auto x = p.x() - textWidth/2;

        // Bounds Check
        x = (x < 0 ? 0 : x);
        x = (x > width()-textWidth ? width()-textWidth : x);

        marker->setRect(QRect(x, 0,textWidth, textHeight));

        while (markerClashes(marker) != nullptr)
        {
            auto newR = marker->getRect();
            newR.setTop(marker->getRect().top() + textHeight+mPadding);
            newR.setBottom(marker->getRect().bottom() + textHeight+mPadding);
            marker->setRect(newR);
        }

        QColor currentColour =  (
            marker.get() == mSelectedMarker ?
            mSelectedMarkerColour :
            mMarkerColour
        );

        painter.fillRect(marker->getRect(), currentColour);

        painter.setPen(currentColour);
        painter.drawLine(p.x(),0,p.x(),height());

        painter.setPen(mTextColour);
        auto markerRect = marker->getRect();
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
WaveformWidget::mouseMoveEvent
(QMouseEvent* event)
{
    mMousePos = event->pos();
    if (mSelectedMarker != nullptr && mMouseButton1Pressed)
    {
        mSelectedMarker->setSampleIndex(widgetXToSample(mMousePos.x()));
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
    }
}

void
WaveformWidget::mousePressEvent
(QMouseEvent* event)
{
    WWMarker* markerPtr = markerAtLocation(event->pos());
    mSelectedMarker = markerPtr;
    if (event->button() == Qt::MouseButton::LeftButton)
    {
        mMouseButton1Pressed = true;
    }
    update();
}

void
WaveformWidget::mouseDoubleClickEvent
(QMouseEvent* event)
{
    WWMarker* markerPtr = markerAtLocation(event->pos());

    if (markerPtr == nullptr)
    {
        shared_ptr<WWMarker> marker = make_shared<WWMarker>();
        marker->setPoint(event->pos());
        marker->setSampleIndex(widgetXToSample(event->pos().x()));
        marker->setIndex(mMarkers.size()+1);
        mMarkers.push_back(marker);
    }
    else
    {
        mSelectedMarker = markerPtr;
    }
    update();
}

WWMarker*
WaveformWidget::markerAtLocation
(QPoint p)
{
    for (shared_ptr<WWMarker> nextMarker: mMarkers)
    {
        if (nextMarker->getRect().contains(p))
        {
            return nextMarker.get();
        }
    }
    return nullptr;
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

WWMarker* WaveformWidget::markerClashes(shared_ptr<WWMarker> mkr)
{
    QRect thisRect = mkr->getRect();
    for (shared_ptr<WWMarker> nextMarker: mMarkers)
    {
        if (mkr == nextMarker)
        {
           continue;
        }

        if (thisRect.intersects(nextMarker->getRect()))
        {
            return nextMarker.get();
        }
    }
    return nullptr;

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

size_t
WaveformWidget::widgetXToSample
(int x)
{
    if (x < 0)       return static_cast<size_t>(mAreaStart);
    if (x > width()) return static_cast<size_t>(mAreaEnd);

    auto log = spdlog::get("WaveformWidget");
    float range = mAreaEnd-mAreaStart;
    float percent = static_cast<float>(x)/static_cast<float>(width());
    float sampleOffset = range * percent;
    size_t retval = mAreaStart + trunc(sampleOffset);
    log->critical("widget X {} to sample {}",x,retval);
    return retval;
}

int
WaveformWidget::sampleToWidgetX
(size_t sampleNum)
{
    auto log = spdlog::get("WaveformWidget");
    if (sampleNum<mAreaStart || sampleNum>mAreaEnd)
    {
        return -1;
    }
    log->critical("Sample is in range");

    int sRange = mAreaEnd-mAreaStart;
    float samplePercent = static_cast<float>(sampleNum-mAreaStart)/sRange;
    int wRange = width();

    int retval = static_cast<int>(wRange*samplePercent);
    log->critical("sample {} to widget X {}",sampleNum,retval);
    return retval;
}
