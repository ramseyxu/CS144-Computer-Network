#include "byte_stream.hh"

#include <algorithm>

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

ByteStream::ByteStream(const size_t capacity )
  : capacity_( capacity ), buffer( deque<string>() ), input_ended_( false )
{}

uint64_t ByteStream::copy_to_buffer( string data )
{
  if ( remaining_capacity() == 0 || data.size() == 0 )
    return 0;
  if ( data.size() > remaining_capacity() )
    data = data.substr( 0, remaining_capacity() );
  bytes_written_ += data.size();
  buffer_size_ += data.size();
  buffer.push_back( std::move( data ) );
  return buffer.back().size();
}

void ByteStream::pop_output( const size_t len )
{
  if ( buffer_empty() )
    return;
  size_t pop_len = min( len, buffer_size_ );
  size_t unpopped_len = pop_len;
  while ( unpopped_len > 0 ) {
    if ( buffer.front().size() > unpopped_len ) {
      buffer.front() = buffer.front().substr( unpopped_len );
      unpopped_len = 0;
    } else {
      unpopped_len -= buffer.front().size();
      buffer.pop_front();
    }
  }
  bytes_read_ += pop_len;
  buffer_size_ -= pop_len;
}

size_t ByteStream::write(const string &data) {
    return copy_to_buffer( data );
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
    size_t canPeek = min(len, buffer_size_);
    string out = "";
    while (canPeek > 0) {
        if (buffer.front().size() > canPeek) {
            out += buffer.front().substr(0, canPeek);
            canPeek = 0;
        } else {
            out += buffer.front();
            canPeek -= buffer.front().size();
        }
    }
    return out;
}

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {
    string out = peek_output(len);
    pop_output(len);
    return out;
}

void ByteStream::end_input() { input_ended_ = true; }

bool ByteStream::input_ended() const { return input_ended_; }

size_t ByteStream::buffer_size() const { return buffer_size_; }

bool ByteStream::buffer_empty() const { return buffer_size_ == 0; }

bool ByteStream::eof() const { return buffer.empty() && input_ended_; }

size_t ByteStream::bytes_written() const { return bytes_written_; }

size_t ByteStream::bytes_read() const { return bytes_read_; }

size_t ByteStream::remaining_capacity() const { return capacity_ - buffer.size(); }
