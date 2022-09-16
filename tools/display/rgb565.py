#!/usr/bin/env python3
import sys
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


if sys.argv[1] == "fromimage":
    for i in range(2, len(sys.argv)):
        convertimage(sys.argv[i])
elif sys.argv[1] == "fromrgb":
    print(hex(torgb565(int(sys.argv[2], 16), int(sys.argv[3], 16), int(sys.argv[4], 16))))
