CC = gcc
CFLAGS = -Wall -g -I. -I../include
LDFLAGS = -lm

SRCS = \
    ../src/hagl_polygon.c \
    ../src/hagl_bitmap.c \
    ../src/hagl_hline.c \
    ../src/hagl_vline.c \
    ../src/hagl_pixel.c \
    ../src/hagl_color.c \
    ../src/hagl_clip.c \
    ../src/hagl_line.c \
    ../src/hagl_rectangle.c \
    ../src/hagl_triangle.c \
    ../src/hagl_circle.c \
    ../src/hagl_ellipse.c \
    ../src/hagl_blit.c \
    ../src/rgb565.c

all: test_fill_polygon test_polygon test_fill_rectangle test_rectangle test_pixel test_line test_hline test_vline test_circle test_ellipse test_fill_ellipse test_clip test_blit test_fontx test_char

test_fill_polygon: test_fill_polygon.c save_image.c $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

test_polygon: test_polygon.c save_image.c $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

test_fill_rectangle: test_fill_rectangle.c save_image.c $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

test_rectangle: test_rectangle.c save_image.c $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

test_pixel: test_pixel.c save_image.c $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

test_line: test_line.c save_image.c $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

test_hline: test_hline.c save_image.c $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

test_vline: test_vline.c save_image.c $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

test_circle: test_circle.c save_image.c $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

test_ellipse: test_ellipse.c save_image.c $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

test_fill_ellipse: test_fill_ellipse.c save_image.c $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

test_clip: test_clip.c $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

test_blit: test_blit.c save_image.c $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

test_fontx: test_fontx.c ../src/fontx.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

test_char: test_char.c save_image.c ../src/hagl_char.c ../src/fontx.c $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

test: test_fill_polygon test_polygon test_fill_rectangle test_rectangle test_pixel test_line test_hline test_vline test_circle test_ellipse test_fill_ellipse test_clip test_blit test_fontx test_char
	./test_fill_polygon
	./test_polygon
	./test_fill_rectangle
	./test_rectangle
	./test_pixel
	./test_line
	./test_hline
	./test_vline
	./test_circle
	./test_ellipse
	./test_fill_ellipse
	./test_clip
	./test_blit
	./test_fontx
	./test_char

clean:
	rm -f test_fill_polygon test_polygon test_fill_rectangle test_rectangle test_pixel test_line test_hline test_vline test_circle test_ellipse test_fill_ellipse test_clip test_blit test_fontx test_char
	rm -rf output

.PHONY: all test clean
