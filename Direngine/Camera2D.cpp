#include "Camera2D.h"

Camera2D::Camera2D() : position(0.0f, 0.0f),
  cameraMatrix(1.0f),
  orthoMatrix(1.0f),
  scale(1.0f),
  requiresUpdate(true),
  screenWidth(500),
  screenHeight(500)
{
}

void Camera2D::Init(int _screenWidth, int _screenHeight) {
  screenWidth = _screenWidth;
  screenHeight = _screenHeight;
  orthoMatrix = glm::ortho(0.0f, (float)screenWidth, 0.0f, (float)screenHeight);
}

void Camera2D::Update() {
  // Only update if position or scale has changed
  if (requiresUpdate) {
    // Camera Translation
    glm::vec3 translate(-position.x + screenWidth / 2, -position.y + screenHeight / 2, 0.0f);
    cameraMatrix = glm::translate(orthoMatrix, translate);

    // Camera Scale
    glm::vec3 scale(scale, scale, 0.0f);
    cameraMatrix = glm::scale(glm::mat4(1.0f), scale) * cameraMatrix;

    requiresUpdate = false;
  }
}

bool Camera2D::IsInView(const glm::vec2& _position, const glm::vec2& _dimensions) {
  glm::vec2 scaledCameraDimensions = glm::vec2(screenWidth, screenHeight) / scale;
  // Minimum distance to calculate "collision"
  const float MIN_DISTANCE_X = _dimensions.x / 2.0F + scaledCameraDimensions.x / 2.0F;
  const float MIN_DISTANCE_Y = _dimensions.y / 2.0F + scaledCameraDimensions.y / 2.0F;

  // Center of the rectangle
  glm::vec2 centerPos = _position + _dimensions / 2.0F;
  // Center position of the camera
  glm::vec2 centerCamPos = _position;
  // Vector from input to camera
  glm::vec2 distance = centerPos - centerCamPos;

  float xDepth = MIN_DISTANCE_X - abs(distance.x);
  float yDepth = MIN_DISTANCE_Y - abs(distance.y);

  // If depth is greater than 0, the camera and object rect are colliding
  if (xDepth > 0 && yDepth > 0)
    return true;

  return false;
}

glm::vec2 Camera2D::TransformScreenToWorld(glm::vec2 _screenCoords) {
  // Invert Y direction
  _screenCoords.y = screenHeight - _screenCoords.y;
  // Make it so that 0 is the center
  _screenCoords -= glm::vec2(screenWidth / 2, screenHeight / 2);
  // Scale the coordinates
  _screenCoords /= scale;
  // Translate with the camera position
  _screenCoords += position;

  return _screenCoords;
}