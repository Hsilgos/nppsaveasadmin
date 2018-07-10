import argparse, ntpath
from pathlib import Path

def convert_file_name_to_buffer_name(filename):
  filename = filename.lower()
  allowed_symbols = "qwertyuiopasdfghjklzxcvbnm1234567890_"
  result = ''
  for ch in filename:
    if ch in allowed_symbols:
      result += ch
    else:
      result += '_'
  return result

def parse_command_line():
  parser = argparse.ArgumentParser(description='Script which translates any binary data into C++ code to read it as buffer')

  parser.add_argument('--input', help='Input file to process', required=True)
  parser.add_argument('--out', help='Output file header/source name. Input file name is used as base if missed')
  parser.add_argument('--buffer-name', help='Buffer name. Access function will be named using this name')

  args = parser.parse_args()

  if not Path(args.input).is_file():
    print("Input file '" + args.input + "' not exists")
    exit(1)

  output_filename = args.out if args.out != None else ntpath.basename(args.input)

  output_h_filename = output_filename + '.h'
  output_cpp_filename = output_filename + '.cpp'

  buffer_name = args.buffer_name if args.buffer_name != None else convert_file_name_to_buffer_name(output_filename)

  return {'input':args.input,
    'buffer_name':buffer_name,
    'file_h':output_h_filename,
    'file_cpp':output_cpp_filename}


def bytes_from_file(filename, chunk_size = 1024 * 8):
  with open(filename, 'rb') as file:
    bytes = file.read(chunk_size)
    while bytes:
      for b in bytes:
        yield b
      bytes = file.read(chunk_size)


header=\
"""#pragma once

const char* get_%s_buffer_ptr();
std::uint64_t get_%s_buffer_length();
"""

src_implementation="""
const char* get_%s_buffer_ptr() {
    return buffer_ptr;
}
std::uint64_t get_%s_buffer_length() {
    return buffer_length;
}
"""

def write_header(header_name, buffer_name):
  with open(header_name, 'w') as file:
    file.write(header % (buffer_name, buffer_name))

def write_src(src_name, buffer_name, binary_file):
  with open(src_name, 'w') as out_file:
    out_file.write('namespace {\n')
    out_file.write('const char* const buffer_ptr = {')

    byte_count = 0
    comma = ''
    for byte in bytes_from_file(binary_file):
        out_file.write(comma)
        if byte_count % 12 == 0:
          out_file.write('\n    ')
        else:
          out_file.write(' ')
        out_file.write("0x")
        out_file.write(format(byte, '02x'))
        comma = ','
        byte_count=byte_count + 1;
    out_file.write('\n    };\n')
    out_file.write('const std::uint64_t buffer_length=%d;\n' % byte_count)
    out_file.write('} // namespace \n')
    out_file.write(src_implementation % (buffer_name, buffer_name))

opts = parse_command_line()

write_header(opts['file_h'], opts['buffer_name'])
write_src(opts['file_cpp'], opts['buffer_name'], opts['input'])
