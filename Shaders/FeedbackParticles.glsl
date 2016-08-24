#version 330
//adapted from http://prideout.net/blog/?tag=opengl-transform-feedback
in vec3 Position;
in float BirthTime;
in vec3 Velocity;

out vec3 vPosition;
out float vBirthTime;
out vec3 vVelocity;

uniform float Time;
uniform float TimeStep = 5.0;
 
float randhash(uint seed, float b)
{
    uint i=(seed^12345391u)*2654435769u;
    i^=(i<<6u)^(i>>26u);
    i*=2654435769u;
    i+=(i<<5u)^(i>>12u);
    return float(b * i) * InverseMaxInt;
}

void main(){
    vPosition = Position;
    vBirthTime = BirthTime;

    //create new again
    if (vBirthTime == 0.0) {
    	uint seed = uint(Time * 1000.0) + uint(gl_VertexID);
    	vPosition.x = randhash(seed++, 1.0);
    	vPosition.y = randhash(seed++, 1.0);
    	vPosition.z = randhash(seed++, 1.0);
    	vBirthTime = Time;
    }

	vVelocity = vec3(randhash(seed++, 1.0),randhash(seed++, 1.0),randhash(seed++, 1.0));
	vPosition += TimeStep * vVelocity;
}