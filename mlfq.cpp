#include <iostream>
#include <queue>
using namespace std;

#define QUEUE_SIZE 2
#define JOB_SIZE 2
#define BOOST_TIME 10

typedef struct Queues {
  int timeSlice;
  int timeAllotment;
  queue<int> jobs;
} Queues;

typedef struct Job {
  int currentQ;
  int ticksLeft;
  int allotLeft;
  int startTime;
  int runTime;
  int timeLeft;
  int firstRun;
  int endTime;
} Job;

struct Queues q[QUEUE_SIZE] = {{
                                   .timeSlice = 10,
                                   .timeAllotment = 20,
                               },
                               {
                                   .timeSlice = 3,
                                   .timeAllotment = 6,
                               }};

struct Job j[JOB_SIZE] = {{
                              .startTime = 2,
                              .runTime = 10,
                          },
                          {
                              .startTime = 0,
                              .runTime = 10,
                          }};

int findJob(int currentTime) {
  int index = -1;
  for (int i = QUEUE_SIZE - 1; i >= 0; i--) {
    if (!q[i].jobs.empty()) {
      return q[i].jobs.front();
    }
  }
  return index;
}

void initJobs(int currentTime) {
  for (int i = 0; i < JOB_SIZE; i++) {
    if (j[i].startTime == currentTime) {
      q[QUEUE_SIZE - 1].jobs.push(i);
      j[i].currentQ = QUEUE_SIZE - 1;
      j[i].allotLeft = q[QUEUE_SIZE - 1].timeAllotment;
      j[i].ticksLeft = q[QUEUE_SIZE - 1].timeSlice;
      j[i].firstRun = -1;
      j[i].timeLeft = j[i].runTime;
    }
  }
}
int main() {

  int finishedJobs = 0;
  int boostTime = 10;
  int currentTime = 0;
  int currentQ = -1;

  while (finishedJobs < JOB_SIZE) {
    initJobs(currentTime);
    int jobIndex = findJob(currentTime);

    if (jobIndex == -1) {
      printf("[ time %2d ] IDLE\n", currentTime);
      currentTime += 1;
      // break;
      continue;
    }
    currentQ = j[jobIndex].currentQ;

    // printf("[ time %2d ] Started job #%d\n", currentTime, jobIndex);
    //  break;

    if (j[jobIndex].firstRun == -1) {
      j[jobIndex].firstRun = currentTime;
      printf("[ time %2d ] Started job #%d\n", currentTime, jobIndex);
      // finishedJobs++;
    }

    j[jobIndex].allotLeft--;
    j[jobIndex].timeLeft--;
    j[jobIndex].ticksLeft--;

    printf(
        "[ time %2d ] Run JOB %d at PRIORITY %d [ TICKS %d ALLOT %d time %2d "
        "(of %d) ]\n",
        currentTime, jobIndex, j[jobIndex].currentQ, j[jobIndex].ticksLeft,
        j[jobIndex].allotLeft, j[jobIndex].timeLeft, j[jobIndex].runTime);

    currentTime += 1;

    if (j[jobIndex].timeLeft == 0) {

      printf("[ time %2d ] FINISHED JOB %d\n", currentTime, jobIndex);
      finishedJobs += 1;
      j[jobIndex].endTime = currentTime;
      // Remove job
      q[currentQ].jobs.pop();
      continue;
    }

    if (j[jobIndex].ticksLeft == 0) {
      if (j[jobIndex].allotLeft == 0 && j[jobIndex].currentQ > 0) {
        printf("[ time %2d ] Lowering Queue for JOB %d\n", currentTime,
               jobIndex);
        j[jobIndex].currentQ -= 1;
        int tmpQ = j[jobIndex].currentQ;
        j[jobIndex].allotLeft = q[tmpQ].timeAllotment;
      }
      int tmpQ = j[jobIndex].currentQ;
      j[jobIndex].ticksLeft = q[tmpQ].timeSlice;
      // push this job to the end
      q[currentQ].jobs.pop();
      q[tmpQ].jobs.push(jobIndex);
    }
  }

  printf("Final statistics:\n");
  int responseSum = 0;
  int turnaroundSum = 0;
  for (int i = 0; i < JOB_SIZE; i++) {
    int response = j[i].firstRun - j[i].startTime;
    int turnaround = j[i].endTime - j[i].startTime;
    printf("Job %2d: startTime %3d - response %3d - turnaround %3d\n", i,
           j[i].startTime, response, turnaround);
    responseSum += response;
    turnaroundSum += turnaround;
  }
  printf("\n  Avg %2d: startTime n/a - response %.2f - turnaround %.2f\n",
         JOB_SIZE, (float)(responseSum) / JOB_SIZE,
         (float)turnaroundSum / JOB_SIZE);
}