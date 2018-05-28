#ifndef __problem__
#define __problem__

#include "ising_solver.h"
#include <vector>
#include <complex>

class Answer;
class Problem {
public:
    std::vector<std::complex<double>> points;
    Problem(std::vector<std::complex<double>>&& points);
    size_t size() const;
    static Problem fromIstream(std::istream& is);
    std::pair<Graph, std::vector<Weight>> getJhForIsing() const;
    Answer getAnswerFromIsing(const Spin& spin) const;
};
class Answer {
    const Problem prob;
    const std::vector<int> order;
public:
    Answer(const Problem& prob, const std::vector<int>& order);
    void output(std::ostream& os);
};

#endif
