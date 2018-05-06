#include "Sobel.h"

// sobel mask
int mask[MASK_N][MASK_X][MASK_Y] = {{{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}},

                                    {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}}};

int color_to_int(int r, int g, int b) { return (r + g + b) / 3; }

int sobel(double threshold, unsigned int width, unsigned int height,
          unsigned int byte_per_pixel, char *image_s, char *image_t) {
  unsigned int x, y, i, v, u; // for loop counter
  unsigned char R, G, B;      // color of R, G, B
  double val[MASK_N] = {0.0};
  int adjustX, adjustY, xBound, yBound;
  double total;

  for (y = 0; y != height; ++y) {
    for (x = 0; x != width; ++x) {
      for (i = 0; i != MASK_N; ++i) {
        adjustX = (MASK_X % 2) ? 1 : 0;
        adjustY = (MASK_Y % 2) ? 1 : 0;
        xBound = MASK_X / 2;
        yBound = MASK_Y / 2;

        val[i] = 0.0;
        for (v = -yBound; v != yBound + adjustY; ++v) {
          for (u = -xBound; u != xBound + adjustX; ++u) {
            if (x + u >= 0 && x + u < width && y + v >= 0 && y + v < height) {
              R = *(image_s + byte_per_pixel * (width * (y + v) + (x + u)) + 2);
              G = *(image_s + byte_per_pixel * (width * (y + v) + (x + u)) + 1);
              B = *(image_s + byte_per_pixel * (width * (y + v) + (x + u)) + 0);

              val[i] += color_to_int(R, G, B) * mask[i][u + xBound][v + yBound];
            }
          }
        }
      }

      total = 0.0;
      for (i = 0; i != MASK_N; ++i) {
        total += val[i] * val[i];
      }

      total = sqrt(total);

      if (total - threshold >= 0) {
        // black
        *(image_t + byte_per_pixel * (width * y + x) + 2) = BLACK;
        *(image_t + byte_per_pixel * (width * y + x) + 1) = BLACK;
        *(image_t + byte_per_pixel * (width * y + x) + 0) = BLACK;
      } else {
        // white
        *(image_t + byte_per_pixel * (width * y + x) + 2) = WHITE;
        *(image_t + byte_per_pixel * (width * y + x) + 1) = WHITE;
        *(image_t + byte_per_pixel * (width * y + x) + 0) = WHITE;
      }
    }
  }

  return 0;
}

Sobel::Sobel(sc_module_name n) : sc_module(n) {}
