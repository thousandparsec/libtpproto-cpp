#ifndef TPPROTO_BUFFER_H
#define TPPROTO_BUFFER_H
/*! \file buffer.h
  \brief Declares Buffer class.
  
  The Buffer class is a buffer.
*/

namespace TPProto{
  
  /*! \brief Buffer for packing and unpacking Frames

  Supports unpacking and packing various base types and creating
  and reading the frame header.
  */
  class Buffer{
  public:

    Buffer();
    ~Buffer();

    void packInt(int val);
    void packInt64(long long val);
    void packString(const char* val);

    int unpackInt();
    long long unpackInt64();
    char* unpackString();

    int peekInt(int offset);

    void createHeader(int ver, int seqnum, int type, int len);
    bool readHeader(int &ver, int &seqnum, int &type, int &len);

    void setData(char* buff, int len);
    char* getData();
    int getLength();

  private:
    char* data;
    int datalen;
    int dataptr;

  };

}

#endif
