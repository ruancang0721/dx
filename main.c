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
#include <math.h>

typedef struct gequ{
    char name[20];//乐曲名
    char singer[20];//歌手名/曲师名
    float level;//难度等级
    int id;//乐曲id
    int bpm;//节拍
    float score;//评分
    struct gequ * next;
    struct gequ * prev;
}SONG;
    SONG * head = NULL; //头指针
    SONG *tail = NULL; //尾指针
    // 增加链表操作函数
void insert_node(SONG **head, SONG **tail, SONG *new_node) {
    if (*head == NULL) {
        *head = *tail = new_node;
    } else {
        (*tail)->next = new_node;
        new_node->prev = *tail;
        *tail = new_node;
    }
}

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
            fprintf(fp,"%s,%s,%.2f,%d,%d,%.4f\n",p->name,p->singer,p->level,p->id,p->bpm,p->score);
            p = p -> next;
        }
        fclose(fp);
        printf("数据已保存!\n");

    }

void loaddata() {
    FILE *fp = fopen("data.txt", "r");
    if (fp == NULL) {
        perror("open file error");
        exit(1);
    }

    // 清空现有链表
    SONG *current = head;
    while (current != NULL) {
        SONG *temp = current;
        current = current->next;
        free(temp);
    }
    head = NULL;
    tail = NULL;

    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        // 处理换行和空行
        line[strcspn(line, "\n")] = 0;
        if (strlen(line) < 5) continue;

        SONG *song = (SONG *)malloc(sizeof(SONG));
        if (!song) {
            perror("malloc error");
            exit(1);
        }

        // 安全解析字段
        int ret = sscanf(line, "%19[^,],%19[^,],%f,%d,%d,%f",
                        song->name,
                        song->singer,
                        &song->level,
                        &song->id,
                        &song->bpm,
                        &song->score);

        if (ret != 6) {
            free(song);
            fprintf(stderr, "格式错误: %s\n", line);
            continue;
        }

        song->next = NULL;
        song->prev = NULL;

        // 使用插入函数
        insert_node(&head, &tail, song);
    }
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
scanf("%f",&(song->level));
printf("请输入乐曲id：");
scanf("%d",&(song->id));
printf("请输入节拍：");
scanf("%d",&(song->bpm));
printf("请输入评分：");
scanf("%f",&(song->score));


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
void baobiao(SONG * p)//报表打印
{
    if(p == NULL)
    {
        perror("没有数据");
        exit(1);
    }
    FILE * fp = fopen("report.txt","w");
    if(fp == NULL)
    {
        perror("open file error");
        exit(1);
    }
    fprintf(fp,"ID\t\t名称\t\t歌手\t\t难度\t\t节拍\t\t评分\n");
    while(p!=NULL)
    {
    fprintf(fp,"%d\t\t%s \t\t%s\t\t %.2lf\t\t %d\t\t%.2lf\n",p->id,p->name,p->singer,p->level,p->bpm,p->score);

        p = p->next;
    }
    fclose(fp);
    printf("报表已生成!\n");
}
void shujutongji(SONG * p)//数据统计
{
    int i=0,j=0,k=0;
    while(p!=NULL)
    {
        if(p->score >= 100)
        {
            j++;
        }
        if(p->level >= 13)
        {
            k++;
        }
        i++;
        p=p->next;
        
    }
    printf("共有%d条数据\n",i);
    printf("有%d条评分大于等于100\n",j);
    printf("有%d条难度大于等于13级\n",k);

}
void sort_list(SONG **head, SONG **tail, int order) {
    if (*head == NULL || (*head)->next == NULL) {
        return; // 链表为空或只有一个节点，无需排序
    }

    SONG *sorted = NULL; // 新的有序链表
    SONG *current = *head;

    while (current != NULL) {
        SONG *next = current->next; // 保存下一个节点
        current->next = NULL;       // 将当前节点分离
        current->prev = NULL;

        // 将当前节点插入到有序链表中
        if (sorted == NULL) {
            sorted = current;
        } else {
            SONG *i = sorted;
            SONG *prev = NULL;
            while (i != NULL && (order * (i->score - current->score)) <= 0) {
                prev = i;
                i = i->next;
            }
            if (prev == NULL) {
                current->next = sorted;
                sorted->prev = current;
                sorted = current;
            } else {
                current->next = prev->next;
                current->prev = prev;
                if (prev->next != NULL) {
                    prev->next->prev = current;
                }
                prev->next = current;
            }
        }

        current = next; // 移动到下一个节点
    }

    // 更新头尾指针
    *head = sorted;
    *tail = *head;
    while ((*tail)->next != NULL) {
        *tail = (*tail)->next;
    }
    savedate(*head);
}


void zhengti(SONG * p)//整体查询
{
    printf("1.显示全部数据   2.报表打印 3.数据统计 4.分数排序升序 5.分数排序降序\n");
    int i = 0;
    scanf("%d",&i);
    if(i == 1)
    {
        printf("ID\t\t名称\t\t歌手\t\t难度\t\t节拍\t\t评分\n");
        while(p!=NULL)
        {
            printf("%d\t\t%s\t\t%s\t\t%.2lf\t\t%d\t\t%.2lf\n",p->id,p->name,p->singer,p->level,p->bpm,p->score);
            p = p->next;
         
        }
    }
    else if(i == 2)
    {
        baobiao(p);
    }
    else if(i == 3)
    {
      shujutongji(p);
    }
    else if(i == 4)
    {
        sort_list(&head, &tail, 1);
        printf("数据已排序！\n");
    }
    else if(i == 5)
    {
         sort_list(&head, &tail, -1);
         printf("数据已排序！\n");

    }
}
void gequname(SONG * p)//歌曲名查询
{ 
     char name[20];
     scanf("%s",name);
     int i =0;
printf("ID\t\t名称\t\t歌手\t\t难度\t\t节拍\t\t评分\n");
     while(p!=NULL)
     {
         if(strcmp(p->name,name) == 0)
         {
             printf("%d\t\t%s\t\t%s\t\t%.2lf\t\t%d\t\t%.2lf\n",p->id,p->name,p->singer,p->level,p->bpm,p->score);
             i++;
         }
         p=p->next;
     }
     printf("共找到%d条数据\n",i);


}
void singername(SONG * p)//歌手名查询
{
         char name[20];
     scanf("%s",name);
     int i =0;
printf("ID\t\t名称\t\t歌手\t\t难度\t\t节拍\t\t评分\n");
     while(p!=NULL)
     {
         if(strcmp(p->singer,name) == 0)
         {
             printf("%d\t\t%s\t\t%s\t\t%.2lf\t\t%d\t\t%.2lf\n",p->id,p->name,p->singer,p->level,p->bpm,p->score);
             i++;
         }
         p=p->next;
     }
     printf("共找到%d条数据\n",i);

}
void nandu(SONG * p)//难度查询
{
    float level;
    scanf("%f",&level);
    int i =0;
printf("ID\t\t名称\t\t歌手\t\t难度\t\t节拍\t\t评分\n");
    while(p!=NULL)
    {
        if(p->level == level)
        {
            printf("%d\t\t%s\t\t%s\t\t%.2lf\t\t%d\t\t%.2lf\n",p->id,p->name,p->singer,p->level,p->bpm,p->score);
            i++;
        }
        p=p->next;
    }
    printf("共找到%d条数据\n",i);
}
void bpmchaxun(SONG * p)//节拍查询
{
    int bpm;
    scanf("%d",&bpm);
    int i =0;
printf("ID\t\t名称\t\t歌手\t\t难度\t\t节拍\t\t评分\n");
    while(p!=NULL)
    {
        if(p->bpm == bpm)
        {
            printf("%d\t\t%s\t\t%s\t\t%.2lf\t\t%d\t\t%.2lf\n",p->id,p->name,p->singer,p->level,p->bpm,p->score);
            i++;
        }
        p=p->next;
    }
    printf("共找到%d条数据\n",i);
}
void scorechaxun(SONG * p)//评分查询
{
    float score;
    scanf("%f",&score);
    int i =0;
printf("ID\t\t名称\t\t歌手\t\t难度\t\t节拍\t\t评分\n");
    while(p!=NULL)
    {
        if(p->score == score)
        {
            printf("%d\t\t%s\t\t%s\t\t%.2lf\t\t%d\t\t%.2lf\n",p->id,p->name,p->singer,p->level,p->bpm,p->score);
            i++;
        }
        p=p->next;
    }
    printf("共找到%d条数据\n",i);
}
void IDchaxun(SONG * p)//ID查询
{
    int id;
    scanf("%d",&id);
    int i =0;
    printf("ID\t\t名称\t\t歌手\t\t难度\t\t节拍\t\t评分\n");
    while(p!=NULL)
    {
        if(p->id == id)
        {
            printf("%d\t\t%s\t\t%s\t\t%.2lf\t\t%d\t\t%.2lf\n",p->id,p->name,p->singer,p->level,p->bpm,p->score);
            i++;
        }
        p=p->next;
    }
    printf("共找到%d条数据\n",i);
}
// 条件输入函数
void get_condition_input(int cond_type, SONG *temp) {
    printf("请输入条件值：");
    switch(cond_type) {
        case 1: // 歌曲名
            scanf("%19s", temp->name);
            break;
        case 2: // 歌手名
            scanf("%19s", temp->singer);
            break;
        case 3: // 难度
            scanf("%f", &temp->level);
            break;
        case 4: // 节拍
            scanf("%d", &temp->bpm);
            break;
        case 5: // 评分
            scanf("%f", &temp->score);
            break;
        case 6: // ID
            scanf("%d", &temp->id);
            break;
    }
    while(getchar() != '\n'); // 清空输入缓冲区
}
int check_single_condition(SONG *p, int cond_type, SONG* temp) {
    switch(cond_type) {
        case 1: // 歌曲名
            return strcmp(p->name, temp->name) == 0;
        case 2: // 歌手名
            return strcmp(p->singer, temp->singer) == 0;
        case 3: // 难度
            return fabs(p->level - temp->level) < 0.001f;
        case 4: // 节拍
            return p->bpm == temp->bpm;
        case 5: // 评分
            return fabs(p->score - temp->score) < 0.001f;
        case 6: // ID
            return p->id == temp->id;
        default:
            return 0;
    }
}
void shuangtiaojian(SONG * p)//双条件查询
{
    int type1=0;
    int type2=0;
    SONG* temp = malloc(sizeof(SONG));
    if(temp == NULL)
    {
        perror("malloc error");
        exit(1);
    }
         printf("查询条件：\n");
        printf("1.歌曲名\t");
        printf("2.歌手名\t");
        printf("3.难度\n");
        printf("4.节拍\t");
        printf("5.评分\t");
        printf("6.ID\n");
        printf("请选择第一个查询条件：");
    while(1) {
        scanf("%d", &type1);
        if(type1 <1 || type1>6) 
        {
            printf("输入错误，请重新输入1-6：");
            while(getchar() != '\n'); // 清空缓冲区
            continue;
        }
        break;
    }
  get_condition_input(type1, temp);
  printf("请选择第二个查询条件：");
  while(1) {
        scanf("%d", &type2);
        if(type2 <1 || type2>6) 
        {
            printf("输入错误，请重新输入1-6：");
            while(getchar() != '\n'); // 清空缓冲区
            continue;
        }
        break;
    }
  get_condition_input(type2, temp);
  int i =0;
  printf("ID\t\t名称\t\t歌手\t\t难度\t\t节拍\t\t评分\n");
  while(p!=NULL)
  {
    int k =0;
      k+=check_single_condition(p,type1,temp);
      k+=check_single_condition(p,type2,temp);
      if(k == 2)
      {
        printf("%d\t\t%s\t\t%s\t\t%.2lf\t\t%d\t\t%.2lf\n",p->id,p->name,p->singer,p->level,p->bpm,p->score);
        i++;
      }
      p=p->next;
  }
  printf("共找到%d条数据\n",i);
}
void tiaojian(SONG * p)//局部查询
{
   printf("请输入查询方式：\n");
   printf("1.单条件查询\t2.双条件查询\n");

   int j =0;
   scanf("%d",&j);
   if(j == 2)
   {
    shuangtiaojian(p);
    return;
   }
        printf("请输入查询条件：\n");
        printf("1.歌曲名\t");
        printf("2.歌手名\t");
        printf("3.难度\n");
        printf("4.节拍\t");
        printf("5.评分\t");
        printf("6.ID\n");
        int i =0;
        scanf("%d",&i);
        switch(i)
        {
            case 1:
            printf("请输入歌曲名：");
            gequname(p);
            break;
            case 2:
            printf("请输入歌手名：");
            singername(p);
            break;
            case 3:
            printf("请输入难度：");
            nandu(p);
            break;
            case 4:
            printf("请输入节拍：");
            bpmchaxun(p);
            break;
            case 5:
            printf("请输入评分：");
            scorechaxun(p);
            break;
            case 6:
            printf("请输入ID：");
            IDchaxun(p);
            break;
            default:
            printf("输入错误，请重新输入！");
            break;
        }
              
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
   loaddata();
   SONG * p = head;
   printf("请输入你要删除的数据的ID：");
   int id;
   scanf("%d",&id);
   while(p!=NULL)
   {
    if(p->id == id)
    {
        if(p->prev == NULL)
        {
            head = p->next;
            if(head != NULL)
            {
                head->prev = NULL;
            }
        }
        else
        {
            p->prev->next = p->next;
            if(p->next != NULL)
            {
                p->next->prev = p->prev;
            }

        }
        free(p);
        break;
    }
    p = p->next;
   }
      if(p == NULL)
   {
    printf("未找到该数据！\n");
    return;
   }
   savedate(head);

    printf("删除成功！\n");
}
void xiugai()//修改数据
{
    loaddata();
    SONG * p = head;
    printf("请输入你要修改的数据的ID：");
    int id;
    scanf("%d",&id);
    while(p!=NULL)
    {
        if(p->id == id)
        {
            printf("请输入你要修改的数据：\n");
            printf("1.歌曲名\t");
            printf("2.歌手名\t");
            printf("3.难度\n");
            printf("4.节拍\t");
            printf("5.评分\t");
            printf("6.ID\n");
            int i =0;
            scanf("%d",&i);
            switch(i)
            {
                case 1:
                printf("请输入新的歌曲名：");
                scanf("%19s",p->name);
                break;
                case 2:
                printf("请输入新的歌手名：");
                scanf("%19s",p->singer);
                break;
                case 3:
                printf("请输入新的难度：");
                scanf("%f",&p->level);
                break;
                case 4:
                printf("请输入新的节拍：");
                scanf("%d",&p->bpm);
                break;
                case 5:
                printf("请输入新的评分：");
                scanf("%f",&p->score);
                break;
                case 6:
                printf("请输入新的ID：");
                scanf("%d",&p->id);
                break;
                default:
                printf("输入错误，请重新输入！");
                break;
            }
            savedate(head);
            printf("修改成功！\n");
            return;
        }
        p=p->next;
    }
    printf("未找到该数据！\n");
}
void tianjia()//插入数据
{
    loaddata();
      SONG * song = (SONG*)malloc(sizeof(SONG));
      if(song == NULL)
      {
          perror("malloc error");
          exit(1);
      }
      printf("请输入你要插入的数据：\n");
      printf("请输入乐曲名：");
      scanf("%s",song->name);
      printf("请输入歌手名/曲师名：");
      scanf("%s",song->singer);
      printf("请输入难度等级：");
      scanf("%f",&song->level);
      printf("请输入节拍：");
      scanf("%d",&song->bpm);
      printf("请输入评分：");
      scanf("%f",&song->score);
      printf("请输入ID：");
      scanf("%d",&song->id);
      song->next = head;
printf("请输入你要插入的位置ID：");
   int id =0;
    scanf("%d",&id);
    SONG * p = head;
    while(p!=NULL)
    {
        if(p->id == id)
        {
            song->next = p->next;
            p->next = song;
            song->prev = p;
            if(song->next != NULL)
            {
                song->next->prev = song;
            }
            savedate(head);
            printf("插入成功！\n");
            return;
        
        }
        p=p->next;
    }
    if(p == NULL)
    {
        printf("未找到该位置！\n");
        free(song);
        return;
    }



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
          
SONG *current = head;
while (current != NULL) {
    SONG *temp = current;
    current = current->next;
    free(temp);
}
return 0;
}