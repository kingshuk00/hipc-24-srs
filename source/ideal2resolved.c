/* ideal2resolved creates transfer-efficiency monitors from recorded and
 * ideal-network simulated traces.
 *
 * Copyright (C) 2024, High Performance Computing Center Stuttgart,
 *                     University of Stuttgart.  All rights reserved.
 *
 * author: Kingshuk Haldar <kingshuk.haldar@hlrs.de>
 */
#include"paraver_reader_util.h"
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<float.h>
#include<math.h>

PrvHeader header;

static struct {
  char *prvfn;
  char *simfn;
} Input= { NULL, NULL };
static int FileInput(const char *const arg_in)
{
  FILE *fp= fopen(arg_in, "r");
  if(NULL== fp) { return 1; }

  char line[1024]= { '\0' };
  int len= (int) (sizeof(line)/ sizeof(line[0]));
  while(NULL!= fgets(line, len, fp)) {
    if(0== strncmp("prv-file", line, 8)) {
      char *fn= strtok(line+ 8, " \n");
      Input.prvfn= (char *) malloc(sizeof(char)* (strlen(fn)+ 1));
      memset(Input.prvfn, 0, sizeof(char)* (strlen(fn)+ 1));
      strcpy(Input.prvfn, fn);
    } else if(0== strncmp("sim-file", line, 8)) {
      char *fn= strtok(line+ 8, " \n");
      Input.simfn= (char *) malloc(sizeof(char)* (strlen(fn)+ 1));
      memset(Input.simfn, 0, sizeof(char)* (strlen(fn)+ 1));
      strcpy(Input.simfn, fn);
    } else {
      printf("Problem reading input line (%s)\n", line);
    }
  }
  fclose(fp); fp= NULL;

#if 1
  printf("Input.prvfn= \"%s\"\n", Input.prvfn);
  printf("Input.simfn= \"%s\"\n", Input.simfn);
#endif

  if(NULL== Input.prvfn|| NULL== Input.simfn) {
    return 1;
  }

  return 0;
}

static struct {
  struct {
    double t;
    int e;
  } prv;

  struct {
    double t;
    int e;
  } sim;

  double at;
  bool isTracing;
} Last= { { 0.0, -1 }, { 0.0, -1 }, 0.0, false };
inline static void SetLastPrvEvt(const double t, const int e) { Last.prv.t= t; Last.prv.e= e; }
inline static void SetLastSimEvt(const double t, const int e) { Last.sim.t= t; Last.sim.e= e; }
inline static void ResetLastEvts(const int resetVal)
{
  SetLastPrvEvt(0.0, resetVal);
  SetLastSimEvt(0.0, resetVal);
}

static struct {
  struct {
    double *t;
    int *e;
    long nevts;
    long ievt;
  } sim;

  double (*extents)[2];

  double tprog[2];
  long nevts;
  long ievt;

  int monRank;
} Trace= { { NULL, NULL, 0, 0 }, NULL, { 0.0, 0.0 }, 0, 0, 0 };

inline static void AllocSimBeforeCount()
{
  const int np= header.numProcs;

  Trace.sim.nevts= 0;
  Trace.sim.ievt= 0;

  Trace.extents= (double (*)[2]) malloc(sizeof(double[2])* np);
  for(int ip= 0; ip< np; ++ip) {
    Trace.extents[ip][0]= Trace.extents[ip][1]= -1.0;
  }
}

inline static void AllocSimBeforeRead()
{
  const int np= header.numProcs;

  Trace.sim.t= (double *) malloc(sizeof(double)* Trace.sim.nevts);
  memset(Trace.sim.t, 0, sizeof(double)* Trace.sim.nevts);

  Trace.sim.e= (int *) malloc(sizeof(int)* Trace.sim.nevts);
  memset(Trace.sim.e, 0, sizeof(int)* Trace.sim.nevts);
}

inline static void AllocPrvBeforeCount()
{
}

static void ProcessFile(FILE *const fp, const size_t nb,
                        void (*processor)(char *const))
{
  ImpSetLineProcessorAndResetFreadTime(processor);
  ProfTime tRead= GetTime();
  ImpProcessParaverFileWithNumBytes(fp, nb);
  tRead= GetTime()- tRead;
  ImpPrintSpeed(0, nb, tRead, 1);
}
static void SimEvtCounter(char *const line)
{
  if(2!= atoi(line)) { return; }
  
  char *ptr= ImpNthNum(line, 3);
  const int p= atoi(ptr)- 1;
  if(Trace.monRank!= p) { return; }
  ptr= ImpNthNum(ptr, 2);
  Last.at= atof(ptr);

  ptr= strchr(ptr, ':');
  while(NULL!= ptr) {
    ++ptr;
    long long type= atoll(ptr);
    ptr= ImpNthNum(ptr, 1);
    switch(type) {
    case 50000001:  /* mpi p2p */
    case 50000002:  /* mpi collective */
    case 50000003:  /* mpi other */
    case 50000004:  /* mpi rma */
    case 50000005:  /* mpi i/o */
      ++(Trace.sim.nevts);
      break;
    case 40000001:  /* application */
      break;
    case 40000002:  /* trace init */
      break;
    case 40000003:  /* flush */
      break;
    case 40000012:  /* tracing enabled/disabled */
      break;
    default:
      break;
    }
    ptr= strchr(ptr, ':');
  }
}
inline static void ReadSimEvt(const double t, const int e)
{
  Trace.sim.t[Trace.sim.ievt]= t;
  Trace.sim.e[Trace.sim.ievt]= e;
  ++(Trace.sim.ievt);
}
static void SimEvtReader(char *const line)
{
  if(2!= atoi(line)) { return; }
  
  char *ptr= ImpNthNum(line, 3);
  const int p= atoi(ptr)- 1;
  if(Trace.monRank!= p) { return; }
  ptr= ImpNthNum(ptr, 2);
  Last.at= atof(ptr);

  ptr= strchr(ptr, ':');
  while(NULL!= ptr) {
    ++ptr;
    long long type= atoll(ptr);
    ptr= ImpNthNum(ptr, 1);
    switch(type) {
    case 50000001:  /* mpi p2p */
    case 50000002:  /* mpi collective */
    case 50000003:  /* mpi other */
    case 50000004:  /* mpi rma */
    case 50000005:  /* mpi i/o */
      ReadSimEvt(Last.at, atoi(ptr));
      break;
    case 40000001:  /* application */
      break;
    case 40000002:  /* trace init */
      break;
    case 40000003:  /* flush */
      break;
    case 40000012:  /* tracing enabled/disabled */
      break;
    default:
      break;
    }
    ptr= strchr(ptr, ':');
  }
}
inline static void SimCount(FILE *const fp, const size_t nb, const int np)
{
  ProcessFile(fp, nb, &SimEvtCounter);
}
inline static void SimRead(FILE *const fp, const size_t nb, const int np)
{
  ProcessFile(fp, nb, &SimEvtReader);
  if(Trace.sim.ievt!= Trace.sim.nevts) {
    printf("Simulated: counted %ld, read %ld\n",
           Trace.sim.nevts, Trace.sim.ievt);
  }
}

static struct {
  FILE *fp;
  double (*instant)[2];
  double cumulative[2];
  int n;
} Writer= { NULL, NULL, { 0.0, 0.0 }, 0 };
#define CRITIC 0
#define ELAPSE 1
/* useful, critical, elapsed */
inline static void WriteInit()
{
  Writer.fp= fopen("resolved.dat", "w");
  fprintf(Writer.fp, "#%14s %15s %15s %15s %15s\n",
          "t1-1", "crit-inst-2", "elps-inst-3", "crit-cum-4", "elps-cum-5");

  Writer.n= (int) sqrt(((double) header.numProcs));
  Writer.instant= (double (*)[2]) malloc(sizeof(double[2])* Writer.n);
  memset(Writer.instant, 0, sizeof(double[2])* Writer.n);
}
inline static void WriteFinalize()
{
  fclose(Writer.fp); Writer.fp= NULL;
}
inline static void WriteOut(const double tr, const double tc, const int e)
{
  if(0!= e) { return; }         /* starting useful */
  for(int i= 1; i< Writer.n; ++i) {
    Writer.instant[i- 1][CRITIC]= Writer.instant[i][CRITIC];
    Writer.instant[i- 1][ELAPSE]= Writer.instant[i][ELAPSE];
  }
  Writer.instant[Writer.n- 1][CRITIC]= tc;
  Writer.instant[Writer.n- 1][ELAPSE]= tr;

  Writer.cumulative[CRITIC]+= Writer.instant[Writer.n- 1][CRITIC]-
    Writer.instant[Writer.n- 2][CRITIC];
  Writer.cumulative[ELAPSE]+= Writer.instant[Writer.n- 1][ELAPSE]-
    Writer.instant[Writer.n- 2][ELAPSE];

  fprintf(Writer.fp, "%.9e %.9e %.9e %.9e %.9e\n",
          Writer.cumulative[ELAPSE],
          Writer.instant[Writer.n- 1][CRITIC]- Writer.instant[0][CRITIC],
          Writer.instant[Writer.n- 1][ELAPSE]- Writer.instant[0][ELAPSE],
          Writer.cumulative[CRITIC], Writer.cumulative[ELAPSE]);
}
static void PlayEvt(const double t, const int e)
{
  if(e!= Trace.sim.e[Trace.sim.ievt]) {
    printf("Problem\n");
    return;
  }

  WriteOut(t, Trace.sim.t[Trace.sim.ievt], e);

  SetLastPrvEvt(t, e);
  SetLastSimEvt(Trace.sim.t[Trace.sim.ievt], e);

  ++(Trace.nevts);
  ++(Trace.sim.ievt);
}
static void PrvEvtPlayer(char *const line)
{
  if(2!= atoi(line)) { return; }
  
  char *ptr= ImpNthNum(line, 3);
  const int p= atoi(ptr)- 1;
  if(Trace.monRank!= p) { return; }
  ptr= ImpNthNum(ptr, 2);
  Last.at= atof(ptr);

  ptr= strchr(ptr, ':');
  while(NULL!= ptr) {
    ++ptr;
    long long type= atoll(ptr);
    ptr= ImpNthNum(ptr, 1);
    switch(type) {
    case 50000001:  /* mpi p2p */
    case 50000002:  /* mpi collective */
    case 50000003:  /* mpi other */
    case 50000004:  /* mpi rma */
    case 50000005:  /* mpi i/o */
      PlayEvt(Last.at, atoi(ptr));
      break;
    case 40000001:  /* application */
      break;
    case 40000002:  /* trace init */
      break;
    case 40000003:  /* flush */
      break;
    case 40000012:  /* tracing enabled/disabled */
      break;
    default:
      break;
    }
    ptr= strchr(ptr, ':');
  }
}
inline static void PrvPlay(FILE *const fp, const size_t nb, const int np)
{
  Trace.sim.ievt= 0;
  WriteInit();
  ProcessFile(fp, nb, &PrvEvtPlayer);
  printf("prv-count: %ld, sim-count= %ld, diff= %ld\n", Trace.nevts, Trace.sim.nevts, Trace.nevts- Trace.sim.nevts);
  WriteFinalize();
}

static void StoreSimulated()
{
  FILE *fp= ImpFileReadExitOnError(Input.simfn);
  ImpProcessParaverHeader(fp);

  fpos_t hpos; fgetpos(fp, &hpos);

  const size_t numBytes= ImpGetNumBytes(fp);

  const int np= header.numProcs;

  AllocSimBeforeCount();

  fsetpos(fp, &hpos);
  SimCount(fp, numBytes, np);

  AllocSimBeforeRead();

  fsetpos(fp, &hpos);
  SimRead(fp, numBytes, np);

  fclose(fp); fp= NULL;
}
static void ProcessParaver()
{
  FILE *fp= ImpFileReadExitOnError(Input.prvfn);
  ImpProcessParaverHeader(fp);

  const size_t numBytes= ImpGetNumBytes(fp);

  const int np= header.numProcs;

  PrvPlay(fp, numBytes, np);

  fclose(fp); fp= NULL;
}
int main(int argc, char *argv[])
{
  if(argc< 2) {
    printf("Usage: notalk.exe <input-file>\n");
    return 0;
  }

  if(0!= FileInput(argv[1])) {
    return 0;
  }

  StoreSimulated();

  ProcessParaver();

  return 0;
}
