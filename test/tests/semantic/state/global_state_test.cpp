
#include <lisple/runtime.h>

#include <gtest/gtest.h>

TEST(Semantic_GlobalState, assoc_bang_on_nested_map__in_function)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval(R"(
     (def state {:selected-categories [:books :movies]
                 :hints {:overlay-type :overlay/context
                         :delay-time 60
                         :counter 60}})

     (defun tick-state! []
       (let [hint-state (:hints state)]
         (assoc! hint-state :counter (- (:counter hint-state) 1))))
                )");

  for (int i = 60; i > 55; i--)
  {
    runtime.eval("(tick-state!)");
    EXPECT_EQ(runtime.eval("(-> state :hints :counter)")->to_string(),
              std::to_string(i - 1));
  }
}

TEST(Semantic_GlobalState,
     assoc_bang_on_nested_map_after_set_bang_on_global_state__in_function)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval(R"(
     (def state {:selected-categories [:books :movies]
                 :iteration 0
                 :hints {:overlay-type :overlay/context
                         :delay-time 60
                         :counter 60}})

     (defun tick-state! []
       (let [hint-state (:hints state)]
         (set! [state] (assoc state :iteration (+ (:iteration state) 1)))
         (assoc! hint-state :counter (- (:counter hint-state) 1))))
                )");

  for (int i = 60; i > 55; i--)
  {
    runtime.eval("(tick-state!)");
    EXPECT_EQ(runtime.eval("(-> state :hints :counter)")->to_string(),
              std::to_string(i - 1));
  }
}

TEST(Semantic_GlobalState, assoc_in_bang_on_nested_map__in_function)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval(R"(
     (def state {:selected-categories [:books :movies]
                 :hints {:overlay-type :overlay/context
                         :delay-time 60
                         :counter 60}})

     (defun tick-state! []
       (let [hint-state (:hints state)]
         (assoc-in! hint-state [:counter] (- (:counter hint-state) 1))))
                )");

  for (int i = 60; i > 55; i--)
  {
    runtime.eval("(tick-state!)");
    EXPECT_EQ(runtime.eval("(-> state :hints :counter)")->to_string(),
              std::to_string(i - 1));
  }
}

TEST(Semantic_GlobalState, assoc_in_bang_on_global_map__in_function)
{
  // Given
  Lisple::Runtime runtime;
  runtime.eval(R"(
     (def state {:selected-categories [:books :movies]
                 :hints {:overlay-type :overlay/context
                         :delay-time 60
                         :counter 60}})

     (defun tick-state! []
       (assoc-in! state [:hints :counter] (- (:counter (:hints state)) 1)))
                )");

  for (int i = 60; i > 55; i--)
  {
    runtime.eval("(tick-state!)");
    EXPECT_EQ(runtime.eval("(-> state :hints :counter)")->to_string(),
              std::to_string(i - 1));
  }
}
