/******************************************************************
 * The helper file that contains the following helper functions:
 * check_arg - Checks if command line input is a number and returns it
 * sem_create - Create number of sempahores required in a semaphore array
 * sem_init - Initialise particular semaphore in semaphore array
 * sem_wait - Waits on a semaphore (akin to down ()) in the semaphore array
 * sem_signal - Signals a semaphore (akin to up ()) in the semaphore array
 * sem_close - Destroy the semaphore array
 ******************************************************************/

# include "helper.h"

const struct timespec timeout_ts = { TIMEOUT, 0 }; // Timespec for timeout

int check_arg (char *buffer)
{
  int i, num = 0, temp = 0;
  if (strlen (buffer) == 0)
    return -1;
  for (i=0; i < (int) strlen (buffer); i++)
  {
    temp = 0 + buffer[i];
    if (temp > 57 || temp < 48)
      return -1;
    num += pow (10, strlen (buffer)-i-1) * (buffer[i] - 48);
  }
  return num;
}


/* Creates `num` semaphores using long integer `key`
   -  0666 indicates permissions
   - `IPC_CREAT` creates the semaphore(s) if it doesn't already exist, or returns
      the `id` if it already does
   - `IPC_EXCL` (when used together with `IPC_CREAT`) returns -1 if the
      semaphore(s) already exist */
int sem_create (key_t key, int num)
{
  int id;
  if ((id = semget (key, num,  0666 | IPC_CREAT | IPC_EXCL)) < 0)
    return -1;
  return id;
}


// Initialises `semval` for the the `num`-th semaphore to `value`
int sem_init (int id, int num, int value)
{
  union semun semctl_arg;
  semctl_arg.val = value;
  if (semctl (id, num, SETVAL, semctl_arg) < 0)
    return -1;
  return 0;
}

/* Decrements `semval` for the `num`th semaphore by 1
   Sleeps if `semval` for the `num`th sempahore is < 1
   SEM_UNDO flag indicates that the semaphore is automaticlly incremented when
   the process ends */
void sem_wait (int id, short unsigned int num)
{
  struct sembuf op[] = {
    {num, -1, SEM_UNDO}
  };
  semop (id, op, 1);
}


int sem_wait_timeout (int id, short unsigned int num)
{
  struct sembuf op[] = {
    {num, -1, SEM_UNDO}
  };
  int ret = semtimedop (id, op, 1, &timeout_ts);
  return ret;
}


/* Increments `semval` for the `num`th semaphore by 1
   SEM_UNDO flag indicates that the semaphore is automatically decremented when
   the process ends */
void sem_signal (int id, short unsigned int num)
{
  struct sembuf op[] = {
    {num, 1, SEM_UNDO}
  };
  semop (id, op, 1);
}


/* Immediately remove the semaphore set, awakening all processes blocked in
   semop calls on the set (with an error return and errno set to EIDRM) */
int sem_close (int id)
{
  if (semctl (id, 0, IPC_RMID, 0) < 0)
    return -1;
  return 0;
}
