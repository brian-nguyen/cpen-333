#ifndef MIDTERM_ROLLERCOASTER_H
#define MIDTERM_ROLLERCOASTER_H

//=============================================================
// TODO: Add names and layouts for any shared resources
//=============================================================

#define ROLLERCOASTER_CAPACITY 4

#define RIDE_SHARED_MEM "q4_mem"
#define LINE_CV "q4_cv"
#define LINE_MUTEX "q4_mutex"

#define BOARD_SEMAPHORE "q4_board"
#define FULL_SEMAPHORE "q4_full"
#define EXIT_SEMAPHORE "q4_exit"
#define EMPTY_SEMAPHORE "q4_empty"

struct RideData {
  int num_passengers;
  int num_waiting;
};

#endif //MIDTERM_ROLLERCOASTER_H
