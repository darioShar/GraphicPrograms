#include <SFML\Graphics.hpp>
#include <cstdio>
#include <Windows.h>
#include "CustomPlayer.h"
#include "FrequencySpectrum.h"
#include "Playlist.h"

#define DEFAULT_FONT_PATH "Fonts\\ariblk.ttf"
#define DEFAULT_CHARACTER_SIZE 30
#define DEFAULT_CHARACTER_COLOR Color::White
#define DEAULT_ICON_PATH "Icons\\Music Audio Wave.png"

#define DEFAULT_MUSIC "Musics\\surely.wav"
#define DEFAULT_MUSIC "D:\\Music\\wav\\Nero - Doomsday.ogg"

#define WIDTH 1280
#define HEIGHT 720

#define REFRESH_RATE 30.0f
#define NUM_BINS 200
#define SCALE_FACTOR 1.02f

#define INTERVAL_SHOW_SETTINGS 1500.0f

#define PLAYING_OFFSET_INTERVAL 5
#define PLAYING_OFFSET_INTERVAL_STRING "5"

using namespace sf;

bool CheckAndLoadSong(std::string& name);
void CheckAndLoadNextValidSong();
void LoadAndPlayMusic();
void ProcessInput(char input);
void ShowSettings();
Clock settingsClock;
bool settingsChanged = false;


void UpdateMusicName(std::string name);

// Create window
RenderWindow window(VideoMode(WIDTH, HEIGHT), "WAV Player");

// For the music
Font font;
Text musicName;
CustomPlayer musicPlayer(REFRESH_RATE);
SoundBuffer musicBuffer;
Playlist playlist;

FrequencySpectrum frequencySpectrum(NUM_BINS, WIDTH, HEIGHT);
GraphType fftType = GraphType::Centred;
bool fftLogScale = true;


int main(int argc, char** argv) {

	window.setFramerateLimit(60);

	// Set window icon
	Image icon;
	if (!icon.loadFromFile(DEAULT_ICON_PATH)) printf("Failed to load app icon\n");
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

	// To manage events
	Event evnt;

	// Set music name display parameters
	font.loadFromFile(DEFAULT_FONT_PATH);
	musicName.setFont(font);
	musicName.setCharacterSize(DEFAULT_CHARACTER_SIZE);
	musicName.setFillColor(DEFAULT_CHARACTER_COLOR);

	// Load songs
	if (argc < 2) {
		printf("No music selected. Default music added to playlist : " DEFAULT_MUSIC "\n");
		playlist.add(DEFAULT_MUSIC);
	}
	else {
		printf("Creating playlist : \n");
		printf("Adding %d songs to the playlist.\n", argc - 1);
		playlist.add(argc - 1, &argv[1]);
	}
	
	// Opening first song
	LoadAndPlayMusic();
	ShowSettings();

	while (window.isOpen()) {
		while (window.pollEvent(evnt)) {
			if (evnt.type == Event::Closed) {
				window.close();
			}
			// Keyboard input
			if (evnt.type == Event::TextEntered) {
				if (evnt.text.unicode < 128) {
					ProcessInput(evnt.text.unicode);
				}
			}
		}

		if (musicPlayer.getStatus() == CustomPlayer::Playing) {
			frequencySpectrum.setFFT(musicPlayer.getFFT(), fftType, fftLogScale);
		}
		else if (musicPlayer.getStatus() == CustomPlayer::Status::Stopped){
			playlist.next();
			LoadAndPlayMusic();
			frequencySpectrum.resetScale();
		}

		window.clear(Color::Black);
		frequencySpectrum.draw(window);
		window.draw(musicName);
		window.display();

		if (settingsChanged && (settingsClock.getElapsedTime().asMilliseconds() > INTERVAL_SHOW_SETTINGS)) {
			ShowSettings();
			settingsChanged = false;
		}
	}
}


bool CheckAndLoadSong(std::string& name) {
	printf("Loading %s into buffer...", name.c_str());
	if (!musicBuffer.loadFromFile(name)) {
		printf(" music failed to load.\n");
		return false;
	}
	printf("music is loaded.\n");
	return true;
}

void CheckAndLoadNextValidSong() {
	while (playlist.size() && !CheckAndLoadSong(playlist.current()))
		playlist.removeCurrent();
	if (!playlist.size()) {
		printf("No more valid music file found to play. Exiting...");
		Sleep(5000);
		exit(-1);
	}
}

void LoadAndPlayMusic() {
	CheckAndLoadNextValidSong();
	musicPlayer.load(musicBuffer);
	musicPlayer.play();
	musicPlayer.setVolume(50.0f);
	printf("Playing %s\n", playlist.current().c_str());
	UpdateMusicName(playlist.current());
}


void ShowSettings() {
	printf("\n\nSETTINGS : \n \
		[N]ext song\n \
		[P]revious song\n \
		" PLAYING_OFFSET_INTERVAL_STRING " seconds forward : [F]\n \
		" PLAYING_OFFSET_INTERVAL_STRING " seconds back : [D]\n \
		Pause/Resume : [Space Bar]\n \
		Volume Up : [+]\n \
		Volume Down : [-]\n\n \
		Change FFT type : [T]\n \
		Change FFT Scale (Log or Linear) : [L]\n \
		Make FFT bigger : [B]\n \
		Make FFT Smaller : [S]\n \
		Reset FFT : [R]\n\n \
		[Q]uit\n\n");
}

void ProcessInput(char input) {
	switch (toupper(input)) {
	case 'N' :
		playlist.next();
		LoadAndPlayMusic();
		frequencySpectrum.resetScale();
		break;
	case 'P' :
		playlist.previous();
		LoadAndPlayMusic();
		frequencySpectrum.resetScale();
		break;
	case 'F' :
		musicPlayer.setPlayingOffset(seconds(musicPlayer.getPlayingOffset().asSeconds() + PLAYING_OFFSET_INTERVAL));
		printf("Playing Offset now is : %f\n", musicPlayer.getPlayingOffset().asSeconds());
		break;
	case 'D' :
		musicPlayer.setPlayingOffset(seconds(musicPlayer.getPlayingOffset().asSeconds() - PLAYING_OFFSET_INTERVAL));
		printf("Playing Offset now is : %f\n", musicPlayer.getPlayingOffset().asSeconds());
		break;
	case ' ' :
		if (musicPlayer.getStatus() != CustomPlayer::Status::Stopped)
			musicPlayer.getStatus() == CustomPlayer::Status::Paused ? musicPlayer.play() : musicPlayer.pause();
		printf("Music state changed. Status is %d\n", musicPlayer.getStatus());
		break;
	case '+' :
		musicPlayer.setVolume(musicPlayer.getVolume() + 2.0f);
		printf("Volume is now %4.f\n", musicPlayer.getVolume());
		break;
	case '-' :
		musicPlayer.setVolume(musicPlayer.getVolume() - 2.0f);
		printf("Volume is now %4.f\n", musicPlayer.getVolume());
		break;
	case 'T' :
		fftType = (GraphType)(((int)fftType + 1) % (GraphType::TotalTypes));
		printf("FFT type changed.\n");
		break;
	case 'L' :
		fftLogScale = !fftLogScale;
		fftLogScale ? printf("FFT Scale is now logarithmic\n") : printf("FFT Scale is now linear\n");
		break;
	case 'B' : 
		frequencySpectrum.scale(SCALE_FACTOR);
		printf("FFT scale now is %f of original.\n", frequencySpectrum.getScale());
		break;
	case 'S' : 
		frequencySpectrum.scale(1.0f / SCALE_FACTOR);
		printf("FFT scale now is %f of original.\n", frequencySpectrum.getScale());
		break;
	case 'R' :
		frequencySpectrum.resetScale();
		frequencySpectrum.setGraph(NUM_BINS, GraphScale::Linear);
		fftType = GraphType::Centred;
		printf("FFT reset.\n");
		break;
	case 'Q' :
		printf("Exiting...");
		exit(0);
		break;
	case 'M' : 
		frequencySpectrum.reloadShader();
		break;
	default :
		return;
	}
	settingsChanged = true;
	settingsClock.restart();
}


void UpdateMusicName(std::string name) {
	// Delete extension
	while (name[name.size() - 1] != '.') name.erase(name.begin() + name.size() - 1);
	name.erase(name.begin() + name.size() - 1);

	// Delete everything before antislash
	int pos = name.size() - 1;
	while (name[pos] != '\\') pos--;
	name.erase(name.begin(), name.begin() + pos + 1);

	musicName.setString(name);
	musicName.setPosition(((d_real)WIDTH - (d_real)musicName.getGlobalBounds().width) / 2, 8 * HEIGHT / 9);
}
