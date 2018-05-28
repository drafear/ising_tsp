#ifndef __ising_solver__
#define __ising_solver__

#include <vector>

using Weight = long long;

struct Edge {
  int to;
  Weight weight;
  Edge(int to, Weight weight);
};

using Graph = std::vector<std::vector<Edge>>;
using Spin = std::vector<int>;

#endif
