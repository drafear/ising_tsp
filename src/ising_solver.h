#ifndef __ising_solver__
#define __ising_solver__

#include <vector>
#include <random>
#include "random_selector.h"

using Weight = long long;

struct Edge {
  int to;
  Weight weight;
  Edge(int to, Weight weight);
};

using Graph = std::vector<std::vector<Edge>>;

// minimize: Σs_i s_j J_{ij} + Σs_i h_i
// s := current_spin
// s_i: {-1, 1}
class IsingSolver {
  const double CoolCoe = 0.99;
  const double FlipRatio = 0.99;
  std::mt19937 rnd;
  RandomSelector random_selector;
  double active_ratio; // 温度: [0, 1]
  Graph J, revJ; // Σs_i s_j J_{ij}
  std::vector<Weight> h; // Σs_i h_i
  std::vector<int> current_spin, optimal_spin;
  Weight calcEnergyDiff(const std::vector<int>& spin, const int node_id) const;
  Weight calcEnergy(const std::vector<int>& spin) const;
  // active_ratio に応じた個数をランダムに current_spin を反転させる
  void randomFlip();
  // active_ratio に応じた個数をランダムな順番で updateNode する
  void updateNodes();
  // 決定的な動作を1ステップ行う
  void updateNode(const int node_id);
  // 温度を下げる
  void cool();
  // より良い解が見つかったら optimal_spin を更新
  void updateOptimalSpin();
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
  size_t getActiveNodeCount() const;
  size_t size() const;
};

#endif
