#include "driver/cmd_parser.hh"

#include <iostream>

namespace cmd {

CmdParser::CmdParser(int argc, const char* const* argv) : parser_(argc, argv) {
  desc_.add_options()("opengl", "Draw with OpenGL");
  parser_.options(desc_).positional(pos_desc_).allow_unregistered();
}

Config CmdParser::run() {
  po::store(parser_.run(), var_map_);
  po::notify(var_map_);

  Config cfg;

  if (var_map_.count("opengl")) {
    cfg.draw = true;
  }
  return cfg;
}

}  // namespace cmd
