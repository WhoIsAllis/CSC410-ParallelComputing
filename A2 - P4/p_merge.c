#include "p_merge.h"

typedef struct {
    int* arr;
    int left;
    int right;
    int threadCount;
} RecursiveData;

typedef struct {
    int left;
    int right;
} Segment;

typedef struct {
    int* arr;
    int left;
    int middle;
    int right;
    int isMerge;
} PoolTask;

typedef struct {
    pthread_t threads[NUM_THREADS];
    PoolTask tasks[NUM_THREADS];
    int threadCount;
    int front;
    int rear;
    int taskCount;
    int workingCount;
    int stop;
    pthread_mutex_t mutex;
    pthread_cond_t hasTasks;
    pthread_cond_t allDone;
} ThreadPool;

static void* recursiveWorker(void* arg);

void merge(int* arr, int l, int m, int r) 
{
    int n1 = m - l + 1;
    int n2 = r - m;

    int* L = (int*)malloc(n1 * sizeof(int));
    int* R = (int*)malloc(n2 * sizeof(int));

    for (int i = 0; i < n1; i++) L[i] = arr[l + i];
    for (int i = 0; i < n2; i++) R[i] = arr[m + 1 + i];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j])
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];

    free(L);
    free(R);
}

void merge_sort_seq(int* arr, int l, int r) {
    if (l < r) {
        int m = (l + r) / 2;
        merge_sort_seq(arr, l, m);
        merge_sort_seq(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

static void createSegments(Segment segments[], int l, int r, int threadCount)
{
    int size = r - l + 1;
    int segmentSize = size / threadCount;
    int current = l;

    for (int i = 0; i < threadCount; i++)
    {
        int length = segmentSize;
        if (i == threadCount - 1)
        {
            length += size % threadCount;
        }

        segments[i].left = current;
        segments[i].right = current + length - 1;
        current += length;
    }
}

void* parallel_merge_sort(void* arg)
{
    thread_data_t* data = (thread_data_t*)arg;
    merge_sort_seq(data->arr, data->left, data->right);
    return NULL;
}

void mergeSortSegment(int* arr, int l, int r, int threadCount)
{
    pthread_t threads[NUM_THREADS];
    thread_data_t data[NUM_THREADS];
    Segment segments[NUM_THREADS];
    int started[NUM_THREADS] = {0};

    createSegments(segments, l, r, threadCount);

    for (int i = 0; i < threadCount; i++)
    {
        data[i].arr = arr;
        data[i].left = segments[i].left;
        data[i].right = segments[i].right;
        data[i].level = 0;

        if (pthread_create(&threads[i], NULL, parallel_merge_sort,
                           &data[i]) == 0)
        {
            started[i] = 1;
        }

        else
        {
            merge_sort_seq(arr, data[i].left, data[i].right);
        }
    }

    for (int i = 0; i < threadCount; i++)
    {
        if (started[i])
        {
            pthread_join(threads[i], NULL);
        }
    }

    int mergedRight = segments[0].right;
    for (int i = 1; i < threadCount; i++)
    {
        merge(arr, l, mergedRight, segments[i].right);
        mergedRight = segments[i].right;
    }
}

void merge_sort_p(int* arr, int l, int r)
{
    mergeSortSegment(arr, l, r, NUM_THREADS);
}

static void recursiveMergeRange(int* arr, int l, int r, int threadCount)
{
    if (l >= r)
    {
        return;
    }

    if (threadCount == 1)
    {
        merge_sort_seq(arr, l, r);
        return;
    }

    int middle = (l + r) / 2;
    int leftThreads = threadCount / 2;
    int rightThreads = threadCount - leftThreads;
    pthread_t thread;
    RecursiveData leftData = {arr, l, middle, leftThreads};

    if (pthread_create(&thread, NULL, recursiveWorker, &leftData) == 0)
    {
        recursiveMergeRange(arr, middle + 1, r, rightThreads);
        pthread_join(thread, NULL);
    }

    else
    {
        merge_sort_seq(arr, l, middle);
        recursiveMergeRange(arr, middle + 1, r, rightThreads);
    }

    merge(arr, l, middle, r);
}

static void* recursiveWorker(void* arg)
{
    RecursiveData* data = (RecursiveData*)arg;
    recursiveMergeRange(data->arr, data->left, data->right, data->threadCount);
    return NULL;
}

void mergeSortRecursive(int* arr, int l, int r, int threadCount)
{
    pthread_t rootThread;
    RecursiveData rootData = {arr, l, r, threadCount};

    if (pthread_create(&rootThread, NULL, recursiveWorker, &rootData) == 0)
    {
        pthread_join(rootThread, NULL);
    }

    else
    {
        merge_sort_seq(arr, l, r);
    }
}

static void* poolWorker(void* arg)
{
    ThreadPool* pool = (ThreadPool*)arg;

    while (1)
    {
        pthread_mutex_lock(&pool->mutex);

        while (pool->taskCount == 0 && !pool->stop)
        {
            pthread_cond_wait(&pool->hasTasks, &pool->mutex);
        }

        if (pool->stop && pool->taskCount == 0)
        {
            pthread_mutex_unlock(&pool->mutex);
            break;
        }

        PoolTask task = pool->tasks[pool->front];
        pool->front = (pool->front + 1) % NUM_THREADS;
        pool->taskCount--;
        pool->workingCount++;
        pthread_mutex_unlock(&pool->mutex);

        if (task.isMerge)
        {
            merge(task.arr, task.left, task.middle, task.right);
        }

        else
        {
            merge_sort_seq(task.arr, task.left, task.right);
        }

        pthread_mutex_lock(&pool->mutex);
        pool->workingCount--;

        if (pool->taskCount == 0 && pool->workingCount == 0)
        {
            pthread_cond_signal(&pool->allDone);
        }

        pthread_mutex_unlock(&pool->mutex);
    }

    return NULL;
}

static void startPool(ThreadPool* pool, int threadCount)
{
    pool->threadCount = threadCount;
    pool->front = 0;
    pool->rear = 0;
    pool->taskCount = 0;
    pool->workingCount = 0;
    pool->stop = 0;

    pthread_mutex_init(&pool->mutex, NULL);
    pthread_cond_init(&pool->hasTasks, NULL);
    pthread_cond_init(&pool->allDone, NULL);

    for (int i = 0; i < threadCount; i++)
    {
        if (pthread_create(&pool->threads[i], NULL, poolWorker, pool) != 0)
        {
            printf("Thread creation failed.\n");
            exit(1);
        }
    }
}

static void addTask(ThreadPool* pool, PoolTask task)
{
    pthread_mutex_lock(&pool->mutex);
    pool->tasks[pool->rear] = task;
    pool->rear = (pool->rear + 1) % NUM_THREADS;
    pool->taskCount++;
    pthread_cond_signal(&pool->hasTasks);
    pthread_mutex_unlock(&pool->mutex);
}

static void waitForPool(ThreadPool* pool)
{
    pthread_mutex_lock(&pool->mutex);
    while (pool->taskCount > 0 || pool->workingCount > 0)
    {
        pthread_cond_wait(&pool->allDone, &pool->mutex);
    }
    
    pthread_mutex_unlock(&pool->mutex);
}

static void stopPool(ThreadPool* pool)
{
    pthread_mutex_lock(&pool->mutex);
    pool->stop = 1;
    pthread_cond_broadcast(&pool->hasTasks);
    pthread_mutex_unlock(&pool->mutex);

    for (int i = 0; i < pool->threadCount; i++)
    {
        pthread_join(pool->threads[i], NULL);
    }

    pthread_mutex_destroy(&pool->mutex);
    pthread_cond_destroy(&pool->hasTasks);
    pthread_cond_destroy(&pool->allDone);
}

void mergeSortPool(int* arr, int l, int r, int threadCount)
{
    ThreadPool pool;
    Segment segments[NUM_THREADS];
    Segment nextSegments[NUM_THREADS];
    int segmentCount = threadCount;

    createSegments(segments, l, r, threadCount);
    startPool(&pool, threadCount);

    for (int i = 0; i < segmentCount; i++)
    {
        PoolTask task = {arr, segments[i].left, 0, segments[i].right, 0};
        addTask(&pool, task);
    }
    waitForPool(&pool);

    while (segmentCount > 1)
    {
        int nextCount = 0;

        for (int i = 0; i < segmentCount; i += 2)
        {
            if (i + 1 < segmentCount)
            {
                PoolTask task = {
                    arr,
                    segments[i].left,
                    segments[i].right,
                    segments[i + 1].right,
                    1
                };
                addTask(&pool, task);

                nextSegments[nextCount].left = segments[i].left;
                nextSegments[nextCount].right = segments[i + 1].right;
            }
            else
            {
                nextSegments[nextCount] = segments[i];
            }
            nextCount++;
        }

        waitForPool(&pool);

        for (int i = 0; i < nextCount; i++)
        {
            segments[i] = nextSegments[i];
        }
        segmentCount = nextCount;
    }

    stopPool(&pool);
}


void printArray(int* arr, int n) 
{
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

double time_diff_ms(struct timespec start, struct timespec end) {
    return (end.tv_sec - start.tv_sec) * 1000.0 +
           (end.tv_nsec - start.tv_nsec) / 1000000.0;
}
