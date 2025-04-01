#version 330 core
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D inputImage;

vec3 RGB2YCrCb(vec3 col)
{
 float R = 255. * col.r;
 float G = 255. * col.g;
 float B = 255. * col.b;

 float Y = 0.257 * R + 0.504 * G + 0.098 * B + 16.;
 float Cb = -0.148 * R - 0.291 * G + 0.439 * B + 128.;
 float Cr = 0.439 * R - 0.368 * G - 0.071 * B + 128.;

 return vec3(Y, Cr, Cb) / 255.;
}

void main()
{
 vec4 color = texture(inputImage, TexCoord);
 vec3 ycrcb = RGB2YCrCb(color.rgb);
 FragColor = vec4(ycrcb, 1.0);
}