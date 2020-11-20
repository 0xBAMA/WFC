#version 430

in vec3 vPosition;
out vec2 v_pos;

void main()
{
  v_pos = vPosition.xy;
  v_pos.y = -v_pos.y;
  
  gl_Position = vec4(vPosition,1.0);
}
