#include <sys/ioctl.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

void print_page(FILE *file, struct winsize *ws);
void flip_pages(FILE *file, struct winsize *ws, int page);

void print_page(FILE *file, struct winsize *ws)
{
    char *buffer = malloc(sizeof(char) * (ws->ws_col + 1));
    for (int i = 0; i < ws->ws_row && !feof(file); i++)
   {
        fgets(buffer, ws->ws_col, file);
        printf("%s", buffer);
    }
    free(buffer);
}

void goAround(FILE *file, struct winsize *ws, int page)
{
    rewind(file);
    if (file == 0)
      return;
    char c;
    int count = 0;
    while (count <= page)
    {
        system("clear");
        print_page(file, ws);
        count++;
    }
    c = getchar();
    if (c == '\n' && !feof(file))
    {
        goAround(file, ws, page + 1);
    }
    else if (c == 'b' && page > 0)
    {
        while (c != '\n')
            c = getchar();
        goAround(file, ws, page - 1);
    }
    else if (c != '\n')
    {
        while (c != '\n')
            c = getchar();
        goAround(file, ws, page);
    }

}


int main(int argc, char *argv[]) {
  FILE* fd = fopen(argv[1],"r");
  struct winsize ws;
  if (ioctl(0,TIOCGWINSZ,&ws)!=0) {
    fprintf(stderr,"TIOCGWINSZ:%s\n",strerror(errno));
    exit(-1);
  }
  flip_pages(fd, &ws, 0);
  printf("row=%d, col=%d, xpixel=%d, ypixel=%d\n",
  ws.ws_row,ws.ws_col,ws.ws_xpixel,ws.ws_ypixel);


  //printf("%s\n",argv[1]); nur zum probieren ob es die richtige File nimmt.

  fclose(fd);
  return 0;
}
