# ray_traCe
Small ray_tracing demo I made to learn how it works

main.c holds an example code:
```c
#include "body.h"
#include "matrix.h"
#include "output.h"
#include "texture.h"
#include "util.h"
#include <stdio.h>
#include <string.h>

int main() {
    body_rep sph1 = body_sphere_new(
        vec3(-15.0, 2.0, 30.0), 4.0,
        texture_new_single_color(color_new(0.1, 0.8, 0.4), 0.7, 0.1));
    body_rep sph2 =
        body_sphere_new(vec3(-5.0, -8.0, 10.0), 4.0,
                        texture_new_single_color(color_white(), 0.8, 0.1));
    body_rep sph3 = body_sphere_new(
        vec3(-15.0, 2.0, 20.0), 4.0,
        texture_new_single_color(color_new(0.5, 0.5, 0.9), 0.9, 0.15));
    body_rep sph4 = body_sphere_new(
        vec3(5.0, -4.0, 10.0), 4.0,
        texture_new_single_color(color_new(0.9, 0.3, 0.9), 0.7, 0.2));
    body_rep sph5 = body_sphere_new(
        vec3(1.0, 8.0, 15.0), 4.0,
        texture_new_single_color(color_new(0.5, 0.2, 0.9), 0.5, 0.05));
    body_rep flr1 = body_floor_new(
        -5.0, texture_new_single_color(color_new(0.0, 0.09, 0.5), 0.5, 0.0));

    const body_rep* bodies[] = {&sph1, &sph2, &sph3, &sph4, &sph5, &flr1};

    char* out = "./test.ppm";
    disp_ppm writer = {out, strlen(out)};
    display dp = display_init(1920, 1080, 60.0, vec_zero(), &writer, &ppm_out,
                              &no_free_func);
    display_run_rays(&dp, bodies, 6);
    display_write(&dp);
}
```

Result:
![Output image in PPM](/res/example.png)

## Building
One can use `make` or `make build` to build the project.

The make variable `DEBUG` can be set in order to build with debug flags. Such as:
`make DEBUG=1 build`

## Building docs
- If not already present, doxygen docs can be built with `make docbuild`
- If the docs are already built, one can rebuild it with `make docregen`
- I have also provided a convenient wrapper (linux only) to open the docs in the browser `make dochtmlopen`

## Note
While I have implemented matrix-matrix and matrix-vector multiplication along with a pretty error output system. I have not yet made use of it. It was just to see if I could and I like it too much to get rid of it right now.
