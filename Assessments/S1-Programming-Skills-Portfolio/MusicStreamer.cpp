#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
using namespace std;

vector<string> loadSongs(const string& filename) {
    vector<string> songs;
    ifstream file(filename);
    string song;

    while (getline(file, song)) {
        if (!song.empty() && song.back() == '\r') {
            song.pop_back();
        }
        if (!song.empty()) {
            songs.push_back(song);
        }
    }

    return songs;
}

int main() {
    vector<string> songs = loadSongs("Resources/songs.txt");

    if (songs.empty()) {
        cout << "Could not load songs. Check songs.txt exists and isn't empty.\n";
        return 1;
    }

    mt19937 generator(random_device{}());
    shuffle(songs.begin(), songs.end(), generator);

    // Songs before this position have already been played.
    size_t nextSong = 0;
    string command;

    cout << "Music Streaming Assistant\n";
    cout << "Enter 'Play a random song' or 'quit'.\n";

    while (true) {
        cout << "\nCommand: ";
        if (!getline(cin, command) || command == "quit") {
            break;
        }

        if (command == "Play a random song") {
            if (nextSong == songs.size()) {
                cout << "All songs have been played! Starting a new shuffle.\n";
                shuffle(songs.begin(), songs.end(), generator);
                nextSong = 0;
            }

            cout << "Now playing: " << songs[nextSong] << '\n';
            nextSong++;
        } else {
            cout << "Unknown command. Enter 'Play a random song' or 'quit'.\n";
        }
    }

    cout << "Goodbye!\n";
    return 0;
}