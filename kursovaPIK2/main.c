#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Velko Milanov, FKST, KSI, 1. kurs, 123220014, 79 grupa:
Да се разработи програма с меню тип файлов индексатор.
Индексатор на файлове пази списък с файловете на компютъра за по-бързото им търсене,
 вместо да ги чете папка по папка от хард-диска. Той трябва да има:
1. Данните за индексираните файловете се съхраняват в двоични файл. Файла започва с броя файлове следван от
файловете. За всеки индексиран файл се пазят следните данни:
• Пълен път (без файла);
• Име на файл;
• Размер на файла;
• Дата на създаване на файла - записана във формата ДД.ММ.ГГГГ.;
• Дата на модифициране на файла - записана във формата ДД.ММ.ГГГГ.;
• Флагове: hidden, read-only, system file, executable
2. Да се извършват следните обработки:
а) Добавяне на нов файл;
б) Намиране на даден файл и извеждане на неговите данни;
в) По зададена от потребителя папка, показва колко файла има в нея и колко място заема тя на хардиска
(файловете и рекурсивно папките в нея).
3. Данните да се поддържат в динамична структура – динамично заделен масив или едносвързан списък в
оперативната памет на ПК.
*/

struct File
{
    char location[100];
    char name[50];
    int size;
    char created[50];
    char modified[50];
    char attributes[50];

    struct File* next;
};
typedef struct File File;

File *new_item()
{
	File *result=NULL;
	result = (File*)malloc(sizeof(File));
	printf("New Item -> location = ");
	scanf("%s",&result->location);
	printf("New Item -> name = ");
	scanf("%s",&result->name);
	printf("New Item -> size = ");
	scanf("%d",&result->size);
	printf("New Item -> created = ");
	scanf("%s",&result->created);
	printf("New Item -> modified = ");
	scanf("%s",&result->modified);
	printf("New Item -> attributes = ");
	scanf("%s",&result->attributes);
	result->next = NULL;
	return result;
}
void print(File* root)
{
	File* curr_item = root;
	while(curr_item != NULL)
	{
		printf("Item has location %s\n",curr_item->location);
		printf("Item has name %s\n",curr_item->name);
		printf("Item has size %d\n",curr_item->size);
		printf("Item has created %s\n",curr_item->created);
		printf("Item has modified %s\n",curr_item->modified);
		printf("Item has attributes %s\n",curr_item->attributes);
		curr_item=curr_item->next;

	}
}
File *create_queue_list(int size)
{
	File *res_root = NULL;
	File *end_item = NULL;
	int i;
	for(i=0; i<size; i++)
	{
		if(res_root == NULL)
		{
			res_root = new_item();
			end_item = res_root;
		}
		else
		{
			end_item->next = new_item();
			end_item = end_item->next;
		}
	}
	return res_root;
}
void free_list(File* root)
{
	File* curr_item = root;
	while(root != NULL)
	{
		root = root->next;
		free(curr_item);
		curr_item= root;
	}
}
void newFile(File *item,File *item2)
{
	item2->next=item->next;
	item->next = item2;
}
void findFile(File *root,char search[50])
{
    int check;
    int i;
    File* curr_item = root;

while(curr_item != NULL)
{
    check=strcmp(search, curr_item->name);

        if(check==0)
        {
        printf("Item has location %s\n",curr_item->location);
		printf("Item has name %s\n",curr_item->name);
		printf("Item has size %d\n",curr_item->size);
		printf("Item has created %s\n",curr_item->created);
		printf("Item has modified %s\n",curr_item->modified);
		printf("Item has attributes %s\n",curr_item->attributes);
        }
        curr_item=curr_item->next;
}



}

void record(File *root, FILE *fp)
{
    if((fp = fopen("index", "wb"))==NULL)
{
printf("Cannot open file. \n");
exit(1);
}


if(fwrite(&root, sizeof(File), 1, fp) != 1)
{
printf("Write error occured. \n");
exit(1);
}

fclose(fp);

}
void byLocation(File *root,char folder[50])
{
    int check,counter=0, sum=0, check2, sub=0;
    int i;
    File* curr_item = root;

while(curr_item != NULL)
{

    check=strcmp(folder, curr_item->location);

        if(check==0)
        {
            counter++;
        sum=sum + curr_item->size;
		printf("Folder has file with name %s\n",curr_item->name);

        }
        char *token = strtok(curr_item->location, "/");


    check2=strcmp(folder, token);
    if(check2==0 && check!=0)
    {
        sub++;
        sum=sum + curr_item->size;
    }
     curr_item=curr_item->next;

    }
    printf("Folder has %d subfolder\n", sub);
    printf("Folder has %d files\n", counter);
printf("Folder has memory of %dMB\n", sum);



}

File *delete_item(File *root, char name[50])
{
    int check;
	File* prev_item=root;
	File* curr_item=root;
	int f=0;
	while(curr_item!=NULL)
	{
	    check=strcmp(curr_item->name, name);
		if(check==0)
		{
			curr_item=NULL;
	}
	}
	return root;
}
int main()
{
File *root = NULL;
FILE *fp;
FILE *fp2;
char deletef[50];
char searchFile[50], searchFolder[50];
int find;
int n;
printf("How many files:\n");
scanf("%d", &n);
root=create_queue_list(n);
printf("delete:\n");
scanf("%s", &deletef);
delete_item(root, deletef);
print(root);
record(root, fp);
printf("Add new file:\n");
newFile(root, new_item());
n++;
print(root);
printf("Search file by name:\n");
scanf("%s", &searchFile);
findFile(root, searchFile);
record(root, fp2);
printf("Search folder:\n");
scanf("%s", &searchFolder);
byLocation(root, searchFolder);


free_list(root);
    return 0;
}
