#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

#include "glm/gtc/matrix_transform.hpp"

using namespace std;

void loadObj (const char *fileName, std::vector<glm::vec3> *verteces, std::vector<int> *indeces);