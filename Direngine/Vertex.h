#pragma once
#include <GL/glew.h>

struct Position {
  float x, y;

	Position(float _x, float _y) : x(_x), y(_y) {}
	Position() : x(0), y(0) {}
};

struct Color {
  GLubyte r, g, b, a;

	Color(GLubyte _r, GLubyte _g, GLubyte _b, GLubyte _a) : r(_r), g(_r), b(_b), a(_a) {}
	Color() : r(255), g(255), b(255), a(255) {}
};

struct UV {
  float u, v;

	UV(float _u, float _v) : u(_u), v(_v) {}
	UV() : u(0), v(0) {}
};

struct Vertex {
  Position position;
  Color color;
  UV uv;

  void SetPosition(float x, float y) {
    position.x = x;
    position.y = y;
  }

  void SetColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a) {
    color.r = r;
    color.g = g;
    color.b = b;
    color.a = a;
  }

  void SetUV(float u, float v) {
    uv.u = u;
    uv.v = v;
  }
};