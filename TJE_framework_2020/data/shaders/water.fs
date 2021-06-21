varying vec3 v_position;
varying vec3 v_world_position;
varying vec3 v_normal;
varying vec2 v_uv;
varying vec4 v_color;

uniform vec4 u_color;
uniform sampler2D u_texture;
uniform sampler2D u_detail_texture;
uniform float u_time;
uniform float u_texture_tiling;
uniform vec3 u_camera_position;

void main()
{
    vec2 uv = v_world_position.xz;

	vec2 nu = uv*0.01;
	nu.x += sin(u_time * 0.3);

	vec2 nu2 = uv*0.03;
	nu2.y += sin(u_time*0.23);

	vec3 normal1 = normalize(texture2D(u_detail_texture, nu).xzy * 2.0 - vec3(1));
	vec3 normal2 = normalize(texture2D(u_detail_texture, nu2).xzy * 2.0 - vec3(1));

	vec3 textureNormal = mix(normal1, normal2, 0.5);

    vec3 N = normalize(vec3(0.0, 1.0, 0.0)); // normal
	float dist = normalize(u_camera_position - v_world_position);
    vec3 E = -normalize(u_camera_position - v_world_position); // eye

    vec3 R = reflect(E,N);

    float yaw = atan(R.x, R.z) / 6.28318531;
    yaw += -0.25;

    float pitch = asin(R.y) / 1.57079633;

    vec2 uv_reflection = vec2(yaw, clamp(pitch, 0.0, 1.0) );

    vec4 sky_color = texture2DLod( u_texture, uv_reflection, 0.0);
    sky_color.a = 0.7;

    gl_FragColor = u_color * sky_color;
}