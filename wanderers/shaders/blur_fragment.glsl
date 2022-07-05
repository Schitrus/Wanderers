#version 460 core
out vec4 frag_color;

in vec2 texture_coordinate;

uniform sampler2D frame;
uniform sampler2D strength;

uniform int width;
uniform int height;

//const int kDim = 5;
//const int kSize = kDim*kDim;

void main(){
	//vec2 offset = vec2(1.0f / width, 1.0f / height);
	//vec2 offsets[kSize];

	//for (int i = 0; i < kSize; i++){
	//	offsets[i] = vec2(0.0f, (i-kSize/2) * offset.y);
	//}

	//vec3 sample_texture[kSize];



	vec3 center_color = vec3(texture(frame, texture_coordinate));
	//int dim = 50;
	//for (int x = 0; x < dim; x++){
	//	for (int y = 0; y < dim; y++){
	//		float dx = (x - dim/2) * offset.x;
	//		float dy = (y - dim/2) * offset.y;
	//		vec3 pixel_color = vec3(texture(frame, texture_coordinate.st + vec2(dx, dy)));
	//		float pixel_strength = pow(vec3(texture(strength, texture_coordinate.st + vec2(dx, dy))).r, 16.0f);
	//		float X = abs(x - dim/2) + 1.0f;
	//		float Y = abs(y - dim/2) + 1.0f;
	//		float limit = 1.0f / (dim);
	//		float distance_strength = pow(max(1.0f / Y - limit, 0.0f), 1.0f);
	//		distance_strength += pow(max(1.0f / X - limit, 0.0f), 1.0f);
	//		center_color = vec3(max(center_color.r, pixel_color.r * distance_strength), 
	//		                    max(center_color.g, pixel_color.g * distance_strength),
	//							max(center_color.b, pixel_color.b * distance_strength));
	//	}
	//}



	//for (int i = 0; i < kSize; i++){
	//	vec3 samp = normalize(vec3(texture(frame, texture_coordinate.st + offsets[i])));
	//	float dis = pow(0.9f, abs(i-kSize/2));
	//	center = vec3(max(center.x, samp.x * dis), max(center.y, samp.y * dis), max(center.z, samp.z * dis));
	//}

	//float str = max(max(max(center_color.x, center_color.y), center_color.z), 1.0f);

	frag_color = vec4(center_color, 1.0f);
}