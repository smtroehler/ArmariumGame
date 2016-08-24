#version 330 core

// Input vertex data, different for all executions of this shader.
in vec3 squareVertices;
in vec4 xyzs; // Position of the center of the particule and size of the square
in vec2 TexCoord;
in float partLife;

// Output data ; will be interpolated for each fragment.
out vec2 UV;
out vec2 fragTexCoord;

// Values that stay constant for the whole mesh.
uniform vec3 CameraRight_worldspace;
uniform vec3 CameraUp_worldspace;
uniform mat4 VP; // Model-View-Projection matrix, but without the Model (the position is in BillboardPos; the orientation depends on the camera)

void main()
{
	float particleSize = xyzs.w; // because we encoded it this way.
	vec3 particleCenter_wordspace = xyzs.xyz;
	
	vec3 vertexPosition_worldspace = 
		particleCenter_wordspace
		+ CameraRight_worldspace * squareVertices.x * particleSize
		+ CameraUp_worldspace * squareVertices.y * particleSize;

	// Output position of the vertex
	gl_Position = VP * vec4(vertexPosition_worldspace, 1.0);

	// UV of the vertex. No special space for this one.
	UV = squareVertices.xy + vec2(0.5, 0.5);
	fragTexCoord = vec2(min(1.0, TexCoord.x +  0.2 * int(mod(partLife, 2.0))), min(1.0, TexCoord.y + 0.2 * int(partLife / 2.0)));
}
