#include <iostream>
#include <string>
#include <map>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include "json.hpp"

using namespace std;
using namespace nlohmann;

int main(int argc, char *argv[]) {
  // Check command line arguments
  if (argc != 2) {
    cout << "Usage: prog1 json" << endl;
    return(1);
  }

  // Read json
  json j;
  try {
    j = json::parse(argv[1]);
  }
  catch (...) {
    cout << "JSON parse error!" << endl;
    return(2);
  }

  // Deserialize
  map <string, string> data;
  for (json::iterator it = j.begin(); it != j.end(); ++it) {
    data[it.key()] = it.value();
  }

  // Find needed keys and their values
  map <string, string> :: iterator it2;
  string progname, url;

  // Find parameter "progname"
  it2 = data.find("progname");
  if (it2 == data.end()) {
      cout << "No key \"progname\" found" << endl;
      return(3);
  }
  else {
    progname = it2->second;
  }

  // Find parameter "url"
  it2 = data.find("url");
  if (it2 == data.end()) {
      cout << "No key \"url\" found, default value used" << endl;
      url = "https://www.ya.ru";
  }
  else {
    url = it2->second;
  }

  // Check program exist
  if (access(progname.c_str(), X_OK) != 0) {
    cout << "Program not found" << endl;
    return(4);
  }

  // Run
  string command, result;
  FILE *pd;
  char buf[1000];
  int result_code;
  bool firstString = true;
  command = progname + " " + url;
  pd = popen(command.c_str(), "r");
  if (pd == NULL) {
    cout << "Program not started" << endl;
    return(5);
  }

  // Read output lines
  while (!feof(pd)) {
    if (fgets(buf,1000,pd)) {
      // Ignore first line (this is call system(...))
      if (firstString) {
        firstString = false;
      }
      else {
        result=result + string(buf);
      }
    }
  }

  // Get exit code
  result_code = WEXITSTATUS(pclose(pd));

  // Make result
  json j2;
  j2["progname"] = progname;
  if (result_code == 0) {
    j2["content"] = result;
  }
  else {
    j2["content"] = "";
    if (result == "") {
      j2["error"] = "No error description, only error code #"
        + to_string(result_code) + " was reported";
    }
    else {
      j2["error"] = result;
    }
  }
  cout << j2.dump() << endl;

  return(0);
}
