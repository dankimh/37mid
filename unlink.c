#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>

int main(){
	int fd,len;
	char buf[20];
	fd=open("tempfile",O_RDWR|O_CREAT|O_TRUNC, 0666);
	if(fd==-1)perror("open1");
	close(fd);

	system("ls -l");

	unlink("tempfile");
	system("ls -l");

	return 0;
}
