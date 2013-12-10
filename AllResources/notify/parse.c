#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

off_t fsize(const char *filename);
int retrieveFile(const char *oldName, const char *newName);

int main() 
{	

	char file_name[25];
	FILE *fp;
	strcpy(file_name, "/root/Desktop/freezelog.txt");

	fp = fopen(file_name, "r");
	
	if (fp==NULL) {
	   perror("Error while opening the log file.\n");
	   exit(0);
	}
	
	
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
			strcpy(root_f, "/root/Desktop/freezer/");
			strcpy(name_f, name);
		
			strcat(root_f, name_f);
			remove(root_f);
			
		}

		else if (!strcmp(event, "MODIFY")) {
		strcpy(root_f, "/root/Desktop/freezer/");
		strcpy(name_f, name);
		
		strcat(root_f, name_f);
		remove(root_f);
		retrieveFile(name, name);
		}
		
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

int retrieveFile (const char *oldName, const char *newName) {


	// create FILE to read orig file and to copy file in freezer director 
	FILE *fp_ice, *fp_freeze;
        
	//manually create paths for freezer and icebox directory
	char icePath[] = "/root/Desktop/.icebox/";	
	char freezePath[] = "/root/Desktop/freezer/";	
	
	//declared for file name 
	char ice_path[100];
	char file_name[100];
	
	//setting up entire path of icebox file (i.e. /root/Desktop/.icebox/test)
	strcpy(ice_path, icePath);
	strcat(ice_path, oldName);
 
	//opens icebox path to copy data
	fp_ice = fopen(ice_path, "r");
	if (fp_ice==NULL) {
	   perror("Error while opening the file.\n");
	   return;
	}

	//copies path to freezer directory (i.e. /root/Desktop/freezer/test)
	strcpy(file_name, freezePath);
	strcat(file_name, newName); 
	
	// should create file with new file name 
	fp_freeze = fopen(file_name, "a+");
	
	if (fp_freeze==NULL) {
	   perror("Error while opening the file.\n");
	   return;
	}
	
	// gets size of 
	off_t size = fsize(icePath);
	char string[size];

	// takes in line from icebox file and copies into freezer directory
	 while (fgets(string, sizeof(string),fp_ice)) {
		fprintf(fp_freeze,"%s",string);

	} 
	
	fclose(fp_ice);
	fclose(fp_freeze);
	return 0;
		
} 
