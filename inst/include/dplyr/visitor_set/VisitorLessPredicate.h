#ifndef dplyr_VisitorLessPredicate_H
#define dplyr_VisitorLessPredicate_H

namespace dplyr {

template <typename Visitor>
class VisitorLessPredicate {
public:
  VisitorLessPredicate(const Visitor& v_) : v(v_) {}

  inline bool operator()(int i, int j) const {
    return v.less(i, j);
  }

private:
  const Visitor& v;
};
}

#endif
