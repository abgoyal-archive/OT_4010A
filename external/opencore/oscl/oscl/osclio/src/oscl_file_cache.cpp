

#include "oscl_base.h"
#include "oscl_file_cache.h"
#include "oscl_mem.h"
#include "oscl_file_io.h"
#include "oscl_file_native.h"
#include "pvlogger.h"

#include "oscl_tickcount.h"
#include "oscl_file_cfg.h"
// ---------------------------------------------------------------------
//
// Ryan Lin !!
#undef LOG_TAG
#define LOG_TAG "OsclFileCache"

#ifndef _WIN32
#include <utils/Log.h>
//#define LOGV(...) LOGD(__VA_ARGS__)

static bool bShowLog = false;
#define LOGP(...) \
{\
    if (bShowLog)\
    {\
        LOGD(__VA_ARGS__);\
    }\
    else\
    {\
        LOGV(__VA_ARGS__);\
    }\
}

#define LOG_INFO(...) \
{\
    if (bLogFrameInfo == true)\
    {\
        LOGD(__VA_ARGS__);\
    }\
    else\
    {\
        LOGV(__VA_ARGS__);\
    }\
}

#undef PVLOGGER_LOGMSG
#define PVLOGGER_LOGMSG(IL, LOGGER, LEVEL, MESSAGE)\
{\
    if (LEVEL == PVLOGMSG_ERR)\
    {\
        ErrMSG MESSAGE;\
    }\
    else\
    {\
        MSG MESSAGE;\
    }\
}
#define ErrMSG(x, ...) LOGE(__VA_ARGS__)
#define MSG(x, ...) LOGP(__VA_ARGS__)

#else

#define LOGD(...)
#define LOGV(...)
#define LOGE(...)
#define LOGP(...)

#endif
// ---------------------------------------------------------------------

static bool bSaveReadTime = false;
static FILE *pf = NULL;

OsclFileCache::OsclFileCache(Oscl_File& aContainer):
        iContainer(aContainer)
        , _cacheSize(0)
        , _pCacheBufferStart(NULL)
        , _cacheFilePosition(0)
        , _currentCachePos(0)
        , _endCachePos(0)
        , _cacheUpdateStart(0)
        , _cacheUpdateEnd(0)
        , _fileSize(0)
        , _nativePosition(0)
        , iLogger(NULL)
{
    bSaveReadTime = Oscl_File_Cfg::LoadCfg(Oscl_File_Cfg::CFG_PLAYER, (char*)"save_fread");
    if (bSaveReadTime)
    {
        pf = fopen("/cread.txt", "w+");
    }
}

OsclFileCache::~OsclFileCache()
{
    if (pf != NULL)
    {
        fclose(pf);
        pf = NULL;
    }
    
    Close();
}

int32 OsclFileCache::Open(uint32 mode, uint32 size)
//Called to open the cache for a newly opened file.
//The NativeOpen was just called prior to this and was successful.
{
    //should not be called with zero-size cache.
    OSCL_ASSERT(size > 0);

    //Save the open parameters
    _cacheSize = size;
    _mode = mode;

    //open logger object only if logging is enabled on this
    //file
    if (iContainer.iLogger)
        iLogger = PVLogger::GetLoggerObject("OsclFileCache");
    else
        iLogger = NULL;

    PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_DEBUG,
                    (0, "OsclFileCache(0x%x)::Open mode %d size %d", (uint32)this, mode, size));

    // allocate memory for cache
    // free any old buffer since its size may be different
    if (_pCacheBufferStart)
    {
        OSCL_FREE(_pCacheBufferStart);
        _pCacheBufferStart = NULL;
    }
    _pCacheBufferStart = (uint8*)OSCL_MALLOC(_cacheSize);
    if (!_pCacheBufferStart)
    {
        PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_DEBUG,
                        (0, "OsclFileCache(0x%x)::Open ERROR no memory", (uint32)this));
        return (-1);//error
    }

    //initialise the cache variables
    SetCachePosition(0);

    //get initial file size & native position
    _fileSize = iContainer.CallNativeSize();
    _nativePosition = iContainer.CallNativeTell();

    return 0;
}


void OsclFileCache::Close()
{
    //flush any cache updates
    SetCachePosition(0);

    //free the memory for cache buffer
    if (_pCacheBufferStart)
    {
        OSCL_FREE(_pCacheBufferStart);
        _pCacheBufferStart = NULL;
    }
}

uint32 OsclFileCache::Read(void* outputBuffer, uint32 size, uint32 numelements)
{
    if (!outputBuffer)
    {
        PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_DEBUG,
                        (0, "OsclFileCache(0x%x)::Read ERROR invalid arg ", (uint32)this));
        return 0;
    }

    //check for a valid read mode
    if ((_mode & Oscl_File::MODE_READWRITE)
            || (_mode & Oscl_File::MODE_READ)
            || (_mode & Oscl_File::MODE_APPEND)
            || (_mode & Oscl_File::MODE_READ_PLUS))
    {
        ;//ok to write
    }
    else
    {
        PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_DEBUG,
                        (0, "OsclFileCache(0x%x)::Read ERROR invalid mode for reading ", (uint32)this));
        return 0;//invalid mode.
    }

    uint8* destBuf = (uint8*)(outputBuffer);

    uint32 bytesToRead = numelements * size;

    //pull data out of the cache until we run out, then re-fill the cache
    //as needed until we get the desired amount.

    while (bytesToRead > 0)
    {
        //Break out of the loop if there isn't enough data left
        //in the file to read a whole element.  We don't want to read
        //a partial element.
        if ((uint32)(FileSize() - Tell()) < size)
            break;

        uint32 bytesInCache = (uint32)(_endCachePos - _currentCachePos);

        if (bytesInCache > 0)
        {
            //pull out either all data in cache, or number of bytes required,
            //whichever is less
            uint32 thisRead = (bytesInCache > bytesToRead) ? bytesToRead : bytesInCache;

            oscl_memcpy(destBuf, _pCacheBufferStart + _currentCachePos, thisRead);
            bytesToRead -= thisRead;
            destBuf += thisRead;

            //update virtual position
            _currentCachePos += thisRead;
        }
        else
        {
            //Re-fill cache from current virtual position
            int32 retval = FillCacheFromFile();
            if (retval != 0)
            {
                PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_DEBUG,
                                (0, "OsclFileCache(0x%x)::Read ERROR FillCacheFromFile failed ", (uint32)this));
                break;//error!
            }

            //When at EOF, cache will be empty after fillcache,
            //so break out of loop.
            if (_endCachePos - _currentCachePos == 0)
                break;
        }
    }

    //return number of whole elements read.
    return (size) ? ((size*numelements - bytesToRead) / size) : 0;

}


uint32 OsclFileCache::Write(const void* inputBuffer, uint32 size, uint32 numelements)
{
    if (inputBuffer == NULL)
    {
        PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_DEBUG,
                        (0, "OsclFileCache(0x%x)::Write ERROR invalid arg ", (uint32)this));
        return 0;
    }

    //write the data only in the mode we are permitted to write
    if ((_mode & Oscl_File::MODE_READWRITE)
            || (_mode & Oscl_File::MODE_APPEND)
            || (_mode & Oscl_File::MODE_READ_PLUS))
    {
        ;//ok to write
    }
    else
    {
        PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_DEBUG,
                        (0, "OsclFileCache(0x%x)::Write ERROR invalid mode for writing ", (uint32)this));
        return 0;//invalid mode.
    }

    //In Append mode, write always happens at the end of file,
    //so relocate the cache now if needed.
    if ((_mode & Oscl_File::MODE_APPEND)
            && (Tell() != FileSize()))
    {
        int32 result = SetCachePosition(FileSize());
        if (result != 0)
        {
            PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_DEBUG,
                            (0, "OsclFileCache(0x%x)::Write ERROR SetCachePosition failed. ", (uint32)this));
            return 0;
        }
    }

    uint8* srcBuf = (uint8*)(inputBuffer);
    uint32 bytesToWrite = size * numelements;

    //write into cache, flushing as needed when it fills up.

    while (bytesToWrite > 0)
    {
        uint32 spaceInCache = (_cacheSize - _currentCachePos);

        if (spaceInCache > 0)
        {
            //write to cache
            uint32 thisWrite = (spaceInCache > bytesToWrite) ? bytesToWrite : spaceInCache;

            oscl_memcpy((_pCacheBufferStart + _currentCachePos), srcBuf, thisWrite);
            bytesToWrite -= thisWrite;
            srcBuf += thisWrite;

            //keep track of the range of data in the cache that has been updated.
            if (_cacheUpdateEnd == _cacheUpdateStart)
            {
                //first update in this cache
                _cacheUpdateStart = _currentCachePos;
                _cacheUpdateEnd = _currentCachePos + thisWrite;
            }
            else
            {
                //cache has already been updated.  Expand the updated range
                //to include this update.

                if (_currentCachePos < _cacheUpdateStart)
                    _cacheUpdateStart = _currentCachePos;

                if ((_currentCachePos + thisWrite) > _cacheUpdateEnd)
                    _cacheUpdateEnd = _currentCachePos + thisWrite;
            }

            //update the virtual position.
            _currentCachePos += thisWrite;

            //extend the end of cache data if needed
            if (_endCachePos < _currentCachePos)
                _endCachePos = _currentCachePos;

            //extend the virtual file size if needed.
            if (_fileSize < (TOsclFileOffset)(_cacheFilePosition + _endCachePos))
                _fileSize = _cacheFilePosition + _endCachePos;

            //consistency checks.  if these asserts fire, there is
            //a logic error.
            OSCL_ASSERT(_cacheUpdateEnd >= _cacheUpdateStart);
            OSCL_ASSERT(_endCachePos >= _currentCachePos);
        }
        else
        {
            //entire cache is full-- flush updates to disk
            //and start a new (empty) cache at the current virtual
            //position.
            int32 retval = SetCachePosition(Tell());
            if (retval != 0)
            {
                PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_DEBUG,
                                (0, "OsclFileCache(0x%x)::Write ERROR SetCachePosition failed ", (uint32)this));
                break;//error!
            }
        }
    }

    //return number of whole elements written.
    return (size) ? ((size*numelements - bytesToWrite) / size) : 0;

}


int32 OsclFileCache::Seek(TOsclFileOffset offset, Oscl_File::seek_type origin)
{
    //figure out the file position we're trying to seek to
    TOsclFileOffset pos;
    switch (origin)
    {
        case Oscl_File::SEEKCUR:
            pos = Tell() + offset;
            break;
        case Oscl_File::SEEKSET:
            pos = 0 + offset;
            break;
        case Oscl_File::SEEKEND:
            pos = FileSize() + offset;
            break;
        default:
            PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_DEBUG,
                            (0, "OsclFileCache(0x%x)::Seek ERROR invalid origin %d", (uint32)this, origin));
            return (-1);//error-- invalid origin!
    }

    //don't allow seeking outside valid file size
    if (pos < 0 || pos > FileSize())
    {
        PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_DEBUG,
                        (0, "OsclFileCache(0x%x)::Seek ERROR invalid seek position %d", (uint32)this, (int32)pos));
        return -1;//error
    }

    //when seek is in current cache range, just update the
    //virtual position.
    if (_cacheFilePosition <= pos && pos <= (TOsclFileOffset)(_cacheFilePosition + _endCachePos))
    {
        _currentCachePos = (uint32)(pos - _cacheFilePosition);
        return 0;//success
    }

    //else seeking outside cache

    //Seek to the real target location.
    //Always use SEEKSET because the actual file end or current
    //position may not be accurate at this point.
    int32 retval;
    retval = iContainer.CallNativeSeek(pos, Oscl_File::SEEKSET);
    if (retval != 0)
    {
        PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_DEBUG,
                        (0, "OsclFileCache(0x%x)::Seek ERROR CallNativeSeek failed", (uint32)this));
        return retval;//error
    }

    //update native position
    _nativePosition = pos;

    //Relocate the cache & virtual position
    retval = SetCachePosition(pos);

    if (retval != 0)
    {
        PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_DEBUG,
                        (0, "OsclFileCache(0x%x)::Seek ERROR SetCachePosition failed", (uint32)this));
    }

    return retval;
}


int32 OsclFileCache::Flush()
{
    if (_cacheUpdateEnd > _cacheUpdateStart)
    {
        //flush updated data to file
        int32 retval = WriteCacheToFile();
        if (retval != 0)
        {
            PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_DEBUG,
                            (0, "OsclFileCache(0x%x)::Flush ERROR WriteCacheToFile failed", (uint32)this));
            return retval;
        }

        //Note: don't reset cache here, since
        //we may still want to use the data
        //in read operations.

        //flush file to disk.
        retval = iContainer.CallNativeFlush();

        if (retval != 0)
        {
            PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_DEBUG,
                            (0, "OsclFileCache(0x%x)::Flush ERROR CallNativeFlush failed", (uint32)this));
        }

        //not sure if native file position is affected
        //by a native flush, so query now.
        _nativePosition = iContainer.CallNativeTell();

        return retval;
    }
    else
    {
        //no updates in cache since last flush
        //so nothing is needed.

        return 0; //success!
    }
}


int32 OsclFileCache::SetCachePosition(TOsclFileOffset aNewPos)
{
    PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_DEBUG,
                    (0, "OsclFileCache(0x%x)::SetCachePosition curpos %d newpos %d", (uint32)this, (int32)_cacheFilePosition, (int32)aNewPos));

    //flush any updated data currently in cache.
    if (_cacheUpdateEnd > _cacheUpdateStart)
    {
        int32 result = WriteCacheToFile();
        if (result != 0)
        {
            PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_DEBUG,
                            (0, "OsclFileCache(0x%x)::SetCachePosition ERROR WriteCacheToFile failed", (uint32)this));
            return result;//error!
        }
    }

    //reset the virtual position and the cache location.
    _cacheFilePosition = aNewPos;
    _currentCachePos = 0;

    //cache is empty
    _endCachePos = 0;
    _cacheUpdateStart = _cacheUpdateEnd = 0;

    return 0;//success
}


int32 OsclFileCache::FillCacheFromFile()
{
    PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_DEBUG,
                    (0, "OsclFileCache(0x%x)::FillCacheFromFile vpos %d", (uint32)this, (int32)Tell()));

    //flush and relocate cache to current virtual
    //position if needed
    TOsclFileOffset newpos = Tell();
    if (_cacheFilePosition != newpos
            || (_cacheUpdateEnd - _cacheUpdateStart) > 0)
    {
        int32 retval = SetCachePosition(newpos);
        if (retval != 0)
        {
            PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_DEBUG,
                            (0, "OsclFileCache(0x%x)::FillCacheFromFile ERROR SetCachePosition failed", (uint32)this));
            return retval;//error!
        }
    }

    //Now seek to the read position if needed.
    if (_nativePosition != newpos)
    {
        int32 result = iContainer.CallNativeSeek(newpos, Oscl_File::SEEKSET);
        if (result != 0)
        {
            PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_DEBUG,
                            (0, "OsclFileCache(0x%x)::FillCacheFromFile ERROR CallNativeSeek failed", (uint32)this));
            return result;//error!
        }

        //keep track of the native file position
        _nativePosition = newpos;
    }

    //try to fill the cache.  If we hit EOF we won't get a full cache.
    long long _start = 0, _end = 0;
    if (bSaveReadTime)
    {
        if (pf != NULL)
        {
            _start = OsclTickCount::getTimeMs();
            fprintf(pf, "read cache, %lld, bytes, %d\n", _start, _cacheSize);
        }
    }
    
    _endCachePos = iContainer.CallNativeRead((void*)_pCacheBufferStart, 1, _cacheSize);

    if (bSaveReadTime)
    {
        if (pf != NULL)
        {
            _end = OsclTickCount::getTimeMs();
            fprintf(pf, "read out, %lld, time, %lld\n", _end, _end - _start);
        }
    }
    
    PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_DEBUG,
                    (0, "OsclFileCache(0x%x)::FillCacheFromFile got %d bytes", (uint32)this, _endCachePos));

    //update native position
    _nativePosition += _endCachePos;

    return 0;//success
}


int32 OsclFileCache::WriteCacheToFile()
{
    PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_DEBUG,
                    (0, "OsclFileCache(0x%x)::WriteCacheToFile vpos %d", (uint32)this, (int32)Tell()));

    if (_cacheUpdateEnd > _cacheUpdateStart)
    {
        uint32 bytesToWrite = (_cacheUpdateEnd - _cacheUpdateStart);

        PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_DEBUG,
                        (0, "OsclFileCache(0x%x)::WriteCacheToFile nbytes %d filepos %d start %d end %d "
                         , (uint32)this, bytesToWrite, (int32)_cacheFilePosition, _cacheUpdateStart, _cacheUpdateEnd));

        //Seek to the correct write location in the file if needed

        TOsclFileOffset pos = _cacheFilePosition + _cacheUpdateStart;

        if (_nativePosition != pos)
        {
            if (_mode & Oscl_File::MODE_APPEND)
            {
                //In Append mode, writes automatically happen at the end of file so there is no
                //need to seek.
                ;
            }
            else
            {
                //seek to the write location
                int32 retval = iContainer.CallNativeSeek(pos, Oscl_File::SEEKSET);
                if (retval != 0)
                {
                    PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_DEBUG,
                                    (0, "OsclFileCache(0x%x)::WriteCacheToFile ERROR CallNativeSeek failed", (uint32)this));
                    return retval;//error!
                }
            }

            _nativePosition = pos;
        }

        //write the updated data range.
        int32 retval = iContainer.CallNativeWrite(_pCacheBufferStart + _cacheUpdateStart, 1, bytesToWrite);

        //keep track of the native file position.
        _nativePosition += retval;

        //clear the updated data range.
        _cacheUpdateStart = _cacheUpdateEnd = 0;

        if ((uint32)retval == bytesToWrite)
            return 0;//success!

        PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_DEBUG,
                        (0, "OsclFileCache(0x%x)::WriteCacheToFile ERROR CallNativeWrite only wrote %d of %d", (uint32)this, retval, bytesToWrite));

        //At this point, the cache is corrupt, since we lost data.
        //Recover by checking actual native file values, emptying the cache,
        //and locating it as close as possible to the desired position.
        _fileSize = iContainer.CallNativeSize();
        _nativePosition = iContainer.CallNativeTell();
        TOsclFileOffset newpos = Tell();
        if (newpos > _fileSize)
            newpos = _fileSize;
        SetCachePosition(newpos);

        return (-1); //error
    }
    return 0;//success-- nothing to do
}


