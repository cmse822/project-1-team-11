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
#include </mnt/home/hejaseba/get_walltime.h>

long perf_event_open(struct perf_event_attr *hw_event, pid_t pid, int cpu, int group_fd, unsigned long flags) {
    return syscall(__NR_perf_event_open, hw_event, pid, cpu, group_fd, flags);
}

void multiplyMatrices(double **mat1, double **mat2, double **result, int rows, int cols)
{


   for (int i = 0; i < rows; ++i)
   {
      for (int j = 0; j < cols; ++j)
      {

        for(int k=0;k<cols;k++)
        {    result[i][j] += mat1[i][k] * mat2[k][j]; }
      }

   }


}




int main(int argc, char *argv[])
{


int fd2 = open("aokz_", O_RDWR | O_APPEND);




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
pe.type = PERF_TYPE_HARDWARE;
pe.config = PERF_COUNT_HW_REF_CPU_CYCLES;
int fd = perf_event_open(&pe, pid, -1, -1, 0);
if (fd == -1) {
        perror("perf_event_open");
        exit(EXIT_FAILURE);
    }


int k_ = 1;
for(;k_<=1000;k_++)
{
printf("%d here: \n",k_);



int rows = k_;
int cols = k_;
double **arr1 = (double **)malloc(rows*sizeof(double *));
double **arr2 = (double **)malloc(rows*sizeof(double *));
double **result = (double **)malloc(rows*sizeof(double *));


int i=0;
int j = 0;

double **temp1 = arr1;
double **temp2 = arr2;
double **temp3 = result;

for(;i<rows;++i)
{
    *(arr1+i) = (double *)malloc(cols*sizeof(double));
  *(arr2+i) = (double *)malloc(cols*sizeof(double));
   *(result+i)  = (double *)malloc(cols*sizeof(double));
 }

arr1 = temp1;
arr2 = temp2;
result = temp3;




srand(time(NULL));

for (i=0;i<rows;i++)
{
for (j=0;j<cols;j++)
{
   arr1[i][j] = rand() / (double) RAND_MAX;
   arr2[i][j] = rand() / (double) RAND_MAX;
   result[i][j] = 0;
}

}





ioctl(fd, PERF_EVENT_IOC_RESET, 0);
ioctl(fd, PERF_EVENT_IOC_ENABLE, 0);
double start_time, end_time;
get_walltime(&start_time);
multiplyMatrices(arr1,arr2,result, rows,cols);
get_walltime(&end_time);
ioctl(fd, PERF_EVENT_IOC_DISABLE, 0);
long long count;
read(fd, &count, sizeof(long long));
printf("cpu cycles: %lld\n", count);

printf("%f \n", end_time-start_time);
double value_ = end_time - start_time;


char buffer[20];
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
