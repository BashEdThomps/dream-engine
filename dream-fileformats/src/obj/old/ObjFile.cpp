/* 
 * Property of Octronic, 2014
 * http://octronic.org
 * All Rights Reserved
 */
package org.octronic.graphicsengine.fileformats.obj;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import org.lwjgl.opengl.GL11;
import org.lwjgl.util.vector.Vector3f;
import org.octronic.graphicsengine.scene.SceneDrawable;
import org.octronic.utilities.tracer.Tracer;

/**
 *
 * @author ashley
 */
public class ObjModel extends SceneDrawable
{

    private static final Tracer mTracer = new Tracer(ObjModel.class);
    private List<String> mComments;
    private String mObjectName;
    private String mMtllib;
    private List<Vector3f> mVertecies;
    private List<Vector3f> mVertexNormals;
    private List<Face> mFaces;
    private List<Material> mMaterials;
    private String mCurrentMaterial;
    private String mPath;
    private float mSmooth = 0.0f;
    public static boolean sDebug = false;
    
    private float mXmin, mXmax;
    private float mYmin, mYmax;
    private float mZmin, mZmax;
    
    public ObjModel(String path)
    {
        this (new File(path));
    }
    public ObjModel(File objFile)
    {
        BufferedReader br = null;
        try
        {
            mComments = new ArrayList<>();
            mVertecies = new ArrayList<>();
            mVertexNormals = new ArrayList<>();
            mFaces = new ArrayList<>();
            mMaterials = new ArrayList<>();
            mCurrentMaterial = null;
            mPath = objFile.getAbsolutePath();
            br = new BufferedReader(new FileReader(objFile));
            String nextLine;
            while ((nextLine = br.readLine()) != null)
            {
                parseLine(nextLine);
            }

        } catch (FileNotFoundException ex)
        {
            mTracer.error(ex);
        } catch (IOException ex)
        {
            mTracer.error(ex);
        } finally
        {
            try
            {
                br.close();
            } catch (IOException ex)
            {
                mTracer.error(ex);
            }
        }
        determineMinMax();
    }
    
    private void determineMinMax()
    {
        mXmin = Float.MAX_VALUE;
        mYmin = Float.MAX_VALUE;
        mZmin = Float.MAX_VALUE;
        
        mXmax = Float.MIN_VALUE;
        mYmax = Float.MIN_VALUE;
        mZmax = Float.MIN_VALUE;
    }

    @Override
    public void draw()
    {
        GL11.glPushMatrix();
        translate();
        rotate();
        wireframe();
        GL11.glPolygonMode(GL11.GL_FRONT_AND_BACK, GL11.GL_LINE_SMOOTH);
        for (Face face : mFaces)
        {
            GL11.glBegin(GL11.GL_TRIANGLES);
            {
                float[] diffuse = face.getMaterial().getKd();
                GL11.glColor3f(diffuse[0], diffuse[1], diffuse[2]);
                Vector3f n1 = mVertexNormals.get(face.getNormals()[0] - 1);
                Vector3f v1 = mVertecies.get(face.getVerts()[0] - 1);
                GL11.glNormal3f(n1.x, n1.y, n1.z);
                GL11.glVertex3f(v1.x, v1.y, v1.z);

                Vector3f n2 = mVertexNormals.get(face.getNormals()[1] - 1);
                Vector3f v2 = mVertecies.get(face.getVerts()[1] - 1);
                GL11.glNormal3f(n2.x, n2.y, n2.z);
                GL11.glVertex3f(v2.x, v2.y, v2.z);

                Vector3f n3 = mVertexNormals.get(face.getNormals()[2] - 1);
                Vector3f v3 = mVertecies.get(face.getVerts()[2] - 1);
                GL11.glNormal3f(n3.x, n3.y, n3.z);
                GL11.glVertex3f(v3.x, v3.y, v3.z);
            }
            GL11.glEnd();
        }
        
        GL11.glPopMatrix();
    }

    public Material getMaterialByName(String name)
    {
        for (Material m : mMaterials)
        {
            if (m.getName().equals(name))
            {
                return m;
            }
        }
        return null;
    }

    private void parseLine(String line)
    {
        String[] tokens = line.split(" ");
        // Vertex
        if (tokens[0].equals("v"))
        {
            // Size Check
            if (tokens.length == 4)
            {
                float x = Float.valueOf(tokens[1]);
                float y = Float.valueOf(tokens[2]);
                float z = Float.valueOf(tokens[3]);
                if (sDebug)
                {
                    mTracer.info(String.format("Adding New Vertex; X:%f Y:%f Z:%f", x, y, z));
                }
                mVertecies.add(new Vector3f(x, y, z));
            } else
            {
                mTracer.error("Invalid vertex line: invalid token count!");
                return;
            }
        } // Material Library file name
        else if (tokens[0].equals("mtllib"))
        {
            if (tokens.length == 2)
            {
                mMtllib = tokens[1];

                if (sDebug)
                {
                    mTracer.info("mtlib: " + mMtllib);
                }

                parseMaterials();
            } else
            {
                mTracer.error("Invalid mtllib line: invalid token count!");
            }
        } // Vertex Normal
        else if (tokens[0].equals("vn"))
        {
            // Size Check
            if (tokens.length == 4)
            {
                float x = Float.valueOf(tokens[1]);
                float y = Float.valueOf(tokens[2]);
                float z = Float.valueOf(tokens[3]);
                if (sDebug)
                {
                    mTracer.info(String.format("Adding New Vertex Normal; X:%f Y:%f Z:%f", x, y, z));
                }
                mVertexNormals.add(new Vector3f(x, y, z));
            } else
            {
                mTracer.error("Invalid vertex line: invalid token count!");
                return;
            }
        } // Object Name
        else if (tokens[0].equals("o"))
        {
            StringBuilder name = new StringBuilder();
            for (int i = 1; i < tokens.length; i++)
            {
                name.append(tokens[i]);
                if (tokens.length > i + 1)
                {
                    name.append(" ");
                }
            }
            mObjectName = name.toString();
            if (sDebug)
            {
                mTracer.info("Setting object name: " + mObjectName);
            }
        } // Face
        else if (tokens[0].equals("f"))
        {
            if (tokens.length == 4)
            {

                // 0 = Vert | 1 = Texture | 2 = Normal
                String[] face1 = tokens[1].split("/");
                String[] face2 = tokens[2].split("/");
                String[] face3 = tokens[3].split("/");

                int[] verts = new int[]
                {
                    Integer.valueOf(face1[0]), Integer.valueOf(face2[0]), Integer.valueOf(face3[0])
                };
                int[] norms = new int[]
                {
                    Integer.valueOf(face1[2]), Integer.valueOf(face2[2]), Integer.valueOf(face3[2])
                };

                Face f = new Face(verts, norms, getMaterialByName(mCurrentMaterial));
                mFaces.add(f);
                if (sDebug)
                {
                    mTracer.info("Adding new face: " + f.toString());
                }
            } else
            {
                mTracer.error("Invalid face line: invalid token count!");
            }
        } // Use Material
        else if (tokens[0].equals("usemtl"))
        {
            if (tokens.length == 2)
            {
                mCurrentMaterial = tokens[1];
                if (sDebug)
                {
                    mTracer.info("Using material: " + mCurrentMaterial);
                }
            } else
            {
                mTracer.error("Invalid usemtl line: invalid token count!");
            }
        } // Comment
        else if (tokens[0].equals("#"))
        {
            String cmt = line.substring(1);
            mComments.add(cmt);
            if (sDebug)
            {
                mTracer.info("Adding comment line: " + cmt);
            }
        } else if (tokens[0].equals("s"))
        {
            mSmooth = Float.valueOf(tokens[1]);
            if (sDebug)
            {
                mTracer.info("Adding Smooth Shading: " + mSmooth);
            }
        } else
        {
            mTracer.error("Invalid line: " + line);
        }
    }

    private void parseMaterials()
    {
        BufferedReader br = null;
        try
        {
            File objFileDir = new File(mPath).getParentFile();
            File materialFile = new File(objFileDir.getAbsolutePath() + System.getProperty("file.separator") + mMtllib);
            if (sDebug)
            {
                mTracer.info("Loading materials from: " + materialFile.getAbsolutePath());
            }
            br = new BufferedReader(new FileReader(materialFile));
            String nextLine = null;
            Material currentMaterial = null;
            while ((nextLine = br.readLine()) != null)
            {
                if (nextLine.isEmpty() || nextLine.startsWith("#"))
                {
                    continue;
                } else if (nextLine.startsWith("newmtl"))
                {
                    // Save current if !null
                    if (currentMaterial != null)
                    {
                        if (sDebug)
                        {
                            mTracer.info("Adding material: " + currentMaterial.getName());
                        }
                        mMaterials.add(currentMaterial);
                    }
                    String name = nextLine.split(" ")[1];
                    currentMaterial = new Material();
                    currentMaterial.setName(name);
                    if (sDebug)
                    {
                        if (sDebug)
                        {
                            mTracer.info("Starting material: " + name);
                        }
                    }
                } else if (nextLine.startsWith("Ns"))
                {
                    if (currentMaterial == null)
                    {
                        mTracer.error("Material parse error!");
                        break;
                    }
                    String[] tokens = nextLine.split(" ");
                    currentMaterial.setNs(Float.valueOf(tokens[1]));
                    if (sDebug)
                    {
                        mTracer.info("Set " + currentMaterial.getName() + " Ns");
                    }

                } else if (nextLine.startsWith("Ka"))
                {
                    if (currentMaterial == null)
                    {
                        mTracer.error("Material parse error!");
                        break;
                    }
                    String[] tokens = nextLine.split(" ");
                    currentMaterial.setKa(Float.valueOf(tokens[1]), Float.valueOf(tokens[2]), Float.valueOf(tokens[3]));
                    if (sDebug)
                    {
                        mTracer.info("Set " + currentMaterial.getName() + " Ka");
                    }
                } else if (nextLine.startsWith("Kd"))
                {
                    if (currentMaterial == null)
                    {
                        mTracer.error("Material parse error!");
                        break;
                    }
                    String[] tokens = nextLine.split(" ");
                    currentMaterial.setKd(Float.valueOf(tokens[1]), Float.valueOf(tokens[2]), Float.valueOf(tokens[3]));
                    if (sDebug)
                    {
                        mTracer.info("Set " + currentMaterial.getName() + " Kd");
                    }
                } else if (nextLine.startsWith("Ks"))
                {
                    if (currentMaterial == null)
                    {
                        mTracer.error("Material parse error!");
                        break;
                    }
                    String[] tokens = nextLine.split(" ");
                    currentMaterial.setKs(Float.valueOf(tokens[1]), Float.valueOf(tokens[2]), Float.valueOf(tokens[3]));
                    if (sDebug)
                    {
                        mTracer.info("Set " + currentMaterial.getName() + " Ks");
                    }
                } else if (nextLine.startsWith("Ni"))
                {
                    if (currentMaterial == null)
                    {
                        mTracer.error("Material parse error!");
                        break;
                    }
                    String[] tokens = nextLine.split(" ");
                    currentMaterial.setNi(Float.valueOf(tokens[1]));
                    if (sDebug)
                    {
                        mTracer.info("Set " + currentMaterial.getName() + " Ni");
                    }
                } else if (nextLine.startsWith("d"))
                {
                    if (currentMaterial == null)
                    {
                        mTracer.error("Material parse error!");
                        break;
                    }
                    String[] tokens = nextLine.split(" ");
                    currentMaterial.setD(Float.valueOf(tokens[1]));
                    if (sDebug)
                    {
                        mTracer.info("Set " + currentMaterial.getName() + " d");
                    }
                } else if (nextLine.startsWith("illum"))
                {
                    if (currentMaterial == null)
                    {
                        mTracer.error("Material parse error!");
                        break;
                    }
                    String[] tokens = nextLine.split(" ");
                    currentMaterial.setIllum(Integer.valueOf(tokens[1]));
                    if (sDebug)
                    {
                        mTracer.info("Set " + currentMaterial.getName() + " illum");
                    }
                } else
                {
                    mTracer.error("Invalid line in material file!");
                }
            }
            // Add the last material @EOL
            if (sDebug)
            {
                mTracer.info("Adding material: " + currentMaterial.getName());
            }
            mMaterials.add(currentMaterial);

        } catch (FileNotFoundException ex)
        {
            mTracer.error(ex);
        } catch (IOException ex)
        {
            mTracer.error(ex);
        } finally
        {
            try
            {
                br.close();
            } catch (IOException ex)
            {
                mTracer.error(ex);
            }
        }
    }

    public boolean isSmoothShading()
    {
        return mSmooth > 0.0f;
    }
}
