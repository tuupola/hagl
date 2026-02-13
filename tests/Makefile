CC = gcc
CFLAGS = -Wall -g -I. -I../include
LDFLAGS = -lm

SRCS = \
    hagl_hal.c \
    ../src/hagl_polygon.c \
    ../src/hagl_bitmap.c \
    ../src/hagl_hline.c \
    ../src/hagl_vline.c \
    ../src/hagl_pixel.c \
    ../src/hagl_color.c \
    ../src/hagl_clip.c \
    ../src/hagl_line.c \
    ../src/hagl.c \
    ../src/hagl_rectangle.c \
    ../src/hagl_triangle.c \
    ../src/rgb565.c

all: test_fill_polygon test_polygon test_fill_rectangle test_rectangle test_pixel

test_fill_polygon: test_fill_polygon.c $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

test_polygon: test_polygon.c $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

test_fill_rectangle: test_fill_rectangle.c $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

test_rectangle: test_rectangle.c $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

test_pixel: test_pixel.c $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

test: test_fill_polygon test_polygon test_fill_rectangle test_rectangle test_pixel
	./test_fill_polygon
	./test_polygon
	./test_fill_rectangle
	./test_rectangle
	./test_pixel

clean:
	rm -f test_fill_polygon test_polygon test_fill_rectangle test_rectangle test_pixel

.PHONY: all test clean
