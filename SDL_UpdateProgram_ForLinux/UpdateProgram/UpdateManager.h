#ifndef _UPDATEMANAGER_
#define _UPDATEMANAGER_

#define MOUNT_DEV_EXIST          (0)
#define MOUNT_DEV_NOT_EXIST      (-1)

#define MOUNT_POINT_EXIST        (0)
#define MOUNT_POINT_NOT_EXIST    (-1)

#define DISK_MOUNTED               (0)
#define DISK_UNMOUNTED             (-1)

#define DIR_EXIST                (0)
#define DIR_NOT_EXIST            (-1)

static char *update_file_img = "/mnt/sd/KLAK_Update.img";
static char *update_file 	 = "/tmp/KLAK_Update.zip";
static char *update_path 	 = "/tmp/fw";

static char *version_info_current = "/root/version.txt";
static char *version_info_update  = "/tmp/fw/version.txt";

static const char 	*bootlogo_file	= "/root/bootlogo.bmp";
static char 		*execute_nvs	= "/root/execute_Tia.txt";
static char 		*wlan0_file		= "/sys/class/net/wlan0";

#pragma once
class UpdateManager
{
public:
	UpdateManager();
    bool updateFW();
	~UpdateManager();
private:
	bool exist_dir(char *dir);
	bool exist_file(char*);
	bool check_ver_file(char*, char*);
};

#endif //_UPDATEMANAGER_