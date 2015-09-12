#version 410 core

subroutine void renderingPass();
subroutine uniform renderingPass fragmentPass;

uniform int shadeMode;

struct MaterialInfo{
    float Metallic;
    float Roughness;
    float F0;
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

// IBL用のワールド座標
in vec3 worldPos;
in vec3 worldNor;

uniform vec3 cameraPosition;

uniform mat4 V;

// テクスチャは５枚まで使える
uniform sampler2D textures[3];
// キューブマップ
uniform samplerCube cubeMap;

// マルチレンダーターゲット
layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec3 PositionData;
layout(location = 2) out vec3 NormalData;
layout(location = 3) out vec3 ColorData;
layout(location = 4) out vec3 MaterialData;

// 2pass目の変数
uniform sampler2D PositionTex,NormalTex,ColorTex,MaterialTex;
out vec4 color;

const float PI = 3.141592f;


// 背景用
vec3 BackGround(vec4 pos,vec3 norm){
    vec3 worldView = normalize(worldPos - cameraPosition);
    return texture(cubeMap,worldView).xyz;
}


float G1V(float dotNV,float k){
    return dotNV/(dotNV*(1.0f-k)+k);
}

float GGX_Spec(vec3 n,vec3 v,vec3 l,float roughness,float F0){

    float alpha = roughness*roughness;
    vec3 h = normalize(v+l);

    float dotNL = max((dot(n,l)),0);
    float dotNV = max((dot(n,v)),0);
    float dotNH = max((dot(n,h)),0);
    float dotLH = max((dot(l,h)),0);

    float F,D,vis;

    // D
    float alphaSqr = alpha*alpha;
    float denom = (dotNH * dotNH)*0.999f * (alphaSqr-1.0) + 1.0f;
    D = alphaSqr/(PI*denom*denom);

    // F
    float dotLH5 = pow(1.0f-dotNL,5);
    F = F0 + (1.0-F0)*dotLH5;

    // V
    float k = (roughness+1)*(roughness+1)/8.0f;
    vis = G1V(dotNL,k)*G1V(dotNV,k);

    float specular = D * F * vis / (4.0) ;
    return specular;
}

float GGX_Diff(vec3 n,vec3 l){
    return max(dot(n,l),0)/PI;
}

vec3 SpecularIBL(vec3 world_n,vec3 world_v,float roughness,float F0){
    
    const uint PREC = 5;
    uint SAMPLE_NUM = PREC*PREC; 

    float thetaStep = 2.0 *PI / (PREC+2.0);
    float phiStep = 2.0 * PI / PREC; 

    world_n = normalize(world_n);
    world_v = normalize(world_v);
    
    vec3 color = vec3(0);
    
    for(int i=0;i<SAMPLE_NUM;++i){
        uint y = i%PREC;
        uint x = (i-y)/PREC;
        
        float theta = (y+1)*thetaStep;
        float phi = x*phiStep;

        vec3 l = vec3(sin(theta)*cos(phi), sin(theta)*sin(phi), cos(theta));
        
        color += (GGX_Spec(world_n,world_v,l,roughness,F0)*max(dot(world_n,l),0) + max(dot(world_n,l),0)) * texture(cubeMap,l).rgb; 
        // color += max(dot(world_n,l),0) * texture(cubeMap,l).rgb;
    }
    return color/SAMPLE_NUM; 
}

subroutine(renderingPass)
void pass1(){

    color.rgb = vec3(0,0,0);

    if(shadeMode==0){
        vec3 n = normalize(fragmentNormal);
        vec3 v = normalize(-fragmentPosition.xyz);
        
        
        for(int lightIndex = 0;lightIndex<10;++lightIndex){
            vec3 l = normalize(Light[lightIndex].position.xyz-fragmentPosition.xyz);

            color.rgb += (GGX_Spec(n,v,l,Material.Roughness,Material.F0) * Light[lightIndex].Ls * Material.Metallic
                    + GGX_Diff(n,l) * Light[lightIndex].Ls*(1-Material.Metallic)) * texture(textures[0],fragmentUV).xyz ;
        }
        // color.rgb += (1-pow(max(dot(fragmentNormal,-fragmentPosition.xyz),0),1)) * vec3(Material.Metallic,Material.Roughness,Material.F0);
        
        color.rgb += SpecularIBL(worldNor,cameraPosition-worldPos,Material.Roughness,Material.F0);
        // color.rgb = normalize(worldNor);
        V;

    }
    else{ color.rgb = BackGround(fragmentPosition,fragmentNormal) * vec3(1,1,1);}

    PositionData = fragmentPosition.xyz;
    NormalData = normalize(fragmentNormal);
    ColorData = vec3(texture(textures[0],fragmentUV));
    MaterialData = vec3(Material.Roughness,Material.Metallic,Material.F0);

}

subroutine(renderingPass)
void pass2(){
    color.rgb = vec3(0);
    /*
    color.rgb  = (vec3(texture(ColorTex,fragmentUV)))/2
        + vec3(texture(ColorTex,fragmentUV+vec2(-0.01,-0.01)))/8
        + vec3(texture(ColorTex,fragmentUV+vec2(0.01,-0.01)))/8
        + vec3(texture(ColorTex,fragmentUV+vec2(-0.01,0.01)))/8
        + vec3(texture(ColorTex,fragmentUV+vec2(0.01,0.01)))/8;
        */
    vec3 pos = vec3(texture(PositionTex,fragmentUV*2));
    vec3 nor = vec3(texture(NormalTex,fragmentUV*2));
    vec3 col = vec3(texture(ColorTex,fragmentUV*2));
    vec3 mat = vec3(texture(MaterialTex,fragmentUV*2));

    for(int lightIndex = 1;lightIndex<10;++lightIndex){
        vec3 l = normalize(Light[lightIndex].position.xyz - pos);

        color.rgb += (GGX_Spec(nor,-pos,l,mat[0],mat[2]) * Light[lightIndex].Ls * mat[1] 
                + GGX_Diff(nor,l) * Light[lightIndex].Ls*(1-mat[1])) * col;
        
    }

    if(fragmentUV.x<0.5 && fragmentUV.y<0.5){
        color.rgb = vec3(texture(PositionTex,fragmentUV*2));
        color.b = -color.b;
    }else if(fragmentUV.x > 0.5 && fragmentUV.y < 0.5){
        color.rgb = vec3(texture(NormalTex,vec2(fragmentUV.x*2,fragmentUV.y*2)));
    }else if(fragmentUV.x < 0.5 && fragmentUV.y > 0.5){
        color.rgb = vec3(texture(ColorTex,fragmentUV*2));
    }
    // color.rgb = normalize(Light[1].position.xyz);
    // color.rg = vec2(0);
    // color.b = -pos.z/100;
}

void main(){
    fragmentPass();
}
