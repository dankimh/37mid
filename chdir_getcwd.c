#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#define PATH_MAX 1024

int main(int argc, char *argv[]){
	char path[PATH_MAX+1];

	if(argc==1)exit(-1);

	if(argc==2){
		if(getcwd(path,PATH_MAX)==NULL)perror("error getcwd");
		else printf("Current working directory changed to %s \n",path);

		if(chdir(argv[1])<0)perror("error chdir");
		else{
			if(getcwd(path,PATH_MAX)==NULL)perror("error getcwd");
			else printf("Current working directory changed to %s\n",path);
		}
	}
	else perror("Too many arguments");
}
