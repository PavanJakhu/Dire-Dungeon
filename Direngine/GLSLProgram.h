#pragma once
#include <GL/glew.h>
#include <string>

// This class handles the compilation, linking, and usage of a GLSL shader program.
// Reference: http://www.opengl.org/wiki/Shader_Compilation
class GLSLProgram {
public:
  GLSLProgram();
  ~GLSLProgram() {}

  void CompileShaders(const std::string& _vertexShaderPath, const std::string& _fragmentShaderPath);
  void LinkShaders();
  void AddAttribute(const std::string& _attrName);

  GLint GetUniformLocation(const std::string& _uniformName);

  void Use();
  void Unuse();
private:
  GLuint programID;
  GLuint vertexShaderID;
  GLuint fragmentShaderID;
  int numAttrs;

  void CompileShader(const std::string& _filePath, GLuint _id);
};