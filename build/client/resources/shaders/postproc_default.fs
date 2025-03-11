#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;

// Input uniform values
uniform sampler2D texture0;

// Output fragment color
out vec4 finalColor;

// Parameters
const float brightnessThreshold = 1.1;
const float bloomIntensity = 2;
const float darkenFactor = 1; // How much darker low-brightness areas get

// 11x11 Gaussian blur kernel (approximated weights)
const float kernel[6] = float[](0.003, 0.014, 0.034, 0.059, 0.080, 0.091);

void main()
{
    // Fetch the original texture color
    vec4 color = texture(texture0, fragTexCoord);

    // Compute luminance (perceived brightness)
    float luminance = dot(color.rgb, vec3(0.299, 0.587, 0.114));

    // Texture size for sampling offsets
    vec2 texelSize = 1.0 / textureSize(texture0, 0);

    // Apply a very soft Gaussian blur (11x11 kernel) to bright areas
    vec3 blurredColor = vec3(0.0);
    float weightSum = 0.0;

    for (int i = -5; i <= 5; i++) {
        for (int j = -5; j <= 5; j++) {
            vec2 offset = vec2(float(i), float(j)) * texelSize * 2.0;
            vec4 sampleColor = texture(texture0, fragTexCoord + offset);

            // Compute luminance for this sample
            float sampleLuminance = dot(sampleColor.rgb, vec3(0.299, 0.587, 0.114));

            // Keep only bright areas
            vec3 brightPart = (sampleLuminance > brightnessThreshold) ? sampleColor.rgb : vec3(0.0);

            // Apply Gaussian weight
            float weight = kernel[abs(i)] * kernel[abs(j)];
            blurredColor += brightPart * weight;
            weightSum += weight;
        }
    }

    // Normalize the blurred color
    blurredColor /= weightSum;

    // Apply bloom intensity
    vec3 bloomEffect = blurredColor * bloomIntensity;

    // Blend bloom with the original image
    vec3 finalImage = color.rgb + bloomEffect;

    // Reduce brightness of low-brightness areas
    if (luminance < brightnessThreshold) {
        finalImage *= darkenFactor;
    }

    // Normalize final image to avoid overexposure
    float maxIntensity = max(finalImage.r, max(finalImage.g, finalImage.b));
    if (maxIntensity > 1.0) {
        finalImage /= maxIntensity;
    }

    // Output final color
    finalColor = vec4(finalImage, 1.0);
}
