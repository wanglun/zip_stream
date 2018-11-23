// operate zip file through stream interface

#ifndef ZIP_STREAM_H
#define ZIP_STREAM_H

#include <cstdio> // BUFSIZ
#include <istream>

#include <unzip.h>

class UnzipStreamBuf : public std::streambuf {
public:
  UnzipStreamBuf();
  virtual ~UnzipStreamBuf();

  bool is_open() const { return _zipfile; }
  UnzipStreamBuf* open(const char* zipfile, const char* file=NULL,
      const char* password=NULL);
  UnzipStreamBuf* close();

  bool is_select() const { return _select; }
  UnzipStreamBuf* select(const char* file, const char* password=NULL);

protected:
  virtual int_type underflow();

private:
  static const unsigned BUFFER_SIZE = BUFSIZ;
  UnzipStreamBuf(const UnzipStreamBuf&);
  UnzipStreamBuf& operator=(const UnzipStreamBuf&);

  bool _select;
  unzFile _zipfile;
  char_type* _buffer;
};

class UnzipStream : public std::istream {
public:
  UnzipStream();
  explicit UnzipStream(const char* zipfile, const char* file=NULL,
      const char* password=NULL);

  UnzipStreamBuf* rdbuf() const { return const_cast<UnzipStreamBuf*>(&_sb); }

  bool is_open() { return _sb.is_open(); }
  void open(const char* zipfile, const char* file=NULL,
      const char* password=NULL);
  void close();

  bool is_select() const { return _sb.is_select(); }
  void select(const char* file, const char* password=NULL);

private:
  UnzipStreamBuf _sb;
};

#endif // ZIP_STREAM_H
