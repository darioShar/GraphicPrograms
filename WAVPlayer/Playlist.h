#pragma once
#include <vector>
#include <string>

class Playlist {
	int _actualPos;
	std::vector<std::string> _playlist;

public :
	Playlist();
	Playlist(int count, char** newPlaylist);
	Playlist(int count, std::string* newPlaylist);
	Playlist(std::vector<std::string> &newPlaylist);
	~Playlist();

	void add(std::string);
	void add(int count, char** newPlaylist);
	void add(int count, std::string* newPlaylist);
	void add(std::vector<std::string> &newPlaylist);
	void remove(int);
	void removeCurrent();
	void clear();

	std::string previous();
	std::string current();
	std::string next();

	std::string setPosition(int);
	std::string operator[](int);

	int size() const;
};