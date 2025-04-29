#version 330 core

in vec3 v_color;
in vec4 shadow_coord;
in float is_dark_side;
in float light_angle;

uniform sampler2D shadow_map;

out vec4 frag_color;

void main() {
  vec3 shadow_coord_norm = shadow_coord.xyz / shadow_coord.w;
  float visibility = 1.0;
  const float visibility_from_another = 0.3;
  const float bias = 0.05;

  if ((texture(shadow_map, shadow_coord_norm.xy).r < shadow_coord_norm.z - bias) ||
      (is_dark_side > 0.5)) {
    visibility = visibility_from_another;
  } else {
    visibility =
        abs(light_angle) * (visibility - visibility_from_another)
                           / visibility
        + visibility_from_another;
  }

  frag_color = vec4(v_color * visibility, 1.0);
}
