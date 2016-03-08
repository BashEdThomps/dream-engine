/* 
 * Property of Octronic, 2014
 * http://octronic.org
 * All Rights Reserved
 */
package org.octronic.graphicsengine.animation;

import java.util.ArrayList;
import java.util.List;
import org.octronic.graphicsengine.scene.Scene;

/**
 *
 * @author ashley
 */
public class KeyFrame implements Comparable
{
    private int mIndex;
    private List<FrameDelta> mDeltas;
    private long mDuration;
    private Scene mParent;
    private boolean mWrap;
    
    public KeyFrame(Scene parent, int index, long duration)
    {
        mIndex = index;
        mDuration = duration;
        mParent = parent;
        mDeltas = new ArrayList<>();
    }
    
    public void addDelta(FrameDelta d)
    {
        mDeltas.add(d);
    }
    
    public List<FrameDelta> getDeltas()
    {
        return mDeltas;
    }
    
    public void setDuration(long duration)
    {
        mDuration = duration;
    }
    
    public long getDuration()
    {
        return mDuration;
    }
    
    public int getIntermediates()
    {
        return (int)( (((float)mDuration/1000))*mParent.getFPS());
    }
    
    public int getIndex()
    {
        return mIndex;
    }
    
    @Override
    public int compareTo(Object t)
    {
        int otherIndex = ((KeyFrame)t).getIndex();
        return new Integer(mIndex).compareTo(otherIndex);
    }

    public FrameDelta getDeltaByDrawableID(int drawableID)
    {
        for (FrameDelta d : mDeltas)
        {
           if (d.getDrawableID() == drawableID) return d; 
        }
        return null;
    }

    public boolean isWrap()
    {
        return mWrap;
    }
    
    public void setWrap(boolean b)
    {
        mWrap = b;
    }
}
