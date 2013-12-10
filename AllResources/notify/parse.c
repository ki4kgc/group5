#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

off_t fsize(const char *filename);
int renameFile(const char *oldName);

int main() 
{	

	char file_name[25];
	FILE *fp;
	strcpy(file_name, "freezelog.txt");

	printf("Getting freezelog.txt...\n");

	fp = fopen(file_name, "r");
	
	if (fp==NULL) {
	   perror("Error while opening the file.\n");
	   exit(0);
	}
	
	printf("The contents of %s file are\n", file_name);
	
	off_t size = fsize(file_name);
	char string[size];
	int count = 0;
	
	char *event;
	char *name;
	char *wd;

	char root_f[200];
	char name_f[200];

	while (fgets(string, sizeof(string),fp)) {
		event = strtok(string, ":");
		name = strtok(NULL, ":");
		wd = strtok(NULL, ":");
	
		if (!strcmp(event, "CREATE")){ 
			if (strstr(name, ".~") != NULL) {
			renameFile(name);
			strcpy(root_f, "/root/Desktop/freezer/");
			strcpy(name_f, name);
			
			strcat(root_f, name_f);
			remove(root_f);
			
			}
			else {
			strcpy(root_f, "/root/Desktop/freezer/");
			strcpy(name_f, name);
		
			strcat(root_f, name_f);
			remove(root_f);
			}
		}

		else if (!strcmp(event, "MODIFY")) {
		strcpy(root_f, "/root/Desktop/freezer/");
		strcpy(name_f, name);
		
		strcat(root_f, name_f);
		remove(root_f);
		}
		printf("%s is event, %s is file, %s is id\n", event,name,wd);
	}	
	
	fclose(fp); 
	return 0;

}

off_t fsize(const char *filename) {
	struct stat st;
	
	if (stat(filename, &st) == 0)
		return st.st_size;
		return -1;
}

int renameFile (const char* oldName) {

	//Create copy of old file name (i.e. .~test) without path
	char cpyName[64];
	strcpy(cpyName, oldName);

	// create FILE to read old file and to create new file 
	FILE *fp_old, *fp_new;
        
	//manually create paths for old file and new file
	char oldPath[] = "/root/Desktop/freezer/";
	char newPath[] = "/root/Desktop/freezer/";
	
	//declared for new file name 
	char file_name[100];
	
	//setting up entire path of old file (i.e. /root/Desktop/~.test)
	 strcat(oldPath, cpyName); 
	
	
	//opens oldPath to copy data
	fp_old = fopen(oldPath, "r");
	if (fp_old==NULL) {
	   perror("Error while opening the file.\n");
	   return;
	}

	
	// Used to parse through old file name to take out .~
	char *s;
	s = strtok(cpyName, "~");	

	//copies path to file then concatenates new file name (i.e. /root/Desktop/test)
	strcpy(file_name, newPath); 
	
		
	strcat(file_name, s);
	
	
	// should create file with new file name 
	fp_new = fopen(file_name, "a+");
	
	if (fp_new==NULL) {
	   perror("Error while opening the file.\n");
	   return;
	}
	
	// gets size of 
	off_t size = fsize(oldPath);
	char string[size];

	printf("old file: %s, new path: %s\n", oldPath, file_name);

	// takes in line from old file and copies into new file
	 while (fgets(string, sizeof(string),fp_old)) {
		fprintf(fp_new,"%s",string);

	} 
	
	fclose(fp_old);
	fclose(fp_new);
	return 0;
		
} 

