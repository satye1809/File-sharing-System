#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<fstream>
#include<string.h>
#include<conio.h>
#include<dirent.h>

sem_t wrt, mutex;
int readcount=0;
void* readFunct(void* unused)
{
    char filename[25];
    printf("ENTER THE FILE YOU WANT TO READ\n");
    scanf("%s",filename);
    sem_wait(&mutex);
    readcount++;
    if(readcount==1)
        sem_wait(&wrt);
    sem_post(&mutex);
    printf("The contents of the file are:\n");
    FILE* fptr;
    fptr=fopen(filename,"r");
    if(fptr==NULL)
       printf("File is empty.\n");
    char ch;
    ch=fgetc(fptr);
    while(ch!=EOF)
    {
        printf("%c",ch);
        ch=fgetc(fptr);
    }
    fclose(fptr);
    sem_wait(&mutex);
    readcount--;
    if(readcount==0)
        sem_post(&wrt);
    sem_post(&mutex);

}
void* writeFunct(void* unused)
{
    sem_wait(&wrt);
    char filename[25];
    printf("ENTER THE FILE YOU WANT TO CREATE OR MODIFY\n");
    scanf("%s",filename);
    FILE* f;
    f=fopen(filename,"a");
    char ipstr[200];
    printf("Enter text you wish to enter : \n");
    scanf("%s",ipstr);
    int i;
    /*for(i=0;i<strlen(ipstr);i++)
    {
        fputc(ipstr[i],f);
    }*/
    //strcat(ipstr,"\n");
    //fputs(ipstr,f);
    fprintf(f, "%s\n",ipstr);
    fclose(f);
    sem_post(&wrt);
}
void* del(void* unused)
{
    int stat;
    char filename[25];
    printf("Enter the name of the file you wish to delete");
    scanf("%s",filename);
    sem_wait(&wrt);
    stat=remove(filename);
    sem_post(&wrt);
    if(stat==0)
        printf("File removed successfully");
    else
        printf("Failed");
}
void* viewDirectories(void* unused)
{
  struct dirent* de;
  DIR* dr=opendir(".");
  if(dr==NULL)
  {
      printf("Could not open current directory");
      return 0;
  }
  while((de=readdir(dr))!=NULL)
    printf("%s\n",de->d_name);
  printf("\n");
  closedir(dr);
}
int main()
{

int ch;
pthread_t tid1,tid2,tid3,tid4;
sem_init(&wrt,0,1);
sem_init(&mutex,0,1);
while(1)
{
printf("\n\nEnter:\n 1 for reading from file\n 2 for writing into  the file\n 3 for deleting a file\n 4 to list all the files in the directory\n");
scanf("%d",&ch);

switch(ch)
{

case 1:

    int status1;
    status1=pthread_create(&tid1,NULL,&readFunct,NULL);
    if(status1!=0)
        printf("ERROR");
    pthread_join(tid1,NULL);
    break;

case 2:

    int status2;
    status2=pthread_create(&tid2,NULL,&writeFunct,NULL);
    if(status2!=0)
        printf("ERROR");
    pthread_join(tid2,NULL);
    break;

case 3:

    int status3;
    status3=pthread_create(&tid3,NULL,&del,NULL);
    if(status3!=0)
         printf("ERROR");
    pthread_join(tid3,NULL);
    break;

case 4:

    int status4;
    status4=pthread_create(&tid4,NULL,&viewDirectories,NULL);
    if(status4!=0)
        printf("ERROR");
    pthread_join(tid4,NULL);
    break;
}
}
 return 0;
}
