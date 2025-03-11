#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

// Output fragment color
out vec4 finalColor;

// Gaussian weights for a larger 7x7 blur kernel
const float kernel[7] = float[](0.00598, 0.06062, 0.24184, 0.38310, 0.24184, 0.06062, 0.00598);

void main()
{
    // Texture size for sampling offsets
    vec2 texelSize = 1.0 / textureSize(texture0, 0);

    // Gaussian blur approximation (7x7 kernel)
    vec3 blurredColor = vec3(0.0);

    for (int i = -3; i <= 3; i++) {
        for (int j = -3; j <= 3; j++) {
            vec2 offset = vec2(float(i), float(j)) * texelSize * 2.0; // Increased blur strength
            blurredColor += texture(texture0, fragTexCoord + offset).rgb * kernel[abs(i)] * kernel[abs(j)];
        }
    }

    // Output blurred texture color
    finalColor = vec4(blurredColor, texture(texture0, fragTexCoord).a);
}
