#include <iostream>
#include <array>
#include <cmath>

using namespace std;

int trackList[] = {55, 58, 39, 18, 90, 160, 150, 38, 184};
int diskSize = 200; 


//scan scheduling policy edited for FSCAN
double scanModScheduling(int headPosition) {
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
    cout << ""; //leave here it breaks otherwise
    int tempArrSize = sizeof(queueArrayColumn)/sizeof(queueArrayColumn[0]);
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
        if(int(ceil(i/nQueues))<rows){ //added to prevent extra creation
		    queueArray[int(ceil(i/nQueues))][i%nQueues] = trackList[i];		
        }
	}
    int queueDecider = 0;
    while(true) {
        if(queueDecider < nQueues) {
            tracksTraversed += scanSchedulingNStep(headPosition, queueArray[queueDecider], rows);
			queueDecider += 1;
        } else {
			//THIS IS NOT RETURNING RIGHT ANSWER.
			return (double)tracksTraversed / trackListLength;
        }
		
    }
}

double FSCAN(int headPosition) {
    int tracksTraversed = 0;
	int trackListLength = sizeof(trackList) / sizeof(trackList[0]);
    bool queueDecider = true;
    while(true) {
        if(queueDecider) {
            //read and run queue 1
            tracksTraversed += scanModScheduling(headPosition);
            queueDecider = !queueDecider;
        } else {
            //read and run on queue 2
            tracksTraversed += scanModScheduling(headPosition);
            queueDecider = !queueDecider;
			return (double)tracksTraversed / trackListLength;
        }
		
    }
	
}

int main() {
    int head = 100;
	cout << "fscan is " << FSCAN(head) << "\n";
	cout << "nstep is " << nStepScan(3, head) << "\n";
}
