/*
* gltest_main.c
*
* GLFW/OpenGL GNU/Linux based testing for DreamSceneGraph.
*
*/

// System
#include <stdio.h>
#include <stdlib.h>
// GLFW
#include <GLFW/glfw3.h>
// Project
#include "../graph.h"

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
	//glVertex3f(-sz, sz, sz);
	//glVertex3f(sz, sz, sz);
	//glVertex3f(sz, -sz, sz);
	//glVertex3f(-sz, -sz, sz);
	/* Back face */
	buffer[12] = -sz; buffer[13] = -sz; buffer[14] = -sz;
	buffer[15] =  sz; buffer[16] = -sz; buffer[17] = -sz;
	buffer[18] =  sz; buffer[19] =  sz; buffer[20] = -sz;
	buffer[21] = -sz; buffer[22] =  sz; buffer[23] = -sz;
	//glVertex3f(-sz, -sz, -sz);
	//glVertex3f(sz, -sz, -sz);
	//glVertex3f(sz, sz, -sz);
	//glVertex3f(-sz, sz, -sz);
	/* Left face */
	buffer[24] = -sz; buffer[25] =  sz; buffer[26] =  sz;
	buffer[27] = -sz; buffer[28] = -sz; buffer[29] =  sz;
	buffer[30] = -sz; buffer[31] = -sz; buffer[32] = -sz;
	buffer[33] = -sz; buffer[34] =  sz; buffer[35] = -sz;
	//glVertex3f(-sz, sz, sz);
	//glVertex3f(-sz, -sz, sz);
	//glVertex3f(-sz, -sz, -sz);
	//glVertex3f(-sz, sz, -sz);
	/* Right face */
	buffer[36] = sz; buffer[37] =  sz; buffer[38] = -sz;
	buffer[39] = sz; buffer[40] = -sz; buffer[41] = -sz;
	buffer[42] = sz; buffer[43] = -sz; buffer[44] =  sz;
	buffer[45] = sz; buffer[46] =  sz; buffer[47] =  sz;
	//glVertex3f(sz, sz, -sz);
	//glVertex3f(sz, -sz, -sz);
	//glVertex3f(sz, -sz, sz);
	//glVertex3f(sz, sz, sz);
	/* Top face */
	buffer[48] =  sz; buffer[49] = sz; buffer[50] =  sz;
	buffer[51] = -sz; buffer[52] = sz; buffer[53] =  sz;
	buffer[54] = -sz; buffer[55] = sz; buffer[56] = -sz;
	buffer[57] =  sz; buffer[58] = sz; buffer[59] = -sz;
	//glVertex3f(sz, sz, sz);
	//glVertex3f(-sz, sz, sz);
	//glVertex3f(-sz, sz, -sz);
	//glVertex3f(sz, sz, -sz);
	/* Bottom face */
	buffer[60] =  sz; buffer[61] = -sz; buffer[62] = -sz;
	buffer[63] = -sz; buffer[64] = -sz; buffer[65] = -sz;
	buffer[66] = -sz; buffer[67] = -sz; buffer[68] =  sz;
	buffer[69] =  sz; buffer[70] = -sz; buffer[71] =  sz;
	//glVertex3f(sz, -sz, -sz);
	//glVertex3f(-sz, -sz, -sz);
	//glVertex3f(-sz, -sz, sz);
	//glVertex3f(sz, -sz, sz);	
	return buffer;
}

graph_t* setupScenegraph() {
	graph_t* graph = dsgGraphInitWithName("GLTestGraph");
	// Create Nodes	
	fprintf(stdout, "Creating Nodes\n");
	node_t *n1, *n2, *n3, *n4, *n5;
	n1 = dsgGraphCreateNodeWithName(graph,"Node1");
	n2 = dsgGraphCreateNodeWithName(graph,"Node2");
	n3 = dsgGraphCreateNodeWithName(graph,"Node3");
	n4 = dsgGraphCreateNodeWithName(graph,"Node4");
	n5 = dsgGraphCreateNodeWithName(graph,"Node5");
	// Establish Relationships
	fprintf(stdout, "Setting Relationships\n");
	dsgGraphSetRootNode(graph, n1);
	dsgNodeSetParent(n1,n2);
	dsgNodeSetParent(n1,n3);
	dsgNodeSetParent(n3,n4);
	dsgNodeSetParent(n3,n5);
	// Create cube vertex buffer
	int cubeBuffer = dsgGraphAddVertexBuffer(graph, cubeVertexBuffer(1.0f));
	fprintf(stdout,"Cube buffer allcated to %s's VBO %d\n",graph->name,cubeBuffer);
	// Update
	fprintf(stdout,"Updating Graph\n");
	dsgGraphUpdateAll(graph);
	dsgGraphPrintGraph(graph);
	return graph;
}

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

void drawGraph(graph_t* graph) {
	node_t* node;
	return;
}

int main(void) {
	graph_t* graph = setupScenegraph();
	GLFWwindow* window;
	/* Initialize the library */
	if (!glfwInit()) {
		return -1;
	}
	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "DreamSceneGraph GLTest", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	/* Setup the key event callback */
	glfwSetKeyCallback(window, keyCallback);
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		drawGraph(graph);
		/* Swap front and back buffers */
		glfwSwapBuffers(window);
		/* Poll for and process events */
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}
