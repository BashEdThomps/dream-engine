/* 
 * Property of Octronic, 2014
 * http://octronic.org
 * All Rights Reserved
 */
package org.octronic.graphicsengine.animation;

import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;
import org.octronic.graphicsengine.scene.SceneDrawable;
import org.octronic.graphicsengine.scene.SceneObject;
import org.octronic.utilities.tracer.Tracer;

/**
 *
 * @author ashley
 */
public class KeyFrameAnimation
{

    private static final Tracer _Tracer = new Tracer(KeyFrameAnimation.class);
    private final List<KeyFrame> mKeyFrames;
    private final List<Frame> mFrames;
    private final List<SceneDrawable> mDrawables;
    private int mCurrentFrame;
    private boolean mDone;

    public KeyFrameAnimation()
    {
        mKeyFrames = new LinkedList<>();
        mFrames = new LinkedList<>();
        mDrawables = new ArrayList<>();
        mCurrentFrame = 0;
    }

    public SceneDrawable getDrawableByID(int id)
    {
        for (SceneDrawable sd : mDrawables)
        {
            if (sd.getID() == id) return sd;
        }
        return null;
    }
    
    public void generateFrames()
    {
        int currentKeyFrame = 0;
        int currentFrame = 0;

        while (true)
        {
            KeyFrame source = getKeyFrame(currentKeyFrame);
            KeyFrame target = getKeyFrame(currentKeyFrame + 1);

            if (source == null)
            {
                _Tracer.debug("Finished generating frames");
                break;
            }

            if (target == null)
            {
                if (source.isWrap())
                {
                    target = getKeyFrame(0);
                } else
                {
                    _Tracer.debug("Finished generating frames");
                    break;
                }
            }

            _Tracer.info("Generating frames for " + source.getIndex() + " >> " + (target.getIndex()));

            int intermediates = source.getIntermediates();
            _Tracer.info("\t with " + intermediates + " intermediate");
            for (int i = 0; i < intermediates; i++)
            {
                Frame f = new Frame(currentFrame);

                for (FrameDelta d : source.getDeltas())
                {
                    //_Tracer.info("Creatng delta for " + d.getDrawableID());
                    FrameDelta dest = target.getDeltaByDrawableID(d.getDrawableID());
                    FrameDelta moveBy = d.getMotionDelta(d, dest, intermediates, i);
                    //moveBy.show();
                    f.addMotionDelta(moveBy);
                }

                mFrames.add(f);
                currentFrame++;
            }
            // Move on to the next KeyFrame
            currentKeyFrame++;
        }
    }

    public boolean isDone()
    {
        return mDone;
    }

    public KeyFrame getKeyFrame(int index)
    {
        for (KeyFrame kf : mKeyFrames)
        {
            if (kf.getIndex() == index)
            {
                //_Tracer.debug("Returning KeyFrame " + index);
                return kf;
            }
        }
        //_Tracer.error("KeyFrame " + index + " was not found!");
        return null;
    }

    public void addFrame(Frame f)
    {
        mFrames.add(f);
    }

    public Frame getFrame(int index)
    {
        for (Frame f : mFrames)
        {
            if (f.getIndex() == index)
            {
                return f;
            }
        }
        return null;
    }

    public void addKeyFrame(KeyFrame kf)
    {
        mKeyFrames.add(kf);
    }

    public void addDrawable(SceneDrawable sd)
    {
        mDrawables.add(sd);
    }

    public void removeDrawable(int dID)
    {
        mDrawables.remove(dID);
    }

    public void drawAnimated()
    {
        for (SceneDrawable sd : mDrawables)
        {
            sd.draw();
        }
    }

    public void setCurrentFrame(int frame)
    {
        mCurrentFrame = 0;
    }

    public void nextFrame()
    {
        // We're done
        if (mDone) return;
        
        //_Tracer.info("Applying next Frame: " + mCurrentFrame);
        Frame currentFrame = getFrame(mCurrentFrame);
        if (currentFrame == null)
        {
            if (getKeyFrame(mKeyFrames.size() - 1).isWrap())
            {
                mCurrentFrame = 0;
            } else
            {
                mDone = true;
                return;
            }
            currentFrame = getFrame(mCurrentFrame);
            //return;
        }
        for (FrameDelta d : currentFrame.getMotionDeltas())
        {
            SceneObject sd = getDrawableByID(d.getDrawableID());
            sd.applyDelta(d);
        }
        mCurrentFrame++;
    }
}
