#version 410 core 

float aspect = 0.625;

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexColor;
layout(location = 2) in vec3 vertexNormal;
layout(location = 3) in vec2 vertexUV;

uniform mat4 MVP;
uniform mat4 MV;
uniform mat4 M;

uniform vec3 cameraPosition;

noperspective out vec4 screenPosition;
out vec4 fragmentPosition;
out vec4 fragmentColor;
out vec3 fragmentNormal;
out vec2 fragmentUV;

out vec3 worldPos;
out vec3 worldNor;
out vec4 viewPosition;

void main(){

    gl_Position.xyz = vertexPosition_modelspace;
    gl_Position.w = 1.0;
    gl_Position = MVP * gl_Position; 
    gl_Position.x = gl_Position.x * aspect;
    screenPosition = gl_Position;
    fragmentPosition =  MV * vec4(vertexPosition_modelspace,1);

    fragmentColor = vec4(vertexColor,0.0);

    vec4 normal = vec4(normalize(vertexNormal),0.0);
    fragmentNormal = (MV* normal).xyz;

    fragmentUV = vertexUV;

    // 反射方向の計算
    worldPos = vec3( M * vec4(vertexPosition_modelspace,1));
    worldNor = normalize(vec3( M * vec4(vertexNormal,0)).xyz);

}


// gl_Position に頂点座標を格納．ラスタライザに送られる
