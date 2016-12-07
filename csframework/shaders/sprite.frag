#version 120

// Interpolated values from the vertex shaders
varying vec2 UV;

uniform vec3 rgb;
uniform float alpha;

// Values that stay constant for the whole mesh.
uniform sampler2D myTextureSampler;

void main()
{
	// Output color = color of the texture at the specified UV
	gl_FragColor = texture2D( myTextureSampler, UV ) * vec4(rgb.x,rgb.y,rgb.z,alpha);
}