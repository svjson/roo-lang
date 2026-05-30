#ifndef ROOTEST__FIXTURES__PIXILS_POINT_H
#define ROOTEST__FIXTURES__PIXILS_POINT_H

#include <string>

namespace RooTest
{
  inline const std::string NS_PIXILS_POINT = R"(
    (ns pixils.point)
                                     )";

  inline const std::string DEFUN_PT_PLUS = R"(
    (defun plus [p1 p2]
      {:x (+ (:x p1) (:x p2))
       :y (+ (:y p1) (:y p2))})
                                   )";

  inline const std::string DEFUN_PT_DIV = R"(
    (defun div [p n]
      {:x (/ (:x p) n)
       :y (/ (:y p) n)})
                                   )";

  inline const std::string DEFUN_PT_ROTATE = R"(
    (defun rotate [p amount]
      (let [s (sin amount)
            c (cos amount)]
        {:x (- (* (:x p) c) (* (:y p) s))
         :y (+ (* (:x p) s) (* (:y p) c))}))
                                     )";

  inline const std::string DEFUN_PT_DISTANCE = R"(
    (defun distance [p1 p2]
      (sqrt (+ (* (- (:x p2) (:x p1)) (- (:x p2) (:x p1)))
               (* (- (:y p2) (:y p1)) (- (:y p2) (:y p1))))))
                                       )";

  inline const std::string PIXILS__POINT__PURE_LISP__DEFS =
    DEFUN_PT_PLUS + DEFUN_PT_DIV + DEFUN_PT_ROTATE + DEFUN_PT_DISTANCE;

  inline const std::string PIXILS__POINT__PURE_LISP =
    NS_PIXILS_POINT + PIXILS__POINT__PURE_LISP__DEFS;

  inline std::string pixils_point_ns__as(const std::string& ns_name)
  {
    return "(ns " + ns_name + ")\n" + PIXILS__POINT__PURE_LISP__DEFS;
  }

} // namespace RooTest

#endif /* ROOTEST__FIXTURES__PIXILS_POINT_H */
