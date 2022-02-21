#include <roboutils/comm/Serial.h>
#include "roboutils/util/strings.h"

#include <stdexcept>
#include <cerrno>
#include <cstring>

#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

using namespace RoboUtils::COMM;

Serial::~Serial()
{
  if (fileDescriptor > 0) {
    close(fileDescriptor);
    fileDescriptor = 0;
  }
}

void Serial::open(const std::string &file)
{
  fileDescriptor = ::open(file.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
  if (fileDescriptor==-1)
    throw std::logic_error("Can't open port");
}

void Serial::configure()
{
  struct termios tios{};

  if (tcgetattr(fileDescriptor, &tios) < 0)
    throw std::logic_error("can't get attr");

  tios.c_cflag = CS8 | CLOCAL | CREAD;
  tios.c_iflag = IGNPAR;
  tios.c_oflag = 0;
  tios.c_lflag = 0;
  tios.c_cc[VMIN] = 1;      // block untill n bytes are received
  tios.c_cc[VTIME] = 0;     // block untill a timer expires (n * 100 mSec.)

  cfsetispeed(&tios, B115200);
  cfsetospeed(&tios, B115200);

  if (tcsetattr(fileDescriptor, TCSANOW, &tios) < 0)
    throw std::logic_error("Error");
}


void Serial::send(const uint8_t *buffer, std::size_t size) const
{
  write(fileDescriptor, buffer, size);
}


std::vector<uint8_t> Serial::receive() const
{
  std::vector<uint8_t> result(1500);

  int n = ::read(fileDescriptor, result.data(), (size_t)result.size());

  if (n < 0) {
    if (errno == EAGAIN)
      return {};
    else
      throw std::logic_error("Can't read");
  }

  result.resize(n);
  return result;
}

std::string Serial::receiveStr() const
{
  auto r = receive();
  return { r.begin(), r.end() };
}