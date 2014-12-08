#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera2D {
public:
  Camera2D();
  ~Camera2D() {}

  //sets up the orthographic matrix and screen dimensions
  void Init(int _screenWidth, int _screenHeight);

  //updates the camera matrix if needed
  void Update();

  bool IsInView(const glm::vec2& _position, const glm::vec2& _dimensions);
  glm::vec2 TransformScreenToWorld(glm::vec2 _screenCoords);

  void SetPosition(const glm::vec2& _position) { position = _position; requiresUpdate = true; }
  void SetScale(float _scale) { scale = _scale; requiresUpdate = true; }

  glm::vec2 GetPosition() { return position; }
  float GetScale() { return scale; }
  glm::mat4 GetCameraMatrix() { return cameraMatrix; }
private:
  glm::vec2 position;
  glm::mat4 cameraMatrix;
  glm::mat4 orthoMatrix;
  float scale;
  bool requiresUpdate;
  int screenWidth, screenHeight;
};