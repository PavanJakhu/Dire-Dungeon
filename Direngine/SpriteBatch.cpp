#include "SpriteBatch.h"
#include <algorithm>

SpriteBatch::SpriteBatch() : vbo(0), vao(0), glyphOrder(0)
{
}

void SpriteBatch::Init() {
  CreateVertexArray();
}

void SpriteBatch::Begin(GlyphSortType _sortType) {
  sortType = _sortType;
  glyphOrder = 0;
  renderBatches.clear();
  glyphs.clear();
}

void SpriteBatch::End() {
  glyphPtrs.resize(glyphs.size());
  // For sorting purposes
  for (unsigned int i = 0; i < glyphs.size(); i++)
    glyphPtrs[i] = &glyphs[i];

  SortGlyphs();
  CreateRenderBatches();

  // Bind our VAO. This sets up the opengl state we need, including the 
  // vertex attribute pointers and it binds the VBO
  glBindVertexArray(vao);

  for (unsigned int i = 0; i < renderBatches.size(); i++) {
    glBindTexture(GL_TEXTURE_2D, renderBatches[i].texture);
    glDrawArrays(GL_TRIANGLES, renderBatches[i].offset, renderBatches[i].numVertices);
  }

  // Unbind the VAO
  glBindVertexArray(0);
}

void SpriteBatch::Draw(const glm::vec4& _destRect, const glm::vec4& _uvRect, GLuint _texture, float _depth, float _rotation, const Color& _color) {
  Glyph newGlyph(_destRect, _uvRect, _texture, _depth, _rotation, _color);

  if (sortType == GlyphSortType::LAST_ON_TOP || sortType == GlyphSortType::LAST_ON_BOTTOM)
    newGlyph.depth = (float)glyphOrder++;

  glyphs.emplace_back(newGlyph);
}

void SpriteBatch::CreateRenderBatches() {
  // This will store all the vertices that we need to upload
  std::vector <Vertex> vertices;
  // Resize the buffer to the exact size we need so we can treat it like an array
  vertices.resize(glyphs.size() * 6);

  if (glyphs.empty())
    return;

  int offset = 0; // current offset
  int cv = 0; // current vertex

  // Add the first batch
  renderBatches.emplace_back(offset, 6, glyphPtrs[0]->texture);
  vertices[cv++] = glyphPtrs[0]->topLeft;
  vertices[cv++] = glyphPtrs[0]->bottomLeft;
  vertices[cv++] = glyphPtrs[0]->bottomRight;
  vertices[cv++] = glyphPtrs[0]->bottomRight;
  vertices[cv++] = glyphPtrs[0]->topRight;
  vertices[cv++] = glyphPtrs[0]->topLeft;
  offset += 6;

  // Add all the rest of the glyphs
  for (unsigned int cg = 1; cg < glyphPtrs.size(); cg++) { // Check if this glyph can be part of the current batch
    if (glyphPtrs[cg]->texture != glyphPtrs[cg - 1]->texture) // Make a new batch
      renderBatches.emplace_back(offset, 6, glyphPtrs[cg]->texture);
    else // If its part of the current batch, just increase numVertices
      renderBatches.back().numVertices += 6;

    vertices[cv++] = glyphPtrs[cg]->topLeft;
    vertices[cv++] = glyphPtrs[cg]->bottomLeft;
    vertices[cv++] = glyphPtrs[cg]->bottomRight;
    vertices[cv++] = glyphPtrs[cg]->bottomRight;
    vertices[cv++] = glyphPtrs[cg]->topRight;
    vertices[cv++] = glyphPtrs[cg]->topLeft;
    offset += 6;
  }

  // Bind our VBO
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  // Orphan the buffer (for speed)
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
  // Upload the data
  glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

  // Unbind the VBO
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void SpriteBatch::CreateVertexArray() {
  // Generate the VAO if it isn't already generated
  if (vao == 0)
    glGenVertexArrays(1, &vao);
  // Bind the VAO. All subsequent opengl calls will modify it's state.
  glBindVertexArray(vao);

  // Generate the VBO if it isn't already generated
  if (vbo == 0)
    glGenBuffers(1, &vbo);
  // Bind the VBO
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  // Tell OpenGL what attribute arrays we need (position, color, uv)
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

  // This is the position attribute pointer
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
  // This is the color attribute pointer
  glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
  // This is the UV attribute pointer
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

  // Unbind the VAO
  glBindVertexArray(0);
}

void SpriteBatch::SortGlyphs() {
  switch (sortType) {
  case GlyphSortType::LAST_ON_BOTTOM:
  case GlyphSortType::BACK_TO_FRONT:
    std::stable_sort(glyphPtrs.begin(), glyphPtrs.end(), CompareB2F);
    break;
  case GlyphSortType::LAST_ON_TOP:
  case GlyphSortType::FRONT_TO_BACK:
    std::stable_sort(glyphPtrs.begin(), glyphPtrs.end(), CompareF2B);
    break;
  case GlyphSortType::TEXTURE:
    std::stable_sort(glyphPtrs.begin(), glyphPtrs.end(), CompareTex);
    break;
  }  
}

bool SpriteBatch::CompareF2B(Glyph* a, Glyph* b) {
  return a->depth < b->depth;
}

bool SpriteBatch::CompareB2F(Glyph* a, Glyph* b) {
  return a->depth > b->depth;
}

bool SpriteBatch::CompareTex(Glyph* a, Glyph* b) {
  return a->texture < b->texture;
}