#version 410 core

subroutine vec3 shadeModelType(vec4 position,vec3 normal);
subroutine uniform shadeModelType shadeModel;

uniform int shadeMode;

struct MaterialInfo{
    vec3 Ka;    // アンビエント反射率
    vec3 Kd;    // ディフューズ反射率
    vec3 Ks;    // スペキュラ反射率
    float shininess;    // スペキュラ輝き係数
    float waterness;    // 水面反射率
};
uniform MaterialInfo Material;

struct LightInfo{
    vec4 position;
    vec3 La;    // アンビエント強度RGB
    vec3 Ld;
    vec3 Ls;
};
uniform LightInfo Light[10];

// ビュー座標に統一
in vec4 fragmentPosition;
in vec4 fragmentColor;
in vec3 fragmentNormal;
in vec2 fragmentUV;

noperspective in vec4 screenPosition;

in vec3 worldPos;
in vec3 worldNor;

uniform vec3 cameraPosition;

uniform mat4 V;

uniform sampler2D textures[5];
// キューブマップ
uniform samplerCube cubeMap;
// フレームバッファが書き込むテクスチャ
uniform sampler2D fbo;

out vec4 color;


vec3 ToonShading(vec3 color){

    int levels = 3;
    return floor(color * levels) / levels; 

}

// 背景用
subroutine(shadeModelType)
vec3 BackGround(vec4 pos,vec3 norm){
    vec3 worldView = normalize(worldPos - cameraPosition);
    return texture(cubeMap,worldView).xyz;
}



// Blinn-Phongモデル
subroutine(shadeModelType)
vec3 BlinnPhongADS(vec4 pos, vec3 norm){

    vec3 ambient = vec3(0,0,0);
    vec3 diffuse = vec3(0,0,0);
    vec3 spec = vec3(0,0,0);

    for(int lightIndex=0;lightIndex<10;lightIndex++){

        norm = normalize(norm);
        // s:positionから見た光の方向　v:視点方向 h:ハーフベクトル
        vec3 s = normalize(Light[lightIndex].position.xyz - pos.xyz);
        vec3 v = normalize(-pos.xyz);
        vec3 h = normalize(s+v);

        ambient = ambient +  Light[lightIndex].La * Material.Ka;

        float SdotN = max( dot(s,norm) , 0 );
        diffuse = diffuse + Light[lightIndex].Ld * Material.Kd * SdotN;

        if(SdotN > 0.0)
            spec = spec + Light[lightIndex].Ls * Material.Ks * pow( max(dot(h,norm),0.0), Material.shininess );

    }

    // キューブマップの参照方向
    vec3 worldView = normalize(worldPos - cameraPosition);
    vec3 reflectDir = reflect(worldView,worldNor); 

    float VdotN = max(dot(normalize(-pos.xyz),normalize(norm)),0);

       // return (ambient + diffuse) * texture(textures[0],fragmentUV).xyz + spec; 

    float A = 0.8f;
    float B = VdotN;
    float C = sqrt(1-A*A*(1-B*B));
    float R = pow((A*B-C)/(A*B+C),2) + pow((A*C-B)/(A*C+B),2);
    float F = 0.05f;
    // R = F + (1-F)*pow(1-VdotN,5);


     return (ambient+diffuse)*0.4 +  
        R*(spec+texture(cubeMap,reflectDir).xyz)
        + (1-R)*(texture(cubeMap,refract(normalize(worldView),worldNor,A)).xyz * 0.7f + vec3(1,0.5,0.5)*0.3f)
        + vec3(0,0,1)*(R);

}


void main(){

    color.rgb = shadeModel(fragmentPosition,fragmentNormal);
    if(shadeMode == 0) color.rgb = BlinnPhongADS(fragmentPosition,fragmentNormal);
    else color.rgb = BackGround(fragmentPosition,fragmentNormal);

}
