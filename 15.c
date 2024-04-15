#include <stdio.h>
#include <string.h>

void Extract(char *archive){
    FILE *arch = fopen(archive, "rb+wb");
    unsigned long long int now_position = 0;
    unsigned long long int start_position = 0;
    int c;
    while ((c = getc(arch)) != EOF){
        start_position++;
        if (c == '\n'){
            break;
        }
    }
    //printf("%d",start_position);
    fseek(arch, 0, SEEK_SET);    
    char filename[128] = {0};
    unsigned long long int filesize;
    FILE *file;
    while (fscanf(arch, "< %s : %llu >", filename, &filesize) != 0){
        file = fopen(filename, "wb");
        if (file == NULL){
            break;
        }
        now_position = ftell(arch);
        fseek(arch, start_position, SEEK_SET);
        start_position += filesize;
        while (filesize-- > 0){
            putc((c = getc(arch)), file);
        }
        fseek(arch, now_position, SEEK_SET);
        fclose(file);
    }
    printf("Ready");
}

void List(char *archive){
    FILE *arcfile = fopen(archive, "rb");
    char filename[128] = {0};
    unsigned long long int filesize;
    printf("List:\n");
    while (fscanf(arcfile, "< %s : %llu >", filename, &filesize) != 0){
        printf("%s\n", filename);
    }
    fclose(arcfile);
}

void Create(char *archive, int filecount, char *files[]){
    int temp;
    FILE *arcfile = fopen(archive, "wb");
    FILE *file;
    unsigned long long int nameandsize[128];
    for (int i = 4; i < filecount; i++){
        file = fopen(files[i], "rb");
        if (file == NULL){
            continue;
        }
        fseek(file, 0, SEEK_END);
        nameandsize[i - 4] = ftell(file);
        fseek(file, 0, SEEK_SET);
        fclose(file);
        
    }
    for (int i = 0; i < filecount - 4; i++){
        fprintf(arcfile, "< %s : %llu >", files[i + 4], nameandsize[i]);
    }
    fprintf(arcfile, "\n");
    for (int i = 4; i < filecount; i++){
        file = fopen(files[i], "rb");
        if (file == NULL){
            printf("ERROR\n");
            continue;
        }else{
            printf("Ready\n");
        }
        while ((temp = getc(file)) != EOF){
            putc(temp, arcfile);
        }
        fclose(file);
    }
}

int main(int argc, char *argv[]){
    FILE *arcfile;
    char *archive;
    for (int i = 0; i < argc; i++){
        if (!strcmp("--file", argv[i])){
            archive = argv[i + 1];
        }else if (!strcmp("--create", argv[i])){
            Create(archive, argc, argv);
            break;
        }else if (!strcmp("--extract", argv[i])){
            Extract(archive);
        }else if (!strcmp("--list", argv[i])){
            List(archive);
        }
    }
}
// gcc -o myprogram 15.c
//./myprogram --file arc --create f1.txt f2.txt
//./myprogram --file arc --extract
//./myprogram --file arc --list