//FileName: Singlethread
//Team members: Saivikas Meda and Nirmal Aagash
//NetID: SXM190011 and nxc180026
//Class: CS5348.001
//Project: Project2




#include<pthread.h>
#include<stdio.h>
#include <stdlib.h>
#include<semaphore.h>
#include<unistd.h>
#include<math.h>
#include<time.h>
#include <sys/time.h>

struct args{
    int *arrA;
    int *arrB;
    int *arrC;
    int start;
    int end;
    int n1;
    int n2;
};
unsigned long long int result=0;
sem_t semaphore;
static int *arrC;

void *multiply(void *arguments){
    struct args *arg = arguments;
    
    int *arrA = (int *)arg -> arrA;
    int *arrB = (int *)arg -> arrB;
    int n1 = arg -> n1;
    int n2 = arg -> n2;
    int start = arg -> start;
    int end = arg -> end;
    
    int i,j,k,ans,borrow=0;
    long long int mid_result=0;
    //printf("\n%d and %d are the start and end\n",start,end);
    
    for(i=start;i<=end;i++){
        k=0;ans=0,borrow=0;
        mid_result=0;
        for(j=n1-1;j>=0;j--){
            ans = arrA[j]*arrB[i];
            borrow = ans/10;
            ans = ans%10;
            
            sem_wait(&semaphore);
            arrC[n1+i-k] += ans;
            arrC[n1+i-k-1] += borrow;

            k++;
            sem_post(&semaphore);

        }

    }
    sem_wait(&semaphore);
    for(i=(n1+n2-1);i>0;i--){
        int borrow = arrC[i]/10;
        arrC[i] = arrC[i]%10;
        
        arrC[i-1] += borrow;
        
    }
    sem_post(&semaphore);
    
    i=0;

}

int main(int argc, char *argv[]){
    struct timeval stop, start;

    FILE *fp1,*fp2;
    
    fp1 = fopen("a.txt","r");
    fp2 = fopen("b.txt","r");
    if (fp1 == NULL || fp2 == NULL){
        printf("Error opening the file.");
    }else{
        //First file reading
        int i=0;
        int static n1;
        static int *arrA;
        
        char c;
        fscanf(fp1,"%d" ,&n1);
//        printf("The value of n is %d\n",n1);
        if(n1 < 0 || n1 > 1000000){
            printf("\nPlease check the number of digits.\n");
            return(0);
        }
        fgetc(fp1);
        arrA = malloc(n1* sizeof(int));
        while((c=fgetc(fp1))!=EOF){
            arrA[i] = c-'0';
            i++;
        }
        fclose(fp1);
        int j;
//        for(j=0;j<n1;j++){
//            printf("%d",arrA[j]);
//        }
        
        
        //Second file reading
        static int *arrB;
        int static n2;
        fscanf(fp2,"%d" ,&n2);
//        printf("The value of n is %d\n",n2);
        fgetc(fp2);
        arrB = malloc(n2* sizeof(int));
        i=0;
        while((c=fgetc(fp2))!=EOF){
            arrB[i] = c-'0';
            i++;
        }
        fclose(fp2);
//        for(j=0;j<n2;j++){
//            printf("%d",arrB[j]);
//        }
        printf("\n");
        
        arrC = malloc((n1+n2)* sizeof(int));
        
        for(i=0;i<(n1+n2);i++){
            arrC[i] = 0;
        }
        
        sem_init( &semaphore, 0, 1 );
        
        pthread_t tid;
        

        struct args argument;
            argument.arrB = arrB;
            argument.n2 = n2;
            argument.arrA = arrA;
            argument.n1 = n1;
            argument.start = 0;
            argument.end = n2-1;
        
//        printf("%d\n",n1+n2);
        gettimeofday(&start, NULL);
        clock_t begin = clock();
        pthread_create(&tid,NULL,multiply,(void **)&argument);

 
        pthread_join(tid,NULL);
        char outputFilePath[100] = "outputFile.txt";
        FILE *fpr = fopen(outputFilePath,"a+");
        if(fpr == NULL){
            printf("invalid result file");
            exit(0);
        }
        int k=0;
        fprintf(fpr,"\n%s:  ","Single Thread");
        gettimeofday(&stop, NULL);
        printf("\ntook %d\n", stop.tv_usec - start.tv_usec);
        printf("Duration MS %'.3f\n", (double) (stop.tv_sec - start.tv_sec) * 1000 + (double) (stop.tv_usec - start.tv_usec) / 1000);
        for(k=0;k<n1+n2;k++){
            if(!(k == 0 && arrC[k]==0)){
                printf("%d",arrC[k]);
                fprintf(fpr,"%d",arrC[k]);
            }
        }
    }
//    
//    gettimeofday(&stop, NULL);
//    printf("\ntook %d\n", stop.tv_usec - start.tv_usec);
//    printf("Duration MS %'.3f\n", (double) (stop.tv_sec - start.tv_sec) * 1000 + (double) (stop.tv_usec - start.tv_usec) / 1000);
    
    clock_t end = clock();
//    double time_taken = (double) (end-begin) * 1000.0 / CLOCKS_PER_SEC;;
//    printf("Time taken to execute is %f",time_taken);
}


