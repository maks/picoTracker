#ifndef _PICOTRACKERFILESYSTEM_H_
#define _PICOTRACKERFILESYSTEM_H_

#include "Externals/SdFat/src/SdFat.h"
#include "System/FileSystem/FileSystem.h"
#include <stdio.h>
#include <string.h>

#define PICO_MAX_FILENAME_LEN 128

class picoTrackerFile : public I_File {
public:
  picoTrackerFile(FsBaseFile file);
  virtual int Read(void *ptr, int size, int nmemb);
  virtual int GetC();
  virtual int Write(const void *ptr, int size, int nmemb);
  virtual void Printf(const char *format, ...);
  virtual void Seek(long offset, int whence);
  virtual long Tell();
  virtual void Close();
  virtual int Error();

private:
  FsBaseFile file_;
};

class picoTrackerDir : public I_Dir {
public:
  picoTrackerDir(const char *path);
  ~picoTrackerDir() { Trace::Log("FILESYSTEM", "DESTRUCT pTDir:%s", path_); delete files_ ; } ;
  void GetContent(const char *mask);
	T_SimpleList<Path>* ListRelative() ;
	void Add(Path *p);
	void Clear();
	void Sort();

private:
  T_SimpleList<Path> *files_;
};

class picoTrackerFileSystem : public FileSystem {
public:
  picoTrackerFileSystem();
  virtual I_File *Open(const char *path, const char *mode);
  virtual I_Dir *Open(const char *path);
  virtual FileType GetFileType(const char *path);
  virtual Result MakeDir(const char *path);
  virtual void Delete(const char *){};

private:
  SdFs SD_;
};

class picoTrackerPath: public Path {
public:
  picoTrackerPath(int index) {
    index_ = index;
  }

private:
  int index_;
};

#endif
