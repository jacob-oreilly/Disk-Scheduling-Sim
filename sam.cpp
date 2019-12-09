#include <iostream>
#include <array>

using namespace std;

int trackList[] = {55, 58, 39, 18, 90, 160, 150, 38, 184};

//fifo scheduling policy
double fifoScheduling(int headPosition) {   
    int tracksTraversed = 0;
    int trackListLength = sizeof(trackList) / sizeof(trackList[0]);
    for (int i = 0; i < trackListLength; i++) {
        if (headPosition > trackList[i]) {
            tracksTraversed += headPosition - trackList[i];
        } else {
            tracksTraversed += trackList[i] - headPosition;
        }
        headPosition = trackList[i];
    }
    return (double) tracksTraversed / trackListLength;
}

//lifo scheduling policy
double lifoScheduling(int headPosition) {
    int tracksTraversed = 0;
    int trackListLength = sizeof(trackList) / sizeof(trackList[0]);
    for (int i = trackListLength - 1; i >= 0; i--) {
        if (headPosition > trackList[i]) {
            tracksTraversed += headPosition - trackList[i];
        } else {
            tracksTraversed += trackList[i] - headPosition;
        }
        headPosition = trackList[i];
    }
    return (double) tracksTraversed / trackListLength;
}

int main() {
  cout << "Average seek length for FIFO scheduling is " << fifoScheduling(100) << ".\n";
  cout << "Average seek length for LIFO scheduling is " << lifoScheduling(100) << ".\n";
}