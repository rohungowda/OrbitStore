#include "../include/Keys.h"

using namespace std;
using namespace Orbit;


Slice::Slice(): data_(nullptr), size(0) {}
Slice::Slice(const std::string &data) : data_(data.data()), size(data.size()) {}
Slice::Slice(const char* data, size_t n) : data_(data), size(n) {}