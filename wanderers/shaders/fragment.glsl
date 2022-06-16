#version 460 core
out vec4 frag_color;

uniform vec3 color;
uniform vec3 light_position;
uniform vec3 camera_position;

uniform bool is_sun;

in vec3 normal_frag;
in vec3 position_frag;
in vec3 model_position;

void main(){
    vec3 light_direction = normalize(light_position - position_frag);
    vec3 camera_direction = normalize(camera_position - position_frag);
    vec3 normal = normalize(normal_frag);
    vec3 reflect_direction = reflect(-camera_direction, normal);

    float ambient = 0.5f;
    float diffuse = max(dot(normal, light_direction), 0.0f);
    float specular = diffuse * 0.5f * pow(max(dot(camera_direction, reflect_direction), 0.0f), 16);

    vec3 object_color = is_sun ? color : color * (ambient + diffuse + specular);

    //vec3 up_color = vec3(1.0f, 0.0f, 0.0f);
    //vec3 down_color = vec3(0.0f, 0.0f, 1.0f);
    //vec3 front_color = vec3(0.0f, 1.0f, 0.0f);
    //
    //frag_color = vec4(mix(mix(mix(object_color, up_color, model_position.y), down_color, -model_position.y), front_color, model_position.x), 1.0f);

    frag_color = vec4(object_color, 1.0f);
} 