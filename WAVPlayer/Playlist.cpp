#include "Playlist.h"


Playlist::Playlist() 
	: _actualPos(0) 
{
}

Playlist::Playlist(int count, char** newPlaylist)
	:	_actualPos(0)
{
	add(count, newPlaylist);
}

Playlist::Playlist(int count, std::string* newPlaylist) 
	: _actualPos(0) 
{
	add(count, newPlaylist);
}

Playlist::Playlist(std::vector<std::string> &newPlaylist)
	: _actualPos(0) 
{
	add(newPlaylist);
}

Playlist::~Playlist() {

}

void Playlist::add(std::string str) {
	printf("Adding %s to the playlist\n", str.c_str());
	_playlist.push_back(str);
}

void Playlist::add(int count, char** newPlaylist) {
	for (int i = 0; i < count; i++) add(newPlaylist[i]);
}

void Playlist::add(int count, std::string* newPlaylist) {
	for (int i = 0; i < count; i++) add(newPlaylist[i]);
}

void Playlist::add(std::vector<std::string> &newPlaylist) {
	for (auto& str : newPlaylist) add(str);
}

void Playlist::remove(int pos) {
	if (pos < 0 || pos >= _playlist.size()) return;
	if (_actualPos >= pos) _actualPos--;
	_playlist.erase(_playlist.begin() + pos);
}

void Playlist::removeCurrent() {
	if (!_playlist.empty()) {
		_playlist.erase(_playlist.begin() + _actualPos);
		if (_actualPos == _playlist.size()) _actualPos--;
	}
}


void Playlist::clear() {
	_actualPos = 0;
	_playlist.clear();
}

std::string Playlist::previous() {
	if (_playlist.empty()) return "";
	_actualPos == 0 ? _actualPos = _playlist.size() - 1 : _actualPos--;
	return _playlist[_actualPos];
}

std::string Playlist::current() {
	return _playlist[_actualPos];
}


std::string Playlist::next() {
	if (_playlist.empty()) return "";
	_actualPos = (_actualPos + 1) % _playlist.size();
	return _playlist[_actualPos];
}


std::string Playlist::setPosition(int pos) {
	if (pos < 0 || pos >= _playlist.size()) return "";
	else return _playlist[_actualPos = pos];
}

std::string Playlist::operator[](int pos) {
	if (pos < 0 || pos >= _playlist.size()) return "";
	else return _playlist[_actualPos = pos];
}

int Playlist::size() const {
	return _playlist.size();
}