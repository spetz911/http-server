#include "file_holder.h"

#include <fcntl.h>
#include <unistd.h>

namespace System
{
  
  FileHolder::FileHolder(std::string const &fileName)
    : Handle(-1)
    , Size(0)
  {
    Open(fileName);
  }
  
  FileHolder::~FileHolder()
  {
    if (Handle != -1)
      close(Handle);
  }
  
  void FileHolder::Open(std::string const &fileName)
  {
    int NewHandle = open(fileName.c_str(), 0);
    if (NewHandle == -1)
      throw FileHolderException("Failed to open file");
    int NewSize = lseek(NewHandle, 0, SEEK_SET);
    if (NewSize == -1 || (NewSize = lseek(NewHandle, 0, SEEK_END)) == -1 ||
        lseek(NewHandle, 0, SEEK_SET) == -1)
    {
      close(NewHandle);
      throw FileHolderException("Failed to set file position");
    }
    if (Handle != -1)
      close(Handle);
    Handle = NewHandle;
    Size = NewSize;
  }
  
  int FileHolder::GetHandle() const
  {
    return Handle;
  }
  
  int FileHolder::GetSize() const
  {
    return Size;
  }
  
  int FileHolder::Detach()
  {
    int Ret = Handle;
    Handle = -1;
    Size = -1;
    return Ret;
  }
  
}
