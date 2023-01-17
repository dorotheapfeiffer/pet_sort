#pragma once

#include "Configuration.h"
#include "DataStructures.h"
#include <vector>

class Sorter {
public:
  Sorter(Configuration &config);
  ~Sorter();
  int ReadFile();

private:
  void SortHits(uint32_t lastTime, bool sortOverflow);
  void StoreHits();
  int CreateOutputFile();

  Configuration m_config;
  std::vector<Hit> m_hits;
  std::vector<Hit> m_hitsNew;
  std::vector<Hit> m_hitsOverflow;
  std::ofstream m_outFile;
};
