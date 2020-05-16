#include <stdint.h>

#include "hsv.h"

rgb_t hsv_to_rgb(hsv_t *hsv)
{
    float r, g, b, h, s, v;
    rgb_t rgb;

    h = hsv->h / 256.0;
    s = hsv->s / 256.0;
    v = hsv->v / 256.0;

    /* Saturation 0 means shade of grey. */
    if (s == 0) {
        r = g = b = v;
    } else {
        float f, p, q, t;
        uint8_t i;
        h *= 6;
        i = (uint8_t)floor(h);
        f = h - i;
        p = v * (1 - s);
        q = v * (1 - (s * f));
        t = v * (1 - (s * (1 - f)));
        switch(i) {
            case 0: r = v; g = t; b = p; break;
            case 1: r = q; g = v; b = p; break;
            case 2: r = p; g = v; b = t; break;
            case 3: r = p; g = q; b = v; break;
            case 4: r = t; g = p; b = v; break;
            case 5: r = v; g = p; b = q; break;
        }
    }

    rgb.r = (uint8_t)(r * 255.0);
    rgb.g = (uint8_t)(g * 255.0);
    rgb.b = (uint8_t)(b * 255.0);

    return rgb;
}