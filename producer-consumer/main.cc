/******************************************************************
 * The Main program with the two functions. A simple
 * example of creating and using a thread is provided.
 ******************************************************************/

#include "helper.h"
#include <vector>
#include <string>
#include <deque>

int init_semaphores(int queue_size);
void *producer (void* p);
void *consumer (void* p);
int rand_between(int min, int max);
int generate_job_id(deque <pair<int, int>> buffer);

struct producer_params_t {
  int sem_id;
  int jobs_per_prod;
  deque <pair<int, int>>* buffer;
  int* id;
};

struct consumer_params_t {
  int sem_id;
  deque <pair<int, int>>* buffer;
  int* id;
};

int main (int argc, char **argv)
{
  // Variables
  deque <pair<int, int>> buffer;
  int queue_size = stoi(argv[1]);
  int jobs_per_prod = stoi(argv[2]);
  int num_producers = stoi(argv[3]);
  int num_consumers = stoi(argv[4]);

  // Initialise semaphores
  int sem_id = init_semaphores(queue_size);
  if (sem_id == -1){
    cout << "Error creating semaphore array." << endl;
    return sem_id;
  }

  vector <pthread_t> pthread_ids;
  int producer_id = 1;
  int consumer_id = 1;

  // Create producers
  producer_params_t prod_params = {sem_id, jobs_per_prod, &buffer, &producer_id};
  for (int i = 0; i < num_producers; i++){
    pthread_t id;
    pthread_create (&id, NULL, producer, (void *)&prod_params);
    pthread_ids.push_back(id);
  }
  // Create consumers
  consumer_params_t cons_params = {sem_id, &buffer, &consumer_id};
  for (int i = 0; i < num_consumers; i++){
    pthread_t id;
    pthread_create (&id, NULL, consumer, (void *)&cons_params);
    pthread_ids.push_back(id);
  }
  // Wait for threads to complete
  for (auto id: pthread_ids)
    pthread_join (id, NULL);

  sem_close(sem_id);
  return 0;
}


void *producer (void *p)
{
  // Initialise variables
  producer_params_t params = *(producer_params_t *) p;
  int sem_id = params.sem_id;
  int jobs_per_prod = params.jobs_per_prod;
  deque <pair<int, int>> &buffer = *(params.buffer);
  int &id = *(params.id);
  int job_duration, job_id, thread_id, queue_size;

  // Get thread id
  sem_wait (sem_id, IDS);
  thread_id = id++;
  sem_signal (sem_id, IDS);

  // Run thread
  while(true){

    // Create job
    if (jobs_per_prod > 0){
      job_duration = rand_between(1, 10);
      jobs_per_prod--;
    } else {
      // STDOUT CRITICAL REGION START
      sem_wait (sem_id, STDOUT);
      cout << "[PRODUCER " << thread_id << "] ";
      cout << "No more jobs to produce." << endl;
      sem_signal (sem_id, STDOUT);
      // STDOUT CRITICAL REGION END
      pthread_exit(0);
    }

    int ret = sem_wait_timeout (sem_id, SPACE);
    if (ret == -1) {
      sem_wait (sem_id, STDOUT);
      cout << "[PRODUCER " << thread_id << "] "
           << "Waited too long for a space in the queue." << endl;
      sem_signal (sem_id, STDOUT);
      pthread_exit(0);
    }
    // JOB QUEUE CRITICAL REGION START
    sem_wait (sem_id, JOB_QUEUE_MUTEX);
    job_id = generate_job_id(buffer);
    buffer.push_back({job_id, job_duration});
    queue_size = buffer.size();
    sem_signal (sem_id, JOB_QUEUE_MUTEX);
    // JOB QUEUE CRITICAL REGION END
    sem_signal (sem_id, EMPTY);

    // STDOUT CRITICAL REGION START
    sem_wait (sem_id, STDOUT);
    cout << "[PRODUCER " << thread_id << "] "
         << "Adding JOB " << job_id
         << " with duration " << job_duration << ". "
         << "Queue is now size " << queue_size << endl;
    sem_signal (sem_id, STDOUT);
    // STDOUT CRITICAL REGION END
    sleep (rand_between(1, 5));
  }
}


void *consumer (void *p)
{
  // Declare variables
  consumer_params_t params = *(consumer_params_t *) p;
  int sem_id = params.sem_id;
  deque <pair<int, int>> &buffer = *(params.buffer);
  int &id = *(params.id);
  int thread_id, queue_size;

  // Get thread id
  sem_wait (sem_id, IDS);
  thread_id = id++;
  sem_signal (sem_id, IDS);

  // Run thread
  while(true){
    int ret = sem_wait_timeout (sem_id, EMPTY);
    if (ret == -1) {
      sem_wait (sem_id, STDOUT);
      cout << "[CONSUMER " << thread_id << "] "
           << "No more items to consume." << endl;
      sem_signal (sem_id, STDOUT);
      pthread_exit(0);
    }
    // JOB QUEUE CRITICAL REGION START
    sem_wait (sem_id, JOB_QUEUE_MUTEX);
    pair<int, int> job = buffer.front();
    buffer.pop_front();
    queue_size = buffer.size();
    sem_signal (sem_id, JOB_QUEUE_MUTEX);
    // JOB QUEUE CRITICAL REGION END
    sem_signal (sem_id, SPACE);

    // STDOUT CRITICAL REGION START
    sem_wait (sem_id, STDOUT);
    cout << "[CONSUMER " << thread_id << "] "
         << "Taking JOB " << job.first
         << " with duration " << job.second << ". "
         << "Queue is now size " << queue_size << endl;
    sem_signal (sem_id, STDOUT);
    // STDOUT CRITICAL REGION END

    sleep (job.second);
  }
}


int init_semaphores(int queue_size){
  int err, sem_id;
  sem_id = sem_create(SEM_KEY, 5);
  // Job queue semaphore - to enforce mutex for the job queue
  err = sem_init (sem_id, JOB_QUEUE_MUTEX, 1);
  if (err) return err;
  // Space semaphore - to ensure that buffer is not full
  err = sem_init (sem_id, SPACE, queue_size);
  if (err) return err;
  // Empty semaphore - to ensure that buffer is not empty
  err = sem_init (sem_id, EMPTY, 0);
  if (err) return err;
  // ID semaphore - to enforce mutex for the ID variable
  err = sem_init (sem_id, IDS, 1);
  if (err) return err;
  // STDOUT sempahore - to enforce mutex for stdout
  err = sem_init (sem_id, STDOUT, 1);
  if (err) return err;

  return sem_id;
}


int rand_between(int min, int max){
  return (rand() % (max - min + 1) + min);
}


int generate_job_id(deque <pair<int, int>> buffer){
  int id = 1;
  for (auto job: buffer){
    if (id == job.first){
      id++; continue;
    }
  }
  return id;
}
