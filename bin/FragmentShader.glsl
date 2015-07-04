#version 410 core


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

// フォーンモデルに基づくADS計算
vec3 phongADS(vec4 pos, vec3 norm,int lightIndex){

    norm = normalize(norm);
    // s:positionから見た光の方向　r:光の反射方向　v:視点方向
    vec3 s = normalize(Light[lightIndex].position.xyz - pos.xyz);
    vec3 r = reflect(-s,norm);
    vec3 v = normalize(-pos.xyz);

    vec3 ambient = Light[lightIndex].La * Material.Ka;

    float SdotN = max( dot(s,norm) , 0 );
    vec3 diffuse = Light[lightIndex].Ld * Material.Kd * SdotN;

    vec3 spec = vec3(0.0);
    if(SdotN > 0.0)
        spec = Light[lightIndex].Ls * Material.Ks * pow( max(dot(r,v),0.0), Material.shininess );

    return (ambient + diffuse) * texture(textures[0],fragmentUV).xyz + spec;

}

// Blinn-Phongモデル
vec3 BlinnPhongADS(vec4 pos, vec3 norm,int lightIndex){

    norm = normalize(norm);
    // s:positionから見た光の方向　v:視点方向 h:ハーフベクトル
    vec3 s = normalize(Light[lightIndex].position.xyz - pos.xyz);
    vec3 v = normalize(-pos.xyz);
    vec3 h = normalize(s+v);

    vec3 ambient = Light[lightIndex].La * Material.Ka;

    float SdotN = max( dot(s,norm) , 0 );
    vec3 diffuse = Light[lightIndex].Ld * Material.Kd * SdotN;

    vec3 spec = vec3(0.0);
    if(SdotN > 0.0)
        spec = Light[lightIndex].Ls * Material.Ks * pow( max(dot(h,norm),0.0), Material.shininess );

    return (ambient + diffuse) * texture(textures[0],fragmentUV).xyz + spec;

}


void main(){
    vec3 lightIntensity = vec3(0.0);
    for(int i=0;i<10;i++){
        lightIntensity += BlinnPhongADS(fragmentPosition,fragmentNormal,i);
    }
    
    color.rgb = lightIntensity;
    
    vec3 worldView = normalize(worldPos - cameraPosition);
    vec3 reflectDir = reflect(worldView,worldNor); 
    color.rgb = lightIntensity + texture(cubeMap,reflectDir).xyz*Material.Ks;

    vec2 uboref = vec2((gl_FragCoord.x-0.5)/2560,-(gl_FragCoord.y-0.5)/1600);
    color.rgb = (1 - Material.waterness)*color.rgb + Material.waterness*texture(fbo,uboref).rgb;
    //color.rgb = (1-Material.waterness)*color.rgb + Material.waterness*vec3(gl_PointCoord.xy,0);
    

}
