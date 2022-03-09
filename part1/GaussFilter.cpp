#include <cmath>

#include "GaussFilter.h"

GaussFilter::GaussFilter(sc_module_name n) : sc_module(n) {
  SC_THREAD(do_filter);
  sensitive << i_clk.pos();
  dont_initialize();
  reset_signal_is(i_rst, false);
}

// Gauss mask
const double mask[MASK_X][MASK_Y] =
{ {1, 2, 1}, {2, 4, 2}, {1, 2, 1} };

double factor = 1.0 / 16.0;
double bias = 0.0;

void GaussFilter::do_filter() {
  while (true) {
    double red = 0.0, green = 0.0, blue = 0.0;
    for (unsigned int v = 0; v < MASK_Y; ++v) {
      for (unsigned int u = 0; u < MASK_X; ++u) {
        red += i_r.read() * mask[u][v];
        green += i_g.read() * mask[u][v];
        blue += i_b.read() * mask[u][v];
      }
    }
    double result_r = (double) std::min(std::max(int(factor * red + bias), 0), 255);
    double result_g = (double) std::min(std::max(int(factor * green + bias), 0), 255);
    double result_b = (double) std::min(std::max(int(factor * blue + bias), 0), 255);
    o_result_r.write(result_r);
    o_result_g.write(result_g);
    o_result_b.write(result_b);
    wait(10); //emulate module delay
  }
}
