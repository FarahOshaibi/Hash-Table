#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <dirent.h>

#define MAX_SIZE 25

typedef struct babies names;
typedef struct freq freqs;
typedef struct Hash_Table HT;

struct freq{
int year;
int total;
};

struct babies{
char name[50];
freqs numbers[25];
int freqSize;
int status;
};

struct Hash_Table{
names* records;
int numOfRecords;
int size;
};

int quadraticHashing(HT, char[]);
int hashFunction(HT, char[]);
void readTableFromFile(HT *table, char[]);
void insertToTable(HT*, names);
void rehash(HT*);
void deleteFromTable(HT, char[]);
int findRecord(HT, char[]);



int requiredTableSize(int);
int isPrime(int);


// returns the index of the parent node
int parent(int i) {
    return (i - 1) / 2;
}

// return the index of the left child
int left_child(int i) {
    return 2*i + 1;
}

// return the index of the right child
int right_child(int i) {
    return 2*i + 2;
}

void swap(freqs y1, freqs y2)
{
    freqs temp; //= (freq)malloc(sizeof(struct frequencies));
    temp.total = y2.total;
    temp.year = y2.year;
    y2.total = y1.total;
    y2.year = y1.year;
    y1.total = temp.total;
    y1.year = temp.year;
}
void insert(freqs a[], freqs data, int *n)
{
    if(*n >= MAX_SIZE)
    {
        printf("%s\n","The heap is full. Cannot insert");
        return;
    }
    //a[*n] = (freq)malloc(sizeof(struct frequencies));
    a[*n].total = data.total;
    a[*n].year = data.year;
    *n = *n + 1;
    int i = *n - 1;
    while(i != 0 && a[parent(i)].total < a[i].total)
    {
        swap(a[parent(i)], a[i]);
        i = parent(i);
    }
}

void max_heapify(freqs a[], int i, int n)
{
    int left = left_child(i);
    int right = right_child(i);
    int largest = i;
    if(left <= n && a[left].total > a[largest].total)
    {
        largest = left;
    }
    if(right <= n && a[right].total > a[largest].total)
    {
        largest = right;
    }
    if(largest != i)
    {
        freqs temp;
        temp.total = a[i].total;
        temp.year = a[i].year;
        a[i].total = a[largest].total;
        a[i].year = a[largest].year;
        a[largest].total = temp.total;
        a[largest].year = temp.year;
        max_heapify(a, largest, n);
    }
}


void build_max_heap(freqs a[], int n) {
    int i;
    for (i = n/2; i >= 0; i--) {
        max_heapify(a, i, n);
    }
}

void heapSort(freqs arr[], int n)
{
    for (int i = n / 2 - 1; i >= 0; i--)
        max_heapify(arr, i, n);

    for (int i = n - 1; i >= 0; i--) {
        swap(arr[0], arr[i]);

        max_heapify(arr, i, 0);
    }
}



freqs get_max(freqs a[])
{
    return a[0];
}

freqs extract_max(freqs a[], int *n)
{
    freqs max_item;
    max_item.year = a[0].year;
    a[0].total = a[*n-1].total;
    a[0].year = a[*n-1].year;
    *n = *n - 1;
    max_heapify(a, 0, *n);
    return max_item;
}

void print_heap(freqs a[], int n)
{
    for(int i = 0; i < n; i++)
    {
        printf("%d, %d \n", a[i].year, a[i].total);
    }
    printf("\n");
}


int main()
{
    HT table = {NULL,0,1};
    table.records = (names*) calloc(1, sizeof(names));

    char option = '0';

    do
    {
        system("cls");
        printf("Choose an option\n");
        printf("1- Load names files from a specific folder and an estimation of data size.\n");
        printf("2- Add new name/gender record.\n");
        printf("3- Delete a name record.\n");
        printf("4- Add a new name's year/freq record.\n");
        printf("5- Update a name's year/freq record.\n");
        printf("6- Search for Name.\n");
        printf("7- Name with maximum frequency.\n");
        printf("8- Exit.\n");

        option = getch();

        switch(option)
        {
        case '1':
            system("cls");
            char folder[20];
            printf("folder name: ");
            fflush(stdin);
            gets(folder);
            fflush(stdin);
            readTableFromFile(&table, folder);
            getch();
            system("cls");
            break;
        case '2':
            system("cls");
            char name[50];
            char gender;
            printf("Name: ");
            fflush(stdin);
            gets(name);
            fflush(stdin);
            printf("Gender: ");
            scanf("%c", &gender);
            if(gender == 'm' || gender == 'f')
                gender -= 32;
            else if(gender == 'M' || gender == 'F')
            {

            }
            else
            {
                printf("No such gender \n");
                getch();
                system("cls");
                break;
            }
            char nameg[50];
            sprintf(nameg, "%s,%c", name,gender);
            int index = findRecord(table, nameg);
            if(index != -1)
            {
                printf("This name exists\n");
                getch();
                system("cls");
                break;
            }
            names newBaby;
            strcpy(newBaby.name, nameg);
            newBaby.freqSize = 0;
            insertToTable(&table,newBaby);
            printf("Done\n");
            getch();
            system("cls");
            break;

        case '3':
            system("cls");
             if(table.numOfRecords == 0)
            {
                printf("No such records in the table\n");
                getch();
                break;
            }
            char dname[50];
            char dgender;
            printf("Name: ");
            fflush(stdin);
            gets(dname);
            fflush(stdin);
            printf("Gender: ");
             scanf("%c", &dgender);
            if(dgender == 'm' || dgender == 'f')
                dgender -= 32;
            else if(dgender == 'M' || dgender == 'F')
            {

            }
            else
            {
                printf("No such gender \n");
                getch();
                system("cls");
                break;
            }
            char dnameg[50];
            sprintf(dnameg, "%s,%c", dname,dgender);
            int dindex = findRecord(table, dnameg);
            if(dindex == -1)
            {
                printf("This name does not exist\n");
                getch();
                system("cls");
                break;
            }
            else
            {
                deleteFromTable(table,dnameg);
                printf("Done\n");
                getch();
                system("cls");
                break;
            }
            break;

        case '4':
            system("cls");
             if(table.numOfRecords == 0)
            {
                printf("No such records in the table\n");
                getch();
                break;
            }
            char aname[50];
            char agender;
            printf("Name: ");
            fflush(stdin);
            gets(aname);
            fflush(stdin);
            printf("Gender: ");
             scanf("%c", &agender);
            if(agender == 'm' || agender == 'f')
                agender -= 32;
            else if(agender == 'M' || agender == 'F')
            {

            }
            else
            {
                printf("No such gender \n");
                getch();
                system("cls");
                break;
            }
            char anameg[50];
            sprintf(anameg, "%s,%c", aname,agender);
            int aindex = findRecord(table, anameg);
            if(aindex == -1)
            {
                printf("This name does not exist\n");
                getch();
                system("cls");
                break;
            }

            int year;
            int frequency;

            printf("New Year: ");
            scanf("%d", &year);
            printf("Frequency: ");
            scanf("%d", &frequency);
            for(int i = 0 ; i < table.records[aindex].freqSize; i++)
            {
                if(table.records[aindex].numbers[i].year == year)
                {
                    table.records[aindex].numbers[i].total+=frequency;
                    build_max_heap(table.records[aindex].numbers, table.records[aindex].freqSize);
                    printf("Year exists, updated frequencies! \n");
                    getch();
                    break;
                }
            }
            freqs newf;
            newf.total = frequency;
            newf.year = year;
            insert(table.records[aindex].numbers, newf, &table.records[aindex].freqSize);
            printf("Done\n");
            getch();
            system("cls");
            break;


        case '5':
            system("cls");
             if(table.numOfRecords == 0)
            {
                printf("No such records in the table\n");
                getch();
                break;
            }
            char uname[50];
            char ugender;
            printf("Name: ");
            fflush(stdin);
            gets(uname);
            fflush(stdin);
            printf("Gender: ");
             scanf("%c", &ugender);
            if(ugender == 'm' || ugender == 'f')
                ugender -= 32;
            else if(ugender == 'M' || ugender == 'F')
            {

            }
            else
            {
                printf("No such gender \n");
                getch();
                system("cls");
                break;
            }
            char unameg[50];
            sprintf(unameg, "%s,%c", uname,ugender);
            int uindex = findRecord(table, unameg);
            if(uindex == -1)
            {
                printf("This name does not exist\n");
                getch();
                system("cls");
                break;
            }

            int uyear;
            int ufrequency;

            printf("Year: ");
            scanf("%d", &uyear);
            printf("Frequency: ");
            scanf("%d", &ufrequency);
            for(int i = 0 ; i < table.records[uindex].freqSize; i++)
            {
                if(table.records[uindex].numbers[i].year == uyear)
                {
                    table.records[uindex].numbers[i].total+=ufrequency;
                    build_max_heap(table.records[uindex].numbers, table.records[uindex].freqSize);
                    printf("Year exists, updated frequencies! \n");
                    getch();
                    break;
                }
            }
            printf("Year does not exist \n");
            getch();
            system("cls");
            break;

        case '6':
            system("cls");
            if(table.numOfRecords == 0)
            {
                printf("No such records in the table\n");
                getch();
                break;
            }
            char sname[50];
            char sgender;
            printf("Name: ");
            fflush(stdin);
            gets(sname);
            fflush(stdin);
            printf("Gender: (M, F, B) ");
            scanf("%c", &sgender);
            if(sgender == 'm' || sgender == 'f' || sgender == 'b')
                sgender -= 32;
            else if(sgender == 'M' || sgender == 'F' || sgender == 'B')
            {

            }
            else
            {
                printf("No such gender \n");
                getch();
                system("cls");
                break;
            }
            char snameg[50];
            if(sgender != 'B')
            {
            sprintf(snameg, "%s,%c", sname,sgender);
            int sindex = findRecord(table, snameg);
            if(sindex == -1)
            {
                printf("This name does not exist\n");
                getch();
                system("cls");
                break;
            }
            printf("Found\n");
            printf("Name: %s, Gender: %c\n", sname, sgender);
            heapSort(table.records[sindex].numbers, table.records[sindex].freqSize);
            printf("Years: ");
            for(int i = 0 ; i < table.records[sindex].freqSize; i ++)
            {
                printf("%d\t",table.records[sindex].numbers[i].year);
            }
            printf("\n");
            printf("Freqs: ");
             for(int i = 0 ; i < table.records[sindex].freqSize; i ++)
            {
                printf("%d\t",table.records[sindex].numbers[i].total);
            }
            printf("\n");
            getch();
            system("cls");
            break;
            }
        else
        {
            sprintf(snameg, "%s,M", sname);
            int sindex = findRecord(table, snameg);
            if(sindex == -1)
            {
                printf("Male does not exist\n");
            }
            else{
            printf("Found Male\n");
            printf("Name: %s, Gender: %c\n", sname, sgender);
            printf("Years: ");
            for(int i = 0 ; i < table.records[sindex].freqSize; i ++)
            {
                printf("%d\t",table.records[sindex].numbers[i].year);
            }
            printf("\n");
            printf("Freqs: ");
             for(int i = 0 ; i < table.records[sindex].freqSize; i ++)
            {
                printf("%d\t",table.records[sindex].numbers[i].total);
            }
        }
            sprintf(snameg, "%s,F", sname);
            sindex = findRecord(table, snameg);
            if(sindex == -1)
            {
                printf("Female does not exist\n");
            }
            else{
            printf("Found Female\n");
            printf("Name: %s, Gender: %c\n", sname, sgender);
            printf("Years: ");
            for(int i = 0 ; i < table.records[sindex].freqSize; i ++)
            {
                printf("%d\t",table.records[sindex].numbers[i].year);
            }
            printf("\n");
            printf("Freqs: ");
             for(int i = 0 ; i < table.records[sindex].freqSize; i ++)
            {
                printf("%d\t",table.records[sindex].numbers[i].total);
            }
        }

        }
        printf("\n");
        getch();
        system("cls");
        break;

        case '7':

            system("cls");
            if(table.numOfRecords == 0)
            {
                printf("No such records in the table\n");
                getch();
                break;
            }
            int maxIndex;
            int MaxFreq = 0;
            int MaxYear = 0;
            for(int i = 0; i < table.size; i ++)
            {
                if(table.records[i].status == 1)
                {
                    for(int j = 0; j < table.records[i].freqSize; j ++)
                    {
                        if(table.records[i].numbers[j].total > MaxFreq)
                        {
                            MaxFreq = table.records[i].numbers[j].total;
                            MaxYear = table.records[i].numbers[j].year;
                            maxIndex = i;
                        }
                    }
                }
            }
            printf("Name with max frequency is: %s", strtok(table.records[maxIndex].name,","));
            printf("\nFrequency is: %d, at year: %d\n", MaxFreq, MaxYear);
            getch();
            break;
        }

    }while(option != '8');

    return 0;
}

void readTableFromFile(HT* table, char folder[])
{
    DIR *d;
    FILE *input;
    struct dirent *dir;
    char subfolder[30];
    sprintf(subfolder,"./%s",folder);
    d = opendir(subfolder);
    dir = readdir(d);
    dir = readdir(d);
    int year;
    char path[20];
    int numOfFiles = 0;
    int NamesCounter = 0;
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            numOfFiles ++;
            sprintf(path,"./%s/%s",folder ,dir->d_name);
            input = fopen(path, "r");
            //char *region =
            strtok(dir->d_name, "b");
            year = atoi(strtok(NULL,"."));
            if(input == NULL)
            {
                perror("");
                break;
            }
            char line[100];

            while (fgets(line,100,input) != NULL)
            {
                NamesCounter ++;
                char *name = strtok(line, ",");
                char *gender = strtok(NULL, ",");
                int numbers = atoi(strtok(NULL,"\n"));
                freqs newNode;// = (freqs)malloc(sizeof(freqs));

                newNode.total = numbers;
                newNode.year = year;


                if(gender[0] == 'f' || gender[0] == 'm')
                    gender[0]-=32;
                char nameg[50];

                sprintf(nameg,"%s,%c", name, gender[0]);
                int index = findRecord(*table, nameg);
                if(index != -1)
                {
                    int inc = 0;
                    for(int i = 0 ; i < table->records[index].freqSize; i++)
                    {
                        if(table->records[index].numbers[i].year == newNode.year)
                        {
                            table->records[index].numbers[i].total += newNode.total;
                            inc = 1;
                            break;
                        }
                    }
                    if(inc == 0)
                    {
                        insert(table->records[index].numbers, newNode, &table->records[index].freqSize);
                    }
                }

                else
                {
                    names record;
                    strcpy(record.name, nameg);
                    record.numbers[0].total = newNode.total;
                    record.numbers[0].year = newNode.year;
                    record.freqSize = 1;
                    build_max_heap(record.numbers, record.freqSize);
                    insertToTable(table, record);
                }
        }
        fclose(input);
        }
        closedir(d);
    }
    printf("Average names is: %0.2f in each file\n", (double)NamesCounter/numOfFiles);
    printf("Done\n");
}


void insertToTable(HT *table, names record)
{
    (*table).numOfRecords++;
    if ( (*table).size < requiredTableSize( (*table).numOfRecords ) )
        rehash(table);
    record.status = 1;
    int index = quadraticHashing(*table, record.name);
    (*table).records[index] = record;
}

int hashFunction(HT table, char name[50])
{
    return strlen(name) % table.size;
}

int quadraticHashing(HT table, char name[50])
{

    int i = 1;

    int index = hashFunction(table, name);
    int quadIndex = index;

    while(table.records[quadIndex].status == 1){
        quadIndex = (index + i*i) % table.size;
        i++;
    }
    return quadIndex;
}

void rehash(HT *table)
{
    int oldTableSize = (*table).size;
    int newTableSize = requiredTableSize(oldTableSize);
    (*table).size = newTableSize;

    names *temp = (*table).records;
    (*table).records = (names *) calloc(newTableSize, sizeof(names) );

    int i;

    for (i = 0 ; i < oldTableSize ; i++)
        if( temp[i].status == 1)
            insertToTable(table, temp[i]);

    free(temp);
}


void deleteFromTable(HT table, char name[50])
{

    int index = findRecord(table, name);


    if (index != -1)
    {
        table.records[index].status = 2;
        table.numOfRecords--;
        printf("Deleted!\n");
    }
        else
        {
            printf("This record is not in the table!\n");
        }
}

int findRecord(HT table, char name[50])
{

    if(table.size == 0)
    {
        return -1;
    }
    int i = 1;

    int index = hashFunction(table, name);
    int quadIndex = index;

        while(table.records[quadIndex].status != 0){


        if (table.records[quadIndex].status != 2 &&  strcmp(table.records[quadIndex].name,name) == 0)
            return quadIndex;

        else{
            quadIndex = (index + i*i) % table.size;
            i++;
        }
    }
    return -1;
}




int requiredTableSize(int n)
{

    int requiredTableSize = 2*n + 1;

    while(!isPrime(requiredTableSize))
        requiredTableSize++;

    return requiredTableSize;
}

int isPrime(int num)
{

    int i;

    for(i = 2 ; i <= num/2 ; i++)
        if (num % i == 0)
            return 0;

    return 1;
}


