#include <cmath>

#include "GaussFilter.h"
using namespace std;

GaussFilter::GaussFilter(sc_module_name n) : sc_module(n) {
  SC_THREAD(do_filter);
  sensitive << i_clk.pos();
  dont_initialize();
  reset_signal_is(i_rst, false);
}

// Gauss mask
const double mask[MASK_X][MASK_Y] = { {1, 2, 1}, {2, 4, 2}, {1, 2, 1} };
double factor = 1.0 / 16.0;
double bias = 0.0;
bool flag = false;
int height = 0;
unsigned char arr_r [3][256] = {0};
unsigned char arr_g [3][256] = {0};
unsigned char arr_b [3][256] = {0};

void GaussFilter::do_filter() {
  while (true) {
    // read rgb array
    if (flag == false) {
      for (int x = 0; x < 3; ++x) {
        for (int y = 0; y < 256; ++y) {
          arr_r[0][y] = arr_r[1][y];
          arr_g[0][y] = arr_g[1][y];
          arr_b[0][y] = arr_b[1][y];
          arr_r[1][y] = arr_r[2][y];
          arr_g[1][y] = arr_g[2][y];
          arr_b[1][y] = arr_b[2][y];
          arr_r[2][y] = i_r.read();
          arr_g[2][y] = i_g.read();
          arr_b[2][y] = i_b.read();
          wait(1);
        }
      }
      flag = true;
      ++height;
    } 
    else {
      for (int y = 0; y < 256; ++y) {
        arr_r[0][y] = arr_r[1][y];
        arr_g[0][y] = arr_g[1][y];
        arr_b[0][y] = arr_b[1][y];
        arr_r[1][y] = arr_r[2][y];
        arr_g[1][y] = arr_g[2][y];
        arr_b[1][y] = arr_b[2][y];
        arr_r[2][y] = i_r.read();
        arr_g[2][y] = i_g.read();
        arr_b[2][y] = i_b.read();
        wait(1);
      }
      ++height;
    }

    // gaussioan blur
    for (int x = 0; x < 256; ++x) {
      double red = 0.0, green = 0.0, blue = 0.0;
      for (int v = -1; v != 2; ++v) {   //-1, 0, 1
        for (int u = -1; u != 2; ++u) { //-1, 0, 1
          if (x + u >= 0 && x + u < 256 && height + v >= 0 && height + v < 256) {
            red += arr_r[v+1][x+u+1] * mask[u+1][v+1];
            green += arr_g[v+1][x+u+1] * mask[u+1][v+1];
            blue += arr_b[v+1][x+u+1] * mask[u+1][v+1];
          } else {
            red += 0;
            green += 0;
            blue += 0;
          }
        }
      }
      double result_r = (double) std::min(std::max(int(factor * red + bias), 0), 255);
      double result_g = (double) std::min(std::max(int(factor * green + bias), 0), 255);
      double result_b = (double) std::min(std::max(int(factor * blue + bias), 0), 255);
      o_result_r.write(result_r);
      o_result_g.write(result_g);
      o_result_b.write(result_b);
      wait(1);
    }
  }
}
