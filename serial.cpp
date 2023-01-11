#include <iostream> 
#include <fstream> 
#include <string> 
#include <sys/types.h> 
#include <sys/wait.h>
#include <unistd.h> 
#include <sstream> 

using namespace std; 

int main(){ 
	ifstream file; 
	pid_t pid; 
	string apiLink = "https://api.open-meteo.com/v1/"; 
	int waitValue; 
	int status; 
	int commandResult; 

	file.open("input.txt"); 

	if (file.is_open()){
		string line; 
		int i = 0; 
	
		while (getline(file, line)){
			pid = fork(); 

			if (pid < 0){
				perror("Error in forking"); 
				exit(1); 
			}

			if (pid == 0){
				cout << "Child PID: " << getpid() << endl; 
				cout << "Child UID: " << getuid() << endl; 
				cout << "Child GID: " << getgid() << endl; 				
	
				istringstream stream(line); 
				string lat, lng; 
				string file_name = "file"; 
				string file_num = to_string(i + 1); 

				stream >> lat >> lng; 
				file_name = file_name + file_num + ".json"; 
				apiLink = apiLink + "forecast?latitude=" + lat + "&longitude=" + lng + "&current_weather=True"; 
				
				commandResult = execlp("/usr/bin/curl", "curl", "-o", file_name.c_str(), apiLink.c_str(), NULL); 

				if (commandResult < 0){
					perror("Command error"); 
					exit(1); 
				}
				

			} else {
				waitValue = wait(&status);
 
				if (waitValue < 0){
					perror("Error with wait value"); 
					exit(1); 
				} 
				
				cout << "Parent PID: " << getppid() << endl;
				cout << "Parent UID: " << getuid() << endl; 
				cout << "Parent GID: " << getgid() << endl;  
			} 
			
			i++; 
		} 
	}
} 
