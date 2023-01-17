#include "Sorter.h"

Sorter::Sorter(Configuration &config) : m_config(config) {}

Sorter::~Sorter() {}

int Sorter::CreateOutputFile() {
  if (m_config.WriteToFile()) {
    m_outFile.open(m_config.GetOutputFilename());
    if (!m_outFile) {
      std::cout << "ERROR: File " << m_config.GetOutputFilename()
                << " cannot be created!" << std::endl;
      return -1;
    }
    m_outFile << "Time\tEnergy" << std::endl;
  }
  return 0;
}

int Sorter::ReadFile() {
  bool doContinue = true;
  bool isOverflow = false;
  bool newFrame = false;
  uint64_t nextFrame = 0;
  uint64_t lastFrame = 0;
  uint64_t hits = 0;
  uint64_t frames = 0;
  uint32_t lastTime = 0;
  uint64_t largestTime = 0;
  RawData rawData;
  std::ifstream inFile(m_config.GetInputFilename(), std::ios::binary);
  if (!inFile) {
    return -1;
  }
  if (CreateOutputFile() != 0) {
    return -1;
  }
  while (doContinue &&
         inFile.read(reinterpret_cast<char *>(&rawData), sizeof(rawData))) {
    // New frame found
    if (hits == nextFrame) {
      newFrame = true;
      lastFrame = nextFrame;
    }
    if (newFrame) {
      frames++;
      // Position of new frame based on number of hits in header
      nextFrame = lastFrame + rawData.first;
      newFrame = false;
      // Sort hits, consider overflow flag
      SortHits(lastTime, isOverflow);
      if (lastTime > m_config.GetLatency()) {
        isOverflow = false;
        largestTime = 0;
      }
      StoreHits();
    } else {
      // Check for overflow of timestamp
      if (static_cast<uint64_t>(rawData.first) +
              static_cast<uint64_t>(m_config.GetLatency()) <
          static_cast<uint64_t>(lastTime)) {
        isOverflow = true;
      }
      // Store data in vectors, considering overflow flag
      if (isOverflow == false) {
        m_hitsNew.emplace_back(std::make_pair(rawData.first, rawData.second));
      } else {
        // Store large timestamps in m_hits_new
        if (static_cast<uint64_t>(rawData.first) +
                static_cast<uint64_t>(m_config.GetLatency()) >
            largestTime) {
          m_hitsNew.emplace_back(std::make_pair(rawData.first, rawData.second));
        }
        // Store small timestamps in m_hits_overflow
        else {
          m_hitsOverflow.emplace_back(
              std::make_pair(rawData.first, rawData.second));
        }
      }

      lastTime = rawData.first;
      if (rawData.first > largestTime) {
        largestTime = rawData.first;
      }
      hits++;
    }
    if (m_config.GetNumberOfFrames() > -1 &&
        m_config.GetNumberOfFrames() <= frames) {
      doContinue = false;
    }
  }
  SortHits(rawData.first + 2 * m_config.GetLatency(), isOverflow);
  StoreHits();

  inFile.close();
  if (m_config.WriteToFile()) {
    m_outFile.close();
  }
  return hits;
}
//====================================================================================================================
void Sorter::SortHits(uint32_t lastTime, bool sortOverflow) {
  if (!sortOverflow) {
    // Sort vector m_hits_new
    std::sort(begin(m_hitsNew), end(m_hitsNew),
              [](const Hit &t1, const Hit &t2) {
                return std::get<0>(t1) < std::get<0>(t2);
              });

    // First hits with timestamp larger than last_time - latency
    auto it =
        std::upper_bound(m_hitsNew.begin(), m_hitsNew.end(),
                         std::make_tuple(lastTime - m_config.GetLatency(), 0),
                         [](const Hit &t1, const Hit &t2) {
                           return std::get<0>(t1) < std::get<0>(t2);
                         });

    int index = std::distance(m_hitsNew.begin(), it);

    // Insert the hits that are ready from m_hits_new into m_hits
    m_hits.insert(m_hits.end(), std::make_move_iterator(m_hitsNew.begin()),
                  std::make_move_iterator(m_hitsNew.begin() + index));
    // Delete the hits from m_hits_new
    m_hitsNew.erase(m_hitsNew.begin(), m_hitsNew.begin() + index);
  } else {
    // Sort vector m_hits_new
    std::sort(begin(m_hitsNew), end(m_hitsNew),
              [](const Hit &t1, const Hit &t2) {
                return std::get<0>(t1) < std::get<0>(t2);
              });

    // Insert all hits from m_hits_new into m_hits
    m_hits.insert(m_hits.end(), std::make_move_iterator(m_hitsNew.begin()),
                  std::make_move_iterator(m_hitsNew.end()));

    // Delete all hits from m_hits_new
    m_hitsNew.clear();

    // Insert the hits that are ready from m_hits_new into m_hits
    m_hitsNew.insert(m_hitsNew.end(),
                     std::make_move_iterator(m_hitsOverflow.begin()),
                     std::make_move_iterator(m_hitsOverflow.end()));

    // Delete all hits from m_hits_overflow
    m_hitsOverflow.clear();
  }
}

void Sorter::StoreHits() {
  if (m_config.WriteToFile()) {
    for (auto const &it : m_hits) {
      m_outFile << std::get<0>(it) << "\t" << std::get<1>(it) << "\n";
    }
  } else {
    for (auto const &it : m_hits) {
      std::cout << std::get<0>(it) << "\t" << std::get<1>(it) << "\n";
    }
  }
  m_hits.clear();
}