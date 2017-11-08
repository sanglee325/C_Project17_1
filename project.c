#include <stdio.h>
#include <stdlib.h>

char **input(int *height, int *width, int *n_query);
/*inputs height, width, and number of query*/

void print_img(int height, char **img);
/*print img*/

char **resize(int arg1, int *height, int *width, char **img);//query1
/*if arg1 is 0, it doubles the img.
 *if arg1 is 1, it divides the height and width of img as 2*/

char **rotate(int *height, int *width, int angle, char **img);//query2
/*this function rotates the img.
 *when the angle input case is 0, it turns 90 degrees. in case 1 it turns 180 degrees. in case 2 it turns 270 degrees*/

void flip(int height, int width, int flag, char **img);//query3
/*this function flips the img upside down of left to right*/

char *copy(int x1, int y1, int c_h, int c_w, int height, int width, char **img);
/*this fuction copies the selected area*/

void paste(int x2, int y2, int height, int width, char **img, char *temp);//query4
/*this fuction paste the copied img to origin img*/

void fill(int y, int x, char color, char **img, int height, int width, int *count);//query5
/*this function changes the selected pixel and the pixels that have same color.
 * The changed pixels must be connect to the selected pixel*/

int main() {
	int query, n_query, testcase, height, width;
    int angle, flag, arg, count = 0;
	int x1, x2, y1, y2, c_h, c_w, i;
	char **img, *temp, color;

	printf("Input testcase : ");
	scanf("%d", &testcase);
    /*gets the number of testcase*/

	while(testcase--) {
		img = input(&height, &width, &n_query);
		/*input the img and if the img is NULL the testcase ends 
         * and start the new testcase if the number of times are left*/
        if(img != NULL){ 
			while(n_query--){
				printf("Input query : ");
				scanf("%d", &query);
				/*input the value of query*/
                switch(query) {		
					case 1 : printf("\nResize\n");
                             printf("Input 0 or 1 : ");
                             scanf("%d", &arg);
                             img = resize(arg, &height, &width, img);
					    	 break;
					/*if query = 1, resize the image*/

                    case 2 : printf("\nRotate\n");
                             printf("Input 0 or 1 or 2 : ");
                        	 scanf("%d", &angle);
                             img = rotate(&height, &width, angle, img);
				    		 break;	
					/*if query = 2, rotate the image*/
					
                    case 3 : printf("\nFlip\n");
                             printf("Input 0 or 1 : ");
                        	 scanf("%d", &flag);
                             flip(height, width, flag, img);
					    	 break;
					/*if query = 3, flip the image*/
                    
                    case 4 : printf("\nCopy and Paste\n");
                             printf("x1, y1, c_h, c_w, x2, y2 : ");
                             scanf("%d %d %d %d %d %d", &x1, &y1, &c_h, &c_w, &x2, &y2);
                             temp = copy(x1, y1, c_h, c_w, height, width, img);
			    			 paste(x2, y2, height, width, img, temp);
				    		 break;
					/*if query = 4, copy and paste the image*/
                    
                    case 5 : count = 0;
                             printf("\nFill\n");
                             printf("Input y, x, color : ");
                             scanf("%d %d %c", &y1, &x1, &color);
                             fill(y1, x1, color, img, height, width, &count);
                             printf("Number of colored pixels = %d\n", count);
						     break;
					/*if query = 5, fill the img with new color*/
					
                    default : printf("Error\n");
					/*if query the query doesn't apply to all above, print error and get a new query*/
				}
                
                print_img(height, img);
				printf("\n");
                /*print result img*/
			}
            for(i = 0; i < height; i++)
			        free(*(img + i));
	        free(img);
		}
	}
	return 0;

}
char **input(int *height, int *width, int *n_query) {
	int i = 0, j = 0, count = 0;
	char **img, temp;

	printf("Input height, width : ");
	scanf("%d %d", height, width);
    scanf("%c", &temp);
	if(*height > 4096 || *width > 4096 || *height <= 0 || *width <= 0) {
		printf("Size Error\n");
		return NULL;
	}
    /*if the height or width excess 4096 print size error and end the fuction*/

    img = (char**)malloc(*height * sizeof(char*));
    for (i = 0; i < *height; i++)
        *(img + i) = (char*)malloc((*width + 1) * sizeof(char));

    printf("Input the image : \n");
    for(i = 0; i < *height; i++){
        for(j = 0; j < *width + 1; j++){
            scanf("%c", &img[i][j]);
            if(img[i][j] == ' ' || img[i][j] == '\t') j--; /*if the img gets space or tab, ignore it*/
            if(img[i][j] == '\n' && j != *width){
                printf("Error\n");
                for(i = 0; i < *height; i++)
                    free(*(img + i));
                free(img);
                /*if the img gets the number of pixel less than setted height or width,
                 * print error and end the testcase*/

                return NULL;
            }
        }
        if(img[i][*width] != '\n') {
            printf("Error\n");
            for(i = 0; i < *height; i++)
                free(*(img + i));
            free(img);
            /*if the img gets the excessive amount of pixels than width, print
             * error and end the test case*/
            return NULL;
        }
    }
    
    for(i = 0; i < *height; i++)
        img[i][*width] = '\0';
 	/*put the \0 in the last pixel of each line because the result img will be
     * printed it string*/
    
    printf("Input number of query : ");
	scanf("%d", n_query);
    if(*n_query < 0) {
        printf("Error\n");
        for(i = 0; i < *height; i++)
                free(*(img + i));
        free(img);

        return NULL;
    }
	/*if the number of query is negative number, it prints the error and wil end
     * the test case*/
	return img;
}

void print_img(int height, char **img){
	int i = 0;
	
    printf("Result Image\n\n");
	for(i = 0; i < height; i++)
		printf("%s\n", *(img + i));
}
/*prints the result img as string*/

char **resize(int arg1, int *height, int *width, char **img) {
	int i = 0, j = 0, temp_i = 0, temp_j = 0;
    int h = *height, flag = 1;
	char **final_img;
	
    char **temp_img = (char**)malloc(*height * sizeof(char*));
	for(i = 0; i < *height; i++)
		*(temp_img + i) = (char*)malloc((*width + 1) * sizeof(char));
	
    for(i = 0; i < *height; i++)
		for(j = 0; j < *width + 1; j++)
			temp_img[i][j] = img[i][j];
    /*copy img to temp_img*/

	if(arg1 == 0){
		if(*height * 2 > 4096 || *width * 2 > 4096) {
			printf("Size Error\n");
			flag = 0;
		}
        /* if change length excess 4096 print error*/
		if(flag == 1){
			*height *=2;
			*width *=2;       
            /* double the length*/
	
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
			}
            /* fill the final_img*/
			for(i = 0; i < *height; i++)
				final_img[i][*width] = '\0';// to print as string
		}
	}
	
    else if(arg1 == 1){
		if(*height / 2 < 1 || *width / 2 < 1) {
			printf("Size Error\n");
			flag = 0;
		}
        /* if length of changed height or width is less than 1 print error*/
		
        if(flag == 1){
			*height /=2;
			*width /=2;       
            /*divide the length as 2*/
	
			final_img = (char**)malloc(*height * sizeof(char*));
			for(i = 0; i < *height; i++)
				*(final_img + i) = (char*)malloc((*width + 1) * sizeof(char));
	
			for(i = 0, temp_i = 0; temp_i < *height * 2; i++, temp_i++){
				for(j = 0, temp_j = 0; temp_j < *width * 2; j++, temp_j++){
					final_img[i][j] = (temp_img[temp_i][temp_j] + temp_img[temp_i + 1][temp_j] + temp_img[temp_i + 1][temp_j] +  temp_img[temp_i + 1][temp_j + 1]) / 4;
					temp_j++;
				}
				temp_i++;
			}
            /* in the final img put the average of origin img*/
		
            for(i = 0; i < *height; i++)
			    final_img[i][*width] = '\0';
		}
	}
	else if(arg1 != 0 && arg1 != 1){
		printf("Error\n");
		flag = 0;
	}
    /* print error when it gets unappropriate arg1*/
	
    if(flag == 1){
		printf("Result, resize height : %d, width : %d :\n", *height, *width);
		for(i = 0; i < h; i++)
			free(*(img + i));
		free(img);
	}
	/*print the altered height and width*/

    for(i = 0; i < h; i++)
		free(*(temp_img + i));
	free(temp_img);

	if(flag == 0) return img;
	return final_img;
}
char **rotate(int *height, int *width, int angle, char **img){
	int new_height, new_width, min_h;
	int i = 0, j = 0;
	int temp;

    char **temp_img = (char**)malloc(*height * sizeof(char*));
	for(i = 0; i < *height; i++)
		*(temp_img + i) = (char*)malloc((*width + 1) * sizeof(char));
	for(i = 0; i < *height; i++)
		for(j = 0; j < *width + 1; j++)
			temp_img[i][j] = img[i][j];
    /*copy the origin img to temp_img*/

	if(angle != 0 && angle != 1 && angle != 2) {
		printf("Error\n");
		for(i = 0; i < *height; i++)
			free(*(temp_img + i));
		free(temp_img);
		return img;
	}
    /*if the angle is not the appropriate case, print error and end the
     * function*/

	angle++;

	while(angle--){	
        new_height = *width;
        new_width = *height;
        
        if(new_height > *height) min_h = *height;
        else min_h = new_height;
        
        for(i = new_height; i < *height; i++)
            free(img[i]);
		img = (char**)realloc(img, new_height * sizeof(char*));
        for(i = 0; i < min_h; i++)
			*(img + i) = (char*)realloc(*(img + i), (new_width + 1) * sizeof(char));
		for (i = min_h; i < new_height; i++)
			*(img + i) = (char*)malloc((new_width + 1) * sizeof(char));

		for(i = 0; i < new_height; i++)
			for(j = 0; j < new_width; j++)
				img[i][j] = temp_img[new_width - 1 - j][i];
			
        for(i = 0; i < new_height; i++)
            img[i][new_width] = '\0';
		

        /*if the number of rotation is left, copy the rotated img to the temp*/
        if(angle != 0){
			for (i = new_height; i < *height; i++)
				free(temp_img[i]);
			temp_img = (char**)realloc(temp_img, new_height * sizeof(char*));
            for (i = 0; i < min_h; i++)
				*(temp_img + i) = (char*)realloc(*(temp_img + i), (new_width + 1) * sizeof(char));
			for (i = min_h; i < new_height; i++)
				*(temp_img + i) = (char*)malloc((new_width + 1) * sizeof(char));
			for(i = 0; i < new_height; i++)
				for(j = 0; j < new_width; j++)
					temp_img[i][j] = img[i][j];
            for(i = 0; i < new_height; i++)
                temp_img[i][new_width] = '\0';
		}

        temp = *height;
        *height = *width;
        *width = temp;
        /*exchange the value of height and width*/
	}

    printf("Result_rotate height : %d, width : %d :\n", *height, *width);

	for(i = 0; i < new_width; i++)
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
			temp_img[i][j] = img[i][j];
    /*copy origin img to tmp_img*/

	if(flag == 0){
		for(i = 0; i < height; i++)
			for(j = 0; j < width; j++)
				img[i][j] = temp_img[height - 1 - i][j];
	}
    /*flip upside down*/
	
    else if(flag == 1){
		for(i = 0; i < height; i++)
			for(j = 0; j < width; j++)
				img[i][j] = temp_img[i][width - 1 - j];
	}
    /*flip left to right*/
	
    else printf("Error\n");
    /*case error*/

	for(i = 0; i < height; i++)
		free(*(temp_img + i));
	free(temp_img);
}

char *copy(int x1, int y1, int c_h, int c_w, int height, int width, char **img){
    int i, j, flag = 1;
    int h, w, t = 0;
    char *temp;
    
    if(0 <= x1 && x1 < width && 0 <= y1 && y1 < height) flag = 1;
	else flag = 0;
    /*if the value of x1 or y1 is not in the right range, set flag = 0*/

    if(c_h <= 0 || c_w <= 0 || c_h > 4096 || c_w > 4096)
        flag = 0;
    /*if the selection of area which will be copied excess the appropriate
     * range, set flag = 0*/

    if(flag == 0){
        printf("Error\n");
        return NULL;
    }
    /*if flag is 0, the cases are Error so print error and end the function*/

    else if(flag == 1){
        h = y1 + c_h;
        w = x1 + c_w;

        if(h > height)
            h = height;
        if(w > width)
            w = width;
        /*check the area which will be copied.
         * limit the range which will be copied to length of height and width*/

        temp = (char*)malloc(((h * (w + 1)) + 1) * sizeof(char));
        /*the temp will contain \n in the place where it seperates the next
         * line and \0 in the end of this array*/

        for(i = y1; i < h; i++){
            for(j = x1; j < w; j++){
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
    /*if temp is NULL end the function*/

	if (0 <= x2 && x2 < width && 0 <= y2 && y2 < height) flag = 1;
	else flag = 0;
    /* if the position of pixel which will be pasted is not in the range of
     * height and width, flag = 0*/

	if (flag == 0) {
		printf("Error\n");
		free(temp);
		return;
	}
    /*position of x2 of y2 is out of range so print error and end the function*/
	
    else if (flag == 1) {
		for (i = y2; i < height; i++) {
			for (j = x2; j < width; j++) {
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
		free(temp);
	}
    /*if the position is appropriate, paste the copied img.
     * if the copied img is larger than the area which will be pasted, ignore
     * the pixels that excess the range*/
}

void fill(int y, int x, char color, char **img, int height, int width, int *count){
    char origin;
    int flag = 0; 
    
    if(0 <= x && x < width && 0 <= y && y < height){
        flag = 1;
    }
    else flag = 0;
    /*if the position of x or y is out of range set flag = 0*/

    if(flag == 0){
        printf("Error\n");
        return;
    }
    /*if flage is 0 print error and end the function*/

    if(color == img[y][x]) return;
    /*if the color is same with the img which will be changed, end the function*/

    if(flag == 1){
        origin = img[y][x];
        img[y][x] = color;
        (*count)++;
        /*counts the number of pixels that have been changed to new color*/

        if(x != width - 1){
            if(origin == img[y][x + 1])
                fill(y, x + 1, color, img, height, width, count);
        }

        if(y != height - 1 ){
            if(origin == img[y + 1][x])
                fill(y + 1, x, color, img, height, width, count);
        }

        if(y != 0){
            if(origin == img[y - 1][x])
                fill(y - 1, x, color, img, height, width, count);
        }

        if(x != 0){
            if(origin == img[y][x - 1])
                fill(y, x - 1, color, img, height, width, count);
        }
    }
    /*if the pixel left, right, up or down has the same color with img[x][y],
     * change the img to the designated color*/
}
