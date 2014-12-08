#pragma once
#include <GL/glew.h>
#include <GLM/glm.hpp>
#include <vector>
#include "Vertex.h"

// Determines how we should sort the glyphs
enum class GlyphSortType {
  NONE, LAST_ON_TOP, LAST_ON_BOTTOM, FRONT_TO_BACK, BACK_TO_FRONT, TEXTURE
};

// A glyph is a single quad. These are added via SpriteBatch::Draw
class Glyph {
public:
  GLuint texture;
  float depth;
  Vertex topLeft;
  Vertex bottomLeft;
  Vertex topRight;
  Vertex bottomRight;

  Glyph() {}
  Glyph(const glm::vec4& _destRect, const glm::vec4& _uvRect, GLuint _texture, float _depth, float _rotation, const Color& _color) : texture(_texture), depth(_depth) {
    glm::vec2 verts[4] = { 
      glm::vec2(-_destRect.z/2.0F,  _destRect.w/2.0F), // Top Left
      glm::vec2(-_destRect.z/2.0F, -_destRect.w/2.0F), // Bottom Left
      glm::vec2( _destRect.z/2.0F, -_destRect.w/2.0F), // Bottom Right
      glm::vec2( _destRect.z/2.0F,  _destRect.w/2.0F)  // Top Right
    };

    glm::mat2 rotation(cos(_rotation), sin(_rotation), -sin(_rotation), cos(_rotation));
    for (int i = 0; i < 4; i++)
      verts[i] = rotation * verts[i];

    topLeft.color = _color;
    topLeft.SetPosition(verts[0].x + _destRect.x, verts[0].y + _destRect.y);
    topLeft.SetUV(_uvRect.x, _uvRect.y + _uvRect.w);

    bottomLeft.color = _color;
    bottomLeft.SetPosition(verts[1].x + _destRect.x, verts[1].y + _destRect.y);
    bottomLeft.SetUV(_uvRect.x, _uvRect.y);

    bottomRight.color = _color;
    bottomRight.SetPosition(verts[2].x + _destRect.x, verts[2].y + _destRect.y);
    bottomRight.SetUV(_uvRect.x + _uvRect.z, _uvRect.y);

    topRight.color = _color;
    topRight.SetPosition(verts[3].x + _destRect.x, verts[3].y + _destRect.y);
    topRight.SetUV(_uvRect.x + _uvRect.z, _uvRect.y + _uvRect.w);
  }
};

// Each render batch is used for a single draw call
class RenderBatch {
public:
  GLuint offset;
  GLuint numVertices;
  GLuint texture;

  RenderBatch(GLuint _offset, GLuint _numVertices, GLuint _texture) : offset(_offset),
    numVertices(_numVertices), texture(_texture) {}
};

// The SpriteBatch class is a more efficient way of drawing sprites
class SpriteBatch {
public:
  // Initializes the spritebatch
  void Init();
  // Begins the spritebatch
  void Begin(GlyphSortType _sortType = GlyphSortType::LAST_ON_TOP);
  // Ends the spritebatch and renders it to the screen
  void End();
  // Adds a glyph to the spritebatch
  void Draw(const glm::vec4& _destRect, const glm::vec4& _uvRect, GLuint _texture, float _depth, float _rotation, const Color& _color);

  SpriteBatch();
  ~SpriteBatch() {}
private:
  GLuint vbo;
  GLuint vao;
  GlyphSortType sortType;
  int glyphOrder;

  std::vector<Glyph*> glyphPtrs;
  std::vector<Glyph> glyphs;
  std::vector<RenderBatch> renderBatches;

  // Creates all the needed RenderBatches
  void CreateRenderBatches();
  // Generates our VAO and VBO
  void CreateVertexArray();
  // Sorts glyphs according to _sortType
  void SortGlyphs();

  // Comparators used by SortGlyphs()
  static bool CompareF2B(Glyph* a, Glyph* b);
  static bool CompareB2F(Glyph* a, Glyph* b);
  static bool CompareTex(Glyph* a, Glyph* b);
};