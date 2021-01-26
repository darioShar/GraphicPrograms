#include "stdafx.h"
#include "utils.h"

std::string RemovePathVerbose(std::string s) {
	// Delete extension
	while (s[s.size() - 1] != '.') s.erase(s.begin() + s.size() - 1);
	s.erase(s.begin() + s.size() - 1);

	// Delete everything before antislash
	int pos = s.size() - 1;
	while (s[pos] != '/') pos--;
	s.erase(s.begin(), s.begin() + pos + 1);

	return s;
}

std::string SourceDirectory(const std::string& source) {
	std::string s = source;
	size_t x = s.find_last_of('/');
	if (x == std::string::npos) x = s.size();
	s.erase(s.begin() + x, s.end());
	return s;
}

bool DoStringsMatch(const char* pat, const char* str) {
	// patter is the regex
	const char *s, *p;
	bool star = false;

loopStart:
	for (s = str, p = pat; *s; ++s, ++p) {
		switch (*p) {
		case '?':
			if (*s == '.') goto starCheck;
			break;
		case '*':
			star = true;
			str = s, pat = p;
			do { ++pat; } while (*pat == '*');
			if (!*pat) return true;
			goto loopStart;
		default:
			if (toupper(*s) != toupper(*p))
				goto starCheck;
			break;
		} /* endswitch */
	} /* endfor */
	while (*p == '*') ++p;
	return (!*p);

starCheck:
	if (!star) return false;
	str++;
	goto loopStart;
}

// Inverse of sRGB "gamma" function. (approx 2.2)
float inv_gam_sRGB(int ic) {
	float c = ic / 255.0f;
	if (c <= 0.04045f)
		return c / 12.92f;
	else
		return powf(((c + 0.055f) / (1.055f)), 2.4f);
}

// sRGB "gamma" function (approx 2.2)
int gam_sRGB(float v) {
	if (v <= 0.0031308f)
		v *= 12.92f;
	else
		v = 1.055f*powf(v, 1.0f / 2.4f) - 0.055f;
	return int(v * 255 + 0.5f); // This is correct in C++. Other languages may not
								// require +0.5
}

// GRAY VALUE ("brightness")
int gray(int r, int g, int b) {
	return gam_sRGB(
		rY*inv_gam_sRGB(r) +
		gY*inv_gam_sRGB(g) +
		bY*inv_gam_sRGB(b)
	);
}