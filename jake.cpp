#include <iostream>
#include <array>

using namespace std;

int trackList[] = {55, 58, 39, 18, 90, 160, 150, 38, 184};
int disk_size = 200; 

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

// double nStepScan(int nReqeusts, int headPosition) {
//     int tracksTraversed = 0;
// 	int trackListLength = sizeof(trackList) / sizeof(trackList[0]);
//     int queue1[trackListLength];
//     int queue2[trackListLength];
//     bool queueDecider = true;
//     for (int i = 0; i < trackListLength; i++) {
//         if(queueDecider) {
//             queue1 = headPosition;
//             scanScheduling(headPosition)
//             queueDecider = !queueDecider
//         } else {
//             queue2 = headPosition;
//             scanScheduling(headPosition)
//             queueDecider = !queueDecider
//         }
//         tracksTraversed++;
//     }
//     return (double)tracksTraversed / trackListLength;
// }

double FSCAN(int headPosition) {
    int tracksTraversed = 0;
	int trackListLength = sizeof(trackList) / sizeof(trackList[0]);
    int queue1[trackListLength];
    int queue2[trackListLength];
    bool queueDecider = true;
    for (int i = 0; i < trackListLength; i++) {
        if(queueDecider) {
            queue1 = headPosition;
            scanScheduling(queue1[i]);
            queueDecider = !queueDecider;
        } else {
            queue2 = headPosition;
            scanScheduling(queue2);
            queueDecider = !queueDecider;
        }
        tracksTraversed++;
    }
    return (double)tracksTraversed / trackListLength;
}

int main() {
    int head = 50;
    // cout << "Average seek length for FIFO scheduling is " << << ".\n";
    // cout << "Average seek length for LIFO scheduling is " << << ".\n";
}