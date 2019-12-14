#include <iostream>
#include <array>

using namespace std;

int trackList[] = { 55, 58, 39, 18, 90, 160, 150, 38, 184 };
int diskSize = 200;

//sstf scheduling policy
double sstfScheduling(int headPosition) {
	int trackListLength = sizeof(trackList) / sizeof(trackList[0]);
	int trackListAccd[trackListLength], move[trackListLength], next[trackListLength];
	int first, min, currentLoc, tracksTraversed = 0;
	for (int i = 0; i < trackListLength; i++)
	{
		trackListAccd[i] = 0;
	}
	for (int i = 0; i < trackListLength; i++)
	{
		first = 0;
		min = 0;
		currentLoc = 0;
		for (int j = 0; j < trackListLength; j++)
		{
			if (trackListAccd[j] == 0)
			{
				if (first == 0)
				{
					next[j] = headPosition - trackList[j];
					if (next[j] < 0) {
						next[j] = trackList[j] - headPosition;
					}
					currentLoc = j;
					min = next[j];
					first++;
				}
				else
				{
					next[j] = headPosition - trackList[j];
					if (next[j] < 0) {
						next[j] = trackList[j] - headPosition;
					}
				}
				if (min > next[j]) {
					currentLoc = j;
					min = next[j];
				}
			}
		}
		trackListAccd[currentLoc] = 1;
		move[i] = trackList[currentLoc] - headPosition;
		if (move[i] < 0) {
			move[i] = headPosition - trackList[currentLoc];
		}
		headPosition = trackList[currentLoc];
	}
	for (int i = 0; i < trackListLength; i++)
	{
		tracksTraversed = tracksTraversed + move[i];
	}
	return (double)tracksTraversed / trackListLength;
}

//scan scheduling policy
double scanScheduling(int headPosition) {
	int tracksTraversed = 0;
	int trackListLength = sizeof(trackList) / sizeof(trackList[0]);
	int trackListAccd[trackListLength];
	int positiveMove = 1;
	int maxElement = 0;
	for (int i = 0; i < trackListLength; i++) {
		if (trackList[i] > maxElement) {
			maxElement = trackList[i];
		}
	}
	int minElement = diskSize;
	for (int i = 0; i < trackListLength; i++) {
		if (trackList[i] < minElement) {
			minElement = trackList[i];
		}
	}
	while (true) {
		int tempCount = 0;
		for (int i = 0; i < trackListLength; i++) {
			if (headPosition == trackList[i] && trackListAccd[i] != 1) {
				trackListAccd[i] = 1;
			}
			tempCount += trackListAccd[i];
			if (tempCount == trackListLength) {
				return (double)tracksTraversed / trackListLength;
			}
		}
		if (headPosition < maxElement && positiveMove == 1) {
			headPosition++;
		}
		else {
			positiveMove = 0;
			headPosition--;
		}
		tracksTraversed++;
	}
}

// cscan scheduling policy
double cscanScheduling(int headPosition) {
	int headStart = headPosition;
	int tracksTraversed = 0;
	int trackListLength = sizeof(trackList) / sizeof(trackList[0]);
	int trackListAccd[trackListLength];
	int currentMove = 0;
	int maxElement = 0;
	for (int i = 0; i < trackListLength; i++) {
		if (trackList[i] > maxElement) {
			maxElement = trackList[i];
		}
	}
	int minElement = diskSize;
	for (int i = 0; i < trackListLength; i++) {
		if (trackList[i] < minElement) {
			minElement = trackList[i];
		}
	}
	while (true) {
		int tempCount = 0;
		for (int i = 0; i < trackListLength; i++) {
			if (headPosition == trackList[i] && trackListAccd[i] != 1) {
				trackListAccd[i] = 1;
			}
			tempCount += trackListAccd[i];
			if (tempCount == trackListLength) {
				return (double)tracksTraversed / trackListLength;
			}
		}
		if (headPosition < maxElement) {
			headPosition++;
		}
		else {
			tracksTraversed += headPosition - minElement;
			headPosition = minElement;
		}
		tracksTraversed++;
	}
}

int main() {
	cout << "Average seek length for SSTF scheduling is " << sstfScheduling(100) << ".\n";
	cout << "Average seek length for SCAN scheduling is " << scanScheduling(100) << ".\n";
	cout << "Average seek length for CSCAN scheduling is " << cscanScheduling(100) << ".\n";
}
