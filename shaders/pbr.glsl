@vs vs
layout(location=0) in vec3 vPos;
layout(location=1) in vec3 vNormal;
layout(location=2) in vec3 vTangent;
layout(location=3) in vec2 vUV;

layout(location=0) out vec3 fPos;
layout(location=1) out vec3 fNormal;
layout(location=2) out vec2 fUV;
layout(location=3) out mat3 fTBN;

uniform mat4 uMVP;
uniform mat4 uModel;

const float normalOffset = 0.1;

void main() {
    // Compute binormal from vertex normal and tangent
    vec3 vertexBinormal = cross(vNormal, vTangent);
    // Compute fragment normal based on normal transformations
    mat3 normalMatrix = transpose(inverse(mat3(uModel)));

    // Compute fragment position based on model transformations
    fPos = vec3(uModel*vec4(vPos, 1.0f));
    fUV = vUV*2.0;
    fNormal = normalize(normalMatrix*vNormal);

    vec3 fragTangent = normalize(normalMatrix*vTangent);
    fragTangent = normalize(fragTangent - dot(fragTangent, fNormal)*fNormal);

    vec3 fragBinormal = normalize(normalMatrix*vertexBinormal);
    fragBinormal = cross(fNormal, fragTangent);

    fTBN = transpose(mat3(fragTangent, fragBinormal, fNormal));

    gl_Position = uMVP * vec4(vPos, 1.0);
}
@end

@fs fs
uniform texture2D iTexChannel0;
uniform sampler iSmpChannel0;
layout(location=0) in vec2 texUV;
layout(location=1) in vec4 iColor;
layout(location=0) out vec4 fragColor;
void main() {
    fragColor = texture(sampler2D(iTexChannel0, iSmpChannel0), texUV) * iColor;
}
@end

@program program vs fs
