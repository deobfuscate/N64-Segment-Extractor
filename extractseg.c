#include <stdio.h>
#include <string.h>

#define EXIT_SUCCESS 0
#define EXIT_FAILURE -1
#define START_TOKEN "_%sSegmentRomStart"
#define END_TOKEN "_%sSegmentRomEnd"
#define OUT_FILE "%s.seg"

int main(int argc, char** argv) {
    if (argc < 3) {
        printf("Usage: extractseg <binary file> <symbol to extract>, symbol table as stdin");
        printf("Example: nm game_sym.out | exctactseg game.n64 code");
        return EXIT_FAILURE;
    }

    int addr = 0, start = -1, end = -1;
    char start_str[BUFSIZ], end_str[BUFSIZ], line[BUFSIZ], type[BUFSIZ], symbol[BUFSIZ];
    sprintf(start_str, START_TOKEN, argv[2]);
    sprintf(end_str, END_TOKEN, argv[2]);

    while (fgets(line, sizeof(line), stdin) &&
        (sscanf(line, "%x %s %s\n", &addr, type, symbol) == 3 || sscanf(line, "%s %s\n", type, symbol) == 2)) {
        if (strcmp(symbol, start_str) == 0)
            start = addr;
        if (strcmp(symbol, end_str) == 0)
            end = addr;
    }

    if (start == -1 || end == -1) {
        printf("Couldn't find segment '%s'", argv[2]);
        return EXIT_FAILURE;
    }
    int seg_size = (end - start);
    printf("Start: 0x%x, End: 0x%x, Size: %d bytes\n", start, end, seg_size);

    // read input file
    FILE* fp;
    char* file_buffer;
    if ((fp = fopen(argv[1], "rb")) == NULL) {
        printf("Error opening file %s", argv[1]);
        return EXIT_FAILURE;
    }
    fseek(fp, start, SEEK_SET);
    file_buffer = (char*)malloc(sizeof(char) * seg_size);
    fread(file_buffer, sizeof(char), seg_size, fp);
    fclose(fp);

    // write output file
    char fppath[BUFSIZ];
    sprintf(fppath, OUT_FILE, argv[2]);
    fp = fopen(fppath, "wb");
    if (!fp) {
        printf("Error opening file %s.seg", argv[2]);
        return EXIT_FAILURE;
    }
    fwrite(file_buffer, sizeof(char), seg_size, fp);
    fclose(fp);

    printf("Wrote file %s.seg", argv[2]);
    return EXIT_SUCCESS;
}