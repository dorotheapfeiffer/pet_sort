#pragma once

#include <fstream>
#include <iostream>

class Configuration {
public:
  Configuration() = default;
  ~Configuration() = default;
  bool ParseCommandLine(int argc, char **argv);
  bool PrintUsage(const std::string &errorMessage);
  std::string GetInputFilename() { return m_inputFile; };
  std::string GetOutputFilename() { return m_outputFile; };
  int GetNumberOfFrames() { return m_numFrames; };
  int GetLatency() { return m_latency; };
  bool WriteToFile() { return m_oFound; }

private:
  bool m_iFound = false;
  bool m_oFound = false;
  bool m_nFound = false;
  bool m_lFound = false;
  std::string m_inputFile;
  std::string m_outputFile;
  int m_numFrames = -1;
  int m_latency = 100;
};