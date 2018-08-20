#ifndef dplyr_LazyGroupedSubsets_H
#define dplyr_LazyGroupedSubsets_H

#include <tools/SymbolMap.h>

#include <dplyr/GroupedDataFrame.h>
#include <dplyr/RowwiseDataFrame.h>
#include <dplyr/NaturalDataFrame.h>

#include <dplyr/SummarisedVariable.h>

#include <dplyr/subset/ILazySubsets.h>
#include <dplyr/subset/get_subset.h>
#include <dplyr/subset/summarised_subset.h>

namespace dplyr {

template <class Data>
class LazySplitSubsets : public ILazySubsets<Data> {
  typedef typename Data::subset subset;
  typedef typename Data::slicing_index slicing_index;

public:
  LazySplitSubsets(const Data& gdf_) :
    gdf(gdf_),
    subsets(),
    symbol_map(),
    resolved(),
    owner(true)
  {
    const DataFrame& data = gdf.data();
    CharacterVector names = data.names();
    int n = data.size();
    LOG_INFO << "processing " << n << " vars: " << names;
    for (int i = 0; i < n; i++) {
      input(names[i], data[i]);
    }
  }

  LazySplitSubsets(const LazySplitSubsets& other) :
    gdf(other.gdf),
    subsets(other.subsets),
    symbol_map(other.symbol_map),
    resolved(other.resolved),
    owner(false)
  {}

  virtual ~LazySplitSubsets() {
    if (owner) {
      for (size_t i = 0; i < subsets.size(); i++) {
        delete subsets[i];
      }
    }
  }

public:
  virtual const SymbolVector get_variable_names() const {
    return symbol_map.get_names();
  }

  virtual SEXP get_variable(int i) const {
    return subsets[i]->get_variable();
  }

  virtual SEXP get_variable(const SymbolString& symbol) const {
    return subsets[symbol_map.get(symbol)]->get_variable();
  }

  virtual SEXP get(const SymbolString& symbol, const slicing_index& indices) const {
    int idx = symbol_map.get(symbol);

    SEXP value = resolved[idx];
    if (value == R_NilValue) {
      resolved[idx] = value = subsets[idx]->get(indices);
    }
    return value;
  }

  virtual bool is_summary(int i) const {
    return subsets[i]->is_summary();
  }

  virtual bool is_summary(const SymbolString& symbol) const {
    return subsets[symbol_map.get(symbol)]->is_summary();
  }

  virtual bool has_variable(const SymbolString& head) const {
    return symbol_map.has(head);
  }

  virtual void input(const SymbolString& symbol, SEXP x) {
    input_subset(symbol, gdf.create_subset(x));
  }

  virtual int size() const {
    return subsets.size();
  }

  virtual int nrows() const {
    return gdf.nrows();
  }

public:
  void clear() {
    for (size_t i = 0; i < resolved.size(); i++) {
      resolved[i] = R_NilValue;
    }
  }

  void input_summarised(const SymbolString& symbol, SummarisedVariable x) {
    input_subset(symbol, summarised_subset<slicing_index>(x));
  }

private:
  const Data& gdf;
  std::vector<subset*> subsets;
  SymbolMap symbol_map;
  mutable std::vector<SEXP> resolved;

  bool owner;

  void input_subset(const SymbolString& symbol, subset* sub) {
    SymbolMapIndex index = symbol_map.insert(symbol);
    if (index.origin == NEW) {
      subsets.push_back(sub);
      resolved.push_back(R_NilValue);
    } else {
      int idx = index.pos;
      delete subsets[idx];
      subsets[idx] = sub;
      resolved[idx] = R_NilValue;
    }
  }
};

typedef LazySplitSubsets<GroupedDataFrame> LazyGroupedSubsets;
typedef LazySplitSubsets<RowwiseDataFrame> LazyRowwiseSubsets;
typedef LazySplitSubsets<NaturalDataFrame> LazyNaturalSubsets;

}
#endif