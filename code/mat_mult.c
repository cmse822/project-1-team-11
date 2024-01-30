#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/perf_event.h>
#include <asm/unistd.h>
#include <sys/wait.h>
#include <time.h>

#include <fcntl.h>
#include <sched.h>
#include "get_walltime.c"

long perf_event_open(struct perf_event_attr *hw_event, pid_t pid, int cpu, int group_fd, unsigned long flags) {
    return syscall(__NR_perf_event_open, hw_event, pid, cpu, group_fd, flags);
}

void multiplyMatrices(double **mat1, double **mat2, double **result, int rows, int cols)
{

   //double start_time, end_time;
   //get_walltime(&start_time);

   for (int i = 0; i < rows; ++i) 
   {
      for (int j = 0; j < cols; ++j)
      {
       
        for(int k=0;k<cols;k++)
        {    result[i][j] += mat1[i][k] * mat2[k][j]; }    
      }
  
   }

   //get_walltime(&end_time);
 

  //printf("%f \n", end_time-start_time);  

}



//int main()

int main(int argc, char *argv[])
{
/*
//int fd = open("helloworld", O_RDWR | O_CREAT, 0644);
int fd2 = open("/mnt/home/hejaseba/helloworlds" , O_RDWR);
if(fd2==-1)
{
perror("failed");
exit(1);
}

if(dup2(fd2,1)==-1)
{
perror("failed");
exit(1);
}
// get affinity first


printf("hello world \n");
*/


// have file helloworldssz existing before opening else use O_CREAT flag ; 
// quick open existing file ; use O_CREAT if file doesn't exist ; not checking errors ; doing it quickly
int fd2 = open("helloworldssz", O_RDWR | O_APPEND);



//float floatValue = 3.14;
//char buffer[20]; // Adjust the size based on your needs
//snprintf(buffer, sizeof(buffer), "%f\n", floatValue);
//ssize_t bytes_written = write(fd2,buffer,strlen(buffer));




cpu_set_t set;
CPU_ZERO(&set);
CPU_SET(6,&set);
int afini_ = sched_setaffinity(0,sizeof(cpu_set_t),&set);
    if (afini_ == -1) {
        perror("sched_setaffinity");
        return 1;
    }


pid_t pid = getpid();
struct perf_event_attr pe;
memset(&pe, 0, sizeof(struct perf_event_attr));
pe.size = sizeof(struct perf_event_attr);
pe.disabled = 1;
pe.exclude_kernel = 1;
pe.exclude_hv = 1;
//pe.type = PERF_TYPE_SOFTWARE;
pe.type = PERF_TYPE_HARDWARE;
//pe.config = PERF_COUNT_HW_CPU_CYCLES;
//pe.config = PERF_COUNT_SW_CONTEXT_SWITCHES;
//pe.config = PERF_COUNT_HW_REF_CPU_CYCLES;
int fd = perf_event_open(&pe, pid, -1, -1, 0);
if (fd == -1) {
        perror("perf_event_open");
        exit(EXIT_FAILURE);
    }


//ioctl(fd, PERF_EVENT_IOC_RESET, 0);
//ioctl(fd, PERF_EVENT_IOC_ENABLE, 0);

int k_ = 1;

for(;k_<=100;k_++)
{
printf("%d here: \n",k_);



int rows = k_;
int cols = k_;

//Allocating an array of pointers (2d array)
double **arr1 = (double **)malloc(rows*sizeof(double *));
double **arr2 = (double **)malloc(rows*sizeof(double *));
double **result = (double **)malloc(rows*sizeof(double *));

//printf("%p first \n",arr1);
//printf("%p first \n",arr2);

int i=0;
int j = 0;

double **temp1 = arr1;
double **temp2 = arr2;
double **temp3 = result;

for(;i<rows;++i)//++arr1,++arr2)
{
  //arr[i] = (double *)malloc(cols*sizeof(int));
  
  // allocating cols double objects to each element of arr1 and arr2 (pointer to double elements
   //arr1[i]
  *(arr1+i) = (double *)malloc(cols*sizeof(double));
  *(arr2+i) = (double *)malloc(cols*sizeof(double));
   //arr2[i] = (double *)malloc(cols*sizeof(double));    
   *(result+i)  = (double *)malloc(cols*sizeof(double)); 
   //result[i] = (double *)malloc(cols*sizeof(double));
}

// restore arr1 and arr2 to point to the first column
arr1 = temp1;
arr2 = temp2;
result = temp3;


//printf("%p first \n",arr1);
//printf("%p first \n",arr2);



// initializing with random values 

srand(time(NULL));

for (i=0;i<rows;i++)
{
for (j=0;j<cols;j++)
{ 
   //*((*(arr1+1))+j) = 100;  
   arr1[i][j] = rand() / (double) RAND_MAX;// % 100000; // values between 0 and 99999
   arr2[i][j] = rand() / (double) RAND_MAX;// 100000;
   
   // alternatively can keep on incremeenting arr1 by 1 and accessing it using *(*(arr +i)+j) 
}

}



// matrix multiplication here


ioctl(fd, PERF_EVENT_IOC_RESET, 0);
ioctl(fd, PERF_EVENT_IOC_ENABLE, 0);
double start_time, end_time;
get_walltime(&start_time);
multiplyMatrices(arr1,arr2,result, rows,cols);
get_walltime(&end_time);
ioctl(fd, PERF_EVENT_IOC_DISABLE, 0);
long long count;
read(fd, &count, sizeof(long long));
//printf("level 1 data cache misses: %lld\n", count);
printf("cpu cycles: %lld\n", count);
//get_walltime(&end_time);

printf("%f \n", end_time-start_time);
double value_ = end_time - start_time;

char buffer[20]; // Adjust the size based on your needs

snprintf(buffer, sizeof(buffer), "%f\n", value_);
ssize_t bytes_written = write(fd2,buffer,strlen(buffer));
char buffer_2;
buffer_2 = '\n';
bytes_written = write(fd2,&buffer_2,1);


for(i=0;i<rows;++i,++arr1,++arr2,++result)
{ free(*arr1); free(*arr2); free(*result); }


arr1 = temp1;
arr2 = temp2;
result = temp3;

free(arr1);
free(arr2);
free(result);
}

return 0;

}
