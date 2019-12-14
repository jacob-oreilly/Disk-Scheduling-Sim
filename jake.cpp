#include <iostream>
#include <array>
#include <cmath>

using namespace std;

int trackList[] = {55, 58, 39, 18, 90, 160, 150, 38, 184};
int queueTrackList[5];
int diskSize = 200; 


//scan scheduling policy edited for FSCAN
double scanScheduling(int headPosition) {
	int tracksTraversed = 0;
	int trackListLength = sizeof(queueTrackList) / sizeof(queueTrackList[0]);
	int trackListAccd[trackListLength];
	int positiveMove = 1;
	int maxElement = 0;
	for (int i = 0; i < trackListLength; i++) {
		if (queueTrackList[i] > maxElement) {
			maxElement = queueTrackList[i];
		}
	}
	int minElement = diskSize;
	for (int i = 0; i < trackListLength; i++) {
		if (queueTrackList[i] < minElement) {
			minElement = queueTrackList[i];
		}
	}
	while (true) {
		int tempCount = 0;
		for (int i = 0; i < trackListLength; i++) {
			if (headPosition == queueTrackList[i] && trackListAccd[i] != 1) {
				trackListAccd[i] = 1;
			}
			tempCount += trackListAccd[i];
			if (tempCount == trackListLength) {
				// return (double)tracksTraversed / trackListLength;
				return tracksTraversed;
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

//scan scheduling policy edited for N-Step
double scanSchedulingNStep(int headPosition, int* queueArrayColumn, int rows) {
	int tracksTraversed = 0;
	int trackListLength = rows;
	int trackListAccd[trackListLength];
	int positiveMove = 1;
	int maxElement = 0;
	for (int i = 0; i < trackListLength; i++) {
		if (*(queueArrayColumn+i) > maxElement) {
			maxElement = *(queueArrayColumn+i);
		}
	}
	int minElement = diskSize;
	for (int i = 0; i < trackListLength; i++) {
		if (*(queueArrayColumn+i) < minElement) {
			minElement = *(queueArrayColumn+i);
		}
	}
	while (true) {
		int tempCount = 0;
		for (int i = 0; i < trackListLength; i++) {
			if (headPosition == *(queueArrayColumn+i) && trackListAccd[i] != 1) {
				trackListAccd[i] = 1;
			}
			tempCount += trackListAccd[i];
			if (tempCount == trackListLength) {
				// return (double)tracksTraversed / trackListLength;
				return tracksTraversed;
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

double nStepScan(int nQueues, int headPosition) {
    int tracksTraversed = 0;
	int trackListLength = sizeof(trackList) / sizeof(trackList[0]);
	int columns = nQueues;
	int rows = (trackListLength / nQueues);
	int queueArray[columns][rows];
	int startIndex = 1;
	for(int i = 0; i < trackList[i]; i++) {
		queueArray[int(ceil(i/nQueues))][i%nQueues] = trackList[i];		
	}
	
    int queueDecider = 0;
    while(true) {
        if(queueDecider <= nQueues) {
            tracksTraversed += scanSchedulingNStep(headPosition, queueArray[queueDecider], rows);
			queueDecider += 1;
        } else {
			// cout << (double)tracksTraversed / trackListLength;
			//THIS IS NOT RETURNING RIGHT ANSWER.
			return (double)tracksTraversed / trackListLength;
        }
		
    }
}

double FSCAN(int headPosition) {
    int tracksTraversed = 0;
	int trackListLength = sizeof(trackList) / sizeof(trackList[0]);
    int queue1[trackListLength / 2];
    int queue2[trackListLength / 2];

	for(int i = 0; i < trackListLength; i++) {
		if((trackListLength / 2) > i) {
			queue1[i] = trackList[i];
			queueTrackList[i] = queue1[i];
		} else {
			queue2[i] = trackList[i];
			queueTrackList[i] = queue2[i];
		}
	}
	
    bool queueDecider = true;
    while(true) {
        if(queueDecider) {
            tracksTraversed += scanScheduling(headPosition);
            queueDecider = !queueDecider;
        } else {
            tracksTraversed += scanScheduling(headPosition);
            queueDecider = !queueDecider;
			cout << (double)tracksTraversed / trackListLength;
			return (double)tracksTraversed / trackListLength;
        }
		
    }
	
}

int main() {
    int head = 50;
	cout << "fscan is " << FSCAN(head) << "\n";
	//cout << nStepScan(3, head);
}