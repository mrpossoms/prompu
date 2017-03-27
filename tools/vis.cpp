#include <math.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ncurses.h>
#include <termios.h>
#include <fcntl.h>
#include <errno.h>
#include "tracer.h"

using namespace Tracer;

struct termios OLD_SETTINGS;

int open_serial(const char* dev, speed_t baud)
{
	int fd = open(dev, O_RDWR);
	struct termios ps;

	if(fd < 0) return -1;

	tcgetattr(fd, &ps);
	OLD_SETTINGS = ps;

	cfmakeraw(&ps);
	ps.c_lflag = ICANON;
	ps.c_iflag = 0;
	ps.c_oflag = OPOST;
	ps.c_cflag = CS8 | CREAD | HUPCL | CLOCAL;
	cfsetspeed(&ps, baud);
	tcsetattr(fd, TCSANOW, &ps);

	return fd;
}

int main(int argc, char* argv[])
{
	BufferInfo info = {
		.pixel_format = tr_fmt_rgb,
	};

	Light light0;
	Plastic plastic;

	// Sphere subject(Vec3(0, 0, 4), 1);
	Plane subject(Vec3(0, 0, 4), Vec3(-0.75f, -0.75f, 0), Vec3(0.75f, 0.75f, 0));
	Sphere sphere1(Vec3(0, 0, 80), 70);

	subject.material = &plastic;
	sphere1.material = &light0;

	Surface* surfaces[] = { &subject, &sphere1, NULL,};
	Scene scene = {
		.view = Viewer(1, 100, M_PI / 2, 1),
		.surfaces = surfaces,
	};

	int serial = open_serial(argv[1], atoi(argv[2]));
	initscr(); cbreak(); noecho();

	float t = 0;
	Quat w, q;

	while(1)
	{
		char s_buf[256] = {};
		ssize_t len = read(serial, s_buf, 256);
		sscanf(s_buf, "%f %f %f %f\n", q.v, q.v + 1, q.v + 2, q.v + 3);

		getmaxyx(stdscr, info.height, info.width);
		uint8_t buf[info.width * info.height * 3];
		info.buffer = buf;

		//q = q * w;
		subject.setOrientation(q);

		trace(&scene, info);

		for(int i = 0; i < info.height; ++i)
		{
			uint8_t* row = buf + (info.width * i * 3);
			move(i, 0);

			for(int j = info.width; j--;)
			{
				// write(1, row + (j * 3), 1);
				addch(row[j * 3]);
			}
		}

		refresh();
		// usleep(1);
	}

	return 0;
}
