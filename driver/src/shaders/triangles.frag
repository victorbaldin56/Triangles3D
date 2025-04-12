#version 330 core

in vec3  v_color;
in vec4  shadow_coord_;
in float is_dark_side;
in float light_angle;

// uniform float time;
uniform sampler2D shadow_map;

void main() {
  vec3 shadow_coord = shadow_coord_.xyz / shadow_coord_.w;
  float visibility = 1.0;
  float visibility_from_another = 0.3;
  float bias = 0.05;
  if ((texture(shadow_map, shadow_coord.xy).r < shadow_coord.z - bias) || (is_dark_side > 0.5)) {
    visibility = visibility_from_another;
  } else {
    visibility = abs(light_angle) * (visibility - visibility_from_another) / visibility + visibility_from_another;
  }

  gl_FragColor = vec4((v_color.x/* + time*/) * visibility,
                      (v_color.y/* + time*/) * visibility,
                      (v_color.z/* + time*/) * visibility,
                      1.0);
}