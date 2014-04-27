#include <cstdlib>
#include <cstdio>

int main(int argc, char** argv) {
    if (argc < 4) {
        fprintf(stderr, "USAGE: %s {sym} {in} {out}\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *sym = argv[1];
    FILE *in = fopen(argv[2], "rb");
    FILE *out = fopen(argv[3], "w");

    if (!in || !out) {
        fprintf(stderr, "Error while opening the file.\n");
        return EXIT_FAILURE;
    }

    fprintf(out, "#include <stdlib.h>\n");
    fprintf(out, "const char %s[] = {\n", sym);

    unsigned char buf[256];
    size_t nread = 0;
    size_t linecount = 0;

    do {
        nread = fread(buf, 1, sizeof(buf), in);
        for (size_t i = 0; i < nread; ++i) {
            fprintf(out, "0x%02x, ", buf[i]);

            if (++linecount % 10 == 0) {
                fprintf(out, "\n");
            }
        }
    } while (nread > 0);

    if (linecount % 10) {
        fprintf(out, "\n");
    }

    fprintf(out, "};\n");
    fprintf(out, "const size_t %s_len = sizeof(%s);\n\n", sym, sym);

    fclose(in);
    fclose(out);

    return EXIT_SUCCESS;
}
