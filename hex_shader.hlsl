/*

HLSL whiteboarding code to test hex grid algos.
https://www.shadertoy.com/new

*/

vec3 cube_round(vec3 frac) {
    float q = round(frac.x);
    float r = round(frac.y);
    float s = round(frac.z);

    float q_diff = abs(q - frac.x);
    float r_diff = abs(r - frac.y);
    float s_diff = abs(s - frac.z);

    if (q_diff > r_diff && q_diff > s_diff) {
        q = -r-s;
    } else if (r_diff > s_diff) {
        r = -q-s;
    } else {
        s = -q-r;
    }
    
    return vec3(q, r, s);
}

vec3 axial_to_cube(vec2 hex) {
    float q = -hex.x - hex.y;
    return vec3(hex.x, hex.y, q);
}
[]
vec2 cube_to_axial(vec3 cube) {
    return vec2(cube.x, cube.y);
}

vec2 axial_round(vec2 hex) {
    return cube_to_axial(cube_round(axial_to_cube(hex)));
}

vec2 axial_round_branchless(vec2 hex) {
  float x = hex.x, y = hex.y;
  float xgrid = round(x), ygrid = round(y);
  x -= xgrid, y -= ygrid;
  float dx = round(x + 0.5*y) * step(y*y, x*x);
  float dy = round(y + 0.5*x) * step(x*x, y*y);
  return vec2(xgrid + dx, ygrid + dy);
}

float size = 50.;


vec2 pixel_to_pointy_hex(vec2 point) {
    float q = (sqrt(3.)/3. * point.x  -  1./3. * point.y) / size;
    float r = (                        2./3. * point.y) / size;
    return axial_round_branchless(vec2(q, r));
    //return axial_round(vec2(q, r));
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    // Normalized pixel coordinates (from 0 to 1)
    vec2 uv = fragCoord/iResolution.xy;

    // Time varying pixel color
    vec3 col = 0.5 + 0.5*cos(iTime+uv.xyx+vec3(0,2,4));


    float fcol = 2.;
    vec2 hex = pixel_to_pointy_hex(fragCoord);
    float qMod = mod(hex.x, fcol) / fcol;
    float rMod = mod(hex.y, fcol) / fcol;

    // Output to screen
    fragColor = vec4(qMod, rMod, 0., 1.0);
}

