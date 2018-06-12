// include opengl
#define GLFW_INCLUDE_GLCOREARB

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include <thread>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "common/shader.cpp"

#define WIDTH 1024
#define HEIGHT 768


GLFWwindow* window;

int main( void )
{
  // Initialise GLFW
  if( !glfwInit() )
  {
    fprintf( stderr, "Failed to initialize GLFW\n" );
    getchar();
    return -1;
  }

  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Open a window and create its OpenGL context
  window = glfwCreateWindow( WIDTH, HEIGHT, "Fractal", NULL, NULL);
  if( window == NULL )
  {
    fprintf( stderr, "Failed to open GLFW window.\n" );
    getchar();
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  // set background color
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  GLuint VertexArrayID;
  glGenVertexArrays(1, &VertexArrayID);
  glBindVertexArray(VertexArrayID);

  // Create and compile our GLSL program from the shaders
  GLuint programID = LoadShaders( "shaders/simple.vert", "shaders/simple.frag" );

  static const GLfloat g_vertex_buffer_data[] = { 
    -1.0f, -1.0f, 0.0f,
     1.0f, -1.0f, 0.0f,
     1.0f,  1.0f, 0.0f,
     1.0f,  1.0f, 0.0f,
    -1.0f, -1.0f, 0.0f,
    -1.0f,  1.0f, 0.0f
  };

  GLuint vertexbuffer;
  glGenBuffers(1, &vertexbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	glm::mat4 projection = glm::ortho(
		0.0f,
		static_cast<float>(WIDTH),
		static_cast<float>(HEIGHT),
		0.0f,
		0.0f,
		100.0f
	);

	glm::mat4 view;
 	glm::mat4 model = glm::mat4(1.0f);
 

  float x = 0.0f;
  do{

    x += 1.0f;

    // Clear the screen
    glClear( GL_COLOR_BUFFER_BIT );

    // Use our shader
    glUseProgram(programID);

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	
		glUniformMatrix4fv( glGetUniformLocation( programID, "u_model" ), 1, GL_FALSE, glm::value_ptr( model ) );
		glUniformMatrix4fv( glGetUniformLocation( programID, "u_view" ), 1, GL_FALSE, glm::value_ptr( view ) );
		glUniformMatrix4fv( glGetUniformLocation( programID, "u_projection" ), 1, GL_FALSE, glm::value_ptr( projection ) );
    
		model = glm::mat4();
		model = glm::translate( model, glm::vec3( x, 0.0f, 0.0f ));
		model = glm::scale( model, glm::vec3( 100.0f, 100.0f, 0.0f ));
		
		glUniformMatrix4fv( glGetUniformLocation( programID, "u_model" ), 1, GL_FALSE, glm::value_ptr( model ) );
    
    glVertexAttribPointer(
      0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
      3,                  // size
      GL_FLOAT,           // type
      GL_FALSE,           // normalized?
      0,                  // stride
      (void*)0            // array buffer offset
    );

    // Draw the triangle !
    glDrawArrays(GL_TRIANGLES, 0, 6); // 3 indices starting at 0 -> 1 triangle

    glDisableVertexAttribArray(0);

    // Swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  } // Check if the ESC key was pressed or the window was closed
  while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
       glfwWindowShouldClose(window) == 0 );

  // Cleanup VBO
  glDeleteBuffers(1, &vertexbuffer);
  glDeleteVertexArrays(1, &VertexArrayID);
  glDeleteProgram(programID);

  // Close OpenGL window and terminate GLFW
  glfwTerminate();

  return 0;
}
