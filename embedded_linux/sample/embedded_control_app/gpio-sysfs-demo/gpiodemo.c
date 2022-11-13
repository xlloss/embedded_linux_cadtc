/*******************************************************************************
* gpiodemo.c
*
* A very simple demo of using the GPIO sysfs interface under Linux by using
* gpiolib library from Technologic Systems.  This can be applied generically to
* any computer utilizing the GPIO sysfs interface.  This specific code block was
* written on a TS-7970, where gpio_pin #59 is connected to a breadboard LED. One
* could also use `gcc -D CTL gpiolib.c -o gpioctl` or `make` to bypass the need
* for this file and use ./gpioctl instead for scripting or use from the shell
* (see gpioctl --help).
*
* Functions provided by gpiolib:
*   - int gpio_export(int gpio);
*   - int gpio_direction(int gpio, int dir);
*   - void gpio_unexport(int gpio);
*   - int gpio_read(int gpio);
*   - int gpio_write(int gpio, int val);
*   - int gpio_setedge(int gpio, int rising, int falling);
*   - int gpio_select(int gpio);
*   - int gpio_getfd(int gpio);
*
* Sources:
*   - https://github.com/embeddedarm/ts4900-utils/blob/master/src/gpiolib.h
*   - https://github.com/embeddedarm/ts4900-utils/blob/master/src/gpiolib.c
*   - http://wiki.embeddedarm.com/wiki/TS-7970#GPIO
*   - https://www.kernel.org/doc/Documentation/gpio/sysfs.txt
* *******************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <unistd.h>
#include "gpiolib.h"

#define GPIO(n) n
#define LED_0 GPIO(75)
#define LED_1 GPIO(146)
#define LED_2 GPIO(150)
#define LED_3 GPIO(149)

#define LED_G GPIO(13)

#define LED_TEST_NUM 4

int led_set[4] = {LED_0, LED_1, LED_2, LED_3};
//int led_set[1] = {LED_0};

pthread_mutex_t mutex;

void *thr_led_green (void *led)
{
    int *setled;

    setled = (int *)led;
    pthread_mutex_lock (&mutex);
    gpio_export(*setled);
    gpio_direction(*setled, GPIO_OUT);
    pthread_mutex_unlock (&mutex);

    while (1) {
        pthread_mutex_lock (&mutex);
        gpio_write(*setled, GPIO_HI);
        pthread_mutex_unlock (&mutex);
        sleep(1);
        pthread_mutex_lock (&mutex);
        gpio_write(*setled, GPIO_LO);
        pthread_mutex_unlock (&mutex);
        sleep(1);
    };

    return NULL;
}

void *thr_led0 (void *led)
{
    int *setled;

    setled = (int *)led;
    pthread_mutex_lock (&mutex);
    gpio_export(*setled);
    gpio_direction(*setled, GPIO_OUT);
    pthread_mutex_unlock (&mutex);

    while (1) {
        pthread_mutex_lock (&mutex);
        gpio_write(*setled, GPIO_HI);
        pthread_mutex_unlock (&mutex);
        sleep(1);
        pthread_mutex_lock (&mutex);
        gpio_write(*setled, GPIO_LO);
        pthread_mutex_unlock (&mutex);
        sleep(1);
    };

    return NULL;
}

void *thr_led1 (void *led)
{
    int *setled;

    setled = (int *)led;
    pthread_mutex_lock (&mutex);
    gpio_export(*setled);
    gpio_direction(*setled, GPIO_OUT);
    pthread_mutex_unlock (&mutex);

    while (1) {
        pthread_mutex_lock (&mutex);
        gpio_write(*setled, GPIO_HI);
        pthread_mutex_unlock (&mutex);
        sleep(3);
        pthread_mutex_lock (&mutex);
        gpio_write(*setled, GPIO_LO);
        pthread_mutex_unlock (&mutex);
        sleep(5);
    };

    return NULL;
}

void *thr_led2 (void *led)
{
    int *setled;

    setled = (int *)led;
    pthread_mutex_lock (&mutex);
    gpio_export(*setled);
    gpio_direction(*setled, GPIO_OUT);
    pthread_mutex_unlock (&mutex);

    while (1) {
        pthread_mutex_lock (&mutex);
        gpio_write(*setled, GPIO_HI);
        pthread_mutex_unlock (&mutex);
        usleep(80000);
        pthread_mutex_lock (&mutex);
        gpio_write(*setled, GPIO_LO);
        pthread_mutex_unlock (&mutex);
        usleep(80000);
    };

    return NULL;
}

void *thr_led3 (void *led)
{
    int *setled;

    setled = (int *)led;
    pthread_mutex_lock (&mutex);
    gpio_export(*setled);
    gpio_direction(*setled, GPIO_OUT);
    pthread_mutex_unlock (&mutex);
    while (1) {
        pthread_mutex_lock (&mutex);
        gpio_write(*setled, GPIO_HI);
        pthread_mutex_unlock (&mutex);
        sleep(6);
        pthread_mutex_lock (&mutex);
        gpio_write(*setled, GPIO_LO);
        pthread_mutex_unlock (&mutex);
        sleep(6);
    };

    return NULL;
}


void *(*start_routine[4])(void *);

int main(int argc, char **argv)  {
    int i, led;
    pthread_t thread[4];

    pthread_mutex_init (&mutex, NULL);


    start_routine[0] = thr_led0;
    start_routine[1] = thr_led1;
    start_routine[2] = thr_led2;
    start_routine[3] = thr_led3;


    //start_routine[0] = thr_led0;

    for (i = 0; i < LED_TEST_NUM; i++) {
        led = led_set[i];
        printf("led %d\r\n", led);
        pthread_create (&thread[i], NULL, (*start_routine[i]), &led_set[i]);
    }

    while (1) {
        usleep(5000);
    };

    return 0;
}
