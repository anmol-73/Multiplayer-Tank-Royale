#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;

// Input uniform values
uniform sampler2D texture0;
uniform float time; // For animated randomness
uniform vec3 fogColor; // Color of the fog
uniform float fogDensity; // Higher value = denser fog

// Output fragment color
out vec4 finalColor;

// Parameters
const float brightnessThreshold = 0.6;
const float bloomIntensity = 1.8;
const float darkenFactor = 0.4;

// Blur settings
const int blurRadius = 6;  // Controls blur strength
const float blurSpread = 1.5;

// Random noise function
float random(vec2 uv) {
    return fract(sin(dot(uv.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

// Cloud function: creates visible smudges
float cloudSmudge(vec2 uv) {
    float noise = random(uv * time * 0.1) * 0.6;
    noise += random(uv * 2.0) * 0.4;
    noise *= step(0.3, noise); // Adds more contrast
    return noise * 0.5; // Increase visibility
}

void main()
{
    // Fetch original texture color
    vec4 color = texture(texture0, fragTexCoord);
    float luminance = dot(color.rgb, vec3(0.299, 0.587, 0.114));
    vec2 texelSize = 1.0 / textureSize(texture0, 0);

    // Box Blur: Apply a soft glow around bright areas
    vec3 blurredColor = vec3(0.0);
    float weightSum = 0.0;

    for (int i = -blurRadius; i <= blurRadius; i++) {
        for (int j = -blurRadius; j <= blurRadius; j++) {
            vec2 offset = vec2(float(i), float(j)) * texelSize * blurSpread;
            vec4 sampleColor = texture(texture0, fragTexCoord + offset);
            float sampleLuminance = dot(sampleColor.rgb, vec3(0.299, 0.587, 0.114));
            vec3 brightPart = (sampleLuminance > brightnessThreshold) ? sampleColor.rgb : vec3(0.0);
            blurredColor += brightPart;
            weightSum += 1.0;
        }
    }

    blurredColor /= weightSum;
    vec3 bloomEffect = blurredColor * bloomIntensity;

    // Blend bloom with the original image
    vec3 finalImage = color.rgb + bloomEffect;

    // Normalize output to avoid overexposure
    float maxIntensity = max(finalImage.r, max(finalImage.g, finalImage.b));
    if (maxIntensity > 1.0) {
        finalImage /= maxIntensity;
    }

    // Darken non-bright areas
    if (luminance < brightnessThreshold) {
        finalImage *= darkenFactor;
    }

    // Apply FOG: Make it cover more of the scene
    float fogFactor = exp(-pow(fragTexCoord.y * fogDensity, 1.5)); // Adjust curve
    finalImage = mix(fogColor, finalImage, fogFactor);

    // Add cloud smudges (Make them visible)
    float cloud = cloudSmudge(fragTexCoord * 6.0);
    finalImage += cloud * 0.4; // Increase intensity of smudges

    // Output final color
    finalColor = vec4(finalImage, 1.0);
}
