#include <stdio.h>
#include <stdlib.h>

char **input(int *height, int *width, int *n_query);
void print_img(int height, char **img);
void swap(int *a, int *b);
char **resize(int arg1, int *height, int *width, char **img);//query1
char **rotate(int *height, int *width, int angle, char **img);//query2
void flip(int height, int width, int flag, char **img);//query3
char *copy(int x1, int y1, int c_h, int c_w, int height, int width, char **img);
void paste(int x2, int y2, int height, int width, char **img, char *temp);//query4
void fill(int x, int y, char color, char **img);//query5


int main() {
	int query, n_query, testcase, height, width;
	int long_height;
    int angle, flag, arg;
	int x1, x2, y1, y2, c_h, c_w, i;
	char **img, *temp, color;

	printf("Input testcase : ");
	scanf("%d", &testcase);

	while(testcase--) {
		img = input(&height, &width, &n_query);
		long_height = height;
		if(img != NULL){ 
			while(n_query--){
				printf("Input query : ");
				scanf("%d", &query);
				switch(query) {		
					case 1 : printf("\nResize\n");
                             printf("Input 0 or 1 : ");
                             scanf("%d", &arg);
                             img = resize(arg, &height, &width, img);
					    	 break;
					case 2 : printf("\nRotate\n");
                             printf("Input 0 or 1 or 2 : ");
                        	 scanf("%d", &angle);
                             img = rotate(&height, &width, angle, img);
				    		 break;	
					case 3 : printf("\nFlip\n");
                             printf("Input 0 or 1 : ");
                        	 scanf("%d", &flag);
                             flip(height, width, flag, img);
					    	 break;
                    case 4 : printf("\nCopy and Paste\n");
                             printf("x1, y1, c_h, c_w, x2, y2 : ");
                             scanf("%d %d %d %d %d %d", &x1, &y1, &c_h, &c_w, &x2, &y2);
                             temp = copy(x1, y1, c_h, c_w, height, width, img);
			    			 paste(x2, y2, height, width, img, temp);
				    		 break;
				/*
                    case 5 : fill(x1, y1, color, img);
						 break;
				 */
					default : printf("Error\n");
				}
				printf("\n");
			}
		}
		if(long_height < height) long_height = height;
		for(i = 0; i < long_height; i++)
			free(*(img + i));
		free(img);
	}
	return 0;

}
char **input(int *height, int *width, int *n_query) {
	int i = 0;
	char **img;

	printf("Input height, width : ");
	scanf("%d %d", height, width);

	if(*height > 4096 || *width > 4096) {
		printf("Size Error\n");
		return NULL;
	}

	img = (char**)malloc(*height * sizeof(char*));
	for (i = 0; i < *height; i++)
		*(img + i) = (char*)malloc((*width + 1) * sizeof(char));

	printf("Input the image : \n");
	for(i = 0; i < *height; i++){
		scanf("%s", *(img + i));
		if(img[i][*width] != '\0'){
			printf("Error\n");
			return NULL;
		}
	}
	printf("Input number of query : ");
	scanf("%d", n_query);
	
	return img;
}

void print_img(int height, char **img){
	int i = 0;
	for(i = 0; i < height; i++)
		printf("%s\n", *(img + i));
}

void swap(int *a, int *b){
    int temp;
    
    temp = *a;
    *a = *b;
    *b = temp;
}

char **resize(int arg1, int *height, int *width, char **img) {
	int i = 0, j = 0, temp_i = 0, temp_j = 0;
	int h = *height, flag = 1;
	char **final_img;
	char **temp_img = (char**)malloc(*height * sizeof(char*));
	for(i = 0; i < h; i++)
		*(temp_img + i) = (char*)malloc((*width + 1) * sizeof(char));
	for(i = 0; i < h; i++)
		for(j = 0; j < *width + 1; j++)
			temp_img[i][j] = img[i][j];

	if(arg1 == 0){
		if(*height * 2 > 4096 || *width * 2 > 4096) {
			printf("Size Error\n");
			flag = 0;
		}// if length excess 4096 print error
		if(flag == 1){
			*height *=2;
			*width *=2;       // double the length
	
			final_img = (char**)malloc(*height * sizeof(char*));
			for(i = 0; i < *height; i++)
				*(final_img + i) = (char*)malloc((*width + 1) * sizeof(char));
	
			for(i = 0, temp_i = 0; i < *height; i++, temp_i++){
				for(j = 0, temp_j = 0; j < *width; j++, temp_j++){
					final_img[i][j] = temp_img[temp_i][temp_j];
					final_img[i+1][j] = temp_img[temp_i][temp_j];
					j++;
					final_img[i][j] = temp_img[temp_i][temp_j];
					final_img[i+1][j] = temp_img[temp_i][temp_j];
				}
				i++;
			}// fill the fianl_img
			for(i = 0; i < *height; i++)
				final_img[i][*width] = '\0';// to print as string
		}
	}
	else if(arg1 == 1){
		if(*height / 2 < 1 || *width / 2 < 1) {
			printf("Size Error\n");
			flag = 0;
		}// if length excess 4096 print error
		if(flag == 1){
			*height /=2;
			*width /=2;       // divide the length as 2
	
			final_img = (char**)malloc(*height * sizeof(char*));
			for(i = 0; i < *height; i++)
				*(final_img + i) = (char*)malloc((*width + 1) * sizeof(char));
	
			for(i = 0, temp_i = 0; temp_i < *height * 2; i++, temp_i++){
				for(j = 0, temp_j = 0; temp_j < *width * 2; j++, temp_j++){
					final_img[i][j] = (temp_img[temp_i][temp_j] + temp_img[temp_i + 1][temp_j] + temp_img[temp_i + 1][temp_j] +  temp_img[temp_i + 1][temp_j + 1]) / 4;
					temp_j++;
				}
				temp_i++;
			}// in the final img put the average of origin img
		for(i = 0; i < *height; i++)
			final_img[i][*width] = '\0';
		}

	}
	else{
		printf("Error\n");
		flag = 0;
	}// print error of arg1
	if(flag == 1){
		printf("Result, resize height : %d, width : %d :\n", *height, *width);
		printf("Result Image\n\n");
		print_img(*height, final_img);
		for(i = 0; i < h; i++)
			free(*(img + i));
		free(img);
	}
	for(i = 0; i < h; i++){
		free(*(temp_img + i));
	}
	free(temp_img);

	if(flag == 0) return img;
	return final_img;
}
char **rotate(int *height, int *width, int angle, char **img){
	int h;
	int i = 0, j = 0;
    if(*height > *width) h = *height;
    else h = *width;

	char **temp_img = (char**)malloc(h * sizeof(char*));
	for(i = 0; i < *height; i++)
		*(temp_img + i) = (char*)malloc((h + 1) * sizeof(char));
	for(i = 0; i < *height; i++)
		for(j = 0; j < *width + 1; j++)
			temp_img[i][j] = img[i][j];

	if(angle != 0 && angle != 1 && angle != 2) {
		printf("Error\n");
		for(i = 0; i < h; i++)
			free(*(temp_img + i));
		free(temp_img);
		return img;
	}

    swap(height, width);
	angle++;

	while(angle--){	
		img = (char**)realloc(img, *height * sizeof(char*));
		for(i = 0; i < *height; i++)
			*(img + i) = (char*)realloc(*(img + i), (*width + 1) * sizeof(char));
		for(i = 0; i < *height; i++){
			for(j = 0; j < *width; j++){
				img[i][j] = temp_img[(*width - 1) - j][i];
			}
		}
        for(i = 0; i < *height; i++)
            img[i][*width] = '\0';
		if(angle != 0){
            swap(height, width);
			for(i = 0; i < *height; i++)
				for(j = 0; j < *width; j++)
					temp_img[i][j] = img[i][j];
            for(i = 0; i < *height; i++)
                temp_img[i][*width] = '\0';
		}
	}

	print_img(*height, img);

	for(i = 0; i < h; i++)
		free(*(temp_img + i));
	free(temp_img);

	return img;
}
void flip(int height, int width, int flag, char **img){
	int i, j;
	
	char **temp_img = (char**)malloc(height * sizeof(char*));
	for(i = 0; i < height; i++)
		*(temp_img + i) = (char*)malloc((width + 1) * sizeof(char));
	
	for(i = 0; i < height; i++)
		for(j = 0; j < width + 1; j++)
			temp_img[i][j] = img[i][j];//copy origin img to tmp_img

	if(flag == 0){
		for(i = 0; i < height; i++)
			for(j = 0; j < width; j++)
				img[i][j] = temp_img[height - 1 - i][j];
	}//flip upside down
	else if(flag == 1){
		for(i = 0; i < height; i++)
			for(j = 0; j < width; j++)
				img[i][j] = temp_img[i][width - 1 - j];
	}//flip left to right
	else printf("Error\n");//case error

	if(flag == 0 || flag == 1){
		printf("Result Image\n\n");
		print_img(height, img);
	}

	for(i = 0; i < height; i++)
		free(*(temp_img + i));
	free(temp_img);//free temp
}

char *copy(int x1, int y1, int c_h, int c_w, int height, int width, char **img){
    int i, j, flag = 1;
    int h, w, t = 0;
    char *temp;
    
    if(0 <= x1 && x1 < height && 0 <= y1 && y1 < width) flag = 1;
	else flag = 0;


    if(c_h <= 0 || c_w <= 0 || c_h > 4096 || c_w > 4096)
        flag = 0;
    
    if(flag == 0){
        printf("Error\n");
        return NULL;
    }

    else if(flag == 1){
        h = x1 + c_h;
        w = y1 + c_w;

        if(h > height)
            h = height;
        if(w > width)
            w = width;

        temp = (char*)malloc(((h * (w + 1)) + 1) * sizeof(char));

        for(i = x1; i < h; i++){
            for(j = y1; j < w; j++){
                temp[t] = img[i][j];
                t++;
            }
            temp[t] = '\n';
            t++;
        }
        temp[t] = '\0';
    }
    return temp;
} 

void paste(int x2, int y2, int height, int width, char **img, char *temp){
    int i, j, t = 0, flag = 1;

    if(temp == NULL) return;
	if (0 <= x2 && x2 < height && 0 <= y2 && y2 < width) flag = 1;
	else flag = 0;

	if (flag == 0) {
		printf("Error\n");
		free(temp);
		return;
	}
	else if (flag == 1) {
		for (i = x2; i < height; i++) {
			for (j = y2; j < width; j++) {
				if (temp[t] == '\n') break;
				img[i][j] = temp[t];
				t++;
			}
			if (temp[t] != '\n') {
				while (1) {
					if (temp[t] == '\n') break;
					t++;
				}
			}
			t++;
			if (temp[t] == '\0') break;
		}
		print_img(height, img);
		free(temp);
	}
}
