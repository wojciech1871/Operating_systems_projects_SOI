/*this header defines keyes to all shared memory units
 * and semaphores for use in all processess */

#define TRUE  1
#define	KEY_EMPTY 100
#define	KEY_FULL 200

#define	KEY_MUTEX 300
#define	KEY_READ 400
#define	KEY_REMOVE 500

#define KEY_BUFF  600
#define KEY_FLAGS 700

#define	BUFF_SIZE 20

#define MSEC 1000
#define RANGE 500

#define BUFF_BEG  flags[0]
#define BUFF_END  flags[1]
#define FLAG_A    flags[2]
#define FLAG_B    flags[3]
#define FLAG_C    flags[4]
#define READ_A    flags[5]
#define READ_B    flags[6]
#define READ_C    flags[7]
#define NUM_EL    flags[8]

int buffer_id;
int flags_id;
int empty_id;
int full_id;
int mutex_id;
int read_id;
int remove_id;
int *flags;
int *buffer;
