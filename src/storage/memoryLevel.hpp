#ifndef NALLJ_DRACHMA_MEMORY_LEVEL
#define NALLJ_DRACHMA_MEMORY_LEVEL

#include <map>

#include "storageUnit.hpp"
#include "../algs/cache/replAlg.hpp"

class memoryLevel : public storageUnit {

  replAlg* replacement_alg_;
  bool contents_lut_indexed_;
  std::map<unsigned, std::map<unsigned, rrModule*>> module_reference_table_;

public:
  memoryLevel();

  // intended for main memory (automatically LUT indexed)
  memoryLevel(
    unsigned unit_size,
    replAlg* repl_alg,
    unsigned short read_latency,
    unsigned short search_latency
  );

  // intended for cache levels
  memoryLevel(
    unsigned unit_size,
    replAlg* repl_alg,
    unsigned short read_latency,
    unsigned short search_latency,
    bool lut_indexed
  );

  memoryLevel(const storageUnit& su);

  // Operational Methods (inherited) //
  void printDetails();

  //void attemptModule(unsigned module_index);
  // never should happen unless you're trying to bring down a rrModule (read done, now copy)
  void attemptModule(unsigned module_index);
  bool isModulePresent(unsigned region_id,unsigned module_id);
  void insertModule(rrModule* new_module);

  // Operational Methods (unique) //
  bool isLutIndexed();
  rrModule* getModule(unsigned region_id, unsigned module_id);
  bool hasSpace();
  void evictModule(unsigned table_index);
};

#endif
