#include "knn.h"
#define LINE_LENGTH 80

/**
 * This function takes in the name of the text file containing the image names
 * This function should NOT assume anything about the image resolutions,
 * and may be tested with images of different sizes. You will need to allocate
 * your own data for the struct, and also for the images / labels.
 */

void Intrim(char *str){
  char *p; 
  p = strchr(str, '\n');
  if(p != NULL){
    *p = '\0';
  }
}

int Num_Image(const char *filename){
  int count = 0;
  int error; 
  char line[LINE_LENGTH];
  FILE *text_file; 
  text_file = fopen(filename, "r");
  if(text_file != NULL){
    while(fgets(line, LINE_LENGTH + 1, text_file) != NULL){
      count ++;
    }
  }
  error = fclose(text_file);
  if(error != 0){
    return 0; 
  }
  return count; 
}

Dataset *load_dataset(const char *filename) {
  Dataset *newData; 
  int error, check; 
  int label_count = 0; 
  FILE *text_file; 
  newData = (Dataset *)calloc(1, sizeof(Dataset));
  int num_image = Num_Image(filename);
  //Put the number of images into the newData variables 
  newData -> num_items = num_image; 
  //Open the text_file properly and store the handle into the variable
  text_file = fopen(filename, "r");
  if(text_file == NULL){
    return NULL;
  }
  char line[LINE_LENGTH];
  //Declaring number of labels using num_image variable 
  newData -> labels = (unsigned char *)malloc(sizeof(unsigned char) * num_image);
  //Declaring number of images using num_image variable 
  newData -> images = (Image *)calloc(num_image, sizeof(Image));

  //Loop through the text_file to go through each pgm files 
  while(fgets(line, LINE_LENGTH + 1, text_file) != NULL){
    Intrim(line);
    FILE *data_file;
    //Finding labels for newData 
    for(int i = 0; i < LINE_LENGTH; i ++){
      if(strncmp(&line[i], "-", 1) == 0){
        newData -> labels[label_count] = atoi(&line[i + 1]);
        //printf("%hhd\n", newData -> labels[label_count]);
      }
    }
    //declaring data_file for the image 
    data_file = fopen(line, "rb");
    if(data_file == NULL){
      fprintf(stderr, "Error opening file\n");
      return NULL; 
    }
    //Declaring new Image 
    Image *newImage; 
    newImage = (Image *)calloc(1, sizeof(Image));
    char matrix[LINE_LENGTH];
    int matrix_count = 0; 
    int current = 0; 
    //Loop through the data_file and fill in each of the items in Image 
    while(fgets(matrix, LINE_LENGTH, data_file) != NULL){
      //Removing '\n' from matrix string
      Intrim(matrix);
      //putting sx and sy to newImage for the second loop 
      if(matrix_count == 1){
        char *ptr; 
        ptr = strtok(matrix, " ");
        int x = atoi(ptr);
        int y = atoi(matrix);
        newImage -> sx = x;
        newImage -> sy = y;
        int size = x * y;
        newImage -> data = (unsigned char *)calloc(size, sizeof(unsigned char));
      }
      //Putting matrix variables into the data arrays one by one from the third loop
      else if(matrix_count >= 3){
        int i = 0; 
        char *ptr; 
        ptr = strtok(matrix, " ");
        newImage -> data[current + i] = atoi(ptr);
        while(ptr != NULL){
          ptr = strtok(NULL, " ");
          if(ptr != NULL){
            newImage -> data[current + i] = atoi(ptr);
          }
          i ++;
        }
        current += i;
      }
      matrix_count ++;
    }
    //Properly placing newImage pointer intot the images array of Dataset 
    newData -> images[label_count] = *newImage;
    //Close the data_file
    check = fclose(data_file);
    if(check != 0){
      return NULL; 
    }
    label_count ++;
  }
  //Close the text_file 
  error = fclose(text_file);
  if(error != 0){
    return NULL; 
  } 

  return newData; 
}

/****************************************************************************/
/* For all the remaining functions you may assume all the images are of the */
/*     same size, you do not need to perform checks to ensure this.         */
/****************************************************************************/

/** 
 * Return the euclidean distance between the image pixels (as vectors).
 */
double distance(Image *a, Image *b) {
  // TODO: Return correct distance
  int row = a -> sx; 
  int column = a -> sy;
  int size = row * column;
  double euclidean = 0.0;
  for(int pos = 0; pos < size; pos ++){
    double margin = pow(b -> data[pos] - a -> data[pos], 2);
    euclidean += margin; 
  }
  euclidean = sqrt(euclidean);
  return euclidean; 
}

/**
 * Given the input training dataset, an image to classify and K,
 *   (1) Find the K most similar images to `input` in the dataset
 *   (2) Return the most frequent label of these K images
 * 
 * Note: If there's multiple images with the same smallest values, pick the
 *      ones that come first. For automarking we will make sure the K smallest
 *      ones are unique, so it doesn't really matter.
 */ 
int knn_predict(Dataset *data, Image *input, int K) {
  // TODO: Replace this with predicted label (0-9)
  int num_image = data -> num_items; 
  int label_index[num_image];
  double distances[num_image];

  for(int i = 0; i < num_image; i ++){
    distances[i] = distance(&data -> images[i], input);
    label_index[i] = i; 
  }

  double temp1; 
  int temp2; 

  for(int a = 0; a < num_image; a ++){
    for(int b = a + 1; b < num_image; b ++){
      if(distances[b] < distances[a]){
        temp1 = distances[a];
        distances[a] = distances[b];
        distances[b] = temp1;
        temp2 = label_index[a];
        label_index[a] = label_index[b];
        label_index[b] = temp2;
      }
    }
  }

  int K_minimum[K]; 

  for(int z = 0; z < K; z ++){
    K_minimum[z] = data -> labels[label_index[z]];
  }

  int mode; 
  int count = 1; 
  int highest_count = 0; ; 
  for(int g = 0; g < K; g ++){
    for(int v = g + 1; v < K; v ++){
      if(K_minimum[g] == K_minimum[v]){
        count ++;
      }
    }
    if(highest_count < count){
      mode = K_minimum[g];
      highest_count = count; 
    }
    count = 1; 
  }
  return mode;
}

/** 
 * Free all the allocated memory for the dataset
 */
void free_dataset(Dataset *data) {
  // TODO: free data
  for(int i = 0; i < data -> num_items; i ++){
    free(data -> images[i].data);
  }
  free(data -> images);
  free(data -> labels);
  free(data);
}
