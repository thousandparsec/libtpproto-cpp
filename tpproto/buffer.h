#ifndef TPPROTO_BUFFER_H
#define TPPROTO_BUFFER_H

namespace TPProto{
  
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
