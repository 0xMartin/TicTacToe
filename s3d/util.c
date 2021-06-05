/**
 * <------------------------------------------------------------------>
 * @name    2D Engine
 * @author  Martin Krcma
 * @date    6. 3. 2021
 * <------------------------------------------------------------------>
 * @file    util.c
 * @brief   Implementation of util.h
 * <------------------------------------------------------------------>
 */

#include "util.h"

#include <stdlib.h>
#include <string.h>


Texture * UTIL_loadTextureBMP(const char * path, bool rgba_mode) {
    if(strlen(path) == 0) return NULL;

    //header of the BMP file
    unsigned char header[54];
    //rgb data
    unsigned char * data;

    // Open the file
    FILE * file = fopen(path,"rb");
    if (!file){
        printf("Image could not be opened\n");
        return NULL;
    }
    if (fread(header, 1, sizeof(header), file) != 54){
        printf("Not a correct BMP file\n");
        return NULL;
    }
    if (header[0] != 'B' || header[1] != 'M'){
        printf("Not a correct BMP file\n");
        return NULL;
    }

    Texture * texture = malloc(sizeof(Texture));
    if(texture == NULL) return NULL;

    // Read ints from the byte array
    texture->imageSize = *(int*)&(header[0x22]);
    texture->width = *(int*)&(header[0x12]);
    texture->height = *(int*)&(header[0x16]);

    // Some BMP files are misformatted, guess missing information
    if (texture->imageSize == 0) texture->imageSize = texture->width * texture->height * (rgba_mode ? 4 : 3);

    // Create a buffer
    data = malloc(sizeof(unsigned char) * texture->imageSize);

    //read the actual data from the file into the buffer
    fread(data, 1, texture->imageSize, file);
    //everything is in memory now, the file can be closed
    fclose(file);

    //create one OpenGL texture
    glGenTextures(1, &texture->textureID);
    //"bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, texture->textureID);
    //give the image to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, rgba_mode ? GL_RGBA : GL_RGB, texture->width,
                 texture->height, 0, rgba_mode ? GL_BGRA : GL_BGR, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    return texture;
}

typedef struct {
    unsigned char chunk_length[4];
    unsigned char chunk_type[4];
    unsigned char * data;
    unsigned char crc[4];
} PNG_CHUNK;

Texture * UTIL_loadTexturePNG(const char * path) {
    if(strlen(path) == 0) return NULL;

    //header of the PNG file (8B)
    unsigned char signature[8];

    //open file
    FILE * file = fopen(path,"rb");
    if (!file){
        printf("Image could not be opened\n");
        return NULL;
    }

    //read header from file
    if (fread(signature, 1, sizeof(signature), file) != 54){
        printf("Not a correct PNG file\n");
        return NULL;
    }

    //check 8 bit png signature
    if (signature[0] != 0x89 ||
            signature[1] != 0x50 ||
            signature[2] != 0x4E ||
            signature[3] != 0x47 ||
            signature[4] != 0x0D ||
            signature[5] != 0x0A ||
            signature[6] != 0x1A ||
            signature[7] != 0x0A){
        printf("Not a correct PNG file\n");
        return NULL;
    }

    Texture * texture = malloc(sizeof(Texture));


    /*
    //create one OpenGL texture
    glGenTextures(1, &texture->textureID);
    //"bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, texture->textureID);
    //give the image to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->width,
                 texture->height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    */
    return texture;
}

int UTIL_randInt(int n) {
    if ((n - 1) == RAND_MAX) {
        return rand();
    } else {
        int end = RAND_MAX / n;
        end *= n;
        int r;
        while((r = rand()) >= end);
        return r % n;
    }
}

__syscall_slong_t UTIL_getSystemNanoTime() {
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    return ts.tv_nsec;
}

__time_t UTIL_getSystemSeconds() {
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    return ts.tv_sec;
}

struct timespec UTIL_getSystemTime() {
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    return ts;
}

void UTIL_simpleDestructor(void * data) {
    if(data != NULL) {
        free(data);
    }
}

