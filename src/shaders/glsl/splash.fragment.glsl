#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

uniform sampler2D texture0;
uniform float iTime;

out vec4 finalColor;

const float scanlineIntensity = 0.35;
const float scanlineCount = 50.0;
const float scanlineSpeed = 2.0;
const float vignetteIntensity = 0.2;
const float distortion = 0.2;
const float colorSeparation = 0.008;

vec2 curveRemapUV(vec2 uv) {
    uv = uv * 2.0 - 1.0;
    vec2 offset = abs(uv.yx) / vec2(4.0, 2.0);
    uv = uv + uv * offset * offset * distortion;
    return uv * 0.5 + 0.5;
}

float scanline(vec2 uv) {
    float scanPos = uv.y * scanlineCount - iTime * scanlineSpeed;
    float line = abs(fract(scanPos) - 0.5) * 2.0;
    return pow(line, 1.5);
}

void main() {
    vec2 remappedUV = curveRemapUV(fragTexCoord);

    if (remappedUV.x < 0.0 || remappedUV.x > 1.0 || remappedUV.y < 0.0 || remappedUV.y > 1.0) {
        finalColor = vec4(0.0, 0.0, 0.0, 1.0);
        return;
    }

    float separation = colorSeparation * (1.0 + 0.4 * sin(iTime * 4.0));
    vec4 baseColor;
    baseColor.r = texture(texture0, vec2(remappedUV.x + separation, remappedUV.y)).r;
    baseColor.g = texture(texture0, remappedUV).g;
    baseColor.b = texture(texture0, vec2(remappedUV.x - separation, remappedUV.y)).b;
    baseColor.a = texture(texture0, remappedUV).a;

    float scanlineEffect = mix(0.5, 1.0, scanline(remappedUV));

    vec4 finalEffect = baseColor * scanlineEffect;

    finalEffect.rgb *= vec3(0.95, 1.0, 0.95);
    finalEffect.rgb = pow(finalEffect.rgb, vec3(1.1));

    finalColor = finalEffect;
}
