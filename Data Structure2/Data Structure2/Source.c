#include <stdio.h>
#include <time.h>
#include "queuesADT.h"
#include "stdbool.h"

typedef struct
{
	int numCust;
	int totSvcTime;
	int totWaitTime;
	int maxQueueSize;
} simStats;

typedef struct
{
	int custNum;
	int arriveTime;
	int startTime;
	int svcTime;
} custStatus;

void newCustomer(QUEUE* queue, int clock, int custNum);
boolean serverFree(QUEUE* queue, int clock, custStatus* status, boolean* moreCusts);
void serviceComplete(QUEUE* queue, int clock, custStatus* status, simStats* runStats, boolean* moreCusts);
void printStats(simStats* run);

int main()
{
	int clock = 0;
	int finishTime = 480;
	int custNum = 0;
	boolean moreCusts = false;
	custStatus* status = (custStatus*)malloc(sizeof(custStatus));
	simStats* run = (simStats*)malloc(sizeof(simStats));
	QUEUE* taffyQueue = createQueue();

	srand(time(NULL));

	run -> maxQueueSize = 0;
	run -> numCust = 0;
	run -> totSvcTime = 0;
	run -> totWaitTime = 0;

 	while((clock <= finishTime) || moreCusts)
	{
		newCustomer(taffyQueue, clock, custNum);
		serverFree(taffyQueue, clock, status, &moreCusts);
		serviceComplete(taffyQueue, clock, status, run, &moreCusts);
		if(!emptyQueue(taffyQueue))
		{
			moreCusts = true;
		}
		clock++;
	}

 	printStats(run);
	printf("Close time: %d:", (9 + clock/60));
	if(clock % 60 >= 10)
		printf("%d\n", clock % 60);
	else
		printf("0%d\n", clock % 60);
	system("pause");
}

void newCustomer(QUEUE* queue, int clock, int custNum)
{
	custStatus* custData;
	int newCust;

	custData = (custStatus*)malloc(sizeof(custStatus));
	newCust = rand() % 4;

	if((newCust == 1) && (clock < 480))
	{
		custNum++;
		custData -> custNum = custNum;
		custData -> arriveTime = clock;
		enqueue(queue, custData);
	}
}

boolean serverFree(QUEUE* queue, int clock, custStatus* status, boolean* moreCusts)
{
	custStatus* custData;
	int svcTime;
	svcTime = (rand() % 10) + 1;
	if(clock > (status -> startTime + status -> svcTime - 1))
	{
		if(!emptyQueue(queue))
		{
			dequeue(queue, (void**)&custData);
			status -> custNum = custData -> custNum;
			status -> arriveTime = custData -> arriveTime;
			status -> startTime = clock;
			status -> svcTime = svcTime;
			*moreCusts = true;
		}
	}
	return true;
}

void serviceComplete(QUEUE* queue, int clock, custStatus* status, simStats* stats, boolean* moreCusts)
{
	int waitTime;
	int queueSize;

	if(clock == status -> startTime + status -> svcTime - 1)
	{
		waitTime = status -> startTime - status -> arriveTime;
		stats -> numCust++;
		stats -> totSvcTime = stats -> totSvcTime + status -> svcTime;
		stats -> totWaitTime = stats -> totWaitTime + waitTime;

		queueSize = queueCount(queue);

		if(stats -> maxQueueSize < queueSize)
		{
			stats -> maxQueueSize = queueSize;
		}
		printf("Customer number: %d\n", stats -> numCust);
		printf("Customer arrive time: ");
		if(9 + status -> arriveTime / 60 >= 10)
			printf("%d:", 9 + status -> arriveTime / 60);
		else
			printf("0%d:", 9 + status -> arriveTime / 60);

		if(status -> arriveTime % 60 >= 10)
			printf("%d \n", status -> arriveTime % 60);
		else
			printf("0%d \n", status -> arriveTime % 60);
		
		printf("Customer start time: ");
		if(9 + status -> startTime / 60 >= 10)
			printf("%d:", 9 + status -> startTime / 60);
		else
			printf("0%d:", 9 + status -> startTime / 60);

		if(status -> startTime % 60 >= 10)
			printf("%d \n", status -> startTime % 60);
		else
			printf("0%d \n", status -> startTime % 60);

		printf("Customer service time: %d minutes \n", status -> svcTime);
		printf("Customer wait time: %d minutes \n", waitTime);
		printf("Customer queue size: %d\n\n", queueSize);
		*moreCusts = false;
	}
}

void printStats(simStats* run)
{
	printf("Simulation Statistics: \n");
	printf("Total customers: %d\n", run -> numCust);
	printf("Total service time: ");
	if(run -> totSvcTime / 60 >= 10)
		printf("%d hours", run -> totSvcTime / 60);
	else
		printf("0%d hours", run -> totSvcTime / 60);
	if(run -> totSvcTime % 60 >= 10)
		printf(" %d minutes \n", run -> totSvcTime % 60);
	else
		printf(" 0%d minutes \n", run -> totSvcTime % 60);

	printf("Average sevice time: %.2f minutes\n", run -> totSvcTime / (double)run -> numCust);
	printf("Average wait time: %.2f minutes \n", run -> totWaitTime / (double)run -> numCust);
	printf("Maximum queue size: %d\n", run -> maxQueueSize);
}