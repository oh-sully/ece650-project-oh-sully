#include <iostream>
#include <string>
#include <vector>
#include <pthread.h>
#include <time.h>
#include <fstream>
#include <iomanip>
#include <locale>
#include <math.h>

//Struct for passing arguments to the threads
struct ArgsStruct {
    std::vector<double>* CPUtimes;
};

double pclock(clockid_t cid){
    struct timespec ts;
    char buffer[20];
    double CPUtime;

    if (clock_gettime(cid, &ts) == -1){
        std::cout << "Error with gettime" << std::endl;
    }

    sprintf(buffer, "%4ld.%06ld\n", ts.tv_sec, ts.tv_nsec / 1000);
    CPUtime = std::stod(buffer);
    return CPUtime;
}

double vectomean(std::vector<double> data){
    double mean = 0;
    int N = 0;
    for (int aa = 0; aa < data.size(); aa++){
        mean += data[aa];
        N++;
    }
    mean = mean / N;
    return mean;
}

double vectosd(std::vector<double> data){
    double u = vectomean(data);
    double sd = 0;
    int N = 0;
    for (int aa = 0; aa < data.size(); aa++){
        sd += (data[aa] - u) * (data[aa] - u);
        N++;
    }
    sd = sd / N;
    sd = sqrt(sd);
    return sd;
}

//thread for io
void *io_thread(void *args){

    struct ArgsStruct *ioArgs;
    ioArgs = (struct ArgsStruct *) args;

    clockid_t cid;
    int retcode;
    retcode = pthread_getcpuclockid(pthread_self(), &cid);
    if(retcode){
        std::cerr << "Error with the damn retcode SAT" << std::endl;
    }
    else{
        double pc = pclock(cid);
        (*(ioArgs->CPUtimes)).push_back(pc);
    }

    pthread_exit(NULL);
}
   
int main() {
    
    std::vector<double> CPUtimes;
    std::vector<double> means;
    means.push_back(10);
    means.push_back(1);
    means.push_back(5);
    means.push_back(5);
    std::cout << vectosd(means) << std::endl;
    
    pthread_t io_pid;
    struct ArgsStruct ioArgs;
    ioArgs.CPUtimes = &CPUtimes;
    int create_io;

    create_io = pthread_create(&io_pid, NULL, io_thread, (void *)&ioArgs);
    if (create_io != 0){
        std::cerr << "Error: Couldn't create io thread; error #" << create_io << std::endl;
    }
    pthread_join(io_pid, NULL);
    
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