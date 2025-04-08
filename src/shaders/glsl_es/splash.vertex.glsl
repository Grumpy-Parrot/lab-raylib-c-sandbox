precision mediump float;

attribute vec3 vertexPosition;
attribute vec2 vertexTexCoord;
attribute vec4 vertexColor;

varying vec2 fragTexCoord;
varying vec4 fragColor;

uniform mat4 mvp;

void main() {
    fragTexCoord = vertexTexCoord;
    fragColor = (vertexColor.x == 0.0 && vertexColor.y == 0.0 && vertexColor.z == 0.0 && vertexColor.w == 0.0)
                ? vec4(1.0) : vertexColor; // Default to white if color is zero
    gl_Position = mvp * vec4(vertexPosition, 1.0);
}
