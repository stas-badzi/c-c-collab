#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <unistd.h>
#include <codecvt>

int main(int argc, char** argv) {
    if (argc < 2) return 1;
    std::wifstream in(argv[1]);
    in.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
    int width,height;
    in >> width >> height;
    struct {
        char32_t c;
        unsigned char f;
        unsigned char b;    
    } tab[height][width];
    for (size_t i = 0; i < height; i++)
    {
        in.get();
        for (size_t j = 0; j < width; j++)
        {
            wchar_t c=in.get(),f,b;
            in >> f >> b;
            tab[i][j].c = c;
            switch (f)
            {
            case L'-':
                tab[i][j].f = 16;
                break;
            case L'0':
                tab[i][j].f = 0;
                break;
            case L'1':
                tab[i][j].f = 1;
                break;
            case L'2':
                tab[i][j].f = 2;
                break;
            case L'3':
                tab[i][j].f = 3;
                break;
            case '4':
                tab[i][j].f = 4;
                break;
            case L'5':
                tab[i][j].f = 5;
                break;
            case L'6':
                tab[i][j].f = 6;
                break;
            case L'7':
                tab[i][j].f = 7;
                break;
            case L'8':
                tab[i][j].f = 8;
                break;
            case L'9':
                tab[i][j].f = 9;
                break;
            case L'A':
                tab[i][j].f = 10;
                break;
            case L'B':
                tab[i][j].f = 11;
                break;
            case L'C':
                tab[i][j].f = 12;
                break;
            case L'D':
                tab[i][j].f = 13;
                break;
            case L'E':
                tab[i][j].f = 14;
                break;
            case L'F':
                tab[i][j].f = 15;
                break;
            default:
                in.close();
                return 1;
            }
            switch (b)
            {
            case L'-':
                tab[i][j].b = 16;
                break;
            case L'0':
                tab[i][j].b = 0;
                break;
            case L'1':
                tab[i][j].b = 1;
                break;
            case L'2':
                tab[i][j].b = 2;
                break;
            case L'3':
                tab[i][j].b = 3;
                break;
            case '4':
                tab[i][j].b = 4;
                break;
            case L'5':
                tab[i][j].b = 5;
                break;
            case L'6':
                tab[i][j].b = 6;
                break;
            case L'7':
                tab[i][j].b = 7;
                break;
            case L'8':
                tab[i][j].b = 8;
                break;
            case L'9':
                tab[i][j].b = 9;
                break;
            case L'A':
                tab[i][j].b = 10;
                break;
            case L'B':
                tab[i][j].b = 11;
                break;
            case L'C':
                tab[i][j].b = 12;
                break;
            case L'D':
                tab[i][j].b = 13;
                break;
            case L'E':
                tab[i][j].b = 14;
                break;
            case L'F':
                tab[i][j].b = 15;
                break;
            default:
                in.close();
                return 1;
            }
        }
        
    }
    in.close();
    FILE* fl = fopen(argv[1],"w");
    if (!fl) perror("Write");
    fwrite(&width,sizeof(int),1,fl);
    fwrite(&height,sizeof(int),1,fl);
    for (size_t i = 0; i < height; i++)
    {
        for (size_t j = 0; j < width; j++) {
            fwrite(&(tab[i][j].c),sizeof(char32_t),1,fl);
            fwrite(&(tab[i][j].f),sizeof(unsigned char),1,fl);
            fwrite(&(tab[i][j].b),sizeof(unsigned char),1,fl);
        }

    }
    fclose(fl);
    return 0;
}