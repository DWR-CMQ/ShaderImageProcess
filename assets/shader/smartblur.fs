#version 330 core
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D inputImage;
uniform int radius = 3;
uniform float threshold = 1.0;
uniform vec2 resolution;

vec3 YCrCb2RGB(vec3 col)
{
 float Y = col.r * 255.;
 float Cr = col.g * 255.;
 float Cb = col.b * 255.;

 float R = 1.164 * (Y - 16.) + 1.596 * (Cr - 128.);
 float G = 1.164 * (Y - 16.) - 0.813 * (Cr - 128.) - 0.392 * (Cb - 128.);
 float B = 1.164 * (Y - 16.) + 2.017 * (Cb - 128.);

 return vec3(R, G, B) / 255.;
}

float SmartBlur(vec2 uv, int radius, float threshold)
{
 float gray = texture(inputImage, uv).r * 255.;

 float low = clamp(gray - threshold, 0., 255.);
 float high = clamp(gray + threshold, 0., 255.);

 float sum = 0.;
 float count = 0.;
 float temp = 0.;
 for (int n = -radius; n <= radius; n++)
 {
 for (int m = -radius; m <= radius; m++)
 {
 vec2 pos = clamp(uv + vec2(m, n) / resolution, 0., 1.);
 temp = texture(inputImage, pos).r * 255.;
 if (temp > low && temp < high)
 {
 sum += temp;
 count++;
 }
 }
 }

 gray = count < 1e-2 ? gray : sum / count;
 gray = clamp(gray, 0., 255.);

 return gray / 255.;
}

void main()
{
 vec4 color = texture(inputImage, TexCoord);

 float y = SmartBlur(TexCoord, radius, threshold);
 vec3 ycrcb = vec3(y, color.gb);

 vec3 rgb = YCrCb2RGB(ycrcb);
 FragColor = vec4(rgb, 1.);
}