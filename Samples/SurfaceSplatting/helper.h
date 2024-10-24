#pragma once

#include <string>
#include <vector>

struct SSurfel;

bool readBinFile(const std::string& vFileName, std::vector<char>& voBuffer);

bool loadRsfFile(const std::string& vFileName, std::vector<SSurfel>& voSurfels);

void configSurfelVAOLayout(int vOffset = 0, bool vIsPerInstance = false);