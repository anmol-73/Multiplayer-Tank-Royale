#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

// Output fragment color
out vec4 finalColor;



// Parameters
const float brightnessThreshold = 0.5;
const float bloomIntensity = 0.13;
const float darkenFactor = 0.85;

// Blur settings
// NOTE: Add your custom variables here

const vec2 size = vec2(1920, 1080);   // Framebuffer size
const float samples = 2;          // Pixels per axis; higher = bigger glow, worse performance
const float quality = 4;          // Defines size factor: Lower = smaller glow, better quality

void main()
{
    vec4 sum = vec4(0);
    vec2 sizeFactor = vec2(1)/size*quality;

    // Texel color fetching from texture sampler
    vec4 source = texture(texture0, fragTexCoord);
    vec4 tmp;

    // Fetch original texture color
    vec4 color = texture(texture0, fragTexCoord);
    float luminance = dot(color.rgb, vec3(0.299, 0.587, 0.114));

    if(luminance < brightnessThreshold) source*= darkenFactor;
    vec2 texelSize = 1.0 / textureSize(texture0, 0);



    // Darken non-bright areas
    if (luminance < brightnessThreshold) {
        finalColor = vec4(0,0,0,0);
    }
    const int range = 2;            // should be = (samples - 1)/2;

    for (int x = -range; x <= range; x++)
    {
        for (int y = -range; y <= range; y++)
        {   
            tmp = texture(texture0, fragTexCoord + vec2(x, y)*sizeFactor);
            luminance = dot(tmp.rgb, vec3(0.299, 0.587, 0.114));
            if(luminance > brightnessThreshold){
                sum +=tmp*bloomIntensity;
            }
        }
    }

    // Calculate final fragment color
    finalColor = ((sum/(samples*samples)) + source)*colDiffuse;
}