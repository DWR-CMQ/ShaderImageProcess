#version 330 core
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D iChannel0;
uniform sampler2D iChannel1;

vec4 LookUp(in vec4 textureColor)
{
    mediump float b = textureColor.b * 63.0;
    mediump vec2 quad1;
    quad1.y = floor(floor(b) / 8.0);
    quad1.x = floor(b) - quad1.y * 8.0;
    mediump vec2 quad2;
    quad2.y = floor(ceil(b) / 8.0);
    quad2.x = ceil(b) - quad2.y * 8.0;

    highp vec2 leftTop;
    leftTop.x = quad1.x * 0.125 + 0.5 / 512.0 + (0.125 - 1.0 / 512.0) * textureColor.r;
    leftTop.y = quad1.y * 0.125 + 0.5 / 512.0 + (0.125 - 1.0 / 512.0) * textureColor.g;

    highp vec2 rightBottom;
    rightBottom.x = quad2.x * 0.125 + 0.5 / 512.0 + (0.125 - 1.0 / 512.0) * textureColor.r;
    rightBottom.y = quad2.y * 0.125 + 0.5 / 512.0 + (0.125 - 1.0 / 512.0) * textureColor.g;

    lowp vec4 newColor1 = texture2D(iChannel1, leftTop);
    lowp vec4 newColor2 = texture2D(iChannel1, rightBottom);
    lowp vec4 newColor = mix(newColor1, newColor2, fract(b));
    return newColor;
}

void main()
{
    vec4 originColor = texture2D(iChannel0, TexCoord);
    // originColor: normalized
    vec4 lutColor = LookUp(originColor);

        /// vignette
    #if 0
        uv *=  1.0 - uv.yx;   //vec2(1.0)- uv.yx; -> 1.-u.yx; Thanks FabriceNeyret !
        float vig = uv.x * uv.y * 15.0; // multiply with sth for intensity
        vig = pow(vig, 0.25); // change pow for modifying the extend of the  vignette
    #endif
    FragColor = vec4(lutColor.rgb, originColor.w);
}