#include <iostream>
#include <array>
#include <stdlib.h>
#include <cmath>

using namespace std;

int testTrackList[1000];
int trackList[] = {55, 58, 39, 18, 90, 160, 150, 38, 184};
int diskSize = 200;

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

//sstf scheduling policy
double sstfScheduling(int headPosition) {
	int trackListLength = sizeof(trackList) / sizeof(trackList[0]);
	int trackListAccd[trackListLength], move[trackListLength], next[trackListLength];
	int first, min, currentLoc, tracksTraversed;
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

void generateRandomThousand() {
	for (int i = 0; i < 1000; i++) {
		testTrackList[i] = rand() % 200;
	}
}

void generateSameTrackAccess() {
	for (int i = 0; i < 1000; i++) {
		if (i == 0) {
			testTrackList[i] = rand() % 200;
		} else {
			int flag = rand() % 10;
			if (flag == 0) {
				testTrackList[i] = testTrackList[i - 1];
			} else {
				testTrackList[i] = rand() % 200;
			}
		}
	}
}

void generateFarthestDistance() {
	int max;
	int min;
	for (int i = 0; i < 1000; i++) {
		if (i == 0) {
			testTrackList[i] = rand() % 200;
		} else if (i == 1) {
			testTrackList[i] = rand() % 200;
			if (testTrackList[i] > testTrackList[i - 1]) {
				max = testTrackList[i];
				min = testTrackList[i - 1];
			} else {
				max = testTrackList[i - 1];
				min = testTrackList[i];
			}
		} else if ((rand() % 100) == 0) {
			if ((testTrackList[i - 1] - min) > (max - testTrackList[i - 1])) {
				testTrackList[i] = min;
			} else {
				testTrackList[i] = max;
			}
		} else {
			testTrackList[i] = rand() % 200;
			if (testTrackList[i] < min) {
				min = testTrackList[i];
			} else if (testTrackList[i] > max) {
				max = testTrackList[i];
			}
		}
	}
}

void generateDistanceDecrease() {
	int min;
	int max;
	for (int i = 0; i < 1000; i++) {
		if (i == 0) {
			testTrackList[i] = rand() % 200;
			min = testTrackList[i];
			max = testTrackList[i];
		} else if (i == 1) {
			testTrackList[i] = rand() % 200;
			if (testTrackList[i] < min) {
				min = testTrackList[i];
			}
			if (testTrackList[i] > max) {
				max = testTrackList[i];
			}
		} else {
			int flag = rand() % 100;
			int range;
			if ((max - testTrackList[i - 1]) > (testTrackList[i - 1] - min)) {
				range = (max - testTrackList[i - 1]) / 7;
			} else {
				range = (testTrackList[i - 1] - min) / 7;
			}
			int greaterOrLess = rand() % 2;
			if (flag < 51) {
				testTrackList[i] = rand() % range + (testTrackList[i - 1] - (range / 2));
			} else if (flag < 76) {
				if (greaterOrLess == 0) {
					testTrackList[i] = rand() % (range / 2) + (testTrackList[i - 1] - range);
				} else {
					testTrackList[i] = rand() % (range / 2) + (testTrackList[i - 1] + range);
				}
			} else if (flag < 88) {
				if (greaterOrLess == 0) {
					testTrackList[i] = rand() % (range / 2) + (testTrackList[i - 1] - ((range / 2) * 3));
				} else {
					testTrackList[i] = rand() % (range / 2) + (testTrackList[i - 1] + ((range / 2) * 3));
				}
			} else if (flag < 94) {
				if (greaterOrLess == 0) {
					testTrackList[i] = rand() % (range / 2) + (testTrackList[i - 1] - (range * 2));
				} else {
					testTrackList[i] = rand() % (range / 2) + (testTrackList[i - 1] + (range * 2));
				}
			} else if (flag < 97) {
				if (greaterOrLess == 0) {
					testTrackList[i] = rand() % (range / 2) + (testTrackList[i - 1] - ((range / 2) * 5));
				} else {
					testTrackList[i] = rand() % (range / 2) + (testTrackList[i - 1] + ((range / 2) * 5));
				}
			} else if (flag < 99) {
				if (greaterOrLess == 0) {
					testTrackList[i] = rand() % (range / 2) + (testTrackList[i - 1] - (range * 3));
				} else {
					testTrackList[i] = rand() % (range / 2) + (testTrackList[i - 1] + (range * 3));
				}
			} else {
				if (greaterOrLess == 0) {
					testTrackList[i] = rand() % (range / 2) + (testTrackList[i - 1] - ((range / 2) * 7));
				} else {
					testTrackList[i] = rand() % (range / 2) + (testTrackList[i - 1] + ((range / 2) * 7));
				}
			}
		}
	}
}

int main() {
    cout << "Average seek length for FIFO scheduling is " << fifoScheduling(100) << ".\n";
    cout << "Average seek length for SSTF scheduling is " << sstfScheduling(100) << ".\n";
    cout << "Average seek length for SCAN scheduling is " << scanScheduling(100) << ".\n";
    cout << "Average seek length for CSCAN scheduling is " << cscanScheduling(100) << ".\n";
	cout << "Average seek length for LIFO scheduling is " << lifoScheduling(199) << ".\n";
	cout << "Average seek length for SSTF scheduling is " << sstfScheduling(199) << ".\n";
    cout << "Average seek length for SCAN scheduling is " << scanScheduling(199) << ".\n";
    cout << "Average seek length for CSCAN scheduling is " << cscanScheduling(199) << ".\n";

	generateRandomThousand();
	cout << "Simulation one with the head starting at track 0.\n";
	cout << "Average seek length for FIFO scheduling is " << fifoScheduling(0) << ".\n";
	cout << "Average seek length for LIFO scheduling is " << lifoScheduling(0) << ".\n";
	cout << "Average seek length for SSTF scheduling is " << sstfScheduling(0) << ".\n";
    cout << "Average seek length for SCAN scheduling is " << scanScheduling(0) << ".\n";
    cout << "Average seek length for CSCAN scheduling is " << cscanScheduling(0) << ".\n";
	cout << "Simulation one with the head starting at track 100.\n";
	cout << "Average seek length for FIFO scheduling is " << fifoScheduling(100) << ".\n";
	cout << "Average seek length for LIFO scheduling is " << lifoScheduling(100) << ".\n";
	cout << "Average seek length for SSTF scheduling is " << sstfScheduling(100) << ".\n";
    cout << "Average seek length for SCAN scheduling is " << scanScheduling(100) << ".\n";
    cout << "Average seek length for CSCAN scheduling is " << cscanScheduling(100) << ".\n";
	cout << "Simulation one with the head starting at track 199.\n";
	cout << "Average seek length for FIFO scheduling is " << fifoScheduling(199) << ".\n";
	cout << "Average seek length for LIFO scheduling is " << lifoScheduling(199) << ".\n";
	cout << "Average seek length for SSTF scheduling is " << sstfScheduling(199) << ".\n";
    cout << "Average seek length for SCAN scheduling is " << scanScheduling(199) << ".\n";
    cout << "Average seek length for CSCAN scheduling is " << cscanScheduling(199) << ".\n";

	generateRandomThousand();
	cout << "Simulation two with the head starting at track 0.\n";
	cout << "Average seek length for FIFO scheduling is " << fifoScheduling(0) << ".\n";
	cout << "Average seek length for LIFO scheduling is " << lifoScheduling(0) << ".\n";
	cout << "Average seek length for SSTF scheduling is " << sstfScheduling(0) << ".\n";
    cout << "Average seek length for SCAN scheduling is " << scanScheduling(0) << ".\n";
    cout << "Average seek length for CSCAN scheduling is " << cscanScheduling(0) << ".\n";
	cout << "Simulation two with the head starting at track 100.\n";
	cout << "Average seek length for FIFO scheduling is " << fifoScheduling(100) << ".\n";
	cout << "Average seek length for LIFO scheduling is " << lifoScheduling(100) << ".\n";
	cout << "Average seek length for SSTF scheduling is " << sstfScheduling(100) << ".\n";
    cout << "Average seek length for SCAN scheduling is " << scanScheduling(100) << ".\n";
    cout << "Average seek length for CSCAN scheduling is " << cscanScheduling(100) << ".\n";
	cout << "Simulation two with the head starting at track 199.\n";
	cout << "Average seek length for FIFO scheduling is " << fifoScheduling(199) << ".\n";
	cout << "Average seek length for LIFO scheduling is " << lifoScheduling(199) << ".\n";
	cout << "Average seek length for SSTF scheduling is " << sstfScheduling(199) << ".\n";
    cout << "Average seek length for SCAN scheduling is " << scanScheduling(199) << ".\n";
    cout << "Average seek length for CSCAN scheduling is " << cscanScheduling(199) << ".\n";

	generateRandomThousand();
	cout << "Simulation three with the head starting at track 0.\n";
	cout << "Average seek length for FIFO scheduling is " << fifoScheduling(0) << ".\n";
	cout << "Average seek length for LIFO scheduling is " << lifoScheduling(0) << ".\n";
	cout << "Average seek length for SSTF scheduling is " << sstfScheduling(0) << ".\n";
    cout << "Average seek length for SCAN scheduling is " << scanScheduling(0) << ".\n";
    cout << "Average seek length for CSCAN scheduling is " << cscanScheduling(0) << ".\n";
	cout << "Simulation three with the head starting at track 100.\n";
	cout << "Average seek length for FIFO scheduling is " << fifoScheduling(100) << ".\n";
	cout << "Average seek length for LIFO scheduling is " << lifoScheduling(100) << ".\n";
	cout << "Average seek length for SSTF scheduling is " << sstfScheduling(100) << ".\n";
    cout << "Average seek length for SCAN scheduling is " << scanScheduling(100) << ".\n";
    cout << "Average seek length for CSCAN scheduling is " << cscanScheduling(100) << ".\n";
	cout << "Simulation three with the head starting at track 199.\n";
	cout << "Average seek length for FIFO scheduling is " << fifoScheduling(199) << ".\n";
	cout << "Average seek length for LIFO scheduling is " << lifoScheduling(199) << ".\n";
	cout << "Average seek length for SSTF scheduling is " << sstfScheduling(199) << ".\n";
    cout << "Average seek length for SCAN scheduling is " << scanScheduling(199) << ".\n";
    cout << "Average seek length for CSCAN scheduling is " << cscanScheduling(199) << ".\n";

	generateRandomThousand();
	cout << "Simulation four with the head starting at track 0.\n";
	cout << "Average seek length for FIFO scheduling is " << fifoScheduling(0) << ".\n";
	cout << "Average seek length for LIFO scheduling is " << lifoScheduling(0) << ".\n";
	cout << "Average seek length for SSTF scheduling is " << sstfScheduling(0) << ".\n";
    cout << "Average seek length for SCAN scheduling is " << scanScheduling(0) << ".\n";
    cout << "Average seek length for CSCAN scheduling is " << cscanScheduling(0) << ".\n";
	cout << "Simulation four with the head starting at track 100.\n";
	cout << "Average seek length for FIFO scheduling is " << fifoScheduling(100) << ".\n";
	cout << "Average seek length for LIFO scheduling is " << lifoScheduling(100) << ".\n";
	cout << "Average seek length for SSTF scheduling is " << sstfScheduling(100) << ".\n";
    cout << "Average seek length for SCAN scheduling is " << scanScheduling(100) << ".\n";
    cout << "Average seek length for CSCAN scheduling is " << cscanScheduling(100) << ".\n";
	cout << "Simulation four with the head starting at track 199.\n";
	cout << "Average seek length for FIFO scheduling is " << fifoScheduling(199) << ".\n";
	cout << "Average seek length for LIFO scheduling is " << lifoScheduling(199) << ".\n";
	cout << "Average seek length for SSTF scheduling is " << sstfScheduling(199) << ".\n";
    cout << "Average seek length for SCAN scheduling is " << scanScheduling(199) << ".\n";
    cout << "Average seek length for CSCAN scheduling is " << cscanScheduling(199) << ".\n";

	generateRandomThousand();
	cout << "Simulation five with the head starting at track 0.\n";
	cout << "Average seek length for FIFO scheduling is " << fifoScheduling(0) << ".\n";
	cout << "Average seek length for LIFO scheduling is " << lifoScheduling(0) << ".\n";
	cout << "Average seek length for SSTF scheduling is " << sstfScheduling(0) << ".\n";
    cout << "Average seek length for SCAN scheduling is " << scanScheduling(0) << ".\n";
    cout << "Average seek length for CSCAN scheduling is " << cscanScheduling(0) << ".\n";
	cout << "Simulation five with the head starting at track 100.\n";
	cout << "Average seek length for FIFO scheduling is " << fifoScheduling(100) << ".\n";
	cout << "Average seek length for LIFO scheduling is " << lifoScheduling(100) << ".\n";
	cout << "Average seek length for SSTF scheduling is " << sstfScheduling(100) << ".\n";
    cout << "Average seek length for SCAN scheduling is " << scanScheduling(100) << ".\n";
    cout << "Average seek length for CSCAN scheduling is " << cscanScheduling(100) << ".\n";
	cout << "Simulation five with the head starting at track 199.\n";
	cout << "Average seek length for FIFO scheduling is " << fifoScheduling(199) << ".\n";
	cout << "Average seek length for LIFO scheduling is " << lifoScheduling(199) << ".\n";
	cout << "Average seek length for SSTF scheduling is " << sstfScheduling(199) << ".\n";
    cout << "Average seek length for SCAN scheduling is " << scanScheduling(199) << ".\n";
    cout << "Average seek length for CSCAN scheduling is " << cscanScheduling(199) << ".\n";



	generateSameTrackAccess();
	cout << "Simulation one with the head starting at track 0 and the probability of accessing the same track next is 10%.\n";
	cout << "Average seek length for FIFO scheduling is " << fifoScheduling(0) << ".\n";
	cout << "Average seek length for LIFO scheduling is " << lifoScheduling(0) << ".\n";
	cout << "Average seek length for SSTF scheduling is " << sstfScheduling(0) << ".\n";
    cout << "Average seek length for SCAN scheduling is " << scanScheduling(0) << ".\n";
    cout << "Average seek length for CSCAN scheduling is " << cscanScheduling(0) << ".\n";
	cout << "Simulation one with the head starting at track 100 and the probability of accessing the same track next is 10%.\n";
	cout << "Average seek length for FIFO scheduling is " << fifoScheduling(100) << ".\n";
	cout << "Average seek length for LIFO scheduling is " << lifoScheduling(100) << ".\n";
	cout << "Average seek length for SSTF scheduling is " << sstfScheduling(100) << ".\n";
    cout << "Average seek length for SCAN scheduling is " << scanScheduling(100) << ".\n";
    cout << "Average seek length for CSCAN scheduling is " << cscanScheduling(100) << ".\n";
	cout << "Simulation one with the head starting at track 199 and the probability of accessing the same track next is 10%.\n";
	cout << "Average seek length for FIFO scheduling is " << fifoScheduling(199) << ".\n";
	cout << "Average seek length for LIFO scheduling is " << lifoScheduling(199) << ".\n";
	cout << "Average seek length for SSTF scheduling is " << sstfScheduling(199) << ".\n";
    cout << "Average seek length for SCAN scheduling is " << scanScheduling(199) << ".\n";
    cout << "Average seek length for CSCAN scheduling is " << cscanScheduling(199) << ".\n";

	generateSameTrackAccess();
	cout << "Simulation two with the head starting at track 0 and the probability of accessing the same track next is 10%.\n";
	cout << "Average seek length for FIFO scheduling is " << fifoScheduling(0) << ".\n";
	cout << "Average seek length for LIFO scheduling is " << lifoScheduling(0) << ".\n";
	cout << "Average seek length for SSTF scheduling is " << sstfScheduling(0) << ".\n";
    cout << "Average seek length for SCAN scheduling is " << scanScheduling(0) << ".\n";
    cout << "Average seek length for CSCAN scheduling is " << cscanScheduling(0) << ".\n";
	cout << "Simulation two with the head starting at track 100 and the probability of accessing the same track next is 10%.\n";
	cout << "Average seek length for FIFO scheduling is " << fifoScheduling(100) << ".\n";
	cout << "Average seek length for LIFO scheduling is " << lifoScheduling(100) << ".\n";
	cout << "Average seek length for SSTF scheduling is " << sstfScheduling(100) << ".\n";
    cout << "Average seek length for SCAN scheduling is " << scanScheduling(100) << ".\n";
    cout << "Average seek length for CSCAN scheduling is " << cscanScheduling(100) << ".\n";
	cout << "Simulation two with the head starting at track 199 and the probability of accessing the same track next is 10%.\n";
	cout << "Average seek length for FIFO scheduling is " << fifoScheduling(199) << ".\n";
	cout << "Average seek length for LIFO scheduling is " << lifoScheduling(199) << ".\n";
	cout << "Average seek length for SSTF scheduling is " << sstfScheduling(199) << ".\n";
    cout << "Average seek length for SCAN scheduling is " << scanScheduling(199) << ".\n";
    cout << "Average seek length for CSCAN scheduling is " << cscanScheduling(199) << ".\n";

	generateSameTrackAccess();
	cout << "Simulation three with the head starting at track 0 and the probability of accessing the same track next is 10%.\n";
	cout << "Average seek length for FIFO scheduling is " << fifoScheduling(0) << ".\n";
	cout << "Average seek length for LIFO scheduling is " << lifoScheduling(0) << ".\n";
	cout << "Average seek length for SSTF scheduling is " << sstfScheduling(0) << ".\n";
    cout << "Average seek length for SCAN scheduling is " << scanScheduling(0) << ".\n";
    cout << "Average seek length for CSCAN scheduling is " << cscanScheduling(0) << ".\n";
	cout << "Simulation three with the head starting at track 100 and the probability of accessing the same track next is 10%.\n";
	cout << "Average seek length for FIFO scheduling is " << fifoScheduling(100) << ".\n";
	cout << "Average seek length for LIFO scheduling is " << lifoScheduling(100) << ".\n";
	cout << "Average seek length for SSTF scheduling is " << sstfScheduling(100) << ".\n";
    cout << "Average seek length for SCAN scheduling is " << scanScheduling(100) << ".\n";
    cout << "Average seek length for CSCAN scheduling is " << cscanScheduling(100) << ".\n";
	cout << "Simulation three with the head starting at track 199 and the probability of accessing the same track next is 10%.\n";
	cout << "Average seek length for FIFO scheduling is " << fifoScheduling(199) << ".\n";
	cout << "Average seek length for LIFO scheduling is " << lifoScheduling(199) << ".\n";
	cout << "Average seek length for SSTF scheduling is " << sstfScheduling(199) << ".\n";
    cout << "Average seek length for SCAN scheduling is " << scanScheduling(199) << ".\n";
    cout << "Average seek length for CSCAN scheduling is " << cscanScheduling(199) << ".\n";

	generateSameTrackAccess();
	cout << "Simulation four with the head starting at track 0 and the probability of accessing the same track next is 10%.\n";
	cout << "Average seek length for FIFO scheduling is " << fifoScheduling(0) << ".\n";
	cout << "Average seek length for LIFO scheduling is " << lifoScheduling(0) << ".\n";
	cout << "Average seek length for SSTF scheduling is " << sstfScheduling(0) << ".\n";
    cout << "Average seek length for SCAN scheduling is " << scanScheduling(0) << ".\n";
    cout << "Average seek length for CSCAN scheduling is " << cscanScheduling(0) << ".\n";
	cout << "Simulation four with the head starting at track 100 and the probability of accessing the same track next is 10%.\n";
	cout << "Average seek length for FIFO scheduling is " << fifoScheduling(100) << ".\n";
	cout << "Average seek length for LIFO scheduling is " << lifoScheduling(100) << ".\n";
	cout << "Average seek length for SSTF scheduling is " << sstfScheduling(100) << ".\n";
    cout << "Average seek length for SCAN scheduling is " << scanScheduling(100) << ".\n";
    cout << "Average seek length for CSCAN scheduling is " << cscanScheduling(100) << ".\n";
	cout << "Simulation four with the head starting at track 199 and the probability of accessing the same track next is 10%.\n";
	cout << "Average seek length for FIFO scheduling is " << fifoScheduling(199) << ".\n";
	cout << "Average seek length for LIFO scheduling is " << lifoScheduling(199) << ".\n";
	cout << "Average seek length for SSTF scheduling is " << sstfScheduling(199) << ".\n";
    cout << "Average seek length for SCAN scheduling is " << scanScheduling(199) << ".\n";
    cout << "Average seek length for CSCAN scheduling is " << cscanScheduling(199) << ".\n";

	generateSameTrackAccess();
	cout << "Simulation five with the head starting at track 0 and the probability of accessing the same track next is 10%.\n";
	cout << "Average seek length for FIFO scheduling is " << fifoScheduling(0) << ".\n";
	cout << "Average seek length for LIFO scheduling is " << lifoScheduling(0) << ".\n";
	cout << "Average seek length for SSTF scheduling is " << sstfScheduling(0) << ".\n";
    cout << "Average seek length for SCAN scheduling is " << scanScheduling(0) << ".\n";
    cout << "Average seek length for CSCAN scheduling is " << cscanScheduling(0) << ".\n";
	cout << "Simulation five with the head starting at track 100 and the probability of accessing the same track next is 10%.\n";
	cout << "Average seek length for FIFO scheduling is " << fifoScheduling(100) << ".\n";
	cout << "Average seek length for LIFO scheduling is " << lifoScheduling(100) << ".\n";
	cout << "Average seek length for SSTF scheduling is " << sstfScheduling(100) << ".\n";
    cout << "Average seek length for SCAN scheduling is " << scanScheduling(100) << ".\n";
    cout << "Average seek length for CSCAN scheduling is " << cscanScheduling(100) << ".\n";
	cout << "Simulation five with the head starting at track 199 and the probability of accessing the same track next is 10%.\n";
	cout << "Average seek length for FIFO scheduling is " << fifoScheduling(199) << ".\n";
	cout << "Average seek length for LIFO scheduling is " << lifoScheduling(199) << ".\n";
	cout << "Average seek length for SSTF scheduling is " << sstfScheduling(199) << ".\n";
    cout << "Average seek length for SCAN scheduling is " << scanScheduling(199) << ".\n";
    cout << "Average seek length for CSCAN scheduling is " << cscanScheduling(199) << ".\n";



	generateFarthestDistance();
	cout << "Simulation one with the head starting at track 0 and the probability of accessing the furthest track from the current track is 1%.\n";
	cout << "Average seek length for FIFO scheduling is " << fifoScheduling(0) << ".\n";
	cout << "Average seek length for LIFO scheduling is " << lifoScheduling(0) << ".\n";
	cout << "Average seek length for SSTF scheduling is " << sstfScheduling(0) << ".\n";
    cout << "Average seek length for SCAN scheduling is " << scanScheduling(0) << ".\n";
    cout << "Average seek length for CSCAN scheduling is " << cscanScheduling(0) << ".\n";
	cout << "Simulation one with the head starting at track 100 and the probability of accessing the furthest track from the current track is 1%.\n";
	cout << "Average seek length for FIFO scheduling is " << fifoScheduling(100) << ".\n";
	cout << "Average seek length for LIFO scheduling is " << lifoScheduling(100) << ".\n";
	cout << "Average seek length for SSTF scheduling is " << sstfScheduling(100) << ".\n";
    cout << "Average seek length for SCAN scheduling is " << scanScheduling(100) << ".\n";
    cout << "Average seek length for CSCAN scheduling is " << cscanScheduling(100) << ".\n";
	cout << "Simulation one with the head starting at track 199 and the probability of accessing the furthest track from the current track is 1%.\n";
	cout << "Average seek length for FIFO scheduling is " << fifoScheduling(199) << ".\n";
	cout << "Average seek length for LIFO scheduling is " << lifoScheduling(199) << ".\n";
	cout << "Average seek length for SSTF scheduling is " << sstfScheduling(199) << ".\n";
    cout << "Average seek length for SCAN scheduling is " << scanScheduling(199) << ".\n";
    cout << "Average seek length for CSCAN scheduling is " << cscanScheduling(199) << ".\n";

	generateFarthestDistance();
	cout << "Simulation two with the head starting at track 0 and the probability of accessing the furthest track from the current track is 1%.\n";
	cout << "Average seek length for FIFO scheduling is " << fifoScheduling(0) << ".\n";
	cout << "Average seek length for LIFO scheduling is " << lifoScheduling(0) << ".\n";
	cout << "Average seek length for SSTF scheduling is " << sstfScheduling(0) << ".\n";
    cout << "Average seek length for SCAN scheduling is " << scanScheduling(0) << ".\n";
    cout << "Average seek length for CSCAN scheduling is " << cscanScheduling(0) << ".\n";
	cout << "Simulation two with the head starting at track 100 and the probability of accessing the furthest track from the current track is 1%.\n";
	cout << "Average seek length for FIFO scheduling is " << fifoScheduling(100) << ".\n";
	cout << "Average seek length for LIFO scheduling is " << lifoScheduling(100) << ".\n";
	cout << "Average seek length for SSTF scheduling is " << sstfScheduling(100) << ".\n";
    cout << "Average seek length for SCAN scheduling is " << scanScheduling(100) << ".\n";
    cout << "Average seek length for CSCAN scheduling is " << cscanScheduling(100) << ".\n";
	cout << "Simulation two with the head starting at track 199 and the probability of accessing the furthest track from the current track is 1%.\n";
	cout << "Average seek length for FIFO scheduling is " << fifoScheduling(199) << ".\n";
	cout << "Average seek length for LIFO scheduling is " << lifoScheduling(199) << ".\n";
	cout << "Average seek length for SSTF scheduling is " << sstfScheduling(199) << ".\n";
    cout << "Average seek length for SCAN scheduling is " << scanScheduling(199) << ".\n";
    cout << "Average seek length for CSCAN scheduling is " << cscanScheduling(199) << ".\n";

	generateFarthestDistance();
	cout << "Simulation three with the head starting at track 0 and the probability of accessing the furthest track from the current track is 1%.\n";
	cout << "Average seek length for FIFO scheduling is " << fifoScheduling(0) << ".\n";
	cout << "Average seek length for LIFO scheduling is " << lifoScheduling(0) << ".\n";
	cout << "Average seek length for SSTF scheduling is " << sstfScheduling(0) << ".\n";
    cout << "Average seek length for SCAN scheduling is " << scanScheduling(0) << ".\n";
    cout << "Average seek length for CSCAN scheduling is " << cscanScheduling(0) << ".\n";
	cout << "Simulation three with the head starting at track 100 and the probability of accessing the furthest track from the current track is 1%.\n";
	cout << "Average seek length for FIFO scheduling is " << fifoScheduling(100) << ".\n";
	cout << "Average seek length for LIFO scheduling is " << lifoScheduling(100) << ".\n";
	cout << "Average seek length for SSTF scheduling is " << sstfScheduling(100) << ".\n";
    cout << "Average seek length for SCAN scheduling is " << scanScheduling(100) << ".\n";
    cout << "Average seek length for CSCAN scheduling is " << cscanScheduling(100) << ".\n";
	cout << "Simulation three with the head starting at track 199 and the probability of accessing the furthest track from the current track is 1%.\n";
	cout << "Average seek length for FIFO scheduling is " << fifoScheduling(199) << ".\n";
	cout << "Average seek length for LIFO scheduling is " << lifoScheduling(199) << ".\n";
	cout << "Average seek length for SSTF scheduling is " << sstfScheduling(199) << ".\n";
    cout << "Average seek length for SCAN scheduling is " << scanScheduling(199) << ".\n";
    cout << "Average seek length for CSCAN scheduling is " << cscanScheduling(199) << ".\n";

	generateFarthestDistance();
	cout << "Simulation four with the head starting at track 0 and the probability of accessing the furthest track from the current track is 1%.\n";
	cout << "Average seek length for FIFO scheduling is " << fifoScheduling(0) << ".\n";
	cout << "Average seek length for LIFO scheduling is " << lifoScheduling(0) << ".\n";
	cout << "Average seek length for SSTF scheduling is " << sstfScheduling(0) << ".\n";
    cout << "Average seek length for SCAN scheduling is " << scanScheduling(0) << ".\n";
    cout << "Average seek length for CSCAN scheduling is " << cscanScheduling(0) << ".\n";
	cout << "Simulation four with the head starting at track 100 and the probability of accessing the furthest track from the current track is 1%.\n";
	cout << "Average seek length for FIFO scheduling is " << fifoScheduling(100) << ".\n";
	cout << "Average seek length for LIFO scheduling is " << lifoScheduling(100) << ".\n";
	cout << "Average seek length for SSTF scheduling is " << sstfScheduling(100) << ".\n";
    cout << "Average seek length for SCAN scheduling is " << scanScheduling(100) << ".\n";
    cout << "Average seek length for CSCAN scheduling is " << cscanScheduling(100) << ".\n";
	cout << "Simulation four with the head starting at track 199 and the probability of accessing the furthest track from the current track is 1%.\n";
	cout << "Average seek length for FIFO scheduling is " << fifoScheduling(199) << ".\n";
	cout << "Average seek length for LIFO scheduling is " << lifoScheduling(199) << ".\n";
	cout << "Average seek length for SSTF scheduling is " << sstfScheduling(199) << ".\n";
    cout << "Average seek length for SCAN scheduling is " << scanScheduling(199) << ".\n";
    cout << "Average seek length for CSCAN scheduling is " << cscanScheduling(199) << ".\n";

	generateFarthestDistance();
	cout << "Simulation five with the head starting at track 0 and the probability of accessing the furthest track from the current track is 1%.\n";
	cout << "Average seek length for FIFO scheduling is " << fifoScheduling(0) << ".\n";
	cout << "Average seek length for LIFO scheduling is " << lifoScheduling(0) << ".\n";
	cout << "Average seek length for SSTF scheduling is " << sstfScheduling(0) << ".\n";
    cout << "Average seek length for SCAN scheduling is " << scanScheduling(0) << ".\n";
    cout << "Average seek length for CSCAN scheduling is " << cscanScheduling(0) << ".\n";
	cout << "Simulation five with the head starting at track 100 and the probability of accessing the furthest track from the current track is 1%.\n";
	cout << "Average seek length for FIFO scheduling is " << fifoScheduling(100) << ".\n";
	cout << "Average seek length for LIFO scheduling is " << lifoScheduling(100) << ".\n";
	cout << "Average seek length for SSTF scheduling is " << sstfScheduling(100) << ".\n";
    cout << "Average seek length for SCAN scheduling is " << scanScheduling(100) << ".\n";
    cout << "Average seek length for CSCAN scheduling is " << cscanScheduling(100) << ".\n";
	cout << "Simulation five with the head starting at track 199 and the probability of accessing the furthest track from the current track is 1%.\n";
	cout << "Average seek length for FIFO scheduling is " << fifoScheduling(199) << ".\n";
	cout << "Average seek length for LIFO scheduling is " << lifoScheduling(199) << ".\n";
	cout << "Average seek length for SSTF scheduling is " << sstfScheduling(199) << ".\n";
    cout << "Average seek length for SCAN scheduling is " << scanScheduling(199) << ".\n";
    cout << "Average seek length for CSCAN scheduling is " << cscanScheduling(199) << ".\n";



	generateDistanceDecrease();
	cout << "Simulation one with the head starting at track 0 and the probability of accessing a track between the current track and the furthest track decreases as you move away from the current track.\n";
	cout << "Average seek length for FIFO scheduling is " << fifoScheduling(0) << ".\n";
	cout << "Average seek length for LIFO scheduling is " << lifoScheduling(0) << ".\n";
	cout << "Average seek length for SSTF scheduling is " << sstfScheduling(0) << ".\n";
    cout << "Average seek length for SCAN scheduling is " << scanScheduling(0) << ".\n";
    cout << "Average seek length for CSCAN scheduling is " << cscanScheduling(0) << ".\n";
	cout << "Simulation one with the head starting at track 100 and the probability of accessing a track between the current track and the furthest track decreases as you move away from the current track.\n";
	cout << "Average seek length for FIFO scheduling is " << fifoScheduling(100) << ".\n";
	cout << "Average seek length for LIFO scheduling is " << lifoScheduling(100) << ".\n";
	cout << "Average seek length for SSTF scheduling is " << sstfScheduling(100) << ".\n";
    cout << "Average seek length for SCAN scheduling is " << scanScheduling(100) << ".\n";
    cout << "Average seek length for CSCAN scheduling is " << cscanScheduling(100) << ".\n";
	cout << "Simulation one with the head starting at track 199 and the probability of accessing a track between the current track and the furthest track decreases as you move away from the current track.\n";
	cout << "Average seek length for FIFO scheduling is " << fifoScheduling(199) << ".\n";
	cout << "Average seek length for LIFO scheduling is " << lifoScheduling(199) << ".\n";
	cout << "Average seek length for SSTF scheduling is " << sstfScheduling(199) << ".\n";
    cout << "Average seek length for SCAN scheduling is " << scanScheduling(199) << ".\n";
    cout << "Average seek length for CSCAN scheduling is " << cscanScheduling(199) << ".\n";

	generateDistanceDecrease();	
	cout << "Simulation two with the head starting at track 0 and the probability of accessing a track between the current track and the furthest track decreases as you move away from the current track.\n";
	cout << "Average seek length for FIFO scheduling is " << fifoScheduling(0) << ".\n";
	cout << "Average seek length for LIFO scheduling is " << lifoScheduling(0) << ".\n";
	cout << "Average seek length for SSTF scheduling is " << sstfScheduling(0) << ".\n";
    cout << "Average seek length for SCAN scheduling is " << scanScheduling(0) << ".\n";
    cout << "Average seek length for CSCAN scheduling is " << cscanScheduling(0) << ".\n";
	cout << "Simulation two with the head starting at track 100 and the probability of accessing a track between the current track and the furthest track decreases as you move away from the current track.\n";
	cout << "Average seek length for FIFO scheduling is " << fifoScheduling(100) << ".\n";
	cout << "Average seek length for LIFO scheduling is " << lifoScheduling(100) << ".\n";
	cout << "Average seek length for SSTF scheduling is " << sstfScheduling(100) << ".\n";
    cout << "Average seek length for SCAN scheduling is " << scanScheduling(100) << ".\n";
    cout << "Average seek length for CSCAN scheduling is " << cscanScheduling(100) << ".\n";
	cout << "Simulation two with the head starting at track 199 and the probability of accessing a track between the current track and the furthest track decreases as you move away from the current track.\n";
	cout << "Average seek length for FIFO scheduling is " << fifoScheduling(199) << ".\n";
	cout << "Average seek length for LIFO scheduling is " << lifoScheduling(199) << ".\n";
	cout << "Average seek length for SSTF scheduling is " << sstfScheduling(199) << ".\n";
    cout << "Average seek length for SCAN scheduling is " << scanScheduling(199) << ".\n";
    cout << "Average seek length for CSCAN scheduling is " << cscanScheduling(199) << ".\n";

	generateDistanceDecrease();
	cout << "Simulation three with the head starting at track 0 and the probability of accessing a track between the current track and the furthest track decreases as you move away from the current track.\n";
	cout << "Average seek length for FIFO scheduling is " << fifoScheduling(0) << ".\n";
	cout << "Average seek length for LIFO scheduling is " << lifoScheduling(0) << ".\n";
	cout << "Average seek length for SSTF scheduling is " << sstfScheduling(0) << ".\n";
    cout << "Average seek length for SCAN scheduling is " << scanScheduling(0) << ".\n";
    cout << "Average seek length for CSCAN scheduling is " << cscanScheduling(0) << ".\n";
	cout << "Simulation three with the head starting at track 100 and the probability of accessing a track between the current track and the furthest track decreases as you move away from the current track.\n";
	cout << "Average seek length for FIFO scheduling is " << fifoScheduling(100) << ".\n";
	cout << "Average seek length for LIFO scheduling is " << lifoScheduling(100) << ".\n";
	cout << "Average seek length for SSTF scheduling is " << sstfScheduling(100) << ".\n";
    cout << "Average seek length for SCAN scheduling is " << scanScheduling(100) << ".\n";
    cout << "Average seek length for CSCAN scheduling is " << cscanScheduling(100) << ".\n";
	cout << "Simulation three with the head starting at track 199 and the probability of accessing a track between the current track and the furthest track decreases as you move away from the current track.\n";
	cout << "Average seek length for FIFO scheduling is " << fifoScheduling(199) << ".\n";
	cout << "Average seek length for LIFO scheduling is " << lifoScheduling(199) << ".\n";
	cout << "Average seek length for SSTF scheduling is " << sstfScheduling(199) << ".\n";
    cout << "Average seek length for SCAN scheduling is " << scanScheduling(199) << ".\n";
    cout << "Average seek length for CSCAN scheduling is " << cscanScheduling(199) << ".\n";

	generateDistanceDecrease();
	cout << "Simulation four with the head starting at track 0 and the probability of accessing a track between the current track and the furthest track decreases as you move away from the current track.\n";
	cout << "Average seek length for FIFO scheduling is " << fifoScheduling(0) << ".\n";
	cout << "Average seek length for LIFO scheduling is " << lifoScheduling(0) << ".\n";
	cout << "Average seek length for SSTF scheduling is " << sstfScheduling(0) << ".\n";
    cout << "Average seek length for SCAN scheduling is " << scanScheduling(0) << ".\n";
    cout << "Average seek length for CSCAN scheduling is " << cscanScheduling(0) << ".\n";
	cout << "Simulation four with the head starting at track 100 and the probability of accessing a track between the current track and the furthest track decreases as you move away from the current track.\n";
	cout << "Average seek length for FIFO scheduling is " << fifoScheduling(100) << ".\n";
	cout << "Average seek length for LIFO scheduling is " << lifoScheduling(100) << ".\n";
	cout << "Average seek length for SSTF scheduling is " << sstfScheduling(100) << ".\n";
    cout << "Average seek length for SCAN scheduling is " << scanScheduling(100) << ".\n";
    cout << "Average seek length for CSCAN scheduling is " << cscanScheduling(100) << ".\n";
	cout << "Simulation four with the head starting at track 199 and the probability of accessing a track between the current track and the furthest track decreases as you move away from the current track.\n";
	cout << "Average seek length for FIFO scheduling is " << fifoScheduling(199) << ".\n";
	cout << "Average seek length for LIFO scheduling is " << lifoScheduling(199) << ".\n";
	cout << "Average seek length for SSTF scheduling is " << sstfScheduling(199) << ".\n";
    cout << "Average seek length for SCAN scheduling is " << scanScheduling(199) << ".\n";
    cout << "Average seek length for CSCAN scheduling is " << cscanScheduling(199) << ".\n";

	generateDistanceDecrease();
	cout << "Simulation five with the head starting at track 0 and the probability of accessing a track between the current track and the furthest track decreases as you move away from the current track.\n";
	cout << "Average seek length for FIFO scheduling is " << fifoScheduling(0) << ".\n";
	cout << "Average seek length for LIFO scheduling is " << lifoScheduling(0) << ".\n";
	cout << "Average seek length for SSTF scheduling is " << sstfScheduling(0) << ".\n";
    cout << "Average seek length for SCAN scheduling is " << scanScheduling(0) << ".\n";
    cout << "Average seek length for CSCAN scheduling is " << cscanScheduling(0) << ".\n";
	cout << "Simulation five with the head starting at track 100 and the probability of accessing a track between the current track and the furthest track decreases as you move away from the current track.\n";
	cout << "Average seek length for FIFO scheduling is " << fifoScheduling(100) << ".\n";
	cout << "Average seek length for LIFO scheduling is " << lifoScheduling(100) << ".\n";
	cout << "Average seek length for SSTF scheduling is " << sstfScheduling(100) << ".\n";
    cout << "Average seek length for SCAN scheduling is " << scanScheduling(100) << ".\n";
    cout << "Average seek length for CSCAN scheduling is " << cscanScheduling(100) << ".\n";
	cout << "Simulation five with the head starting at track 199 and the probability of accessing a track between the current track and the furthest track decreases as you move away from the current track.\n";
	cout << "Average seek length for FIFO scheduling is " << fifoScheduling(199) << ".\n";
	cout << "Average seek length for LIFO scheduling is " << lifoScheduling(199) << ".\n";
	cout << "Average seek length for SSTF scheduling is " << sstfScheduling(199) << ".\n";
    cout << "Average seek length for SCAN scheduling is " << scanScheduling(199) << ".\n";
    cout << "Average seek length for CSCAN scheduling is " << cscanScheduling(199) << ".\n";
}