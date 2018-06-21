#include "mylib.h"
#include "problem.h"
#include "ising_solver.h"
#include "mid.h"
#include "mid_grid.h"
#include "lib/cmdline.h"

#include <iostream>
#include <fstream>
#include <tuple>

using namespace std;

void run(const cmdline::parser& parser) {
  const string& input_file_path = parser.rest()[0];
  ifstream ifs(input_file_path);
  if (ifs.fail()) {
    cerr << "can't open the file: " << input_file_path << endl;
    exit(1);
  }
  MidWithGrid mid(Problem::fromIstream(ifs), 8);
  // Mid mid(Problem::fromIstream(ifs));
  const CostFunction cf = mid.getCostFunction();
  IsingSolver solver(cf);
  solver.init(IsingSolver::InitMode::Random, parser.get<double>("cool"), parser.get<double>("update-ratio"));
  bool is_detail = parser.exist("detail");
  const int ExtraStepCount = 10;
  bool is_first = true;
  while (solver.getStep() < solver.getTotalStep()+ExtraStepCount) {
    if (!is_first) solver.step();
    else is_first = false;
    cout << "[Step " << solver.getStep() << " / " << solver.getTotalStep()+ExtraStepCount << "]" << endl;
    cout << "energy: " << solver.getCurrentEnergy() << endl;
    if (is_detail) cout << "spin: " << solver.getCurrentSpin() << endl;
    cout << "flip: " << solver.getActiveNodeCount() << " / " << solver.size() << endl;
    Answer ans = mid.getAnswerFromSpin(solver.getCurrentSpin());
    ans.output(cout, is_detail);
    cout << "is_answer: " << boolalpha << ans.verify() << endl;
    cout << endl;
  }
  cout << "[Answer]" << endl;
  cout << "energy: " << solver.getOptimalEnergy() << endl;
  if (is_detail) cout << "spin: " << solver.getOptimalSpin() << endl;
  Answer ans = mid.getAnswerFromSpin(solver.getOptimalSpin());
  ans.output(cout, true);
  cout << "is_answer: " << boolalpha << ans.verify() << endl;
}
cmdline::parser get_command_line_parser() {
  cmdline::parser parser;
  parser.add<double>("cool", 'c', "coefficient of cooling", false, 0.999);
  parser.add<double>("update-ratio", 'u', "the ratio of nodes to update in 1 step", false, 0.3);
  parser.add("detail", 'd', "print log in detail");
  parser.footer("filename");
  return parser;
}
int main(int argc, char *argv[]) {
  auto parser = get_command_line_parser();
  parser.parse_check(argc, argv);
  if (parser.rest().size() < 1) {
    cerr << parser.usage();
    exit(1);
  }
  run(parser);
}
