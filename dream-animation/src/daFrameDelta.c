/* 
 * Property of Octronic, 2014
 * http://octronic.org
 * All Rights Reserved
 */
package org.octronic.graphicsengine.animation;

import org.lwjgl.util.vector.Vector3f;
import org.octronic.graphicsengine.scene.SceneObject;
import org.octronic.utilities.tracer.Tracer;

/**
 *
 * @author ashley
 */
public class FrameDelta
{

    private static final Tracer _Tracer = new Tracer(FrameDelta.class);
    private final int mDrawableID;
    private Vector3f mPositionDelta;
    private Vector3f mRotationDelta;
    private Vector3f mOrbitOffset;
    private float mScaleDelta;
    private DeltaOperation mOperation;

    public FrameDelta(int drawableId, DeltaOperation operation)
    {
        mOperation     = operation;
        mDrawableID    = drawableId;
        mPositionDelta = new Vector3f(0,0,0);
        mRotationDelta = new Vector3f(0,0,0);
        mOrbitOffset   = new Vector3f(0,0,0);
    }

    public FrameDelta(SceneObject drawable, DeltaOperation operation)
    {
        mOperation = operation;
        mDrawableID = drawable.getID();
        mPositionDelta = drawable.getPosition();
        mRotationDelta = drawable.getRotation();
    }
    
    public void setOrbitOffset(Vector3f orbitOffset)
    {
        mOrbitOffset = orbitOffset;
    }
    
    public FrameDelta getMotionDelta(FrameDelta start, FrameDelta end, int steps, int stepIndex)
    {
        FrameDelta d = new FrameDelta(start.getDrawableID(), start.getOperation());
        Vector3f posDelta = getVectorDelta(start.getPositioinDelta(), end.getPositioinDelta());
        Vector3f rotDelta = getVectorDelta(start.getRotationDelta(), end.getRotationDelta());

        switch (d.getOperation())
        {
            default:
            case LINEAR:
                posDelta.x /= steps;
                posDelta.y /= steps;
                posDelta.z /= steps;
                d.setPositionDelta(posDelta);
                break;
            case ORBIT:
                break;
            case BEZIER:
                break;
        }
        
        rotDelta.x /= steps;
        rotDelta.y /= steps;
        rotDelta.z /= steps;
        d.setRotationDelta(rotDelta);
        
        return d;
    }

    public void setOperation(DeltaOperation operation)
    {
        mOperation = operation;
    }

    public DeltaOperation getOperation()
    {
        return mOperation;
    }

    public Vector3f getVectorDelta(Vector3f a, Vector3f b)
    {
//        float x = Math.max(a.x,b.x) - Math.min(a.x,b.x);
//        float y = Math.max(a.y,b.y) - Math.min(a.y,b.y);
//        float z = Math.max(a.z,b.z) - Math.min(a.z,b.z);

        float x = a.x - b.x;
        float y = a.y - b.y;
        float z = a.z - b.z;

        return new Vector3f(x, y, z);
    }

    public void setScaleDelta(float sd)
    {
        mScaleDelta = sd;
    }

    public float getScaleDelta()
    {
        return mScaleDelta;
    }

    public Vector3f getPositioinDelta()
    {
        return mPositionDelta;
    }

    public void setPositionDelta(Vector3f delta)
    {
        mPositionDelta = delta;
    }

    public Vector3f getRotationDelta()
    {
        return mRotationDelta;
    }

    public void setRotationDelta(Vector3f delta)
    {
        mRotationDelta = delta;
    }

    public int getDrawableID()
    {
        return mDrawableID;
    }

    public void show()
    {
        _Tracer.info("Delta for ID: " + mDrawableID);
        _Tracer.info(String.format("\tPOS: X:%f Y:%f Z:%f", mPositionDelta.x, mPositionDelta.y, mPositionDelta.z));
        _Tracer.info(String.format("\tROT: X:%f Y:%f Z:%f", mRotationDelta.x, mRotationDelta.y, mRotationDelta.z));
    }
}
