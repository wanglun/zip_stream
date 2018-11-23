#include "zip_stream.h"

UnzipStreamBuf::UnzipStreamBuf()
  : _select(false)
  , _zipfile(NULL)
  , _buffer(NULL)
  {}

UnzipStreamBuf::~UnzipStreamBuf() {
  close();
}

UnzipStreamBuf* UnzipStreamBuf::open(const char* zipfile, const char* file,
    const char* password) {
  if (is_open()) {
    return NULL;
  }
  if (!zipfile) {
    return NULL;
  }

  // open zip file
  _zipfile = unzOpen64(zipfile);
  if (!_zipfile) {
    return NULL;
  }

  // allocate buffer
  _buffer = new char_type[BUFFER_SIZE];
  setg(_buffer, _buffer, _buffer);

  if (file) {
    return select(file, password);
  }
  return this;
}

UnzipStreamBuf* UnzipStreamBuf::close() {
  if (!is_open()) {
    return NULL;
  }

  if (is_select()) {
    unzCloseCurrentFile(_zipfile);
    _select = false;
  }

  if (_buffer) {
    delete[] _buffer;
    _buffer = NULL;
    setg(0, 0, 0);
  }

  if (unzClose(_zipfile) != UNZ_OK) {
    return NULL;
  }
  _zipfile = NULL;

  return this;
}

UnzipStreamBuf* UnzipStreamBuf::select(const char* file, const char* password) {
  if (!is_open()) {
    return NULL;
  }

  if (is_select()) {
    unzCloseCurrentFile(_zipfile);
    _select = false;
  }

  if (unzLocateFile(_zipfile, file, NULL/*strcmp*/) != UNZ_OK) {
    return NULL;
  }
  int ret = UNZ_OK;
  if (password) {
    ret = unzOpenCurrentFilePassword(_zipfile, password);
  } else {
    ret = unzOpenCurrentFile(_zipfile);
  }
  if (ret != UNZ_OK) {
    return NULL;
  }
  _select = true;
  setg(_buffer, _buffer, _buffer);

  return this;
}

std::streambuf::int_type UnzipStreamBuf::underflow() {
  if (!is_open() || !is_select()) {
    return traits_type::eof();
  }

  if (gptr() < egptr()) {
    return traits_type::to_int_type(*gptr());
  }

  int n = unzReadCurrentFile(_zipfile, _buffer, BUFFER_SIZE);
  if (n <= 0) {
    setg(_buffer, _buffer, _buffer);
    return traits_type::eof();
  }
  setg(_buffer, _buffer, _buffer + n);

  return traits_type::to_int_type(*gptr());
}

UnzipStream::UnzipStream() : std::istream(NULL) {
  init(&_sb); 
}

UnzipStream::UnzipStream(const char* zipfile, const char* file,
    const char* password) : std::istream(NULL) {
  init(&_sb);
  open(zipfile, file, password);
}

void UnzipStream::open(const char* zipfile, const char* file,
    const char* password) {
  if (!_sb.open(zipfile, file, password)) {
    setstate(std::ios_base::failbit);
  } else {
    clear();
  }
}

void UnzipStream::close() {
  if (!_sb.close()) {
    setstate(std::ios_base::failbit);
  }
}

void UnzipStream::select(const char* file, const char* password) {
  if (!_sb.select(file, password)) {
    setstate(std::ios_base::failbit);
  } else {
    clear();
  }
}
