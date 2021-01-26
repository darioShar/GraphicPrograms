#pragma once
#include <vector>
#include <string>

std::string RemovePathVerbose(std::string s);

std::string SourceDirectory(const std::string& source);

bool DoStringsMatch(const char* pat, const char* str);


// sRGB luminance(Y) values
const float rY = 0.212655f;
const float gY = 0.715158f;
const float bY = 0.072187f;

// Inverse of sRGB "gamma" function. (approx 2.2)
float inv_gam_sRGB(int ic);

// sRGB "gamma" function (approx 2.2)
int gam_sRGB(float v);

// GRAY VALUE ("brightness")
int gray(int r, int g, int b);