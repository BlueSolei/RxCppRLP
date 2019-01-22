#include <iostream>
#include <rxcpp/rx.hpp>
#include <sstream>

using namespace std;

auto CurrentTID() {
  std::stringstream ss;
  ss << std::this_thread::get_id();
  uint64_t id = std::stoull(ss.str());
  return id;
}

int main() {
  using namespace rxcpp::operators;

  //------- Instantiate a run_loop object
  //------- which will loop in the main thread
  // once defined, its now the default scheduler.
  rxcpp::schedulers::run_loop rlp;

  auto rlpValues1 =
      rxcpp::observable<>::range(0, 15)
          .map([](int i) {
            printf("RLP mode: Map %d, tid: %lld\n", i, CurrentTID());
            return i;
          })
          .take(5);

  rlpValues1.subscribe(
      [&](int i) { printf("RLP mode: Sub %d, tid: %lld\n", i, CurrentTID()); });

  printf("This will be the first line, as the above expression is evaluated in "
         "the "
         "RLP\n\n");

  printf("Immediate mode\n");
  auto immediateValues =
      rxcpp::observable<>::range(1, 5, rxcpp::identity_immediate());
  immediateValues.subscribe(
      [](int v) { printf("OnNext: %d, tid: %lld\n", v, CurrentTID()); },
      []() { printf("OnCompleted, tid: %lld\n", CurrentTID()); });
  printf("\n");

  auto rlpValues2 =
      rxcpp::observable<>::range(45, 100)
          .map([](int i) {
            printf("Explicit RLP mode: Map %d, tid: %lld\n", i, CurrentTID());
            return i;
          })
          .take(5);

  rlpValues2.subscribe([&](int i) {
    printf("Explicit RLP mode: Sub %d, tid: %lld\n", i, CurrentTID());
  });

  //------------ Execute the Run Loop
  while (!rlp.empty()) {
    while (!rlp.empty() && rlp.peek().when < rlp.now()) {
      rlp.dispatch();
    }
  }

  printf("\n");
  printf("Bye\n");

  return 0;
}