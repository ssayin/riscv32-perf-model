#ifndef MODEL_MEMORYREQUEST_H
#define MODEL_MEMORYREQUEST_H

#include <cstdint>
#include <vector>

template <class T> class Input {
public:
  void Write(T value) { t = value; };
  T Read() { return t; }

private:
  T t;
};

template <class T> class Output {
public:
  void Write(T value) { t = value; };
  T Read() { return t; }

private:
  T t;
};

struct MemoryRequest {
  enum Type : bool { READ, WRITE };
  uint32_t AddrBase;
  uint32_t NumBytes;
  Type ReqType;
  std::vector<uint8_t> Bytes;
};

#endif /* end of include guard: MODEL_MEMORYREQUEST_H */
