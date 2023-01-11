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
	pid_t pid[5];
	int waitValue; 
	int childValueInfo;
	int commandResult; 
	string apiLink = "https://api.open-meteo.com/v1/"; 
 	

	file.open("input.txt"); 

	if (file.is_open()){
		string line;
		int i = 0; 

		while (getline(file, line)){
		
			pid[i]  = fork();

			if (pid[i] < 0){
				perror("Error in making fork"); 
				exit(1); 
			}
 
			if (pid[i] == 0){
				istringstream stream(line); 
				string lat; 
				string lng;
				string file_name = "file"; 				
				string file_num = to_string(i + 1); 
					
				stream >> lat >> lng; 
				file_name = file_name + file_num + ".json";
				apiLink = apiLink + "forecast?latitude=" + lat + "&longitude=" + lng + "&current_weather=True"; 	
				
				commandResult = execlp("/usr/bin/curl", "curl", "-o", file_name.c_str(), apiLink.c_str(), NULL);  
			
				if (commandResult < 0){
					perror("command error");
					exit(1);  
				}	
				 				
				break; 			
			}  

			i++; 
		}

		pid_t pid_return; 
		int status; 
		while ((pid_return = wait(&status)) > 0){
				cout << pid_return << endl; 
				cout << "Status: " << status << endl;
				--i;
 
	   }
	}
} 
