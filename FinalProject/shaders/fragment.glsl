#version 330

const float RED_LUM_CONSTANT   = 0.2126;
const float GREEN_LUM_CONSTANT = 0.7152;
const float BLUE_LUM_CONSTANT  = 0.0722;
const float LINEAR_TERM        = 0.0005;  // linear term
const float QUADRATIC_TERM     = 0.00009; // quadratic term
const float MIN_BRIGHTNESS     = 0.05;    // avoid total darkness

uniform sampler2D texture0;
uniform vec2 lightPosition;

uniform int effect;

in vec2 fragTexCoord;
in vec2 fragPosition;

out vec4 finalColor;

float attenuate(float distance, float linearTerm, float quadraticTerm)
{
    float attenuation = 1.0 / (1.0 + 
                               linearTerm * distance + 
                               quadraticTerm * distance * distance);

    return max(attenuation, MIN_BRIGHTNESS);
}

void main()
{
    vec4 color = texture(texture0, fragTexCoord);
    if (effect == 1) {
        vec3 luminance = vec3(
            dot(
                vec3(RED_LUM_CONSTANT, GREEN_LUM_CONSTANT, BLUE_LUM_CONSTANT), 
                color.rgb
            )
        );
        float dist = distance(lightPosition, fragPosition);
        float brightness = attenuate(dist, LINEAR_TERM, QUADRATIC_TERM);
        finalColor = vec4(luminance.rgb*brightness, color.a);
    }
    else if (effect == 2) {
        vec3 redTint = vec3(1.2, 0.5, 0.8);
        vec3 darkened = color.rgb * 0.4;
        finalColor = vec4(darkened * redTint, color.a);
    }
    else if (effect == 3) {
        vec3 redTint = vec3(1.5, 0.2, 0.2);
        vec3 darkened = color.rgb * 0.5;
        finalColor = vec4(darkened * redTint, color.a);
    }
    
    else finalColor = vec4(color.rgb, color.a);

}