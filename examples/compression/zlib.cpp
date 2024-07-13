#include <sincpp/compression/zlib.hpp>

#include <iostream>

int main() {
  std::string_view const data = "Bouh!";
  std::cout << "Data = " << data << std::endl;

  auto const &[compressed, compressed_error_code, compressed_error_message] =
      sincpp::zlib::compress(data);

  if (compressed_error_message.empty() == false) {
    std::cout << "Compression error: " << compressed_error_message
              << "(error code = " << compressed_error_code << ")" << std::endl;
  }
  std::cout << "Compressed data = ";
  for (std::byte const b : compressed) {
    std::cout << int(b) << " ";
  }
  std::cout << std::endl;

  auto const &[decompressed, decompressed_error_code,
               decompressed_error_message] =
      sincpp::zlib::decompress<std::string>(compressed);

  if (compressed_error_message.empty() == false) {
    std::cout << "Decompression error: " << compressed_error_message
              << "(error code = " << decompressed_error_code << ")"
              << std::endl;
  }
  std::cout << "Decompressed data = " << decompressed << std::endl;

  // Output:
  // Data = Bouh!
  // Compressed data = 120 218 115 202 47 205 80 4 0 5 91 1 176
  // Decompressed data = Bouh!

  return 0;
}
