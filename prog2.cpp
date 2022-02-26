#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/stat.h>

using namespace std;

int main(int argc, char *argv[]) {
  // Check command line arguments
  if (argc != 2) {
    cerr << "Usage: prog2 url" << endl;
    return(1);
  }

  string command;
  FILE *pd, *fd;
  char buf[1000];
  int result_code;

  // Check result file exists
  if (access("data.tmp", F_OK) == 0) {
    unlink("data.tmp");
  }

  // Can't remove file
  if (access("data.tmp", F_OK) == 0) {
    cerr << "Can't remove result file" << endl;
    return(2);
  }

  // Run process
  command = "wget -O data.tmp " + string(argv[1]) + string(" >/dev/null 2>/dev/null");
  system(command.c_str());
  cout << command << endl;

  // Get information about result file
  struct stat fi;
  if (stat("data.tmp",&fi) == -1) {
    cerr << "Can't open result file" << endl;
    return(2);
  }

  // Check file size
  if (fi.st_size == 0) {
    cerr << "Unknown error" << endl;
    return(2);
  }

  // Read data
  if ((fd = fopen("data.tmp", "r")) == NULL) {
    cerr << "Can't open result file" << endl;
    return(2);
  }

  // Read result and put it to stdout
  while (!feof(fd)) {
    fgets(buf, 1000, fd);
    cout << buf;
  }
  fclose(fd);

  // Delete result file
  unlink("data.tmp");

  return(0);
}
