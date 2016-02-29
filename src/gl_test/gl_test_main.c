/*
* gltest_main.c
*
* GLFW/glew GNU/Linux based testing for DreamSceneGraph.
*
*/

// System
#define _POSIX_C_SOURCE 201602L
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// GL & GLU
#include <GL/glut.h>    // Header File For The GLUT Library 
#include <GL/gl.h>	// Header File For The OpenGL32 Library
#include <GL/glu.h>	// Header File For The GLu32 Library
// Project
#include "../graph.h"

/* ASCII code for the escape key. */
#define ESCAPE 27

float windowWidth  = 1600.0f;
float windowHeight = 900.0f;
graph_t* graph;
int window;
struct timespec sleepTime;
node_t *n1, *n2, *n3, *n4, *n5;

float* cubeVertexBuffer(float sz) {
	int bufferSz = 3* // Dimensions
		       4* // Vertex per face
		       6; // Faces
	fprintf(stdout,"Allocating %d floats for cube VBO\n",bufferSz);
	float* buffer = (float*)malloc(sizeof(float)*bufferSz);
	/* Front face */
	buffer[0] = -sz; buffer[1]  =  sz; buffer[2]  = sz;
	buffer[3] =  sz; buffer[4]  =  sz; buffer[5]  = sz;
	buffer[6] =  sz; buffer[7]  = -sz; buffer[8]  = sz;
	buffer[9] = -sz; buffer[10] = -sz; buffer[11] = sz;
	/* Back face */
	buffer[12] = -sz; buffer[13] = -sz; buffer[14] = -sz;
	buffer[15] =  sz; buffer[16] = -sz; buffer[17] = -sz;
	buffer[18] =  sz; buffer[19] =  sz; buffer[20] = -sz;
	buffer[21] = -sz; buffer[22] =  sz; buffer[23] = -sz;
	/* Left face */
	buffer[24] = -sz; buffer[25] =  sz; buffer[26] =  sz;
	buffer[27] = -sz; buffer[28] = -sz; buffer[29] =  sz;
	buffer[30] = -sz; buffer[31] = -sz; buffer[32] = -sz;
	buffer[33] = -sz; buffer[34] =  sz; buffer[35] = -sz;
	/* Right face */
	buffer[36] = sz; buffer[37] =  sz; buffer[38] = -sz;
	buffer[39] = sz; buffer[40] = -sz; buffer[41] = -sz;
	buffer[42] = sz; buffer[43] = -sz; buffer[44] =  sz;
	buffer[45] = sz; buffer[46] =  sz; buffer[47] =  sz;
	/* Top face */
	buffer[48] =  sz; buffer[49] = sz; buffer[50] =  sz;
	buffer[51] = -sz; buffer[52] = sz; buffer[53] =  sz;
	buffer[54] = -sz; buffer[55] = sz; buffer[56] = -sz;
	buffer[57] =  sz; buffer[58] = sz; buffer[59] = -sz;
	/* Bottom face */
	buffer[60] =  sz; buffer[61] = -sz; buffer[62] = -sz;
	buffer[63] = -sz; buffer[64] = -sz; buffer[65] = -sz;
	buffer[66] = -sz; buffer[67] = -sz; buffer[68] =  sz;
	buffer[69] =  sz; buffer[70] = -sz; buffer[71] =  sz;
	return buffer;
}

graph_t* setupScenegraph() {
	graph_t* graph = dsgGraphInitWithName("GLTestGraph");

	// Create cube vertex buffer
	int cubeBuffer = dsgGraphAddVertexBuffer(graph, cubeVertexBuffer(1.5f));
	fprintf(stdout,"Cube buffer allcated to %s's VBO %d\n",graph->name,cubeBuffer);

	// Create Nodes	
	fprintf(stdout, "Creating Nodes\n");
	n1 = dsgGraphCreateNodeWithName(graph,"Node1");
	n1->vertexBufferIndex = cubeBuffer;

	n2 = dsgGraphCreateNodeWithName(graph,"Node2");
	n2->vertexBufferIndex = cubeBuffer;
	dsgNodeSetTranslation(n2,3.0f,2.0f,3.0f);

	n3 = dsgGraphCreateNodeWithName(graph,"Node3");
	n3->vertexBufferIndex = cubeBuffer;
	dsgNodeSetTranslation(n3,2.0f,1.0f,3.0f);

	n4 = dsgGraphCreateNodeWithName(graph,"Node4");
	n4->vertexBufferIndex = cubeBuffer;
	dsgNodeSetTranslation(n4,2.0f,2.0f,1.0f);

	n5 = dsgGraphCreateNodeWithName(graph,"Node5");
	n5->vertexBufferIndex = cubeBuffer;
	dsgNodeSetTranslation(n5,1.0f,2.0f,3.0f);
	
	// Establish Relationships
	fprintf(stdout, "Setting Relationships\n");
	dsgGraphSetRootNode(graph, n1);
	dsgGraphNodeSetParent(graph,n1,n2);
	dsgGraphNodeSetParent(graph,n2,n3);
	dsgGraphNodeSetParent(graph,n3,n4);
	dsgGraphNodeSetParent(graph,n3,n5);
	
	// Update
	fprintf(stdout,"Updating Graph\n");
	dsgGraphUpdatePaths(graph);
	fprintf(stdout,"Updated\n");
	dsgGraphPrintGraph(graph);
	return graph;
}

void nodeTraverseFunction(node_t* node,void* arg) {
	glTranslatef(node->translation[NODE_X], 
		     node->translation[NODE_Y], 
		     node->translation[NODE_Z]);

	glRotatef(node->rotation[NODE_X],1.0f,0.0f,0.0f);
	glRotatef(node->rotation[NODE_Y],0.0f,1.0f,0.0f);
	glRotatef(node->rotation[NODE_Z],0.0f,0.0f,1.0f);
	return;
}

void drawNode(node_t* node, void* arg) {
	graph_t* graph = (graph_t*)arg;
	int vbufIndex = node->vertexBufferIndex;
	dsgGraphTraversePath(graph,node->path,nodeTraverseFunction,NULL);

	glBegin(GL_QUADS);
	/* Send data : 24 vertices */
	int i;
	for (i=0; i<72; i+=3) {
		glVertex3f(graph->vertexBuffers[vbufIndex][i],
		           graph->vertexBuffers[vbufIndex][i+1],
		           graph->vertexBuffers[vbufIndex][i+2]);
	}
	/* Cleanup states */
	glEnd();
	return; 
}

void drawGLScene() {
	nanosleep(&sleepTime,NULL);
	n1->rotation[NODE_Y] += 0.5f;
	if(n1->rotation[NODE_Y]  > 360.0f) {
		n1->rotation[NODE_Y]  = 0.0f;
	}
	n1->rotation[NODE_Z] += 0.5f;
	if(n1->rotation[NODE_Z]  > 360.0f) {
		n1->rotation[NODE_Z]  = 0.0f;
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(0.0f,0.0f,75.f,
		   0.0f,0.0f,0.0f,
		   0.0f,1.0f,0.0f);
	/* Render here */
	glColor3f(1.0f,1.0f,1.0f); 
	glPushMatrix();
	dsgGraphTraverseNodeVector(graph,&drawNode,NULL);
	glPopMatrix();
	glutSwapBuffers();
	return;
}

void setupGL(){ 
	/* GL Setup */
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClearDepth(1.0f);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST); 
	glShadeModel(GL_SMOOTH);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90, windowWidth/windowHeight,0.1f,1000.0f);
	glMatrixMode(GL_MODELVIEW);	
	return;
}

void resizeGLScene(int width, int height) { 
	// Prevent A Divide By Zero If The Window Is Too Small
	if (height == 0){
		height = 1;
	}
	windowWidth = width;
	windowHeight = height;
	// Reset The Current Viewport And Perspective Transformation
	glViewport(0, 0, windowWidth, windowHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f,(GLfloat)windowWidth/(GLfloat)windowHeight,0.1f,100.0f);
	glMatrixMode(GL_MODELVIEW);
}

/* The function called whenever a key is pressed. */
void keyPressed(unsigned char key, int x, int y) {
    /* If escape is pressed, kill everything. */
    if (key == ESCAPE) { 
          /* shut down our window */
          glutDestroyWindow(window); 
          /* exit the program...normal termination. */
          exit(0);                   
        }
}

int main(int argc, char** argv) {
	graph = setupScenegraph();
	sleepTime.tv_sec = 0L;
	sleepTime.tv_nsec = 1000000000L/60;
	glutInit(&argc,argv);
	/* 
	*  Select type of Display mode:   
	*  Double buffer 
	*  RGBA color
	*  Alpha components supported 
	*  Depth buffered for automatic clipping 
	*/  
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);  
	glutInitWindowSize(windowWidth,windowHeight);  
	/* The window starts at the upper left corner of the screen */
	glutInitWindowPosition(0, 0);  
	/* Open a window */  
	window = glutCreateWindow("DreamSceneGraph GLTest");  
	/* Register the function to do all our OpenGL drawing. */
	glutDisplayFunc(&drawGLScene);  
	/* Even if there are no events, redraw our gl scene. */
	glutIdleFunc(&drawGLScene);
	/* Register the function called when our window is resized. */
	glutReshapeFunc(&resizeGLScene);
	/* Register the function called when the keyboard is pressed. */
	glutKeyboardFunc(&keyPressed);
	/* Setup Scene */
	setupGL();
	/* Start Event Processing Engine */  
	glutMainLoop();  
	return 0;
}
