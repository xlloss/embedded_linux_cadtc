// SPDX-License-Identifier: GPL-2.0-or-later
/* PWM Buzzer Test On Linux
 *
 * 2012 Slash <slash.linux.c@gmail.com>
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <getopt.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

#define DEVF "/sys/class/pwm/pwmchip"
#define DEVF_EXPORT "export"
#define DEVF_UNEXPORT "unexport"
#define DEVF_PWM_ENABLE "enable"
#define DEVF_PWM_PERIOD "period"
#define DEVF_PWM_DUTY "duty_cycle"
#define DEFAULT_DUTY 0
#define DEFAULT_PERIOD 1000
#define GB 1000000000

/*
 * Note		Frequency (Hz)	Wavelength (cm)
 *
 * 	C4			261.63			131.87
 *	C#4/Db4 	277.18			124.47
 *	D4			293.66			117.48
 *	D#4/Eb4 	311.13			110.89
 *	E4			329.63			104.66
 *	F4			349.23			98.79
 *	F#4/Gb4 	369.99			93.24
 *	G4			392.00			88.01
 *	G#4/Ab4 	415.30			83.07
 *	A4			440.00			78.41
 *	A#4/Bb4 	466.16			74.01
 *	B4			493.88			69.85
 *  C5			523.25			65.93
 */

int meldy_freq[8] = {261, 293, 329, 349, 392, 440, 493, 523};
#define MELDY_DO 261
#define MELDY_RE 229
#define MELDY_ME 329
#define MELDY_FA 349
#define MELDY_SO 392
#define MELDY_LA 440
#define MELDY_SI 493
#define MELDY_DO_H 523

struct sys_pwm {
	int port;
	int channel;
	int enable;
	char *con_buf;
    char *con_path;
};

int pwm_chan_export(struct sys_pwm *pwm)
{
    int fd;
    char writebuf[5];
    int w_len;
    int pwm_port, pwm_channel;
    char *conbuf;

    conbuf = pwm->con_buf;
    pwm_port = pwm->port;
    pwm_channel = pwm->channel;

    sprintf(conbuf, "%s%d/%s", DEVF,
        pwm_port, DEVF_EXPORT);

    fd = open(conbuf, O_WRONLY);
    if (fd < 0) {
        perror("open");
        return 0;
    }


    sprintf(writebuf, "%d", pwm_channel);
    w_len = write(fd, writebuf, 1);
    if (w_len < 0)
        perror("write");

    close(fd);
}

int pwm_chan_unexport(struct sys_pwm *pwm)
{
    int fd;
    char writebuf[5];
    int w_len;
    int pwm_channel, pwm_port;
    char *conbuf;

    conbuf = pwm->con_buf;
    pwm_channel = pwm->channel;
    pwm_port = pwm->port;
    sprintf(conbuf, "%s/%d/%d/%s", DEVF,
        pwm_port, pwm_channel, DEVF_UNEXPORT);

    fd = open(conbuf, O_WRONLY);
    if (fd < 0) {
        perror("open");
        return 0;
    }

    sprintf(writebuf, "0");
    w_len = write(fd, writebuf, 1);
    if (w_len < 0)
        perror("write");

    close(fd);
}

int pwm_set_duty(struct sys_pwm *pwm, int duty_time)
{
    int fd;
    char writebuf[20];
    int w_len;
    int pwm_channel, pwm_port;
    char *conbuf;

    conbuf = pwm->con_buf;
    pwm_channel = pwm->channel;
    pwm_port = pwm->port;
    sprintf(conbuf, "%s/%s", pwm->con_path, DEVF_PWM_DUTY);


    fd = open(conbuf, O_WRONLY);
    if (fd < 0) {
        perror("open");
        return 0;
    }

    sprintf(writebuf, "%d", duty_time);
    w_len = write(fd, writebuf, strlen(writebuf));
    if (w_len < 0)
        perror("write");

    close(fd);
}

int pwm_set_period(struct sys_pwm *pwm, int period_time)
{
    int fd;
    char writebuf[20];
    int w_len;
    int pwm_channel, pwm_port;
    char *conbuf;

    conbuf = pwm->con_buf;
    pwm_channel = pwm->channel;
    pwm_port = pwm->port;

    sprintf(conbuf, "%s/%s", pwm->con_path, DEVF_PWM_PERIOD);

    fd = open(conbuf, O_WRONLY);
    if (fd < 0) {
        perror("open");
        return 0;
    }

    sprintf(writebuf, "%d", period_time);
    w_len = write(fd, writebuf, strlen(writebuf));
    if (w_len < 0)
        perror("write");

    close(fd);
}

int pwm_set_enable(struct sys_pwm *pwm)
{
    int fd;
    char writebuf[20];
    int w_len;
    int pwm_channel, pwm_port, enable;
    char *conbuf;

    conbuf = pwm->con_buf;
    pwm_channel = pwm->channel;
    pwm_port = pwm->port;

    sprintf(conbuf, "%s/%s", pwm->con_path, DEVF_PWM_ENABLE);

    fd = open(conbuf, O_WRONLY);
    if (fd < 0) {
        perror("open");
        return 0;
    }

    enable = pwm->enable;
    sprintf(writebuf, "%d", enable);
    w_len = write(fd, writebuf, strlen(writebuf));
    if (w_len < 0)
        perror("write");

    close(fd);
}

int pwm_get_enable(struct sys_pwm *pwm)
{
    int fd;
    char readbuf[20];
    int r_len;
    int pwm_channel, pwm_port, enable;
    char *conbuf;

    conbuf = pwm->con_buf;
    pwm_channel = pwm->channel;
    pwm_port = pwm->port;

    sprintf(conbuf, "%s/%s", pwm->con_path, DEVF_PWM_ENABLE);

    fd = open(conbuf, O_WRONLY);
    if (fd < 0) {
        perror("open");
        return 0;
    }

    r_len = read(fd, readbuf, 1);
    if (r_len < 0)
        perror("read");

    close(fd);

    pwm->enable = atoi(readbuf);
}

#ifdef PWM_TEST
int main(int argc, char** argv)
{
    int port, duty, channel, period, start, freq;
    int set_duty = 0, set_period = 0;
    int opts;
    int option_index = 0;
    char *pwm_ch = "0\r\n";
    char pwm_path[50];
    char pwm_con_buf[50];
    int i, ret;
    struct sys_pwm control_pwm;

    /* getopt_long stores the option index here. */
    struct option long_options[] = {
        {"freq", required_argument, 0, 'f'},
        {"duty", required_argument, 0, 'd'},
        {"port", required_argument, 0, 'o'},
        {"channel", required_argument, 0, 'c'},
        {"?",  no_argument, 0, '?'},
        {0, 0, 0, 0}
    };

    while (1) {
        opts = getopt_long (argc, argv,
        "f:d:o:c:", long_options, &option_index);

        /* Detect the end of the options. */
        if (opts == -1)
            break;

        switch (opts) {
        case 'f':
            freq = atoi(optarg);
            set_period = 1;
            break;

        case 'd':
            duty = atoi(optarg);
            set_duty = 1;
            break;

        case 'o':
            port = atoi(optarg);
            break;

        case 'c':
            channel = atoi(optarg);
            break;

        case '?':
        default:
            printf("help");
            break;
        }
    }

    control_pwm.port = port;
    control_pwm.channel = channel;
    control_pwm.con_buf = &pwm_con_buf[0];

    sprintf(pwm_path, "%s%d/pwm%d", DEVF, port, channel);
    control_pwm.con_path = pwm_path;

    ret = access(pwm_path, R_OK | W_OK);
    if (ret) {
        pwm_chan_export(&control_pwm);
    }

	control_pwm.enable = 0;
	pwm_set_enable(&control_pwm);

	period = GB / freq;
	pwm_set_duty(&control_pwm, DEFAULT_DUTY);
	pwm_set_period(&control_pwm, DEFAULT_PERIOD);

    if (set_period == 1) {
		/*
		 * period = (1 / freq) * 10^9
		 * period(ns) = (1 / freq)
		 */
        pwm_set_period(&control_pwm, period);
	}

    if (set_duty == 1) {
		/*
		 * duty = period * proportion(%)
		 */
        pwm_set_duty(&control_pwm, (duty * period) / 100);
	}

	control_pwm.enable = 1;
	pwm_set_enable(&control_pwm);

    return 0;
}
#else

void set_pwm(int freq, int duty)
{
	struct sys_pwm control_pwm;
	char pwm_con_buf[50];
	char pwm_path[50];
	int period, ret;

	printf("%s-freq %d\n", __func__, freq);
    control_pwm.port = 1;
    control_pwm.channel = 0;
    control_pwm.con_buf = &pwm_con_buf[0];

    sprintf(pwm_path, "%s%d/pwm%d", DEVF,
		control_pwm.port, control_pwm.channel);
    control_pwm.con_path = pwm_path;

    ret = access(pwm_path, R_OK | W_OK);
    if (ret) {
        pwm_chan_export(&control_pwm);
    }

	control_pwm.enable = 0;
	pwm_set_enable(&control_pwm);

	period = GB / freq;
	pwm_set_duty(&control_pwm, DEFAULT_DUTY);
	pwm_set_period(&control_pwm, DEFAULT_PERIOD);
    pwm_set_period(&control_pwm, period);
    pwm_set_duty(&control_pwm, (duty * period) / 100);

	control_pwm.enable = 1;
	pwm_set_enable(&control_pwm);

}

void enable_buzzer(int enable)
{
	struct sys_pwm control_pwm;
	char pwm_con_buf[50];
	char pwm_path[50];
	int period, ret;

    control_pwm.port = 1;
    control_pwm.channel = 0;
    control_pwm.con_buf = &pwm_con_buf[0];

    sprintf(pwm_path, "%s%d/pwm%d", DEVF,
		control_pwm.port, control_pwm.channel);
    control_pwm.con_path = pwm_path;
	control_pwm.enable = enable;

    ret = access(pwm_path, R_OK | W_OK);
    if (ret) {
        pwm_chan_export(&control_pwm);
    }

	pwm_set_enable(&control_pwm);
}

void play_ff_voctory()
{
	set_pwm(MELDY_DO_H, 10);
	usleep(500000);
	enable_buzzer(0);
	usleep(100000);

	set_pwm(MELDY_DO_H, 10);
	usleep(200000);
	enable_buzzer(0);
	usleep(100000);

	set_pwm(MELDY_DO_H, 10);
	usleep(500000);
	enable_buzzer(0);
	usleep(100000);

	set_pwm(MELDY_SO, 10);
	usleep(500000);
	enable_buzzer(0);
	usleep(100000);

	set_pwm(MELDY_LA, 10);
	usleep(500000);
	enable_buzzer(0);
	usleep(100000);

	set_pwm(MELDY_DO_H, 10);
	usleep(500000);
	enable_buzzer(0);
	usleep(100000);

	set_pwm(MELDY_SI, 10);
	usleep(100000);
	enable_buzzer(0);
	usleep(10000);

	set_pwm(MELDY_DO_H, 10);
	usleep(500000);
	enable_buzzer(0);
	usleep(100000);
}

void for_sys_int(int sig)
{
	printf("for_sys_int\n");
	enable_buzzer(0);
	exit(0);
}

int main(int argc, char** argv)
{
	int i;

	signal(SIGINT, for_sys_int);
	enable_buzzer(1);
	//for (i = 0; i < 8; i++) {
	//	set_pwm(meldy_freq[i], 10);
	//	usleep(300000);
	//}
	play_ff_voctory();
	enable_buzzer(0);

	return 0;
}
#endif

