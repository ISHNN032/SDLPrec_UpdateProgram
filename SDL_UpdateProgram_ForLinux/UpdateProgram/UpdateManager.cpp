#include "UpdateManager.h"
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>

UpdateManager::UpdateManager(){
	
}

UpdateManager::~UpdateManager(){

}

bool UpdateManager::updateFW(){
	char cmd[100];
	char ver_info_cur[50] = {0,};
	char ver_info_update[50] = {0,};
	int execute_update = 0;

	struct dirent **items;
	int nitems, i, j;

	memset(cmd, 0, 100);
	sprintf(cmd, "mkdir %s", update_path);
	system(cmd);

	usleep(100000);
	
    if(exist_dir(update_path)) {
		printf("Failed to create %s directory!!!\n", update_path);
		return false;
	}

	memset(cmd, 0, 100);
	sprintf(cmd, "cp %s %s", update_file_img, update_file);
	system(cmd);

	memset(cmd, 0, 100);
	sprintf(cmd, "unzip %s -d %s", update_file, update_path);
	system(cmd);

	sleep(1);

	if(exist_file(update_file)) {
		return false;
	}

	/*
	if ( check_ver_file("/root", ver_info_cur) ) { 			
		if ( check_ver_file("/tmp/fw/root", ver_info_update) ) {
			if( strncmp(ver_info_cur, ver_info_update, strlen(ver_info_update)) != 0 ) {
				execute_update = 1;
			}
			else {
				std::cout<<"UpdateManager : Version is Latest. No need to Update."<<std::endl;
			}
		}
	}
	else
		execute_update = 1;
		
	if(!execute_update) 
		return 0;
	*/

	nitems = scandir(update_path, &items, NULL, alphasort);

	for(i=0;i<nitems;i++) {
		if(!strcmp(items[i]->d_name, ".") || !strcmp(items[i]->d_name, ".."))
			continue;

		sleep(1);

		if(!strcmp(items[i]->d_name, "uboot")) { 

		}
		else if(!strcmp(items[i]->d_name, "boot.img")) {
			memset(cmd, 0, 100);
			sprintf(cmd, "dd if=%s/%s of=/dev/mmcblk0p6 bs=2048", update_path, items[i]->d_name);
			printf("execute update kernel cmd : %s\n", cmd);
			system(cmd);
		}
		else {
			/* Remove under "/root/" directory. */
			memset(cmd, 0, 100);
			sprintf(cmd, "rm -rf /root/*");
			printf("execute update dir cmd : %s\n", cmd);
			system(cmd);
			usleep(1000);

			/* Copying newer files. */
			memset(cmd, 0, 100);
			sprintf(cmd, "cp -a %s/%s/* /%s", update_path, items[i]->d_name, items[i]->d_name);
			printf("execute update dir cmd : %s\n", cmd);
			system(cmd);

			memset(cmd, 0, 100);
			sprintf(cmd, "chmod -R 755 /%s/*", items[i]->d_name);
			system(cmd);
		}		
	}

	//memset(cmd, 0, 100);
	//sprintf(cmd, "rm -rf %s", update_file_img);
	//system(cmd);

	std::cout<<"UpdateManager : Update Complited."<<std::endl;

	sleep(10);

	system("reboot");

    return true;
}

bool UpdateManager::exist_dir(char *dir)
{
	DIR *dp  = opendir(dir);

	if(dp == NULL)
	{
		return -1;
	}
	else
	{
		closedir(dp);
		return 0;
	}

}

bool UpdateManager::exist_file(char* filename){
	int fd = open(filename,O_RDONLY);
	if(fd != -1) {
		close(fd);
		return 0;
	}
	else
		return -1;
}

bool UpdateManager::check_ver_file(char* path, char* file_name)
{	
	bool ret = true;
	struct dirent **items = NULL;
	char *f_name;
	int FileIdx = 0;
	char tmp[50] = {0,};
	int nitems = scandir(path, &items, NULL, alphasort);
	while(FileIdx < nitems) {
		sprintf(tmp, "%s", items[FileIdx]->d_name);
		f_name = strtok(tmp, ".");
		if((f_name != NULL) && (strcmp(f_name, "SYS") == 0)) {
			sprintf(file_name, "%s", items[FileIdx]->d_name);
			ret = false;
			break;
		}
		FileIdx++;
	}
	return ret;	
}