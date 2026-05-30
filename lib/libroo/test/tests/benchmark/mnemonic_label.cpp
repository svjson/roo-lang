#include "benchmark.h"
#include <gtest/gtest.h>

const std::string DEF_PARSE_MNEMONIC_LABEL = R"(
(def parse-mnemonic-label
  (fn [label marker trigger style index out mnemonic]
  (if (>= index (count label))
    {:text out
     :mnemonic mnemonic}
    (let [ch (nth label index)]
      (if (not= (str ch) marker)
        (parse-mnemonic-label label
                              marker
                              trigger
                              style
                              (inc index)
                              (str out ch)
                              mnemonic)
        (if (= index (dec (count label)))
          (parse-mnemonic-label label
                                marker
                                trigger
                                style
                                (inc index)
                                (str out ch)
                                mnemonic)
          (let [next-ch (nth label (inc index))]
            (if (= (str next-ch) marker)
              (parse-mnemonic-label label
                                    marker
                                    trigger
                                    style
                                    (+ index 2)
                                    (str out marker)
                                    mnemonic)
              (parse-mnemonic-label label
                                    marker
                                    trigger
                                    style
                                    (+ index 2)
                                    (str out next-ch)
                                    (or mnemonic
                                         {:char next-ch
                                          :index (count out)
                                          :trigger trigger
                                         :style style}))))))))))
)";

TEST(Benchmark_Lowering, parse_mnemonic_label__lower_x1000__exec_x10000)
{
  RooTest::SnippetBenchmark bm("parse_mnemonic_label__lower_x1000__exec_x10000",
                               "(do " + DEF_PARSE_MNEMONIC_LABEL +
                                 R"(
(dotimes [n 10000]
  (parse-mnemonic-label "Open _File __ menu _Again _"
                        "_"
                        :alt
                        :underline
                        0
                        ""
                        nil))
))");

  bm.with_lower_iterations(1000).run();
}
