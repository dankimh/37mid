#include <sys/types.h> 
#include <sys/mkdev.h> 
#include <sys/stat.h> 
#include <fcntl.h>
#include <dirent.h> 
#include <unistd.h> 
#include <time.h> 
#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <sys/sysmacros.h>
#include <limits.h>

#define MAX_PATH 1024
#define MAX_DESC 1024
#define DIRECTION_NUM 8
#define bzero(x,y) memset(x,0,y)
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

char conf_path[]="/home/user/37mid/listfiles/myls.conf";


/* typeOfFile - return the letter indicating the file type. */ 
char typeOfFile(mode_t mode) 
{ 
	switch (mode & S_IFMT) { 
 		case S_IFREG: return('-'); 
		case S_IFDIR: return('d'); 
		case S_IFCHR: return('c'); 
		case S_IFBLK: return('b'); 
		case S_IFLNK: return('l'); 
		case S_IFIFO: return('p'); 
		case S_IFSOCK: return('s'); 
 	} 
 	return('?'); 
}

/* permOfFile - return the file permissions in an "ls"-like string. */ 
char* permOfFile(mode_t mode) { 
 	int i; 
 	char *p; 
 	static char perms[10]; 
 	p = perms; 
 	strcpy(perms, "---------"); 
 	for (i=0; i < 3; i++) { 
 		if (mode & (S_IREAD >> i*3)) *p = 'r'; 
 		p++; 
 		if (mode & (S_IWRITE >> i*3)) *p = 'w'; 
 		p++; 
 		if (mode & (S_IEXEC >> i*3)) *p = 'x'; 
 		p++; 
 	} 
 	if ((mode & S_ISUID) != 0) perms[2] = 's'; 
 	if ((mode & S_ISGID) != 0) perms[5] = 's'; 
 	return(perms); 
}

/* outputStatInfo - print out the contents of the stat structure. */ 
void outputStatInfo(char *pathname, char *filename, struct stat *st) { 
 	int n; 
 	char slink[BUFSIZ+1]; 
 	printf("%5d ", st->st_blocks); 
 	printf("%c%s ", typeOfFile(st->st_mode), permOfFile(st->st_mode)); 
 	printf("%3d ", st->st_nlink); 
 	printf("%5d/%-5d ", st->st_uid, st->st_gid); 
 
 	if (((st->st_mode & S_IFMT)!=S_IFCHR) && ((st->st_mode & S_IFMT)!=S_IFBLK)) 
 		printf("%9d ", st->st_size); 
 	else 
 		printf("%4d,%4d ", major(st->st_rdev), minor(st->st_rdev)); 
 
 	printf("%.12s ", ctime(&st->st_mtime) + 4); 
 	printf("%s", filename); 
 	if ((st->st_mode & S_IFMT) == S_IFLNK) { 
 		if ((n = readlink(pathname, slink, sizeof(slink))) < 0) 
 			printf(" -> ???"); 
 		else printf(" -> %.*s", n, slink); 
 	} 
}

/* casez - function of each directions. */
void case1(char *dirname,DIR *dp){
	char filename[BUFSIZ+1];
	struct stat st;
	struct dirent *d;
	int ch=1;
	while((d=readdir(dp))!=NULL){
		sprintf(filename, "%s/%s", dirname, d->d_name);
		if(lstat(filename,&st)<0)perror(filename);
		if((st.st_mode&S_ISUID)!=0){
			if(ch==1){printf("warning! setuid executable file found!\n");ch=0;}
			outputStatInfo(filename,d->d_name,&st);
			putchar('\n');
		}
	}
}

void case2(char *dirname,DIR *dp){
	char filename[BUFSIZ+1];
	struct stat st;
	struct dirent *d;
	int ch=1;
	while((d=readdir(dp))!=NULL){
		sprintf(filename, "%s/%s", dirname, d->d_name);
		if(lstat(filename,&st)<0)perror(filename);
		if(!(d->d_name[0]=='.'&&strlen(d->d_name)==1)&&!(d->d_name[0]=='.'&&d->d_name[1]=='.'&&strlen(d->d_name)==2)&&(d->d_name[0]=='.')){
			if(ch==1){printf("warning! .???? directory found! it's suspicious, needs manual check!\n");ch=0;}
			outputStatInfo(filename,d->d_name,&st);
			putchar('\n');
		}
	}
}
void case3(char *dirname,DIR *dp){printf("f3\n");}
void case4(char *dirname,DIR *dp){printf("f4\n");}
void case5(char *dirname,DIR *dp){printf("f5\n");}
void case6(char *dirname,DIR *dp){printf("f6\n");}
void case7(char *dirname,DIR *dp){printf("f7\n");}
void case8(char *dirname,DIR *dp){printf("f8\n");}


int main(int argc, char **argv) { 
 	DIR *dp; 
 	char *dirname; 
 	char *realdir;
 	int f;
 	struct stat st; 
 	struct dirent *d; 
 	char filename[BUFSIZ+1]; 
 	void (*fp[DIRECTION_NUM])(char*,DIR*)={case1,case2,case3,case4,case5,case6,case7,case8};
 	
 	/* For each directory on the command line... */ 
 	if(argc==2){
 		while (--argc) { 
 			dirname = *++argv; 
 			if ((dp = opendir(dirname)) == NULL) /* Open the directory */ 
 				perror(dirname); 
 			printf("%s:\n", dirname); 
 			while ((d = readdir(dp)) != NULL) { /* For each file in the directory... */ 
 				/* Create the full file name. */ 
 				sprintf(filename, "%s/%s", dirname, d->d_name); 
 				//printf("%s\n",filename);
 				if (lstat(filename, &st) < 0) /* Find out about it. */ 
 					perror(filename); 
 				outputStatInfo(filename, d->d_name, &st); /* Print out the info. */ 
 				putchar('\n'); 
 			} 
 			putchar('\n'); 
 			closedir(dp); 
 		}
 	}
 	
 	else if(argc==3){
 		if(!strcmp(*++argv,"-S")){
 			dirname=*++argv;
 			if((dp=opendir(dirname))==NULL)
 				perror(dirname);
 			realdir=realpath(dirname,realdir);
 			realdir[strlen(realdir)]='\0';
 			//printf("%s:\n",realdir);
 			if((f=open(conf_path,O_RDONLY))>=0){
 				++argv;
 				int i=0,ch=0;
 				char c;
 				while(read(f,&c,1)>0)if(c=='!')break;
 				lseek(f,sizeof(char),SEEK_CUR);
 				
 				char path[MAX_PATH];
 				char desc[MAX_DESC];
 				while(read(f,&c,1)>0){
 					if(c=='\n'){
 						desc[i] ='\0';
 						ch=0;
 						i=0;
 						
 						//printf("%s %s\n",path,desc);
 						if(!strncmp(realdir,path,MAX(strlen(realdir),strlen(path)))||(path[0]=='*'&&strlen(path)==1)){
 							int op=atoi(desc);
 							//printf("op%d\n",op);
 							fp[op-1](realdir,dp);
 						}
 						bzero(path,MAX_PATH);
 						bzero(desc,MAX_DESC);
 						
 					}
 					else if(c=='\t'){
 						path[i]='\0';
 						ch=1;
 						i=0;
 						
 					}
 					else if(ch==0){
						path[i]=c;
						i++;
					}
					else if(ch==1){
						desc[i]=c;
						i++;
					}
					
 				}
 				//buf[i]=='\0';
 				//printf("a%s\n",buf);
 			}
 			close(f);
 		}
 		else perror("wrong option");
 	}
 	
// 	printf("%dff",f);
 	
 	//char buf[1024];
 	//if(realpath("/home/user/37mid",buf)==NULL)return 1;
 	//printf("%s",buf);
 	return 0; 
}
