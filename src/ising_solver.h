#ifndef __ising_solver__
#define __ising_solver__

#include <vector>
#include <random>

using Weight = long long;

struct Edge {
  int to;
  Weight weight;
  Edge(int to, Weight weight);
};

using Graph = std::vector<std::vector<Edge>>;

class IsingSolver {
  std::mt19937 rnd;
  Graph J;
  std::vector<Weight> h;
  std::vector<int> current_spin, optimal_spin;
  Weight calcEnergyCoe(const std::vector<int>& spin, const int node_id) const;
  Weight calcEnergy(const std::vector<int>& spin) const;
public:
  enum InitMode {
    Negative, Positive, Random
  };
  IsingSolver(const Graph& J, const std::vector<Weight>& h);
  Weight getCurrentEnergy() const;
  Weight getOptimalEnergy() const;
  const std::vector<int>& getCurrentSpin() const;
  const std::vector<int>& getOptimalSpin() const;
  void step();
  void init(const InitMode mode);
  void init(const InitMode mode, const int seed);
  size_t size() const;
};

#endif
