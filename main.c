#include <stdio.h>
#include <string.h>

//CLionProjects\laba4C\cmake-build-debug\laba4C.exe --filepath="C:\Users\gstri\CLionProjects\laba4C\cmake-build-debug\music.mp3" --show
//CLionProjects\laba4C\cmake-build-debug\laba4C.exe --filepath="C:\Users\gstri\CLionProjects\laba4C\cmake-build-debug\music.mp3" --get=TIT2

unsigned int convert(unsigned int byte) {
    return(((byte >> 24) & 0x000000ff) | ((byte >> 8) & 0x0000ff00) | ((byte << 8) & 0x00ff0000) | ((byte << 24) & 0xff000000));
}

struct tag {
    unsigned char name[3];
    unsigned char v1;
    unsigned char v2;
    unsigned char flag;
    unsigned int size;
};

struct frame {
    unsigned char name[4];
    unsigned int size;
    unsigned char flag1;
    unsigned char flag2;
};

void show_frames(char* filename) {
    FILE *file;
    file = fopen(filename, "rb");
    struct tag head;
    fread(&head, 1, 10, file);
    printf("%sv%d.%d\n", head.name, head.v1, head.v2);
    struct frame new_frame;
    fread(&new_frame, 1, 10, file);
    while (ftell(file) < convert(head.size) + 10 && new_frame.name[0] != 0) {
        int converted_new_frame_size = convert(new_frame.size);
        printf("%s ----- ", new_frame.name);
        char frame_content;
        for (int i = 0; i < converted_new_frame_size; i++) {
            frame_content = fgetc(file);
            printf("%c", frame_content);
        }
        printf("\n");
        fread(&new_frame, 1, 10, file);
    }
}

void get_frame(char* filename, char* framename) {
    FILE *file;
    file = fopen(filename, "rb");
    struct tag head;
    fread(&head, 1, 10, file);
    struct frame new_frame;
    fread(&new_frame, 1, 10, file);
    while (ftell(file) < convert(head.size) + 10 && new_frame.name[0] != 0 && strcmp(framename, new_frame.name) != 0) {
        int converted_new_frame_size = convert(new_frame.size);
        char frame_content;
        for (int i = 0; i < converted_new_frame_size; i++) {
            frame_content = fgetc(file);
        }
        fread(&new_frame, 1, 10, file);
    }
    int converted_new_frame_size = convert(new_frame.size);
    char frame_content;
    printf("%s ----- ", new_frame.name);
    for (int i = 0; i < converted_new_frame_size; i++) {
        frame_content = fgetc(file);
        printf("%c", frame_content);
    }
    printf("\n");
}

int main(int argc, char* argv[]) {
    char* file_name;
    file_name = strpbrk(argv[1], "=") + 1;
    if (strcmp(argv[2], "--show") == 0) {
        show_frames(file_name);
    } else if (argv[3] == NULL) {
        char* frame_name;
        frame_name = strpbrk(argv[2], "=") + 1;
        get_frame(file_name, frame_name);
    }
    return 0;
}