#!/usr/bin/env python3

import sys
import os

from PIL import Image


def main():
    if len(sys.argv) != 3:
        print("Usage: %s <image> <output file>" % sys.argv[0])
        sys.exit(-1)

    img = Image.open(sys.argv[1]).convert('RGB')

    width, height = img.size

    print("Size: %d x %d = %d bytes" % (width, height, width * height / 8))

    file_name = os.path.basename(sys.argv[2])
    include_guard =  file_name.replace('.','_').upper()
    variable_name = '_'.join(file_name.split('.')[:-1])

    output_file = open(sys.argv[2], 'w')
    output_file.write("#include <avr/pgmspace.h>\n\n")
    output_file.write("#ifndef _%s_\n" % include_guard)
    output_file.write("#define _%s_\n" % include_guard)
    output_file.write("static const uint8_t PROGMEM %s[] = {\n" % variable_name)
    output_file.write("\t0x%x, 0x%x,\n" % (width, height))

    bit_pos = 0
    bits = 0

    for y in range(0, height):
        output_file.write("\t")
        for x in range(0, width):
            r, g, b = img.getpixel((x, y))

            if (r + b + g) / 3 > 127:
                bits = bits | (1 << (bit_pos % 8))

            bit_pos += 1

            if bit_pos % 8 == 0:
                output_file.write("0x%x, " % bits)
                bits = 0

        output_file.write("\n")

    output_file.write("};\n")
    output_file.write("#endif\n")
    output_file.close()


if __name__ == '__main__':
    main()
