/**
 * Autogenerated by Thrift Compiler (1.0.0-dev)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#include "Pose3DAnnNew_types.h"

#include <algorithm>
#include <ostream>

#include <thrift/TToString.h>

namespace ThriftPose3DAnn {


Image::~Image() throw() {
}


void Image::__set_imgBin(const std::string& val) {
  this->imgBin = val;
}

void Image::__set_width(const int32_t val) {
  this->width = val;
}

void Image::__set_height(const int32_t val) {
  this->height = val;
}

void Image::__set_channel(const int32_t val) {
  this->channel = val;
}
std::ostream& operator<<(std::ostream& out, const Image& obj)
{
  obj.printTo(out);
  return out;
}


uint32_t Image::read(::apache::thrift::protocol::TProtocol* iprot) {

  apache::thrift::protocol::TInputRecursionTracker tracker(*iprot);
  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readBinary(this->imgBin);
          this->__isset.imgBin = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->width);
          this->__isset.width = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->height);
          this->__isset.height = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 4:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->channel);
          this->__isset.channel = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t Image::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  apache::thrift::protocol::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("Image");

  xfer += oprot->writeFieldBegin("imgBin", ::apache::thrift::protocol::T_STRING, 1);
  xfer += oprot->writeBinary(this->imgBin);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("width", ::apache::thrift::protocol::T_I32, 2);
  xfer += oprot->writeI32(this->width);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("height", ::apache::thrift::protocol::T_I32, 3);
  xfer += oprot->writeI32(this->height);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("channel", ::apache::thrift::protocol::T_I32, 4);
  xfer += oprot->writeI32(this->channel);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(Image &a, Image &b) {
  using ::std::swap;
  swap(a.imgBin, b.imgBin);
  swap(a.width, b.width);
  swap(a.height, b.height);
  swap(a.channel, b.channel);
  swap(a.__isset, b.__isset);
}

Image::Image(const Image& other0) {
  imgBin = other0.imgBin;
  width = other0.width;
  height = other0.height;
  channel = other0.channel;
  __isset = other0.__isset;
}
Image& Image::operator=(const Image& other1) {
  imgBin = other1.imgBin;
  width = other1.width;
  height = other1.height;
  channel = other1.channel;
  __isset = other1.__isset;
  return *this;
}
void Image::printTo(std::ostream& out) const {
  using ::apache::thrift::to_string;
  out << "Image(";
  out << "imgBin=" << to_string(imgBin);
  out << ", " << "width=" << to_string(width);
  out << ", " << "height=" << to_string(height);
  out << ", " << "channel=" << to_string(channel);
  out << ")";
}


Vector3DInt::~Vector3DInt() throw() {
}


void Vector3DInt::__set_x(const int32_t val) {
  this->x = val;
}

void Vector3DInt::__set_y(const int32_t val) {
  this->y = val;
}

void Vector3DInt::__set_z(const int32_t val) {
  this->z = val;
}
std::ostream& operator<<(std::ostream& out, const Vector3DInt& obj)
{
  obj.printTo(out);
  return out;
}


uint32_t Vector3DInt::read(::apache::thrift::protocol::TProtocol* iprot) {

  apache::thrift::protocol::TInputRecursionTracker tracker(*iprot);
  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->x);
          this->__isset.x = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->y);
          this->__isset.y = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->z);
          this->__isset.z = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t Vector3DInt::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  apache::thrift::protocol::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("Vector3DInt");

  xfer += oprot->writeFieldBegin("x", ::apache::thrift::protocol::T_I32, 1);
  xfer += oprot->writeI32(this->x);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("y", ::apache::thrift::protocol::T_I32, 2);
  xfer += oprot->writeI32(this->y);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("z", ::apache::thrift::protocol::T_I32, 3);
  xfer += oprot->writeI32(this->z);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(Vector3DInt &a, Vector3DInt &b) {
  using ::std::swap;
  swap(a.x, b.x);
  swap(a.y, b.y);
  swap(a.z, b.z);
  swap(a.__isset, b.__isset);
}

Vector3DInt::Vector3DInt(const Vector3DInt& other2) {
  x = other2.x;
  y = other2.y;
  z = other2.z;
  __isset = other2.__isset;
}
Vector3DInt& Vector3DInt::operator=(const Vector3DInt& other3) {
  x = other3.x;
  y = other3.y;
  z = other3.z;
  __isset = other3.__isset;
  return *this;
}
void Vector3DInt::printTo(std::ostream& out) const {
  using ::apache::thrift::to_string;
  out << "Vector3DInt(";
  out << "x=" << to_string(x);
  out << ", " << "y=" << to_string(y);
  out << ", " << "z=" << to_string(z);
  out << ")";
}


Vector3DFloat::~Vector3DFloat() throw() {
}


void Vector3DFloat::__set_x(const double val) {
  this->x = val;
}

void Vector3DFloat::__set_y(const double val) {
  this->y = val;
}

void Vector3DFloat::__set_z(const double val) {
  this->z = val;
}
std::ostream& operator<<(std::ostream& out, const Vector3DFloat& obj)
{
  obj.printTo(out);
  return out;
}


uint32_t Vector3DFloat::read(::apache::thrift::protocol::TProtocol* iprot) {

  apache::thrift::protocol::TInputRecursionTracker tracker(*iprot);
  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_DOUBLE) {
          xfer += iprot->readDouble(this->x);
          this->__isset.x = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_DOUBLE) {
          xfer += iprot->readDouble(this->y);
          this->__isset.y = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_DOUBLE) {
          xfer += iprot->readDouble(this->z);
          this->__isset.z = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t Vector3DFloat::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  apache::thrift::protocol::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("Vector3DFloat");

  xfer += oprot->writeFieldBegin("x", ::apache::thrift::protocol::T_DOUBLE, 1);
  xfer += oprot->writeDouble(this->x);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("y", ::apache::thrift::protocol::T_DOUBLE, 2);
  xfer += oprot->writeDouble(this->y);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("z", ::apache::thrift::protocol::T_DOUBLE, 3);
  xfer += oprot->writeDouble(this->z);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(Vector3DFloat &a, Vector3DFloat &b) {
  using ::std::swap;
  swap(a.x, b.x);
  swap(a.y, b.y);
  swap(a.z, b.z);
  swap(a.__isset, b.__isset);
}

Vector3DFloat::Vector3DFloat(const Vector3DFloat& other4) {
  x = other4.x;
  y = other4.y;
  z = other4.z;
  __isset = other4.__isset;
}
Vector3DFloat& Vector3DFloat::operator=(const Vector3DFloat& other5) {
  x = other5.x;
  y = other5.y;
  z = other5.z;
  __isset = other5.__isset;
  return *this;
}
void Vector3DFloat::printTo(std::ostream& out) const {
  using ::apache::thrift::to_string;
  out << "Vector3DFloat(";
  out << "x=" << to_string(x);
  out << ", " << "y=" << to_string(y);
  out << ", " << "z=" << to_string(z);
  out << ")";
}


Orientation::~Orientation() throw() {
}


void Orientation::__set_xAxis(const Vector3DFloat& val) {
  this->xAxis = val;
}

void Orientation::__set_yAxis(const Vector3DFloat& val) {
  this->yAxis = val;
}

void Orientation::__set_zAxis(const Vector3DFloat& val) {
  this->zAxis = val;
}
std::ostream& operator<<(std::ostream& out, const Orientation& obj)
{
  obj.printTo(out);
  return out;
}


uint32_t Orientation::read(::apache::thrift::protocol::TProtocol* iprot) {

  apache::thrift::protocol::TInputRecursionTracker tracker(*iprot);
  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_STRUCT) {
          xfer += this->xAxis.read(iprot);
          this->__isset.xAxis = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_STRUCT) {
          xfer += this->yAxis.read(iprot);
          this->__isset.yAxis = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_STRUCT) {
          xfer += this->zAxis.read(iprot);
          this->__isset.zAxis = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t Orientation::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  apache::thrift::protocol::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("Orientation");

  xfer += oprot->writeFieldBegin("xAxis", ::apache::thrift::protocol::T_STRUCT, 1);
  xfer += this->xAxis.write(oprot);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("yAxis", ::apache::thrift::protocol::T_STRUCT, 2);
  xfer += this->yAxis.write(oprot);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("zAxis", ::apache::thrift::protocol::T_STRUCT, 3);
  xfer += this->zAxis.write(oprot);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(Orientation &a, Orientation &b) {
  using ::std::swap;
  swap(a.xAxis, b.xAxis);
  swap(a.yAxis, b.yAxis);
  swap(a.zAxis, b.zAxis);
  swap(a.__isset, b.__isset);
}

Orientation::Orientation(const Orientation& other6) {
  xAxis = other6.xAxis;
  yAxis = other6.yAxis;
  zAxis = other6.zAxis;
  __isset = other6.__isset;
}
Orientation& Orientation::operator=(const Orientation& other7) {
  xAxis = other7.xAxis;
  yAxis = other7.yAxis;
  zAxis = other7.zAxis;
  __isset = other7.__isset;
  return *this;
}
void Orientation::printTo(std::ostream& out) const {
  using ::apache::thrift::to_string;
  out << "Orientation(";
  out << "xAxis=" << to_string(xAxis);
  out << ", " << "yAxis=" << to_string(yAxis);
  out << ", " << "zAxis=" << to_string(zAxis);
  out << ")";
}


CameraInfo::~CameraInfo() throw() {
}


void CameraInfo::__set_camMatrix(const std::vector<double> & val) {
  this->camMatrix = val;
}
std::ostream& operator<<(std::ostream& out, const CameraInfo& obj)
{
  obj.printTo(out);
  return out;
}


uint32_t CameraInfo::read(::apache::thrift::protocol::TProtocol* iprot) {

  apache::thrift::protocol::TInputRecursionTracker tracker(*iprot);
  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_LIST) {
          {
            this->camMatrix.clear();
            uint32_t _size8;
            ::apache::thrift::protocol::TType _etype11;
            xfer += iprot->readListBegin(_etype11, _size8);
            this->camMatrix.resize(_size8);
            uint32_t _i12;
            for (_i12 = 0; _i12 < _size8; ++_i12)
            {
              xfer += iprot->readDouble(this->camMatrix[_i12]);
            }
            xfer += iprot->readListEnd();
          }
          this->__isset.camMatrix = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t CameraInfo::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  apache::thrift::protocol::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("CameraInfo");

  xfer += oprot->writeFieldBegin("camMatrix", ::apache::thrift::protocol::T_LIST, 1);
  {
    xfer += oprot->writeListBegin(::apache::thrift::protocol::T_DOUBLE, static_cast<uint32_t>(this->camMatrix.size()));
    std::vector<double> ::const_iterator _iter13;
    for (_iter13 = this->camMatrix.begin(); _iter13 != this->camMatrix.end(); ++_iter13)
    {
      xfer += oprot->writeDouble((*_iter13));
    }
    xfer += oprot->writeListEnd();
  }
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(CameraInfo &a, CameraInfo &b) {
  using ::std::swap;
  swap(a.camMatrix, b.camMatrix);
  swap(a.__isset, b.__isset);
}

CameraInfo::CameraInfo(const CameraInfo& other14) {
  camMatrix = other14.camMatrix;
  __isset = other14.__isset;
}
CameraInfo& CameraInfo::operator=(const CameraInfo& other15) {
  camMatrix = other15.camMatrix;
  __isset = other15.__isset;
  return *this;
}
void CameraInfo::printTo(std::ostream& out) const {
  using ::apache::thrift::to_string;
  out << "CameraInfo(";
  out << "camMatrix=" << to_string(camMatrix);
  out << ")";
}


GuessInfo::~GuessInfo() throw() {
}


void GuessInfo::__set_guessOrien(const std::vector<Orientation> & val) {
  this->guessOrien = val;
}

void GuessInfo::__set_guessJoints(const std::vector<Vector3DFloat> & val) {
  this->guessJoints = val;
}

void GuessInfo::__set_camInfo(const CameraInfo& val) {
  this->camInfo = val;
}

void GuessInfo::__set_isSuccess(const bool val) {
  this->isSuccess = val;
}
std::ostream& operator<<(std::ostream& out, const GuessInfo& obj)
{
  obj.printTo(out);
  return out;
}


uint32_t GuessInfo::read(::apache::thrift::protocol::TProtocol* iprot) {

  apache::thrift::protocol::TInputRecursionTracker tracker(*iprot);
  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_LIST) {
          {
            this->guessOrien.clear();
            uint32_t _size16;
            ::apache::thrift::protocol::TType _etype19;
            xfer += iprot->readListBegin(_etype19, _size16);
            this->guessOrien.resize(_size16);
            uint32_t _i20;
            for (_i20 = 0; _i20 < _size16; ++_i20)
            {
              xfer += this->guessOrien[_i20].read(iprot);
            }
            xfer += iprot->readListEnd();
          }
          this->__isset.guessOrien = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_LIST) {
          {
            this->guessJoints.clear();
            uint32_t _size21;
            ::apache::thrift::protocol::TType _etype24;
            xfer += iprot->readListBegin(_etype24, _size21);
            this->guessJoints.resize(_size21);
            uint32_t _i25;
            for (_i25 = 0; _i25 < _size21; ++_i25)
            {
              xfer += this->guessJoints[_i25].read(iprot);
            }
            xfer += iprot->readListEnd();
          }
          this->__isset.guessJoints = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_STRUCT) {
          xfer += this->camInfo.read(iprot);
          this->__isset.camInfo = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 4:
        if (ftype == ::apache::thrift::protocol::T_BOOL) {
          xfer += iprot->readBool(this->isSuccess);
          this->__isset.isSuccess = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t GuessInfo::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  apache::thrift::protocol::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("GuessInfo");

  xfer += oprot->writeFieldBegin("guessOrien", ::apache::thrift::protocol::T_LIST, 1);
  {
    xfer += oprot->writeListBegin(::apache::thrift::protocol::T_STRUCT, static_cast<uint32_t>(this->guessOrien.size()));
    std::vector<Orientation> ::const_iterator _iter26;
    for (_iter26 = this->guessOrien.begin(); _iter26 != this->guessOrien.end(); ++_iter26)
    {
      xfer += (*_iter26).write(oprot);
    }
    xfer += oprot->writeListEnd();
  }
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("guessJoints", ::apache::thrift::protocol::T_LIST, 2);
  {
    xfer += oprot->writeListBegin(::apache::thrift::protocol::T_STRUCT, static_cast<uint32_t>(this->guessJoints.size()));
    std::vector<Vector3DFloat> ::const_iterator _iter27;
    for (_iter27 = this->guessJoints.begin(); _iter27 != this->guessJoints.end(); ++_iter27)
    {
      xfer += (*_iter27).write(oprot);
    }
    xfer += oprot->writeListEnd();
  }
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("camInfo", ::apache::thrift::protocol::T_STRUCT, 3);
  xfer += this->camInfo.write(oprot);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("isSuccess", ::apache::thrift::protocol::T_BOOL, 4);
  xfer += oprot->writeBool(this->isSuccess);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(GuessInfo &a, GuessInfo &b) {
  using ::std::swap;
  swap(a.guessOrien, b.guessOrien);
  swap(a.guessJoints, b.guessJoints);
  swap(a.camInfo, b.camInfo);
  swap(a.isSuccess, b.isSuccess);
  swap(a.__isset, b.__isset);
}

GuessInfo::GuessInfo(const GuessInfo& other28) {
  guessOrien = other28.guessOrien;
  guessJoints = other28.guessJoints;
  camInfo = other28.camInfo;
  isSuccess = other28.isSuccess;
  __isset = other28.__isset;
}
GuessInfo& GuessInfo::operator=(const GuessInfo& other29) {
  guessOrien = other29.guessOrien;
  guessJoints = other29.guessJoints;
  camInfo = other29.camInfo;
  isSuccess = other29.isSuccess;
  __isset = other29.__isset;
  return *this;
}
void GuessInfo::printTo(std::ostream& out) const {
  using ::apache::thrift::to_string;
  out << "GuessInfo(";
  out << "guessOrien=" << to_string(guessOrien);
  out << ", " << "guessJoints=" << to_string(guessJoints);
  out << ", " << "camInfo=" << to_string(camInfo);
  out << ", " << "isSuccess=" << to_string(isSuccess);
  out << ")";
}


HumanPose3DInfo::~HumanPose3DInfo() throw() {
}


void HumanPose3DInfo::__set_joints(const std::vector<Vector3DFloat> & val) {
  this->joints = val;
}

void HumanPose3DInfo::__set_orien(const Orientation& val) {
  this->orien = val;
}

void HumanPose3DInfo::__set_camInfo(const CameraInfo& val) {
  this->camInfo = val;
}
std::ostream& operator<<(std::ostream& out, const HumanPose3DInfo& obj)
{
  obj.printTo(out);
  return out;
}


uint32_t HumanPose3DInfo::read(::apache::thrift::protocol::TProtocol* iprot) {

  apache::thrift::protocol::TInputRecursionTracker tracker(*iprot);
  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_LIST) {
          {
            this->joints.clear();
            uint32_t _size30;
            ::apache::thrift::protocol::TType _etype33;
            xfer += iprot->readListBegin(_etype33, _size30);
            this->joints.resize(_size30);
            uint32_t _i34;
            for (_i34 = 0; _i34 < _size30; ++_i34)
            {
              xfer += this->joints[_i34].read(iprot);
            }
            xfer += iprot->readListEnd();
          }
          this->__isset.joints = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_STRUCT) {
          xfer += this->orien.read(iprot);
          this->__isset.orien = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_STRUCT) {
          xfer += this->camInfo.read(iprot);
          this->__isset.camInfo = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t HumanPose3DInfo::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  apache::thrift::protocol::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("HumanPose3DInfo");

  xfer += oprot->writeFieldBegin("joints", ::apache::thrift::protocol::T_LIST, 1);
  {
    xfer += oprot->writeListBegin(::apache::thrift::protocol::T_STRUCT, static_cast<uint32_t>(this->joints.size()));
    std::vector<Vector3DFloat> ::const_iterator _iter35;
    for (_iter35 = this->joints.begin(); _iter35 != this->joints.end(); ++_iter35)
    {
      xfer += (*_iter35).write(oprot);
    }
    xfer += oprot->writeListEnd();
  }
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("orien", ::apache::thrift::protocol::T_STRUCT, 2);
  xfer += this->orien.write(oprot);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("camInfo", ::apache::thrift::protocol::T_STRUCT, 3);
  xfer += this->camInfo.write(oprot);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(HumanPose3DInfo &a, HumanPose3DInfo &b) {
  using ::std::swap;
  swap(a.joints, b.joints);
  swap(a.orien, b.orien);
  swap(a.camInfo, b.camInfo);
  swap(a.__isset, b.__isset);
}

HumanPose3DInfo::HumanPose3DInfo(const HumanPose3DInfo& other36) {
  joints = other36.joints;
  orien = other36.orien;
  camInfo = other36.camInfo;
  __isset = other36.__isset;
}
HumanPose3DInfo& HumanPose3DInfo::operator=(const HumanPose3DInfo& other37) {
  joints = other37.joints;
  orien = other37.orien;
  camInfo = other37.camInfo;
  __isset = other37.__isset;
  return *this;
}
void HumanPose3DInfo::printTo(std::ostream& out) const {
  using ::apache::thrift::to_string;
  out << "HumanPose3DInfo(";
  out << "joints=" << to_string(joints);
  out << ", " << "orien=" << to_string(orien);
  out << ", " << "camInfo=" << to_string(camInfo);
  out << ")";
}

} // namespace
