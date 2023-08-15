#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

//echo > PubxP7FB/p3XBYgjG
static char buf[512];
void find(char *path, char *filename) {
  char *p;
  char *pathP;
  int i, fd;
  struct dirent de;
  struct stat st;

  //printf("1 path=%s flie=%s\n", path, filename);

  if ((fd = open(path, 0)) < 0) {
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if (fstat(fd, &st) < 0) {
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch (st.type) {
    case T_FILE:    
    	
      pathP = path;
      for(i=strlen(path); i>0; i--)
      {
          if(path[i] == '/')
          {
             pathP = &path[i+1];
             break;
          }
      }
        
      if (strcmp(pathP, filename) == 0) {
        printf("%s\n", path);
      }
      else
      {
         //printf("6 path=%s flie=%s\n", path, filename);
      }
      break;

    case T_DIR:
      if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
        printf("find: path too long\n");
        break;
      }
      
      
      strcpy(buf, path);
      p = buf + strlen(path);
      *p++ = '/';
      //printf("2 buf=%s DIRSIZ=%d\n", buf, DIRSIZ);
      
      
      while (read(fd, &de, sizeof(de)) == sizeof(de)) {
        if (de.inum == 0) {
          continue;
        }
        
        //memmove(p, de.name, DIRSIZ);
        //p[DIRSIZ] = 0;           
        strcpy(p, de.name);     
        //printf("3 buf=%s\n", buf);
      
        if (stat(buf, &st) < 0) {
          printf("find: cannot stat %s\n", buf);
          continue;
        }
        
        if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) {
          continue;
        }  
        //printf("4 buf=%s flie=%s \n", buf, filename);
        find(buf, filename);
        //printf("5\n");
      }
      break;
  }

  close(fd);
}

int main(int argc, char * * argv) {
  if (argc != 3) {
    fprintf(2, "Usage: find <path> <filename>\n");
    exit(1);
  }

  //printf("argv[2]=%s\n",argv[2]);
  find(argv[1], argv[2]);

  exit(0);
}
