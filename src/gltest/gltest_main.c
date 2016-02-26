/*
* gltest_main.c
*
* GLFW/OpenGL GNU/Linux based testing for DreamSceneGraph.
*
*/

// System
#include <stdio.h>
// GLFW
#include <GLFW/glfw3.h>
// Project
#include "../graph.h"

graph_t* setupScenegraph() {
	graph_t* graph = dsgGraphInitWithName("GLTestGraph");
	dsgGraphPrintGraph(graph);
	return graph;
}
static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
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
		/* Swap front and back buffers */
		glfwSwapBuffers(window);
		/* Poll for and process events */
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}
