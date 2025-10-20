#ifndef __PARAVER_READER_UTIL_H_
#define __PARAVER_READER_UTIL_H_

/* paraver_reader_util.h is a generalised framework for reading Paraver files.
 *
 * Copyright (C) 2024, High Performance Computing Center Stuttgart,
 *                     University of Stuttgart.  All rights reserved.
 *
 * author: Kingshuk Haldar <kingshuk.haldar@hlrs.de>
 */

#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<limits.h>
#include<time.h>




#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))

#define FREE_IF(a) if(NULL!= a) { free(a); a= NULL; }

#define DEBUG_WAIT(a)                                    \
  if(a) {                                                \
    printf("%d\n", getpid()); fflush(stdout);            \
    int temp_inside= -1;                                 \
    scanf("%d", &temp_inside);                           \
  }

#if 0
#define GET_TIME()                                              \
{                                                               \
  {struct timespec tp_inside;                                    \
  long long t_inside= -1;                                       \
  if(0== clock_gettime(CLOCK_MONOTONIC, &tp_inside)) {          \
    t_inside= (long long) (tp_inside.tv_sec* 1000000000)+       \
              (long long) (tp_inside.tv_nsec);                  \
  }}                                                            \
  t_inside;                                                     \
}
#endif

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define NUM_MPI_FUNCS 194
#define IMP_FLUSH_EVENT (NUM_MPI_FUNCS+0)
#define NUM_IMP_EVENTS (NUM_MPI_FUNCS+1)
const char *MPINames[NUM_MPI_FUNCS]= {
/* 0-8 */
"Useful", "Send", "Recv", "Isend", "Irecv", "Wait", "Waitall", "Bcast", "Barrier",
/* 9-15 */
"Reduce", "Allreduce", "Alltoall", "Alltoallv", "Gather", "Gatherv", "Scatter",
/* 16-21 */
"Scatterv", "Allgather", "Allgatherv", "Comm_rank", "Comm_size", "Comm_create",
/* 22-26 */
"Comm_dup", "Comm_split", "Comm_group", "Comm_free", "Comm_remote_group",
/* 27-31 */
"Comm_remote_size", "Comm_test_inter", "Comm_compare", "Scan", "Init",
/* 32-39 */
"Finalize", "Bsend", "Ssend", "Rsend", "Ibsend", "Issend", "Irsend", "Test",
/* 40-44 */
"Cancel", "Sendrecv", "Sendrecv_replace", "Cart_create", "Cart_shift",
/* 45-50 */
"Cart_coords", "Cart_get", "Cart_map", "Cart_rank", "Cart_sub", "Cartdim_get",
/* 51-55 */
"Dims_create", "Graph_get", "Graph_map", "Graph_create", "Graph_neighbors",
/* 56-60 */
"Graphdims_get", "Graph_neighbors_count", "Topo_test", "Waitany", "Waitsome",
/* 61-67 */
"Probe", "Iprobe", "Win_create", "Win_free", "Put", "Get", "Accumulate",
/* 68-73 */
"Win_fence", "Win_start", "Win_complete", "Win_post", "Win_wait", "Win_test",
/* 74-79 */
"Win_lock", "Win_unlock", "Pack", "Unpack", "Op_create", "Op_free",
/* 80-84 */
"Reduce_scatter", "Attr_delete", "Attr_get", "Attr_put", "Group_difference",
/* 85-89 */
"Group_excl", "Group_free", "Group_incl", "Group_intersection", "Group_rank",
/* 90-93 */
"Group_range_excl", "Group_range_incl", "Group_size", "Group_translate_ranks",
/* 94-97 */
"Group_union", "Group_compare", "Intercomm_create", "Intercomm_merge",
/* 98-102 */
"Keyval_free", "Keyval_create", "Abort", "Error_class", "Errhandler_create",
/* 103-106 */
"Errhandler_free", "Errhandler_get", "Error_string", "Errhandler_set",
/* 107-111 */
"Get_processor_name", "Initialized", "Wtick", "Wtime", "Address",
/* 112-116 */
"Bsend_init", "Buffer_attach", "Buffer_detach", "Request_free", "Recv_init",
/* 117-121 */
"Send_init", "Get_count", "Get_elements", "Pack_size", "Rsend_init",
/* 122-127 */
"Ssend_init", "Start", "Startall", "Testall", "Testany", "Test_cancelled",
/* 128-132 */
"Testsome", "Type_commit", "Type_contiguous", "Type_extent", "Type_free",
/* 133-137 */
"Type_hindexed", "Type_hvector", "Type_indexed", "Type_lb", "Type_size",
/* 138-143 */
"Type_struct", "Type_ub", "Type_vector", "File_open", "File_close", "File_read",
/* 144-147 */
"File_read_all", "File_write", "File_write_all", "File_read_at",
/* 148-151 */
"File_read_at_all", "File_write_at", "File_write_at_all", "Comm_spawn",
/* 152-155 */
"Comm_spawn_multiple", "Request_get_status", "Ireduce", "Iallreduce",
/* 156-161 */
"Ibarrier", "Ibcast", "Ialltoall", "Ialltoallv", "Iallgather", "Iallgatherv",
/* 162-167 */
"Igather", "Igatherv", "Iscatter", "Iscatterv", "Ireducescat", "Iscan",
/* 168-171 */
"Reduce_scatter_block", "Ireduce_scatter_block", "Alltoallw", "Ialltoallw",
/* 172-174 */
"Get_accumulate", "Dist_graph_create", "Neighbor_allgather",
/* 175-177 */
"Ineighbor_allgather", "Neighbor_allgatherv", "Ineighbor_allgatherv",
/* 178-180 */
"Neighbor_alltoall", "Ineighbor_alltoall", "Neighbor_alltoallv",
/* 181-183 */
"Ineighbor_alltoallv", "Neighbor_alltoallw", "Ineighboralltoallw",
/* 184-187 */
"Fetch_and_op", "Compare_and_swap", "Win_flush", "Win_flush_all",
/* 188-192 */
"Win_flush_local", "Win_flush_local_all", "Mprobe", "Improbe", "Mrecv",
/* 193-193 */
"Imrecv",
};

#define NUM_VALID_EVENTS 7
const long long ValidEventIds[NUM_VALID_EVENTS]= {
40000001,  /* Application */
40000003,  /* Flushing Traces */
50000001,  /* MPI Point-to-point */
50000002,  /* MPI Collective Comm */
50000003,  /* MPI Other */
50000004,  /* MPI RMA */
50000005   /* MPI I/O */
};

#define NUM_MPI_EVENTS 5
const long long MPIEventIds[NUM_MPI_EVENTS]= {
50000001,  /* MPI Point-to-point */
50000002,  /* MPI Collective Comm */
50000003,  /* MPI Other */
50000004,  /* MPI RMA */
50000005   /* MPI I/o */
};
bool ImpEventTypeIsMPI(const long long type)
{
  bool isValid= false;
  for(int i= 0; i< NUM_MPI_EVENTS; ++i) {
    if(MPIEventIds[i]== type) {
      isValid= true;
      break;
    }
  }
  return isValid;
}

bool ImpMPIEventIdIsGlobal(const int id)
{
  bool ret= false;
  switch(id) {
  case 7:
  case 8:
  case 9:
  case 10:
  case 12:
  case 13:
  case 14:
  case 15:
  case 16:
  case 154:
  case 155:
  case 156:
  case 157:
  case 158:
  case 159:
  case 160:
  case 161:
  case 162:
  case 163:
  case 164:
  case 165:
  case 166:
  case 167:
  case 168:
  case 169:
  case 170:
  case 171:
  case 174:
  case 175:
  case 176:
  case 177:
  case 178:
  case 179:
  case 180:
  case 181:
  case 182:
  case 183:
    ret= true;
    break;
  default:
    break;
  }
  return ret;
}

#define IMP_NUM_DIGITS(a) (((int) floor(log10((double) a)))+ 1)
int ImpNumDigits(const long long unsigned n)
{
  register long long unsigned a= 1;
  for(int i= 1; i< 20; ++i) {
    a*= 10;
    if(n< a) { return i; }
  }
  return 20;
}
long long TenPower_int_ll(const int p)
{
  long long ret= 1;
  for(int i= 0; i< p; ++i) {
    ret*= 10;
  }
  return ret;
}

typedef long long ProfTime;
#define PROFTIME_MAX LLONG_MAX
#define PROFTIME_MIN LLONG_MIN
const ProfTime ProfTimeInvalid= -1;

long long GetTime()
{
  struct timespec tp;
  long long t= -1;
  if(0== clock_gettime(CLOCK_MONOTONIC, &tp)) {
    t= (long long) (tp.tv_sec* 1000000000)+ (long long) (tp.tv_nsec);
  }
  return t;
}

ProfTime ImpFreadTime= 0;
static size_t TimedFread(char *const ptr, const size_t size, const size_t nmemb, FILE *const fp)
{
  ImpFreadTime-= GetTime();
  size_t nitems= fread(ptr, size, nmemb, fp);
  ImpFreadTime+= GetTime();
  return nitems;
}

size_t ImpGetNumBytes(FILE *const fp)
{
  size_t numBytes= 0;
  fpos_t fpos; fgetpos(fp, &fpos);
#define MAXBUF 32768  /* 32x1024 */
  char line[MAXBUF]; int readCount;
  while(0!= (readCount= TimedFread(line, 1, MAXBUF, fp))) {
    numBytes+= ((size_t) readCount);
  }
#undef MAXBUF
  fsetpos(fp, &fpos);
  return numBytes;
}

inline static char *ImpNthNum(char *const p, const int n)
{
  char *ptr= p;
  for(int i= 0; i< n; ++i) {
    ptr= strchr(ptr, ':')+ 1;
  }
  return ptr;
}

typedef struct _PrvHeader_type__ {
  long long runTime;
  int numNodes;
  int numProcs;
  int numComms;
} PrvHeader;

extern PrvHeader header;

static void printProgramRuntimeWithDot(const long long rt, const char *const u, const int numdots)
{
  long long d= 1;
  for(int i= 0; i< numdots; ++i) { d*= 1000; }
  printf("%lld", (rt/ d));
  for(int i= 0; i< numdots; ++i) {
    d/= 1000;
    printf(".%03lld", (rt/ d)% 1000);
  }
  printf(" %s\n", u);
  /* for(int i= numdots; i> 0; --i) { */
  /*   long long d= 1; */
  /*   for(int j= 0; j< i; ++j) { d*= 1000; } */
  /*   /\* printf("%lld.", (rt/ d)% 1000); *\/ */
  /*   printf("%lld.", (rt% d)/ ); */
  /* } */
  /* printf("%lld %s\n", rt% 1000, u); */
}
static void printProgramRuntime(const long long rt, const char *const u)
{
  printf("Program-runtime= ");
  if(0== strcmp("ns", u)) {
    printProgramRuntimeWithDot(rt, u, 3);
  } else if(0== strcmp("us", u)) {
    printProgramRuntimeWithDot(rt, u, 2);
  } else if(0== strcmp("ms", u)) {
    printProgramRuntimeWithDot(rt, u, 1);
  } else {
    printf("%lld %s\n", rt, u);
  }
}
void ImpProcessParaverHeader(FILE *fp)
{
#define MAX_HEADER_SIZE 131072  /* 128x1024 */
  char buf[MAX_HEADER_SIZE];
  if(NULL== fgets(buf, MAX_HEADER_SIZE, fp)) {
    printf("Problem reading header. Exiting.\n"); exit(0);
  }
  char *ptr= strtok(buf, ":"); ptr= strtok(NULL, ":"); ptr= strtok(NULL, ":");
  ptr[strlen(ptr)- 3]= '\0'; header.runTime= atoll(ptr);
  printProgramRuntime(header.runTime, ptr+ strlen(ptr)+ 1);
  printf("Program-runtime= %lld %s\n", header.runTime, ptr+ strlen(ptr)+ 1);

  ptr= strtok(NULL, "("); header.numNodes= atoi(ptr);
  header.numProcs= 0;
  for(int i= 1; i< header.numNodes; ++i) {
    ptr= strtok(NULL, ","); header.numProcs+= atoi(ptr);
  }
  ptr= strtok(NULL, ")"); header.numProcs+= atoi(ptr);

  ptr= strtok(NULL, "\n"); ptr= strrchr(ptr, ',');
  char a[32]= { '\0' }; strcpy(a, ptr+ 1); if(strchr(a, ')')) { return; }
  header.numComms= atoi(a);
  for(int i= 0; i< header.numComms; ++i) {
    if(NULL== fgets(buf, MAX_HEADER_SIZE, fp)) {
      printf("Problem reading communicators. Exiting.\n"); exit(0);
    }
  }
#undef MAX_HEADER_SIZE
}

void (*ImpProcessLine)(char *const)= NULL;
void ImpSetLineProcessorAndResetFreadTime(void (*fn)(char *const))
{
  ImpProcessLine= fn;
  ImpFreadTime= 0;
}
void ImpProcessBuffer(char *const buf)
{
  if(NULL== ImpProcessLine) { return; }
  char *ptr= strtok(buf, "\n");
  while(NULL!= ptr) {
    ImpProcessLine(ptr);
    ptr= strtok(NULL, "\n");
  }
}

size_t ImpGetLastNewlinePos(const char *const buf, const size_t buflen, const size_t len)
{
  size_t ret= ULLONG_MAX;
  if('\0'== buf[0]|| 0== buflen) {
    char tmp[11]= { '\0' }; strncpy(tmp, buf, 10);
    printf("returning ULLONG_MAX (buffer= \"%s\", buflen= %lu\n", tmp, buflen);
    return ret;
  }
  size_t i= 0== len? buflen- 1: len- 1;
  for(; i> 0; --i) { if('\n'== buf[i]) { break; } }
  if('\n'== buf[i]) { ret= i; }
  return ret;
}

bool ImpProcessParaverBufferSilently= false;
void ImpProcessParaverFileWithNumBytes(FILE *fp, const size_t numBytes)
{
  if(!ImpProcessParaverBufferSilently) {
    printf("Size after comms section: %.1lf MB\n", ((double) numBytes)/ 1024.0/ 1024.0);
    printf("Processed %02d%%...", 0); fflush(stdout);
  }
  const size_t buflen= 32*1024*1024;
  char *buf= malloc(sizeof(char)* (buflen+ 1)); buf[buflen]= '\0';
  size_t car= 0, rem= buflen- 1, numBytesRead= 0, numBytesProcessed= 0;

  while(0!= (numBytesRead= TimedFread(buf+ car, 1, rem, fp)+ car)) {
    size_t len= ImpGetLastNewlinePos(buf, buflen, numBytesRead);
    buf[len]= '\0';
    numBytesProcessed+= len+ 1;
    ImpProcessBuffer(buf);
    car= numBytesRead- len- 1;
    rem= numBytesRead- car;
    memmove(buf, buf+ len+ 1, car);
    if(!ImpProcessParaverBufferSilently) {
      printf("\rProcessed %02d%%...", (int) (numBytesProcessed* 100/ numBytes)); fflush(stdout);
    }
  }
  if(!ImpProcessParaverBufferSilently) {
    printf("\n"); fflush(stdout);
  }
  if(buf) { free(buf); buf= NULL; }
}

void ImpProcessParaverFile(FILE *fp)
{
  const size_t numBytes= ImpGetNumBytes(fp);
  ImpProcessParaverFileWithNumBytes(fp, numBytes);
}

void ImpProcessPrvHeader(FILE *fp)
{
  ImpProcessParaverHeader(fp);
}

FILE *ImpFileReadExitOnError(const char *const fn)
{
  FILE *fp= fopen(fn, "r");
  if(NULL== fp) {
    char msg[512]= { '\0' };
    sprintf(msg, "Error opening \"%s\"", fn);
    msg[511]= '\0';
    perror(msg);
    exit(0);
  }
  return fp;
}

ProfTime ImpGetTimePointNS(const char *const input)
{
  ProfTime tp= atoll(input);
  if(0== tp) { return tp; }
  if(-1== tp) {
    printf("Will read till end of file.\n");
    return tp;
  }
  char unit[3]= { '\0' };
  sscanf(input, "%*d%2s", unit);
  if(0== strlen(unit)) {
    printf("No unit, using \"ns\"\n");
    strcpy(unit, "ns");
  }
  printf("Time-point= \"%lld\" \"%s\" -> ", tp, unit);
  if(0== strcmp(unit, "ns")) { tp*= 1; }
  else if(0== strcmp(unit, "us")) { tp*= 1000; }
  else if(0== strcmp(unit, "ms")) { tp*= 1000000; }
  else if(0== strcmp(unit, "s")) { tp*= 1000000000; }
  else {
    printf("Invalid unit (\"%s\")\n", unit);
    exit(0);
  }
  printf("%lld\n", tp);
  return tp;
}

bool ImpArgvHelpNeeded(const int argc, char **const argv)
{
  const bool ret= 0== strcmp("-h", argv[1])|| 0== strcmp("--help", argv[1]);
  return ret;
}

/* pass: 0, 1, 2, etc. */
/* factor: multiplication factor to make it ns */
void ImpPrintSpeed(const int pass, const size_t B, const ProfTime readtime, const size_t factor)
{
  const double rts= ((double) (readtime* factor))* 1.0e-9;
  printf(" Pass %2d: %.3lf s (speed: ", pass, rts);
  const double Bps= ((double) B)/ rts;
  if(Bps> 1024.0* 1024.0* 1024.0) {
    printf("%.0lf GBps", Bps/ (1024.0* 1024.0* 1024.0));
  } else if(Bps> 1024.0* 1024.0) {
    printf("%.0lf MBps", Bps/ (1024.0* 1024.0));
  } else if(Bps> 1024.0) {
    printf("%.0lf kBps", Bps/ 1024.0);
  } else {
    printf("%.0lf Bps", Bps);
  }
  printf("); I/O calls: %.3lf s\n", ((double) ImpFreadTime)* 1.0e-9);
}

int ImpArrayElemMax_int(const int *const arr, const size_t n)
{
  int max= INT_MIN;
  for(size_t i= 0; i< n; ++i) {
    max= MAX(max,arr[i]);
  }
  return max;
}

ProfTime ImpArrayElemMax_ProfTime(const ProfTime *const arr, const size_t n)
{
  ProfTime max= PROFTIME_MIN;
  for(size_t i= 0; i< n; ++i) {
    max= MAX(max,arr[i]);
  }
  return max;
}


#endif  /* __PARAVER_READER_UTIL_H_ */
