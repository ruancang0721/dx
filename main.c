#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/wait.h>

typedef struct gequ{
    char name[20];//乐曲名
    char singer[20];//歌手名/曲师名
    double level;//难度等级
    int id;//乐曲id
    int bpm;//节拍
    double score;//评分
    struct gequ * next;
    struct gequ * prev;
}SONG;
    SONG * head = NULL; //头指针
    void savedate(SONG * head)
    {
        FILE * fp = fopen("data.txt","w");
        if(fp == NULL)
        {
            perror("open file error");
            exit(1);
        }
        SONG * p = head;
        while(p!=NULL)
        {
            fprintf(fp,"%s,%s,%lf,%d,%d,%lf\n",p->name,p->singer,p->level,p->id,p->bpm,p->score);
            p = p -> next;
        }
        fclose(fp);
        printf("数据已保存!\n");

    }

    void loaddata()
{
        FILE * fp = fopen("data.txt","r");
        if(fp == NULL)
        {
            perror("open file error");
            exit(1);
        }
           // 清空现有链表
    SONG *current = head;
    while(current != NULL)
    {
        SONG *temp = current;
        current = current->next;
        free(temp);
    }
    head = NULL;

 while(1)
 {
    SONG * song = (SONG*)malloc(sizeof(SONG));
    if(song == NULL)
    {
        perror("malloc error");
        exit(1);
    }
    int ret = fscanf(fp,"%s,%s,%lf,%d,%d,%lf\n",song->name,song->singer,&(song->level),&(song->id),&(song->bpm),&(song->score));
    if(ret!= 6)
    {
        free(song);
        break;
    }
    song->next = NULL;
    song->prev = NULL;
    if(head == NULL)
    {
        head = song;
    }
    else{
        SONG * p = head;
        while(p->next!=NULL)
        {
            p = p->next;
        }
        p->next = song;
        song->prev = p;
    }
 }
    fclose(fp);
    printf("数据已加载!\n");
    }

void luru()//录入初始数据
{


    SONG * song = (SONG*)malloc(sizeof(SONG));
    if(song == NULL)
    {
        perror("malloc error");
        exit(1);
    }
    
printf("请输入你要录入的数据：\n");
printf("请输入乐曲名：");
scanf("%s",song->name);
printf("请输入歌手名/曲师名：");
scanf("%s",song->singer);
printf("请输入难度等级：");
scanf("%lf",&(song->level));
printf("请输入乐曲id：");
scanf("%d",&(song->id));
printf("请输入节拍：");
scanf("%d",&(song->bpm));
printf("请输入评分：");
scanf("%lf",&(song->score));


    song->next = NULL;
    song->prev = NULL;


    if(head == NULL)
    {
        head = song;
    }
    else{
        SONG * p = head;
        while(p->next!=NULL)
        {
            p = p->next;
        }
        p->next = song;
        song->prev = p;
    }
    printf("录入成功！\n");
    savedate(head);
    sleep(1);

}
void zhengti(SONG * p)//整体查询
{
    printf("1.显示全部数据   2.b50\n");
    int i = 0;
    scanf("%d",&i);
    if(i == 1)
    {
        printf("ID\t名称\t\t歌手\t\t难度\t节拍\t评分\n");
        while(p!=NULL)
        {
            printf("%d\t%s\t%s\t%.2lf\t%d\t%.2lf\n",p->id,p->name,p->singer,p->level,p->bpm,p->score);
            p = p->next;
        
        }
    }
}
void tiaojian(SONG * p)//局部查询
{

}
void chaxun()//查询数据
{
  loaddata();
  SONG * p = head;
  printf("请输入你要查询的方式\n");
  printf("1.整体查询\n");
  printf("2.条件查询\n");
  int i =0;
  scanf("%d",&i);
  if(i == 1)
  {
     zhengti(p);
  }
  else
  {
     tiaojian(p);
  }
   
}
void shanchu()//删除数据
{

}
void xiugai()//修改数据
{

    }
void tianjia()//插入数据
{

}
void lsmode()  //显示主菜单
{
     printf("1.录入数据\t");
     printf("2.查询数据\t");
     printf("3.删除数据\n");
     printf("4.修改数据\t");
     printf("5.插入数据\t");
     printf("6.退出系统\n");
}
int main()
{
      int choice =0;
          while(choice!=6)
          {
           lsmode();
          choice =0;
          printf("请输入您的选择：");
          scanf("%d",&choice);
          switch(choice)
          {
            case 1:
                printf("录入数据\n");
                luru();

                break;
            case 2:
                printf("查询数据\n");
                chaxun();       
                break;
            case 3:
                printf("删除数据\n");
                shanchu();
                break;
            case 4:
                printf("修改数据\n");
                xiugai();
                break;
            case 5:
                printf("插入数据\n");
                tianjia();
                break;
            case 6:
                printf("退出系统\n");
                exit(1);
                break;
            default:
                printf("输入错误，请重新输入！\n");
                break;
          }
          }
}