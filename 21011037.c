#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
/*
@brief iki alt diziyi birlestirir
@param n1 1.dizinin boyutu
@param n2 2.dizinin boyutu
@param leftArr sol dizi
@param rightArr sag dizi
@param arr siralanacak ve siralanmis dizi
*/
void merge(int *arr, int left, int mid, int right){
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;
    int leftArr[n1], rightArr[n2];

    // diziyi gecici dizilere kopyala
    for (i=0; i<n1; i++)
        leftArr[i] = arr[left + i];
    for (j=0; j< n2; j++)
        rightArr[j] = arr[mid + 1 + j];

    // Gecici dizileri orijinal diziye birlestir
    i = 0;
    j = 0;
    k = left;
    while(i< n1 && j < n2){
        if(leftArr[i] <= rightArr[j]){
            arr[k] = leftArr[i];
            i++;
        }
        else{
            arr[k] = rightArr[j];
            j++;
        }
        k++;
    }
    //varsa kalan elemanlari kopyala
    while(i < n1){
        arr[k] = leftArr[i];
        i++;
        k++;
    }
    while(j < n2){
        arr[k] = rightArr[j];
        j++;
        k++;
    }
}
/*
    @brief diziyi siralar
    @param arr siralanacak dizi
    @param left dizinin sol indeksi
    @param right dizinin sag indeksi
    @param mid dizinin orta indeksi
*/
void mergeSort(int *arr, int left, int right){
    if (left< right){
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}
/*
    @brief K-way merge islemi: Diziyi K'ya kadar parcalara bolerek siralar
    @param arr siralanacak dizi
    @param n dizinin boyutu
    @param n2 dizinin boyutu minvalue karsilastirmasi icin
    @param K K degeri
    @param partSize olusacak alt dizilerin boyutu
    @param result sonuc dizisi
*/
void kWayMerge(int *arr, int n, int n2, int K){
    int partSize = (n + K - 1) / K;  // her alt dizinin boyutu
    // alt dizileri siralama islemi
    int start = 0;
    while (start < n){
        int end = start + partSize;
        if(end> n)
            end = n;
        // eger alt dizinin boyutu K'dan kucukse, bu diziyi merge sort ile sirala
        if(end - start < K){
            mergeSort(arr, start, end - 1);
        } 
        else{
            kWayMerge(arr + start, end - start, n2, K);  // kdan kucuk degilse alt diziyi yine k'ya bolerek devam et
        }

        start = end;
    }

    // dizilerin en kucuk elemanlarini bulma ve birlestirme
    int *result = (int*)malloc(sizeof(int)*n);  // sonuc dizisi
    int indices[K];  // alt dizilerin ilk elemanlarinin indeksleri
    int i;
    for(i= 0; i<K; i++){
        indices[i]=i*partSize;  // ilk elemanlarin indekslerini hesapla
    } 

    int index = 0;
    int flag = 1;
    while(flag){
        int minVal =n2;
        int minIndex = -1;
        // alt dizilerin en kucuk elemanlarini bulma
        for (i = 0; i < K; i++) {
            int start= i* partSize;
            int end =start + partSize;
            if (end > n) 
                end = n;
            if (indices[i] < end && arr[indices[i]] < minVal){
                minVal = arr[indices[i]];
                minIndex = i;
            }
        }
        // eger alt diziler bitmisse cik
        if(minIndex == -1){
            flag = 0;
        }
        else{
        result[index++] = minVal;  // en kucuk elemani sonuca ekle
        indices[minIndex]++;  // en kucuk elemanin bulundugu alt dizinin indeksini bir arttir         
        }
    }
    // sonuc dizisini orijinal diziye kopyalama. diger k degerleri icin siralanmis diziyi kullanmamak icin burayi calistirmadim
    //calistirirsak dizin siralanmis hal orijinale kopyalanir 
    for (i=0; i<n; i++) {
         arr[i] = result[i];
     }
    free(result);
}

/*
    @brief diziyi ekrana yazdirma fonksiyonu
    @param arr yazdirilacak dizi
*/
void printArray(int *arr, int n){
    int i;
    for(i= 0; i<n; i++){
        printf("%d ", arr[i]);
    }
    printf("\n");
}

/*
    @brief diziyi olusturma fonksiyonu
    @param n dizinin boyutu
    @return olusturulan dizi
*/
int* createArray(int n){
	int i, *arr;
	arr = (int*)malloc(n*sizeof(int));
	for(i=0;i<n;i++){
		arr[i]=i+1;
	}
    return arr;
}

/*
    @brief diziyi karistirma fonksiyonu
    @param arr karistirilacak dizi
    @param n dizinin boyutu
*/
void shuffleArray(int *arr,int n){
    int i, j, temp;
    for(i=0;i<n;i++){
        j = rand()% (n-1);
        temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}
int main(){
	srand(time(NULL));
	//N = 100, N = 1000, N = 10000, N = 100000, N = 1000000, N = 10000000
    int i,j,k,***arrays,n=100;
    struct timeval start, end;
    //FILE *file = fopen("output.csv", "w");
    //fprintf(file, "n,k,sure\n");
    arrays = (int***)malloc(6*sizeof(int**));
    for(i=0;i<6;i++){//6 farklı n degeri var
        arrays[i] = (int**)malloc(10*sizeof(int*));
        for(j=0;j<10;j++){//her n degeri  icin 10 farkli dizi
            arrays[i][j] = (int*)malloc(n*sizeof(int));
            arrays[i][j] = createArray(n);

            for(k=2;k<=10;k++){//her diziyi 9 farklı k degeri ile sirala
                shuffleArray(arrays[i][j],n);
                printArray(arrays[i][j],n);
                gettimeofday(&start, NULL);
                kWayMerge(arrays[i][j],n,n, k);
                printArray(arrays[i][j],n);//bunun icin kwaymergesortta orijinal diziye kopyalama islemini acman lazım
                gettimeofday(&end, NULL);
                double duration = (end.tv_sec - start.tv_sec) * 1000.0;
                duration += (end.tv_usec - start.tv_usec) / 1000.0;

                printf("n = %d %d.dizi k= %d K-way merge sort suresi: %.3f milisaniye\n",n,j, k, duration);    
                //fprintf(file, "%d,%d,%.3f\n", n, k, duration);
            }
            
        }
        n*=10;
    }
    //fclose(file);
    free(arrays);
    for(i=0;i<6;i++){
        free(arrays[i]);
        for(j=0;j<10;j++){
            free(arrays[i][j]);
        }
    }
    
    return 0;
}

