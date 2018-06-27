#ifndef __ising_solver__
#define __ising_solver__

#include <vector>
#include <random>
#include "random_selector.h"
#include "cost_function.h"

// minimize: Σs_i s_j J_{ij} + Σs_i h_i
// s := current_spin
// s_i: {-1, 1}
class IsingSolver {
  int steps, total_step;
  double cool_coe, update_ratio;
  std::mt19937 rnd;
  RandomSelector random_selector;
  double active_ratio; // 温度: [0, 1]
  const CostFunction cf;
  std::vector<int> current_spin, optimal_spin;
  Weight calcEnergyDiff(const std::vector<int>& spin, const int node_id) const;
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
  int calcTotalStep() const;
public:
  enum InitMode {
    Negative, Positive, Random
  };
  IsingSolver(const CostFunction& cf);
  Weight getCurrentEnergy() const;
  Weight getOptimalEnergy() const; // ただし現在の目的関数で計算
  Weight calcEnergy(const std::vector<int>& spin) const;
  const std::vector<int>& getCurrentSpin() const;
  const std::vector<int>& getOptimalSpin() const;
  void setCurrentSpin(const std::vector<int>& new_spin);
  void step();
  void init(const InitMode mode, const double cool_coe, const double update_ratio);
  void init(const InitMode mode, const int seed, const double cool_coe, const double update_ratio);
  size_t getActiveNodeCount() const;
  size_t getUpdateNodeCount() const;
  size_t size() const;
  int getStep() const;
  int getTotalStep() const;
  double getCurrentPer() const;
};

#endif
