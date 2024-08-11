#pragma once

#include <string>
#include <vector>
#include <glad/glad.h>

struct SSurfel;

bool readBinFile(const std::string& vFileName, std::vector<char>& voBuffer);

bool loadRsfFile(const std::string& vFileName, std::vector<SSurfel>& voSurfels);

GLuint createVAO(const std::vector<SSurfel>& vSurfels);