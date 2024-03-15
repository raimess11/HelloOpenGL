#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#define GLT_IMPLEMENTATION
#include <GLText/gltext.h>

#include <iostream>
#include <cmath>
#include <ctime>

#include<shaderClass.h>
#include<VAO.h>
#include<VBO.h>
#include<EBO.h>
#include<Texture.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

using namespace glm;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

//vertices coordinate
GLfloat vertices[] =
{ //     COORDINATES     /        COLORS      /   TexCoord  //
    -0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
    -0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
    0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	    0.0f, 0.0f,
    0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	    5.0f, 0.0f,
    0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	    2.5f, 5.0f
};

// Indices for vertices order
GLuint indices[] =
{
    0, 1, 2,
    0, 2, 3,
    0, 1, 4,
    1, 2, 4,
    2, 3, 4,
    3, 0, 4
};

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();

    //tell GLFW what version we use
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    //tell GLFW we use the core profile/modern functions only
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    


#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "HelloOpenGL", NULL, NULL);
    //check if got fails to create the window
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    //show the window to current context
    glfwMakeContextCurrent(window);

    //load GLAD so it configures openGL
    gladLoadGL();

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }  

    //set viewport goes form vvv
    glViewport(0,0,SCR_WIDTH,SCR_HEIGHT);
    // glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    // Generates Shader object using shaders defualt.vert and default.frag
	Shader shaderProgram("../src/Shader/default.vert", "../src/Shader/default.frag");

    

	// Generates Vertex Array Object and binds it
	VAO VAO1;
	VAO1.Bind();

	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO1(vertices, sizeof(vertices));
	// Generates Element Buffer Object and links it to indices
	EBO EBO1(indices, sizeof(indices));

	// Links VBO to VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();
    
    // get ID uniform
    GLuint uniID_scale = glGetUniformLocation(shaderProgram.ID, "scale");
    GLuint uniID_time = glGetUniformLocation(shaderProgram.ID, "time");

	// Texture
	Texture brickTex("../src/Textures/brick.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	brickTex.texUnit(shaderProgram, "tex0", 0);  

    // Initialize glText
    // gltInit();

    // Creating text
    GLTtext *text = gltCreateText();
    gltSetText(text, "Hello World!");

    float rotation = 0.0f;
	double prevTime = glfwGetTime();

    glEnable(GL_DEPTH_TEST);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.075f, 0.2f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // // Begin text drawing (this for instance calls glUseProgram)
        // gltBeginDraw();

        // // Draw any amount of text between begin and end
        // gltColor(1.0f, 1.0f, 1.0f, 1.0f);
        // gltDrawText2D(text, 0, 0, 3.0f);

        // // Finish drawing text
        // gltEndDraw();
        
        // Tell OpenGL which Shader Program we want to use
        shaderProgram.Activate();

        // Simple timer
		double crntTime = glfwGetTime();
		if (crntTime - prevTime >= 1 / 60)
		{
			rotation += 0.025f;
			prevTime = crntTime;
		}

        mat4 model = mat4(1.0f);
        mat4 view = mat4(1.0f);
        mat4 proj = mat4(1.0f);

        model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
        view = translate(view, vec3(0.0f, -0.5f, -2.f));
        proj = perspective(radians(45.0f), (float)(SCR_HEIGHT/SCR_HEIGHT), 0.1f, 100.0f);

        int modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));
        int viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));
        int projLoc = glGetUniformLocation(shaderProgram.ID, "proj");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(proj));
        
        glUniform1f(uniID_scale, 0.2f);
        float current = clock();
        glUniform1f(uniID_time, current);
        brickTex.Bind();
        
		// Bind the VAO so OpenGL knows to use it
		VAO1.Bind();
        //Draw the triangle using the GL_TRIANGLES primitive
        glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents(); //so the window not just inactive
    }

    gltDeleteText(text);
    gltTerminate();

    //delete gl vertex and buffer
    VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
    brickTex.Delete();
	shaderProgram.Delete();

    // glfw: delete and terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
