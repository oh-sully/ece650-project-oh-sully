#include <iostream>
#include <string>
#include <pthread.h>
#include <time.h>

//Struct for passing arguments to the threads
struct ArgsStruct {
    std::string user_input;
};

//thread for io
void *io_thread(void *args){

    struct ArgsStruct *ioArgs;
    ioArgs = (struct ArgsStruct *) args;
    ioArgs->user_input = "APPLES";

    std::cout << "user_input (io) = " << ioArgs->user_input << std::endl;
    pthread_exit(NULL);
}

//main function, duh    
int main() {
    
    std::string user_input = "V 0";
    
    pthread_t io_pid;
    struct ArgsStruct ioArgs;
    ioArgs.user_input = &user_input;
    int create_io;

    create_io = pthread_create(&io_pid, NULL, io_thread, (void *)&ioArgs);
    if (create_io != 0){
        std::cerr << "Error: Couldn't create io thread; error #" << create_io << std::endl;
    }
    pthread_join(io_pid, NULL);
    
    std::cout << "user_input (main) = " << user_input << std::endl;
    std::cout << "user_input (main) = " << &user_input << std::endl;
    
    return 0;
}

/*
//thread for VC1 algorithm
void *VC1_thread(void *args){
    struct ArgsStruct *VC1Args;
    VC1Args = (struct ArgsStruct *) args;

    pthread_exit(NULL);
}


//Thread for VC2 algorithm
void *VC2_thread(void *args){
    struct ArgsStruct *VC2Args;
    VC2Args = (struct ArgsStruct *) args;

    pthread_exit(NULL);
}


//Thread for VCSAT algorithm(a4)
void *VCSAT_thread(void *args){
    struct ArgsStruct *VCSATArgs;
    VCSATArgs = (struct ArgsStruct *) args;

    pthread_exit(NULL);
}
*/

//for iothread after first two lines
/*
    pthread_t VCSAT_pid, VC1_pid, VC2_pid;
    struct ArgsStruct VCSATArgs, VC1Args, VC2Args;
    int create_VCSAT, create_VC1, create_VC2;
        
    //sets the arguments for the threads in their respective structs
    VC1Args.user_input = ioArgs->user_input;
    VC1Args.edges = ioArgs->edges;
    VC1Args.num_vert = ioArgs->num_vert;

    VC2Args.user_input = ioArgs->user_input;
    VC2Args.edges = ioArgs->edges;
    VC2Args.num_vert = ioArgs->num_vert;

    VCSATArgs.user_input = ioArgs->user_input;
    VCSATArgs.edges = ioArgs->edges;
    VCSATArgs.num_vert = ioArgs->num_vert;
    VCSATArgs.num_edges = ioArgs->num_edges;

    //set range to run_number < 10 when you want the 10 runs
    for (int run_number = 0; run_number < 1; run_number++){
        
        create_VCSAT = pthread_create(&VCSAT_pid, NULL, VCSAT_thread, (void *)&VCSATArgs);
        pthread_join(VCSAT_pid, NULL);

        create_VC1 = pthread_create(&VC1_pid, NULL, VC1_thread, (void *)&VC1Args);
        pthread_join(VC1_pid, NULL);

        create_VC2 = pthread_create(&VC2_pid, NULL, VC2_thread, (void *)&VC2Args);
        pthread_join(VC2_pid, NULL);
    }
*/