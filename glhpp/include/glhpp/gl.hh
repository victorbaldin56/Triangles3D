#pragma once

#include "detail/error_handler.hh"

namespace glhpp {

inline auto init() {
  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK) {
    throw Error("glewInit failed");
  }

  GLHPP_DETAIL_ERROR_HANDLER(glEnable, GL_DEPTH_TEST);
  GLHPP_DETAIL_ERROR_HANDLER(glEnable, GL_CULL_FACE);
  GLHPP_DETAIL_ERROR_HANDLER(glCullFace, GL_BACK);
  GLHPP_DETAIL_ERROR_HANDLER(glDepthFunc, GL_LESS);
  GLHPP_DETAIL_ERROR_HANDLER(glDepthMask, GL_TRUE);
  GLHPP_DETAIL_ERROR_HANDLER(glClearColor, 0.f, 0.f, 0.f, 0.f);
}
}
