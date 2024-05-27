/*

  // Producer Consumer
  #include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 10
typedef struct {
  int buf[BUFFER_SIZE];
  size_t len;
  pthread_mutex_t mutex;
  pthread_cond_t can_produce;
  pthread_cond_t can_consume;
} buffer_t;

void *producer(void *arg) {
  buffer_t *buffer = (buffer_t *)arg;
  while (1) {
#ifdef UNDERFLOW
    sleep(5)
#endif
        pthread_mutex_lock(&buffer->mutex);
    if (buffer->len == BUFFER_SIZE) {
      pthread_cond_wait(&buffer->can_produce, &buffer->mutex);
    }
    int randnum = rand();
    printf("Produced Items %d ", randnum);
    buffer->buf[buffer->len] = randnum;
    ++buffer->len;

    pthread_cond_signal(&buffer->can_consume);
    pthread_mutex_unlock(&buffer->mutex);

    printf("FULL: %zu\n", buffer->len);
    printf("Empty: %zu\n", BUFFER_SIZE - buffer->len);
  }
}
void *consumer(void *arg) {
  buffer_t *buffer = (buffer_t *)arg;
  while (1) {
#ifdef OVERFLOW
    sleep(5);
#endif

    pthread_mutex_lock(&buffer->mutex);
    if (buffer->len == 0) {
      pthread_cond_wait(&buffer->can_consume, &buffer->mutex);
    }
    int data = buffer->buf[buffer->len];
    printf("Consumed : %d ", data);
    --buffer->len;

    pthread_cond_signal(&buffer->can_produce);
    pthread_mutex_unlock(&buffer->mutex);
    printf("FULL: %zu\n", buffer->len);
    printf("Empty: %zu\n", BUFFER_SIZE - buffer->len);
  }
}
int main(int argc, char *argv[]) {
  buffer_t buffer = {.len = 0,
                     .mutex = PTHREAD_MUTEX_INITIALIZER,
                     .can_consume = PTHREAD_COND_INITIALIZER,
                     .can_produce = PTHREAD_COND_INITIALIZER};
  pthread_t prod, cons;
  pthread_create(&prod, NULL, producer, (void *)&buffer);
  pthread_create(&cons, NULL, consumer, (void *)&buffer);
  pthread_join(prod, NULL);
  pthread_join(cons, NULL);
  return 0;
}

// Reader Writer
#include<pthread.h>
#include<semaphore.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

sem_t mutex,writeblock;
int data=0,rcount=0;

void *reader(void * arg){
  int f;
  f = *((int *)arg);
  sem_wait(&mutex);
  rcount++;

  if(rcount == 1){
    sem_wait(&writeblock);
  }
  sem_post(&mutex);
  printf("Reader Is Reading Data %d ",data);
  sleep(2);
  sem_wait(&mutex);
  rcount--;
  if(rcount == 0 ){
    sem_post(&writeblock);
  }
  sem_post(&mutex);
  return NULL;
}
void *writer (void *arg){
  int f ;
  f = *((int *)arg);
  sem_wait(&writeblock);
  data++;
  printf("Writer Is Writing Data ");
  sleep(2);
  sem_post(&writeblock);
  return NULL;
}

int main(){
  int i;
  pthread_t rtid[3],wtid[3];
  sem_init(&mutex,0,1);
  sem_init(&writeblock,0,1);
  while(1){
    for(int i=0;i<3;i++){
      pthread_create(&wtid[i],NULL,writer,&i);
      pthread_create(&rtid[i],NULL,reader,&i);
    }
    for(int i=0;i<3;i++){
      pthread_join(wtid[i],NULL);
      pthread_join(rtid[i],NULL);
    }
  }
}



// Dinning Philosopher
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct {
  pthread_mutex_t mutex;
  pthread_cond_t cond[5];
  int state[5];
} Monitor;
Monitor Philosophers;
void test(int i) {
  if (Philosophers.state[(i + 4) % 5] != 2 && Philosophers.state[i] == 1 &&
      Philosophers.state[(i + 1) % 5] != 2) {
    Philosophers.state[i] = 2;
    pthread_cond_signal(&Philosophers.cond[i]);
  }
}
void pickup(int i) {
  pthread_mutex_lock(&Philosophers.mutex);
  Philosophers.state[i] = 1;
  test(i);
  if (Philosophers.state[i] != 2) {
    pthread_cond_wait(&Philosophers.cond[i], &Philosophers.mutex);
  }
  pthread_mutex_unlock(&Philosophers.mutex);
}
void putdown(int i) {
  pthread_mutex_lock(&Philosophers.mutex);
  Philosophers.state[i] = 0;
  test((i + 4) % 5);
  test((i + 1) % 5);
  pthread_mutex_unlock(&Philosophers.mutex);
}

void *eat(void *arg){
  int i =*((int *)arg);
  int count = 2;
  while(count >0){
    pickup(i);
    printf("Philosoher %d Is Eating ",i);
    sleep(2);
    putdown(i);
    printf("Philosoher %d Is Done Eating ",i);
    sleep(1);
    count--;
  }return NULL;
}
int main(){
  pthread_t philosophers[5];
  pthread_mutex_init(&Philosophers.mutex,NULL);
  for(int i=0;i<5;i++){
    pthread_cond_init(&Philosophers.cond[i],NULL);
  }
  for(int i=0;i<5;i++){
    pthread_create(philosophers[i],NULL,eat,(void *)i);
  }
  for(int i=0;i<5;i++){
    pthread_join(philosophers[i],NULL);
  }
  pthread_mutex_destroy(&Philosophers.mutex);
  for(int i=0;i<5;i++){
    pthread_cond_destroy(&Philosophers.cond[i]);
  }
  return 0;
}

// Seheduling Algorithms
// 1.FCFS
#include <stdio.h>

struct ps {
  char pname[10];
  int at, bt, ct, tat, wt, flag;
};

int n;

void sort_by_arrival(struct ps p[]) {
  for (int i = 0; i < n - 1; i++) {
    for (int j = 0; j < n - i - 1; j++) {
      if (p[j].at > p[j + 1].at) {
        struct ps temp = p[j];
        p[j] = p[j + 1];
        p[j + 1] = temp;
      }
    }
  }
}

void accept(int n, struct ps p[]) {
  for (int i = 0; i < n; i++) {
    sprintf(p[i].pname, "P%d", i + 1);
    printf("Enter Arrival Time for Process %d: ", i + 1);
    scanf("%d", &p[i].at);
    printf("Enter Burst Time for Process %d: ", i + 1);
    scanf("%d", &p[i].bt);
    p[i].flag = 0;
  }
}

int main() {
  printf("Enter Number Of Processes: ");
  scanf("%d", &n);

  struct ps p[n];
  accept(n, p);

  float total_tat = 0, total_wt = 0;
  int time = 0;

  sort_by_arrival(p);

  int alldone = 0;
  int i = 0;

  while (alldone < n) {
    if (p[i].flag == 0 && time >= p[i].at) {
      p[i].ct = time + p[i].bt;
      time += p[i].bt;
      p[i].flag = 1;
      p[i].tat = p[i].ct - p[i].at;
      p[i].wt = p[i].tat - p[i].bt;
      total_tat += p[i].tat;
      total_wt += p[i].wt;
      alldone++;
    } else {
      time++;
    }

    // Move to next process
    if (i == n - 1) {
      i = 0;
    } else {
      i++;
    }
  }

  float avg_tat = total_tat / n;
  float avg_wt = total_wt / n;

  printf("Avg TurnAround Time: %.2f\n", avg_tat);
  printf("Avg Waiting Time: %.2f\n", avg_wt);

  printf("\nPS \t AT \t BT \t CT \t TAT \t WT \n");
  for (int i = 0; i < n; i++) {
    printf("%s \t %d \t %d \t %d \t %d \t %d\n", p[i].pname, p[i].at, p[i].bt,
           p[i].ct, p[i].tat, p[i].wt);
  }

  return 0;
}

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

struct ps {
  char pname[10];
  int at, bt, ct, tat, wt, flag;
};

void accept(int n, struct ps p[]) {
  for (int i = 0; i < n; i++) {
    sprintf(p[i].pname, "P%d", i + 1);
    printf("Enter Arrival Time for Process %d: ", i + 1);
    scanf("%d", &p[i].at);
    printf("Enter Burst Time for Process %d: ", i + 1);
    scanf("%d", &p[i].bt);
    p[i].flag = 0;
  }
}

int main() {
  int n;
  float total_tat = 0, total_wt = 0;
  int time = 0, done = 0;

  printf("Enter Number of Processes: ");
  scanf("%d", &n);
  struct ps p[n];

  accept(n, p);

  while (done < n) {
    int smallest_burst_index = -1;
    int smallest_burst = INT_MAX;

    for (int i = 0; i < n; i++) {
      if (p[i].at <= time && p[i].flag == 0 && p[i].bt < smallest_burst) {
        smallest_burst = p[i].bt;
        smallest_burst_index = i;
      }
    }

    if (smallest_burst_index != -1) {
      time += p[smallest_burst_index].bt;
      p[smallest_burst_index].ct = time;
      p[smallest_burst_index].tat =
          p[smallest_burst_index].ct - p[smallest_burst_index].at;
      p[smallest_burst_index].wt =
          p[smallest_burst_index].tat - p[smallest_burst_index].bt;
      p[smallest_burst_index].flag = 1;
      total_tat += p[smallest_burst_index].tat;
      total_wt += p[smallest_burst_index].wt;
      done++;
    } else {
      time++;
    }
  }

  float avg_tat = total_tat / n;
  float avg_wt = total_wt / n;

  printf("Average Turnaround Time: %.2f\n", avg_tat);
  printf("Average Waiting Time: %.2f\n", avg_wt);

  printf("\nPS \t AT \t BT \t CT \t TAT \t WT\n");
  for (int i = 0; i < n; i++) {
    printf("%s \t %d \t %d \t %d \t %d \t %d\n", p[i].pname, p[i].at, p[i].bt,
           p[i].ct, p[i].tat, p[i].wt);
  }
  return 0;
}


*/

// SJF Preemptive
/*
#include <stdio.h>

struct ps {
char pname[10];
  int at, bt, ct, tat, wt, flag;
};
void sort_by_arrival(struct ps p[],int n) {
  for (int i = 0; i < n - 1; i++) {
    for (int j = 0; j < n - i - 1; j++) {
      if (p[j].at > p[j + 1].at) {
        struct ps temp = p[j];
        p[j] = p[j + 1];
        p[j + 1] = temp;
      }
    }
  }
}
int findMinIndex(int arr[], int n) {
    if (n <= 0) {
        return -1; // Return -1 for an empty array
    }

    int minIndex = 0; // Assume the first element is the smallest
    for (int i = 1; i < n; i++) {
        if (arr[i] < arr[minIndex]) {
            minIndex = i;
        }
    }
    return minIndex;
}
void accept(int n, struct ps p[],int rb[]) {
  for (int i = 0; i < n; i++) {
    sprintf(p[i].pname, "P%d", i + 1);
    printf("Enter Arrival Time for Process %d: ", i + 1);
    scanf("%d", &p[i].at);
    printf("Enter Burst Time for Process %d: ", i + 1);
    scanf("%d", &p[i].bt);
    p[i].flag = 0;
    rb[i]=p[i].bt;
  }
}
int main() {
  int n;
  int time=0,done=0;
  float total_tat=0,total_wt=0;
  float avg_tat,avg_wt;
  printf("Enter Number of Processes: ");
  scanf("%d", &n);

  struct ps p[n];
  int remainingBurst[n];
  accept(n, p,remainingBurst);
  sort_by_arrival(p, n);
  if(p[0].at>time){
    time = p[0].at;
  }
  int cp = 0; //currentprocess
  for(int i=0;i<n;i++){
    if(p[cp].at<=time){
      int minIndex = findMinIndex(remainingBurst, n);
      if(remainingBurst[i]>remainingBurst[minIndex]){
        cp=minIndex;
        continue;
      }else{
        time += 1;
        remainingBurst[i]-=1;
        if(remainingBurst[i]==0){
          done++;
          p[i].ct=time;
          p[i].tat=p[i].ct-p[i].at;
          p[i].wt=p[i].tat;
          total_tat+=p[i].tat;
          total_wt+=p[i].wt;
        }
      }
    }

  }
  avg_tat = total_tat/n;
  avg_wt = total_wt/n;

  printf("\n Avg TurnAround : %f ",avg_tat);
  printf("\n Avg Waiting : %f ",avg_wt);
  return 0;
}
*/
// Round RObin

/*
#include <stdio.h>
struct ps {
  char p_name[20];
  int at, bt, ct, tat, wt, flag, rt;
};
void accept(int n, struct ps p[]) {
  for (int i = 0; i < n; i++) {
    sprintf(p[i].p_name, "P%d", i + 1);
    printf("Enter Arrival Time for Process %d: ", i + 1);
    scanf("%d", &p[i].at);
    printf("Enter Burst Time for Process %d: ", i + 1);
    scanf("%d", &p[i].bt);
    p[i].flag = 0;
    p[i].rt = p[i].bt;
  }
}
// struct queue{
//   int front,rear;
// struct ps a[100];
// };

int main() {
  int n;
  int time = 0, done = 0;
  float total_tat = 0, total_wt = 0;
  float avg_tat, avg_wt;
  int TQ = 2;
  printf("Enter Number of Processes: ");
  scanf("%d", &n);

  struct ps p[n];
  accept(n, p);
  int i = 0;
  while (done < n) {
    printf("0");
    if ((p[i].flag == 0) || (p[i].rt > 0)) {
      printf("8");
      if (p[i].rt >= TQ) {
        printf(".%d", i);
        p[i].rt -= TQ;
        time += TQ;
        if (p[i].rt == 0) {
          done++;
          p[i].ct = time;
          p[i].tat = p[i].ct - p[i].at;
          p[i].wt = p[i].tat - p[i].bt;
          p[i].flag = 1;
          total_tat += p[i].tat;
          total_wt += p[i].wt;
          printf("%d - ", i);
          i++;
          continue;
        } else {
          i++;
          continue;
        }
      } else if (p[i].rt < TQ) {
        printf("*");
        time += p[i].rt;
        p[i].rt = 0;
        done++;
        p[i].ct = time;
        p[i].tat = p[i].ct - p[i].at;
        p[i].wt = p[i].tat - p[i].bt;
        p[i].flag = 1;
        total_tat += p[i].tat;
        total_wt += p[i].wt;
        printf("%d - ", i);
        i++;
        continue;
      }
    } else {
      i++;
      continue;
    }
  }

  avg_tat = total_tat / n;
  avg_wt = total_wt / n;

  printf("\n Avg TurnAround : %f ", avg_tat);
  printf("\n Avg Waiting : %f ", avg_wt);
  printf("\nPS \t AT \t BT \t CT \t TAT \t WT\n");
  for (int i = 0; i < n; i++) {
    printf("%s \t %d \t %d \t %d \t %d \t %d\n", p[i].p_name, p[i].at, p[i].bt,
           p[i].ct, p[i].tat, p[i].wt);
  }
  return 0;
}

*/
// Bankers Algorithm
/*
#include <stdio.h>

int m, n, i, j, need[10][10];
int temp, z, y, p;
int max[10][10] = {0};
int allocation[10][10] = {0};
int available[3] = {3, 3, 2};

void calculateNeed() {
  for (i = 0; i < m; i++) {
    for (j = 0; j < n; j++) {
      need[i][j] = max[i][j] - allocation[i][j;]
    }
  }
}
int bankers() {
  int finish[10] = {0};
  p = 1;
  y = 0;
  while (p != 0) {
    p = 0;
    for (i = 0; i < m; i++) {
      z = 0;
      for (int j = 0; j < n; j++) {
        if (need[i][j] <= available[j]) {
          z++;
        }
      }

      if (z == n && finish[i] == 0) {
        printf("-> P%d ", i);
        y++;
        finish[i] = 1; // setting already Executed

        for (int k = 0; k < n; k++) {
          available[k] += allocation[i][k];
        }
        printf("\n");
        p = 1;
      }
    }
  }
  if (y != m) {
    printf("System Is In UnsafeState");
  } else {
    printf("System Is In Safe State ");
  }
  return y == m;
}

int req[10][10] = {{10, 4, 12}, {2, 0, 2}, {0, 0, 0}, {1, 0, 0}, {0, 0, 2}};
int work[3] = {0, 0, 0};
int deadlockps;
void reqRes(int process, int request[]) {
  for (j = 0; j < n; j++) {
    if (request[j] > need[process][j]) {
      printf("Process Has Exceeded Its Maximum Claim ");
      return;
    }
    if (request[j] > available[j]) {
      printf("Requested Resouces Are Not Available");
      return;
    }
  }

  for (j = 0; j < n; j++) {
    available[j] -= request[j];
    allocation[process][j] += request[j];
    need[process][j] -= request[j];
  }

  if (bankers()) {
    printf("Resources Allocated Successfully .System Is In Safe Sate ");
  } else {
    printf("System WOuld BE In Unsafe State,Rolling Back");
    for (j = 0; j < n; j++) {
      available[j] += request[j];
      allocation[process][j] -= request[j];
      need[process][j] += request[j];
    }
  }
}
int main() {
  printf("Enter Number Of Processes \n");
  scanf("%d", &m);
  printf("Enter Number Of Processes \n");
  scanf("%d", &n);
  for (int i = 0; i < m; i++) {
    printf("Enter Max Matrix P %d\n", i + 1);
    for (int j = 0; j < n; j++) {
      scanf("%d", &max[i][j]);
    }
  }
  for (int i = 0; i < m; i++) {
    printf("Enter Allocation Matrix P %d\n", i + 1);
    for (int j = 0; j < n; j++) {
      scanf("%d", &allocation[i][j]);
    }
  }
  calculateNeed();

  printf("\n Allocation Matrix : \n");
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      printf("%d  ", allocation[i][j]);
      printf("\n");
    }
  }
  printf("\n MAX Matrix : \n");
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      printf("%d  ", max[i][j]);
    }
    printf("\n");
  }
  printf("\n Need Matrix : \n");
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      printf("%d  ", need[i][j]);
    }
    printf("\n");
  }
  bankers();
  int process;
  int req[3];
  while (1) {
    printf("Enter The Process Number Making The Request \n ");
    scanf("%d", &process);
    printf("ENter The Additional Request Values ");
    for (i = 0; i < n; i++) {
      scanf("%d", &req[i]);
    }
    reqRes(process, req);
    char cont;
    printf("Do Ypu want to continuue : (y/n) ");
    scanf("%d", &cont);
    if (cont == 'y') {
      break;
    }
  }
}

*/

// Partitioning

/*
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#define MEMORY_SIZE 1024
#define FIXED_PARTITION_SIZE 256

typedef struct MemoryBlock {
  int size;
  bool allocated;
} MemoryBlock;

void fixedPartioning() {
  MemoryBlock memory[MEMORY_SIZE / FIXED_PARTITION_SIZE];

  for (int i = 0; i < MEMORY_SIZE / FIXED_PARTITION_SIZE; i++) {
    memory[i].size = FIXED_PARTITION_SIZE;
    memory[i].allocated = false;
  }

  // Allocate Memory
  int num_processes, process_size;
  printf("Enter the number of processes: ");
  scanf("%d", &num_processes);
  printf("Enter The Size Of Each Process ");
  for (int i = 0; i < num_processes; i++) {
    scanf("%d", &process_size);
    bool allocated = false;
    for (int j = 0; j < MEMORY_SIZE / FIXED_PARTITION_SIZE; j++) {
      if (!memory[j].allocated && memory[j].size >= process_size) {
        memory[j].allocated = true;
        allocated = true;
        printf("Process %d Is Allocated To Meory Block %d", i + 1, j);
        break;
      }
    }

    if (!allocated) {
      printf("Insufficient Memory To Allocate for process %d", i + 1);
    }
  }
}

void dynamicPartioning() {
  int memory[MEMORY_SIZE];
  int num_processes, process_size;
  for (int i = 0; i < MEMORY_SIZE; ++i) {
    memory[i] = -1;
  }
  printf("Enter Number OF Processes : \n");
  scanf("%d", &num_processes);
  printf("Enter The Size of eac process : \n ");
  for (int i = 0; i < num_processes; ++i) {
    scanf("%d", &process_size);
    bool allocated = false;

    for (int j = 0; j < MEMORY_SIZE; ++j) {
      if (memory[j] == -1) {
        int k;
        for (k = j; k < j + process_size; ++k) {
          if (memory[k] != -1) {
            break; // Black Is To Small
          }
        }
        if (k == j + process_size) {
          for (int l = j; l < j + process_size; ++l) {
            memory[l] = i;
          }
          printf("Process %d Allocated Meory Started From %d", i + 1, j);
          allocated = true;
          break;
        }
      }
      if (!allocated) {
        printf("Insufficient memory to allocate process %d\n", i + 1);
      }
    }
  }
}
int main() {
  int choise;
  printf("1.FIXED SIZE PARTITIONING \n");
  printf("2.Dynamic SIZE PARTITIONING \n");
  scanf("%d", &choise);
  switch (choise) {
  case 1:
    fixedPartioning();
    break;
  case 2:
    dynamicPartioning();
    break;
  default:
    printf("Invalid Choise");
  }
  return 0;
}*/

// PAGING AND SEGMENTATION

/*
#include <stdio.h>
#include <stdlib.h>
#define PAGE_SIZE 4096
#define SEGMENT_SIZE 8192
#define NUM_PAGE 8
#define NUM_SEGMENTS 4

void translatePaging(int virtual_address) {
  int page_number = virtual_address / PAGE_SIZE;
  int offset = virtual_address % PAGE_SIZE;
  printf("Virtual Address : %d \n ", virtual_address);
  printf("Page Number : %d \n", page_number);
  printf("Offset : %d ", offset);
  printf("Physical Address : %d ", page_number * PAGE_SIZE + offset);
}
void translateSegmentation(int virtual_address) {
  int segment_number = virtual_address / SEGMENT_SIZE;
  int segment_offset = virtual_address % SEGMENT_SIZE;
  printf("VIRTUAL Address : %d \n ", virtual_address);
  printf("Segment Number  : %d \n ", segment_number);
  printf("Segment Offset : %d \n ", segment_offset);
  printf("Physical Address : %d \n ",
         segment_number * SEGMENT_SIZE + segment_offset);
}
int main() {
  translatePaging(3045);
  translateSegmentation(3045);
  return 0;
}

*/

// Page Replacement
/*
#include <stdbool.h>
#include <stdio.h>
#define MAX_FRAMES 10
#define MAX_PAGE 30

void FIFO(int pages[], int n, int capacity) {
  int frames[MAX_FRAMES], index = 0, faults = 0;
  for (int i = 0; i < capacity; i++)
    frames[i] = -1;

  for (int i = 0; i < n; i++) {
    bool found = false;
    for (int j = 0; j < capacity; j++) {
      if (frames[j] == pages[i]) {
        found = true;
        break;
      }
    }
    if (!found) {
      frames[index] = pages[i];
      index = (index + 1) % capacity;
      faults++;
    }
    printf("Frame :");
    for (int j = 0; j < capacity; j++) {
      printf("%d ", frames[j]);
    }
  }
  printf("TOtal Page Faults : %d", faults);
}

void LRU(int pages[], int n, int capacity) {
  int frames[MAX_FRAMES], index[MAX_FRAMES], faults = 0;
  int time = 0;
  for (int i = 0; i < capacity; i++) {
    frames[i] = -1;
    index[i] = 0;
  }

  for (int i = 0; i < n; i++) {
    bool found = false;
    int pos = 0;
    for (int j = 0; j < capacity; j++) {
      if (frames[j] == pages[i]) {
        found = true;
        index[j] = time++;
        break;
      }
      if (index[j] < index[pos])
        pos = j;
    }
    if (!found) {
      frames[pos] = pages[i];
      index[pos] = time++;
      faults++;
    }
    printf("Frames : ");
    for (int j = 0; j < capacity; j++) {
      printf("%d ", frames[j]);
    }
  }
  printf("Total Faults %d", faults);
}
int predict(int pages[], int frames[], int n, int index, int capacity) {
  int res = -1, farthest = index;
  for (int i = 0; i < capacity; i++) {
    int j;
    for (j = index; j < n; j++) {
      if (frames[i] == pages[j]) {
        if (j > farthest) {
          farthest = j;
          res = i;
        }
        break;
      }
    }
    if (j == n)
      return i;
  }
  return (res == -1) ? 0 : res;
}
void Optimal(int pages[], int n, int capacity) {
  int frame[MAX_FRAMES], faults = 0;
  for (int i = 0; i < capacity; i++)
    frame[i] = -1;

  for (int i = 0; i < n; i++) {
    bool found = false;
    for (int j = 0; j < capacity; j++) {
      if (frame[j] == pages[i]) {
        found = true;
        break;
      }
    }
    if (!found) {
      if (i < capacity) {
        frame[i] = pages[i];
      } else {
        int j = predict(pages, frame, n, i, capacity);
        frame[j] = pages[i];
      }
      faults++;
    }
    printf("Frame: ");
    for (int j = 0; j < capacity; j++)
      printf("%d ", frame[j]);
    printf("\n");
  }
  printf("Total page faults: %d\n", faults);
}


int main() {
    int pages[MAX_PAGES], n, capacity, choice;

    printf("Enter the number of pages: ");
    scanf("%d", &n);
    printf("Enter the pages: ");
    for (int i = 0; i < n; i++)
        scanf("%d", &pages[i]);
    printf("Enter the number of frames: ");
    scanf("%d", &capacity);

    printf("Choose the page replacement policy:\n");
    printf("1. FIFO\n");
    printf("2. LRU\n");
    printf("3. Optimal\n");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            FIFO(pages, n, capacity);
            break;
        case 2:
            LRU(pages, n, capacity);
            break;
        case 3:
            Optimal(pages, n, capacity);
            break;
        default:
            printf("Invalid choice.\n");
            break;
    }

    return 0;
}


*/

// Disk Scheduling
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// Function to simulate FCFS disk scheduling
void fcfs(int requests[], int num_requests, int head) {
  printf("FCFS Disk Scheduling:\n");
  printf("Head movement order: %d", head);
  for (int i = 0; i < num_requests; ++i) {
    printf(" -> %d", requests[i]);
  }
  printf("\n");
}

// Function to simulate SCAN disk scheduling
void scan(int requests[], int num_requests, int head, int max_cylinder) {
  printf("SCAN Disk Scheduling:\n");
  printf("Head movement order: ");

  bool direction = true; // true for moving right, false for moving left
  int current = head;

  while (true) {
    printf("%d ", current);
    if (direction) {
      bool found = false;
      for (int i = 0; i < num_requests; ++i) {
        if (requests[i] == current) {
          printf("-> %d ", requests[i]);
          found = true;
          break;
        }
      }
      if (found) {
        direction = false;
      }
      if (current == max_cylinder) {
        direction = false;
      }
      current++;
    } else {
      bool found = false;
      for (int i = 0; i < num_requests; ++i) {
        if (requests[i] == current) {
          printf("-> %d ", requests[i]);
          found = true;
          break;
        }
      }
      if (found) {
        direction = true;
      }
      if (current == 0) {
        direction = true;
      }
      current--;
    }
    if (current > max_cylinder) {
      current = max_cylinder;
    }
    if (current < 0) {
      current = 0;
    }

    if (current == head) {
      break;
    }
  }

  printf("\n");
}

// Function to simulate C-SCAN disk scheduling
void cscan(int requests[], int num_requests, int head, int max_cylinder) {
  printf("C-SCAN Disk Scheduling:\n");
  printf("Head movement order: ");

  int current = head;
  printf("%d ", current);

  bool direction = true; // true for moving right, false for moving left

  while (true) {
    printf("-> ");
    if (direction) {
      bool found = false;
      for (int i = 0; i < num_requests; ++i) {
        if (requests[i] == current) {
          printf("%d ", requests[i]);
          found = true;
          break;
        }
      }
      if (found) {
        current++;
      }
      if (current == max_cylinder) {
        printf("%d ", max_cylinder);
        current = 0;
      }
      if (current > max_cylinder) {
        current = 0;
      }
    } else {
      bool found = false;
      for (int i = num_requests - 1; i >= 0; --i) {
        if (requests[i] == current) {
          printf("%d ", requests[i]);
          found = true;
          break;
        }
      }
      if (found) {
        current--;
      }
      if (current == 0) {
        printf("0 ");
        current = max_cylinder;
      }
      if (current < 0) {
        current = max_cylinder;
      }
    }

    if (current == head) {
      break;
    }
  }

  printf("\n");
}

// Function to simulate SSTF disk scheduling
void sstf(int requests[], int num_requests, int head) {
  printf("SSTF Disk Scheduling:\n");
  printf("Head movement order: ");

  int current = head;
  bool processed[num_requests];
  for (int i = 0; i < num_requests; ++i) {
    processed[i] = false;
  }

  for (int i = 0; i < num_requests; ++i) {
    int min_distance = INT_MAX;
    int next_index = -1;
    for (int j = 0; j < num_requests; ++j) {
      if (!processed[j]) {
        int distance = abs(current - requests[j]);
        if (distance < min_distance) {
          min_distance = distance;
          next_index = j;
        }
      }
    }
    printf("%d ", current);
    current = requests[next_index];
    processed[next_index] = true;
  }
  printf("\n");
}

int main() {
  int requests[] = {98, 183, 37, 122, 14, 124, 65, 67}; // Sample disk requests
  int num_requests = sizeof(requests) / sizeof(requests[0]);
  int head = 53;          // Sample initial head position
  int max_cylinder = 199; // Sample maximum cylinder

  fcfs(requests, num_requests, head);
  scan(requests, num_requests, head, max_cylinder);
  cscan(requests, num_requests, head, max_cylinder);
  sstf(requests, num_requests, head);

  return 0;
}


// FCFS Disk Scheduling:
// Head movement order: 53 -> 98 -> 183 -> 37 -> 122 -> 14 -> 124 -> 65 -> 67 
// SCAN Disk Scheduling:
// Head movement order: 53 -> 65 -> 67 -> 98 -> 122 -> 124 -> 183 -> 199 -> 0 -> 14 
// C-SCAN Disk Scheduling:
// Head movement order: 53 -> 65 -> 67 -> 98 -> 122 -> 124 -> 183 -> 199 -> 0 -> 14 
// SSTF Disk Scheduling:
// Head movement order: 53 -> 65 -> 67 -> 37 -> 14 -> 98 -> 122 -> 124 -> 183 -> 199 
