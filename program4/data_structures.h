#ifndef DATA_STRUCTURES
#define DATA_STRUCTURES

struct request {
    int id;
    int memoryUnits;
    int burstTime;
};

struct response {
  int responseId;
  int responseCode;
  int framesAllocated;
  float fragmentation;
  int burstTime;
  int completionTime;
};

struct frameAvailability {
    int frames;
    int sizePerFrame;
};

#endif