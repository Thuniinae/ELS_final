#include "Filter.h"
Filter::Filter( sc_module_name n ): sc_module( n )
{
	SC_THREAD( do_filter );
	sensitive << i_clk.pos();
	dont_initialize();
}

Filter::~Filter() {}

// sobel mask
int mask[MASK_N][MASK_X][MASK_Y] = {{{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}},

                                    {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}}};

int color_to_int(int r, int g, int b) { return (r + g + b) / 3; }

void Filter::do_filter() {
	unsigned char rSpace[MASK_X][MASK_Y] = {0};
	unsigned char gSpace[MASK_X][MASK_Y] = {0};
	unsigned char bSpace[MASK_X][MASK_Y] = {0};
  int val[MASK_N] = {0};
	while (true) {
		for (unsigned int v = 0; v<MASK_Y; ++v) {
			for (unsigned int u = 0; u<MASK_X; ++u) {
				rSpace[u][v] = i_r.read();
				gSpace[u][v] = i_g.read();
				bSpace[u][v] = i_b.read();
			}
		}
		for (unsigned int i = 0; i<MASK_N; ++i) {
			val[i] = 0;
		}
		for (unsigned int v = 0; v<MASK_Y; ++v) {
			for (unsigned int u = 0; u<MASK_X; ++u) {
				for (unsigned int i = 0; i != MASK_N; ++i) {
					val[i] += color_to_int(rSpace[u][v], gSpace[u][v], bSpace[u][v]) * mask[i][u][v];
				}
			}
		}
		int total = 0;
		for (unsigned int i = 0; i != MASK_N; ++i) {
			total += val[i] * val[i];
		}
		o_result.write(sqrt(total));
	}
}
