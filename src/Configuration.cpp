#include "Configuration.h"

bool Configuration::PrintUsage(const std::string &errorMessage) {
  if (errorMessage != "Help") {
    std::cout << "\nERROR: " << errorMessage << std::endl;
  }
  std::cout << "\nUsage:" << std::endl;
  std::cout
      << "./sortFile -i rawData.dat -o sortedData.txt -n frames -l latency"
      << std::endl;

  std::cout << "\nFlags:" << std::endl;
  std::cout << "-i: input file with raw data" << std::endl;
  std::cout << "-o: file to store sorted output (default: std::out)"
            << std::endl;
  std::cout << "-n: number of frames to analyze (default: all frames)"
            << std::endl;
  std::cout << "-l: transmission delay/latency of data (default: 100ns)"
            << std::endl;
  std::cout << "-h: Show program usage" << std::endl;

  return false;
}

bool Configuration::ParseCommandLine(int argc, char **argv) {

  if (argc == 1) {
    return PrintUsage("Help");
  } else if (argc > 1 && argc % 2 == 0) {
    if (strncmp(argv[1], "-h", 2) == 0) {
      return PrintUsage("Help");
    } else {
      return PrintUsage("Wrong number of arguments!");
    }
  }
  for (int i = 1; i < argc; i += 2) {
    if (strncmp(argv[i], "-i", 2) == 0) {
      m_iFound = true;
      m_inputFile = argv[i + 1];
    } else if (strncmp(argv[i], "-o", 2) == 0) {
      m_oFound = true;
      m_outputFile = argv[i + 1];
    } else if (strncmp(argv[i], "-n", 2) == 0) {
      m_nFound = true;
      m_numFrames = atoi(argv[i + 1]);
    } else if (strncmp(argv[i], "-l", 2) == 0) {
      m_lFound = true;
      m_latency = atoi(argv[i + 1]);
    }
  }
  if (!m_iFound) {
    return PrintUsage("Input file name missing!");
  }
  return true;
}
