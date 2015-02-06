


#include "test_case_file_io.h"
#include <stdlib.h>

#include "pvfile.h"
#include "oscl_tickcount.h"

#include <utils/Log.h>
#undef  LOG_TAG
#define LOG_TAG "oscl_file_io_test"

#define SHOW_PROFILE 1
#define READ_SIZE 102400

class file_io_test : public test_case
{
    virtual void test(void)
    {
        
        Oscl_File pvReadFile, pvWriteFile;
        Oscl_FileServer iFs1, iFs2;
        const oscl_wchar* szInputFileName = _STRLIT_WCHAR("/sdcard/test/penguinD1_720x480.avi");
        const oscl_wchar* szOutputFileName = _STRLIT_WCHAR("/sdcard/test/output");

        // start to test
        uint32 starttickcount, endtickcount;
        LOGD("Start to test, tickcount = %d", OsclTickCount::TickCount(true));
        
        // open file
        starttickcount = OsclTickCount::TickCount();
        if (pvReadFile.Open(szInputFileName, 
            (uint32)(Oscl_File::MODE_READ | Oscl_File::MODE_BINARY), iFs1) != 0)
        {
            LOGE("file open read file %s error", szInputFileName);
            return;
        }
        endtickcount = OsclTickCount::TickCount();
        
        LOGD("file open read file %s successfully, tickcount: %d", szInputFileName, endtickcount - starttickcount);

        if (pvWriteFile.Open(szOutputFileName, 
            (uint32)(Oscl_File::MODE_READWRITE | Oscl_File::MODE_BINARY), iFs2) != 0)
        {
            LOGE("file open read file %s error", szOutputFileName);
            return;
        }
        LOGD("file open read file %s successfully", szOutputFileName);
        
        // calculate file size
        uint32 filesize = 0;
        pvReadFile.Seek(0, Oscl_File::SEEKEND);
        filesize = pvReadFile.Tell();
        pvReadFile.Seek(0, Oscl_File::SEEKSET);
        LOGD("input file size is %d", filesize);        
        
        uint32 readsize, readsize_total = 0;
        uint32 writesize, writesize_total = 0;
        uint32 readtime, writetime;
        char buf[READ_SIZE];

        int count = 0;
        while (readsize_total < filesize)
        {
            starttickcount = OsclTickCount::TickCount();
            readsize = pvReadFile.Read(buf, 1, READ_SIZE);
            endtickcount = OsclTickCount::TickCount();
            readtime = endtickcount - starttickcount;
            
            readsize_total += readsize;
            if (readsize == 0)
            {
                break;
            }            

            starttickcount = OsclTickCount::TickCount();
            writesize = pvWriteFile.Write(buf, 1, READ_SIZE);
            endtickcount = OsclTickCount::TickCount();
            writetime = endtickcount - starttickcount;

            writesize_total += writesize;

            LOGD("file io profiling: read size: %d, read time: %d ms, wirte size: %d, write time: %d", 
                readsize, readtime, writesize, writetime);
            if (count / 1000 == 0)
            {
                //LOGD("total spend tickcount: %d", OsclTickCount::TickCount());
                count ++;
            }
        }

        LOGD("!!!!! test end !!!!!");
    }
};


file_io_test_suite::file_io_test_suite()
{
    adopt_test_case(new file_io_test);
}

