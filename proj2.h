/**
 * Function, which put 'text' on stderr and return exit(EXIT_FAILED)
 * @param text - Text to stderr
 */
void error(char* text);

/**
 * Initialize arguments. Check if it's number.
 * @param argc - count of arguments
 * @param args - array arguments
 * @param count - temporary check
 */
void initArgs(int argc,char** args,int count);

/**
 * Serves process with Riders.
 * @param param - struct of parameters.
 */
void createRider(Params param);

/**
 *Serves process with Bus.
 * @param param
 */
void createBus(Params param);

/**
 * Free all resources from memory.
 */
void freeSources();

/**
 * Initialization semaphores.
 */
void initSem();

/**
 * Initialization shared memory.
 */
void initSHM();

/**
 * Free all resources from memory and kill all process.
 */
void killAll();

/**
 * Increment number of line in file.
 */
void incLineID();

/**
 * Check if string is int. If not, return -1
 * @param string
 * @return 0/1
 */
int isNumber(char* string);