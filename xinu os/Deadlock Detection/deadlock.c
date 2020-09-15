#include <xinu.h>

/**
 * DFS implementation using colors to check for back edges
 * in search tree. Returns index of source vertex upon
 * back edge detection or NOCYCLE of no cycles are detected.
 * Also keeps track of path through graph.
 *
 *  @param  u        root of search tree
 *  @param  color[]  vertex colors
 *  @param  cycle[]  recursive path through graph
 *  @param  depth      level of recursion
 *  @return v        vertex in RAG or NOCYCLE if no cycle exists
**/
int dfs(int u, int color[], int cycle[], int depth) {
      // Root already processed
  if (color[u] == BLACK)
  {
    cycle[depth] = NOCYCLE;
    return NOCYCLE;
  }   // Root already encountered
      // i.e. backedge exists in search tree
  else if (color[u] == GREY)
  {
    cycle[depth] = u;
    //color[u] = BLACK;
    return NOCYCLE;
  }
  // if vertex not processed, recurse
  else if (color[u] == WHITE)
  {
    // mark root as being processed
    color[u] = GREY;
    for (int v = 0; v < V; v++)
    {
      if(RAG[u][v] == 1 && color[v] == WHITE)
      {
        // keep track of recursive path through graph
        if(dfs(v, color, cycle, (depth+1)) != NOCYCLE)
        {
          cycle[depth] = v;
        }
      }
    }
  }
  // mark root as processed
  color[u] = BLACK;
}
/**
 *  Util function that prints the IDs of deadlocked
 *  processes and locks.
 *  @param  cycle[]  cycle in RAG
**/
void print_deadlock(int cycle[]){
  if (cycle[0] == NOCYCLE)
  {
    return;
  }
  printf("\nDEADLOCK\n");
  lid32 lockid;
  for (int i = V-1; i >= 0; i--){
    if (cycle[i] != NOCYCLE) {
      printf("\ncycle[%d]: %d-> ",i, cycle[i]);
      if (cycle[i] >= NLOCK) {
      printf("pid=%d ", (cycle[i] - NLOCK));
      }
      else {
      lockid = cycle[i];
      printf("lockid=%d ", cycle[i]);
      }
    }
  }
  printf("\n");
  deadlock_recover(lockid);
}
/**
 *  Detects deadlocks by detecting cycles in the RAG via DFS.
**/
void deadlock_detect(void){
  int* color = (int*) malloc(V*sizeof(int));
  int* cycle = (int*) malloc(V*sizeof(int));
  // Initialize every vertex as being white (not processed)
  // and not part of a cycle
  for (int v = 0; v < V; v++)
  {
    color[v] = WHITE;
    cycle[v] = NOCYCLE;
  }
  // DFS traversal over RAG processes
  for (int u = 0; u < V; u++)
  {
    // start with a fresh cycle each iteration
    for (int v = 0; v < V; v++)
    {
      cycle[v] = NOCYCLE;
      //color[v] = WHITE;
    }
    dfs(u,color,cycle,0);
    print_deadlock(cycle);
  }
}

void deadlock_recover(lid32 lockid){
  struct lockentry *lptr;
  lptr = &locktab[lockid];
  pid32 victimPid;
  for (int i = NLOCK; i < V; i++){
    if (RAG[lockid][i] == 1){
      victimPid = i;
      kill(victimPid);
      break;
    }
  }

  for (int i = 0; i < NLOCK; i++){
    lptr = &locktab[i];
    struct qentry *entry = lptr->wait_queue->head;
    for (int j = 0; j < lptr->wait_queue->size; j++){
      remove(victimPid, lptr->wait_queue);
      entry = entry->next;
    }
  }
  lptr = &locktab[lockid];
  mutex_unlock(&(lptr->lock));
  rag_dealloc(victimPid, lockid);
  printf("DEADLOCK RECOVER %d %d", victimPid, lockid);
}

void rag_request(int pid, int lockid){
  RAG[NLOCK+pid][lockid] = 1;
}
void rag_alloc(int pid, int lockid){
  RAG[lockid][NLOCK+pid] = 1;
  RAG[NLOCK+pid][lockid] = 0; // new
}
void rag_dealloc(int pid, int lockid){
  RAG[lockid][NLOCK+pid] = 0;
  RAG[NLOCK+pid][lockid] = 0;
}
void rag_print(){
  for (int i = 0; i < V; i++){
    printf("\n");
    for (int j = 0; j < V; j++){
      printf("%d ", RAG[i][j]);
    }
  }
  printf("\n");
}
