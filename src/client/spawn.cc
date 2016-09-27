#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <string>

#include "logger.hpp" 


class Job {

public:
  #define READ 0
  #define WRITE 1

  Job(const std::string c, const std::vector<std::string> a)
  { 
    args = (char **)calloc(a.size() + 2, sizeof(char *));
    
    args[0] = strdup(c.c_str());

    for (unsigned int i = 0; i < a.size(); ++i) {
      args[i+1] = strdup(a[i].c_str());
    }

    env = (char **)calloc(1, sizeof(char *));
    env[0] = NULL;

    msg = NULL;
  }
    
    
  void createChild()
  {
    int std_in[2];
    int std_out[2];
    pid_t pid;
    char c;
    int rc;
    
    if (pipe(std_in) < 0) {
      perror("allocating pipe for child input redirect");
      return;
    }
    if (pipe(std_out) < 0) {
      close(std_in[READ]);
      close(std_in[WRITE]);
      perror("allocating pipe for child output redirect");
      return;
    }
    
    pid = fork();
    if (pid == 0) {
      // child continues here
      
      // redirect stdin
      if (dup2(std_in[READ], STDIN_FILENO) == -1) {
	perror("redirecting stdin");
	return;
      }
      
      // redirect stdout
      if (dup2(std_out[WRITE], STDOUT_FILENO) == -1) {
	perror("redirecting stdout");
	return;
      }
      
      // redirect stderr
      if (dup2(std_out[WRITE], STDERR_FILENO) == -1) {
	perror("redirecting stderr");
	return;
      }
      
      // all these are for use by parent only
      close(std_in[READ]);
      close(std_in[WRITE]);
      close(std_out[READ]);
      close(std_out[WRITE]);
     
      // run child process image
      // replace this with any exec* function find easier to use ("man exec")
      rc = execve(args[0], args, env);
      
      // if we get here at all, an error occurred, but we are in the child
      // process, so just exit
      perror("exec of the child process");
      exit(rc);
    } else if (pid > 0) {
      // parent continues here
      
      // close unused file descriptors, these are for child only
      close(std_in[READ]);
      close(std_out[WRITE]);
      
      if (NULL != msg) {
	write(std_in[WRITE], msg, strlen(msg));
      }
      
      // Just a char by char read here, you can change it accordingly
      while (read(std_out[READ], &c, 1) == 1) {
	write(STDOUT_FILENO, &c, 1);
      }
      
      // done with these in this example program, you would normally keep these
      // open of course as long as you want to talk to the child
      close(std_in[WRITE]);
      close(std_out[READ]);
    } else {
      // failed to create child
      close(std_in[READ]);
      close(std_in[WRITE]);
      close(std_out[READ]);
      close(std_out[WRITE]);
    }
  }
  
  
  
private:
  char** args;
  char** env;
  char*  msg;
  
};





int main()
{
  Logger log("spawn.log");

  std::string cmd = "a.out";
  log << INFO << "starting Job. cmd = " << cmd << std::endl;
  Job a = Job(cmd, std::vector<std::string>());
  a.createChild();
  log << INFO << "end of job" << std::endl;
    
  return 0;
}
