#define RGB565(r, g, b)  (((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3))
#define RGB332(r, g, b)  ((((r & 0xE0) >> 5) | (((g & 0xE0) >> 5) | (((b & 0xC0) >> 6))

static inline int min(int a, int b) {
    if (a > b) {
        return b;
    };
    return a;
}

static inline int max(int a, int b) {
    if (a > b) {
        return a;
    }
    return b;
}

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} rgb_t;

typedef struct {
    double h;
    double s;
    double l;
} hsl_t;


void hsl2rgb(hsl_t *hsl, rgb_t *rgb);
void rgb2hsl(rgb_t *rgb, hsl_t *hsl);

rgb_t rgb565_to_rgb888(uint16_t *input);
uint16_t rgb888_to_rgb565(rgb_t *input);