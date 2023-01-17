#include "Configuration.h"
#include "DataStructures.h"
#include "Sorter.h"
#include <chrono>

int main(int argc, char **argv) {

  std::chrono::time_point<std::chrono::system_clock> timeEnd, timeStart;
  timeStart = std::chrono::system_clock::now();
  Configuration m_config;
  uint64_t hits = 0;
  if (m_config.ParseCommandLine(argc, argv)) {
    timeStart = std::chrono::system_clock::now();
    Sorter *sorter = new Sorter(m_config);
    hits = sorter->ReadFile();
    if (hits == -1) {
      std::cout << "ERROR: File " << m_config.GetInputFilename()
                << " cannot be read!" << std::endl;
      delete sorter;
      return -1;
    }
    delete sorter;
  } else {
    return -1;
  }
  timeEnd = std::chrono::system_clock::now();
  std::chrono::duration<double> tDiff = timeEnd - timeStart;
  std::cout << "Sorted  " << hits << " hits in " << tDiff.count() << " s"
            << std::endl;
  return 0;
}
