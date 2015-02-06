
#include "oscl_file_cfg.h"

#include <utils/Log.h>

#undef LOG_TAG
#define LOG_TAG "FILE_CFG"

#define LOGP(...)\
{\
    if (ret == 1)\
    {\
        LOGD(__VA_ARGS__);\
    }\
    else\
    {\
        LOGV(__VA_ARGS__);\
    }\
}

OSCL_EXPORT_REF int Oscl_File_Cfg::LoadCfg(CFG_TYPE aType, char* aCfgItem)
{
    static bool bFirstLoad[CFG_MAX];
    
    int ret = 0;

    char* _szCfgName = NULL;
    switch (aType)
    {
    case CFG_PLAYER:
        _szCfgName = (char*)PLAYER_CFG_FILE_NAME;
        break;

    case CFG_AUTHOR:
        break;

    case CFG_2WAY:
        break;
    }

    if (bFirstLoad[aType] == false)
    {
        if (_szCfgName == NULL)
        {
            LOGE("warning: type %d no identify configure file - not support configure", aType);
            bFirstLoad[aType] = true;
            return ret;
        }
    }
    
	char* _szPathList[] = { (char*)"/",
                            (char*)"/sdcard", 
                            (char*)"/etc"};
	char _szFilePath[1024];
	
	uint32 _listnum = sizeof(_szPathList) / sizeof(char*);

	FILE *pFile = NULL;
	char _string [100];

	for (uint32 i = 0; i < _listnum; i++)
	{
		memset(_szFilePath, 0, 1024);
		sprintf(_szFilePath, "%s/%s", _szPathList[i], _szCfgName);
		pFile = fopen(_szFilePath, "r+");
		if (pFile != NULL)
		{
			break;
		}
	}

    if (pFile == NULL)
	{
        if (bFirstLoad[aType] == false)
        {
            bFirstLoad[aType] = true;
        	        
            LOGD("!!!!!**************************************************!!!!!");
    		LOGD("!!!!! configure file %s not found - use default configure", _szCfgName);
            LOGD("!!!!!**************************************************!!!!!");
        }

        return ret;
	}
	else
	{
        if (bFirstLoad[aType] == false)
        {
            bFirstLoad[aType] = true;
        	        
            LOGD("!!!!!**************************************************!!!!!");
    		LOGD("!!!!! load configure file %s", _szFilePath);
            LOGD("!!!!!**************************************************!!!!!");
        }        
	}

	char* item = NULL;
	char* value = NULL;
	while(fgets(_string, 100, pFile) != NULL)
	{
		item = strtok(_string, "=");
        if (item == NULL)
        {
            break;
        }
		if (strcmp(item, aCfgItem) == (int32)NULL)
		{
			value = strtok(NULL, "\r\n");
			break;
		}
	}

    fclose(pFile);
    
	if (value != NULL)
	{
		ret = atoi(value);
	}

    LOGP("***** configure: %s: %s *****", aCfgItem, ret > 0? "on":"off");    
    
    return ret;
}

