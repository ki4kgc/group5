#include <string.h> /*May not want this*/
#include <stdio.h>
#include <stdlib.h>
#include <linux/inotify.h>
 
#define MAX_EVENTS 1024 /*Max. number of events to process at one go*/
#define LEN_NAME 16 /*Assuming that the length of the filename won't exceed 16 bytes*/
#define EVENT_SIZE  ( sizeof (struct inotify_event) ) /*size of one event*/
#define BUF_LEN     ( MAX_EVENTS * ( EVENT_SIZE + LEN_NAME )) /*buffer to store the data of events*/

void createBackup(char path[], char target[]);

 
int main(int argc, char **argv)
{
	int length, i = 0, wd;
  	int fd;
  	char buffer[BUF_LEN];
	
	FILE *file;
	file = fopen("/root/Desktop/freezelog.txt", "a+"); /*If log does not exsist, it is created*/
 
  	/* Initialize Inotify*/
  	fd = inotify_init();
  	if ( fd < 0 ) {
    		perror( "Couldn't initialize inotify");
  	}
 
  	/* add watch to starting directory */
  	wd = inotify_add_watch(fd, "/root/Desktop/freezer", IN_CREATE | IN_MODIFY | IN_DELETE);
 
  	if (wd == -1) {
      		printf("Could not watch freezer folder\n");
    	}
  	else {
      		printf("Watching freezer folder\n");
    	}
 
  	/* do it forever*/
  	while(1)
    	{
      		i = 0;
      		length = read(fd, buffer, BUF_LEN ); 
		//char freezeFolder[] = "/root/Desktop/freezer/";
 
      		if (length < 0) {
        		perror("read");
      		} 
 
      		while (i < length) {
        		struct inotify_event *event = (struct inotify_event *) &buffer[ i ];
        		if (event->len) {
          			if (event->mask & IN_CREATE) {
            				if (event->mask & IN_ISDIR){
              					printf("The directory %s was Created.\n", event->name );  
						fprintf(file, "DIRCREATE:%s\n", event->name );     
					}
            				else{
              					printf("The file %s was Created with WD %d\n", event->name, event->wd );
	      					fprintf(file, "CREATE:%s:%d\n", event->name, event->wd );
						/*createBackup(freezeFolder, event->name);*/

					}
          			}
           
          			if (event->mask & IN_MODIFY) {
            				if (event->mask & IN_ISDIR){
              					printf("The directory %s was modified.\n", event->name ); 
						fprintf(file, "DIRMODIFY:%s\n", event->name );     
					}
	            			else {
	              				printf("The file %s was modified with WD %d\n", event->name, event->wd );   
						fprintf(file, "MODIFY:%s:%d\n", event->name, event->wd );
					} 
          			}
           
	          		if (event->mask & IN_DELETE) {
	            			if (event->mask & IN_ISDIR){
						printf("The mask is:%d\n", event->mask);
	              				printf("The directory %s was deleted.\n", event->name );    
						fprintf(file, "DIRDELETE:%s\n", event->name );   
					}  
	            			else{
	              				printf("The file %s was deleted with WD %d\n", event->name, event->wd );   
						fprintf(file,"DELETE:%s:%d\n", event->name, event->wd );     
					}
	          		} 
			/*
				if (event->mask & IN_OPEN) {
	            			if (event->mask & IN_ISDIR){
						printf("The mask is:%d\n", event->mask);
	              				printf("The directory %s was OPENED.\n", event->name );    
						fprintf(file, "OPENED:%s\n", event->name );   
					}  
	            			else{
	              				printf("The file %s was OPENED with WD %d\n", event->name, event->wd );   
						fprintf(file,"OPENED:%s:%d\n", event->name, event->wd );     
					}
	          		} 
			*/

	 			fflush(file);
	 
	          		i += EVENT_SIZE + event->len;
	        	}
		}
	}
 
  	/* Clean up*/
  	inotify_rm_watch( fd, wd );
  	close( fd );
  	fclose(file);
  	getchar();
  	return 0;
}
/* Changed approach to 
void createBackup(char path[], char target[]){
	char orig[100];
	char bkup[100];
	char bkupfile[100] = ".~";
	strcpy(orig, path);
	strcat(orig, target);
	
	strcat(bkupfile, target);

	strcpy(bkup, path);
	strcat(bkup, bkupfile);

	// bkup contains the fully qualified file location
	// bkupfile contains only the ".~file
	 
	
	if ((fopen(bkup, "r"))==NULL){ //Look for backup file, not actuall file 
		printf("Backup does not exsist\n");
		// If backup file does not exist, make it 
	}
	else{
		printf("Backup does exsist\n");
	}

printf("Path is :%s\n", path);
	
}
*/
