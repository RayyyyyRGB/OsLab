#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MAXARG 32

int main(int argc, char *argv[]) {
  char buf[513];
  char rcv[513];
  char *args[MAXARG];
  int i, j, n;


  //for(i=1;i<argc;i++)
    // printf("1 argv[%d]=%s\n",i,argv[i]);
		
    // 逐行读取输入
    i=0;
    j=0;  
    n=0;
    //printf("1.0 ");
    while (read(0, &rcv[i], 1) > 0) 
    {
  	//printf("%x ",rcv[i]);
  	
  	if(rcv[i] != '\"')
    	{
    	    buf[j] = rcv[i];
    	    j++;
      	    n++;
    	}
  	i++;
    }
    //printf("\n");     
    //printf("2 buf = %s  n=%d\n",buf, n);
    
    // 解析参数
    i = 0;
    //args[0] = argv[1]; 命令
    for(i=0;i<argc-1;i++)
    {
	args[i] = argv[i+1]; 	
	//printf("3 args[%d]=%s\n",i,args[i]);
    }
    
    
    args[i] = &buf[0];
    for (j = 0; j < n; j++) 
    {
      if (j+1 < n && buf[j] == '\\'&& buf[j+1] == 'n')
      {   
        buf[j] = '\0';   
        //printf("4.1 args[%d]=%s\n",i,args[i]);     
    	args[i+1] = 0;
        if (fork() == 0) 
        {
            exec(args[0], args);
            exit(0);
         } 
         else 
         {         
             wait(0);
         } 
         j++;
              
         args[i] = &buf[j+1];
      }
      else  if (buf[j] == 0x0a) //'\n'
      {   
        buf[j] = '\0';   
        //printf("4.2 args[%d]=%s\n",i,args[i]);     
    	args[i+1] = 0;
        if (fork() == 0) 
        {
            exec(args[0], args);
            exit(0);
         } 
         else 
         {         
             wait(0);
         }               
         args[i] = &buf[j+1];
      }
    }    
    //buf[n-1] = '\0';
    //printf("5 args[%d]=%s\n",i,args[i]);     
    //args[i+1] = 0;

    // 创建子进程并执行命令
    //if (fork() == 0) 
    //{
      //exec(args[0], args);
     // exit(0);
    //} 
    //else 
    //{
    //  wait(0);
    //}
  

  exit(0);
}
