#!/usr/bin/env python3
import sys
import argparse
from os import path
from PIL import Image


def swapbytes(int):
    return (((int & 0xFF) << 8) | (int >> 8))


def torgb565(r, g, b):
    return swapbytes(((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3))


def convertimage(name):
    image = Image.open(name)
    image = image.convert("RGBA")

    name, extension = path.splitext(path.basename(name))
    print("uint16_t " + name + "[" + str(image.size[0]*image.size[1]) + "] = {", end="")

    for y in range(image.size[1]):
        print("\n\t", end="")
        for x in range(image.size[0]):
            pixel = image.getpixel((x, y))
            if pixel[3] == 0:
                print("0xFFFF", end=",")
            elif pixel == (255,255,255,255):
                print("0xFFFE", end=",")
            else:
                print(hex(torgb565(pixel[0], pixel[1], pixel[2])), end=",")

    print("\n};")


def converthex(rgb):
    if len(rgb) != 7 or rgb[0] != '#':
        sys.exit("ERROR: hex must be in form #FFFFFF")

    r = int(rgb[1:3], 16)
    g = int(rgb[3:5], 16)
    b = int(rgb[5:7], 16)

    ret = hex(torgb565(r, g, b))

    print(ret)


def main():
    parser = argparse.ArgumentParser(description='Convert a hex value or an image file into RGB565.')
    group = parser.add_mutually_exclusive_group(required=True)
    group.add_argument('-f', '--file', help='image file to convert')
    group.add_argument('--hex', help='color hex to convert')
    args = parser.parse_args()

    if args.hex is not None:
        converthex(args.hex)
    elif args.file is not None:
        convertimage(args.file)


if __name__ == "__main__":
    main()
