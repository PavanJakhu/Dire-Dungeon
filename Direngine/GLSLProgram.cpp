#include "GLSLProgram.h"
#include "Errors.h"
#include <vector>
#include <fstream>

GLSLProgram::GLSLProgram() : programID(0), vertexShaderID(0), fragmentShaderID(0), numAttrs(0)
{
}

// Compiles the shaders into a form that your GPU can understand
void GLSLProgram::CompileShaders(const std::string& _vertexShaderFilePath, const std::string& _fragmentShaderFilepath) {
  // Vertex and fragment shaders are successfully compiled.
  // Now time to link them together into a program.
  // Get a program object.
  programID = glCreateProgram();

  // Create the vertex shader object, and store its ID
  vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
  if (vertexShaderID == 0)
    Debug::FatalError("Vertex shader failed to be created!");

  // Create the fragment shader object, and store its ID
  fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
  if (fragmentShaderID == 0)
    Debug::FatalError("Fragment shader failed to be created!");

  CompileShader(_vertexShaderFilePath, vertexShaderID);
  CompileShader(_fragmentShaderFilepath, fragmentShaderID);
}

void GLSLProgram::LinkShaders() {
  // Attach our shaders to our program
  glAttachShader(programID, vertexShaderID);
  glAttachShader(programID, fragmentShaderID);

  // Link our program
  glLinkProgram(programID);

  // Note the different functions here: glGetProgram* instead of glGetShader*.
  GLint isLinked = 0;
  glGetProgramiv(programID, GL_LINK_STATUS, (int*)&isLinked);

  if (isLinked == GL_FALSE) {
    GLint maxLength = 0;
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &maxLength);

    // The maxLength includes the NULL character
    std::vector<char> errorLog(maxLength);
    glGetProgramInfoLog(programID, maxLength, &maxLength, &errorLog[0]);

    // We don't need the program anymore.
    glDeleteProgram(programID);
    // Don't leak shaders either.
    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);

    // Print the error log and quit
    std::printf("%s\n", &(errorLog[0]));
    Debug::FatalError("Shaders failed to link!");
  }

  // Always detach shaders after a successful link.
  glDetachShader(programID, vertexShaderID);
  glDetachShader(programID, fragmentShaderID);
  glDeleteShader(vertexShaderID);
  glDeleteShader(fragmentShaderID);
}

// Adds an attribute to our shader. SHould be called between compiling and linking.
void GLSLProgram::AddAttribute(const std::string& _attrName) {
  glBindAttribLocation(programID, numAttrs++, _attrName.c_str());
}

GLint GLSLProgram::GetUniformLocation(const std::string& _uniformName) {
  GLint location = glGetUniformLocation(programID, _uniformName.c_str());

  if (location == GL_INVALID_INDEX)
    Debug::FatalError("Uniform " + _uniformName + " not found in shader!");

  return location;
}

// Enable the shader, and all its attributes
void GLSLProgram::Use() {
  glUseProgram(programID);

  // Enable all the attributes we added with addAttribute
  for (int i = 0; i < numAttrs; i++)
    glEnableVertexAttribArray(i);
}

// Disable the shader
void GLSLProgram::Unuse() {
  glUseProgram(0);

  for (int i = 0; i < numAttrs; i++)
    glDisableVertexAttribArray(i);
}

// Compiles a single shader file
void GLSLProgram::CompileShader(const std::string& _filePath, GLuint _id) {
  // Open the file
  std::ifstream shaderFile(_filePath);
  if (shaderFile.fail()) {
    perror(_filePath.c_str());
    Debug::FatalError("Failed to open " + _filePath);
  }

  // File contents stores all the text in the file
  std::string fileContents = "";
  std::string line;

  // Get all the lines in the file and add it to the contents
  while (std::getline(shaderFile, line))
    fileContents += line + "\n";

  shaderFile.close();

  // Get a pointer to our file contents c string;
  const char* contentsPtr = fileContents.c_str();
  // Tell opengl that we want to use fileContents as the contents of the shader file
  glShaderSource(_id, 1, &contentsPtr, nullptr);
  // Compile the shader
  glCompileShader(_id);

  // Check for errors
  GLint success = 0;
  glGetShaderiv(_id, GL_COMPILE_STATUS, &success);

  if (success == GL_FALSE) {
    GLint maxLength = 0;
    glGetShaderiv(_id, GL_INFO_LOG_LENGTH, &maxLength);

    // The maxLength includes the NULL character
    std::vector<char> errorLog(maxLength);
    glGetShaderInfoLog(_id, maxLength, &maxLength, &errorLog[0]);

    // Provide the infolog in whatever manor you deem best.
    // Exit with failure.
    glDeleteShader(_id); // Don't leak the shader.

    // Print error log and quit
    std::printf("%s\n", &(errorLog[0]));
    Debug::FatalError("Shader " + _filePath + " failed to compile");
  }
}