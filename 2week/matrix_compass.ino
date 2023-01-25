# SPDX-FileCopyrightText: 2021 ladyada for Adafruit Industries
# SPDX-License-Identifier: MIT

""" Display compass heading data five times per second """
import time
from math import atan2, degrees
import board
import adafruit_lis3mdl
from adafruit_ht16k33 import matrix

i2c = board.I2C()
sensor = adafruit_lis3mdl.LIS3MDL(i2c)
m = matrix.Matrix8x8(board.I2C(), 0x70)

def vector_2_degrees(x, y):
    angle = degrees(atan2(y, x))
    if angle < 0:
        angle += 360
    return angle


def get_heading(_sensor):
    magnet_x, magnet_y, _ = _sensor.magnetic
    return vector_2_degrees(magnet_x, magnet_y)

def rotated(a):
    n = len(a)
    m = len(a[0])
    result = [[0] * n for _ in range(m)]
    for i in range(n):
        for j in range(m):
            result[j][n-i-1] = a[i][j]
    return result
    
def update_matrix(m, s):
    for i in range(8):
        for j in range(8):
            m[i, j] = s[i][j]

arrow = [[0] * 8 for _ in range(8)]
diagonal_arrow = [[0] * 8 for _ in range(8)]
for i in range(8):
    for j in range(8):
        if i == j:
            diagonal_arrow[i][j] = 1
        if j == 3 or j == 4:
            arrow[i][j] = 1
diagonal_arrow[0][1] = diagonal_arrow[1][0] = 1
arrow[1][2] = arrow[1][5] = 1

while True:
    angle = get_heading(sensor)
    print("heading: {:.2f} degrees".format(angle))
    if 315 <= angle or angle <= 45:
        update_matrix(m, arrow)
    elif angle <= 90:
        update_matrix(m, rotated(arrow))
    else:
        m.fill(0)

    time.sleep(0.2)
