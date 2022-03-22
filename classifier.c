#include "knn.h"

/**
 * Compilation command
 *    gcc -Wall -std=c99 -lm -o classifier classifier.c knn.c
 *
 * Decompress dataset into individual images:
 *    tar xvzf datasets.tgz
 *
 * Running quick test with 1k training and 1k testing images, K = 1:
 *    ./classifier 1 lists/training_1k.txt lists/testing_1k.txt
 *
 * Running full evaulation with all images, K = 7: (Will take a while)
 *    ./classifier 7 lists/training_full.txt lists/testing_full.txt
 */

/*****************************************************************************/
/* Do not add anything outside the main function here. Any core logic other  */
/* than what is described below should go into `knn.c`. You've been warned!  */
/*****************************************************************************/

/**
 * main() takes in 3 command line arguments:
 *    - K : The K value for K nearest neighbours 
 *    - training_list: Name of a text file with paths to the training images
 *    - testing_list:  Name of a text file with paths to the testing images
 *
 * You need to do the following:
 *    - Parse the command line arguments, call `load_dataset()` appropriately.
 *    - For each test image, call `knn_predict()` and compare with real label
 *    - Print out (only) one integer to stdout representing the number of 
 *        test images that were correctly predicted.
 *    - Free all the data allocated and exit.
 */
int main(int argc, char *argv[]) {
  int total_correct = 0;
  Dataset *test_data = NULL;
  Dataset *training_data = NULL;

  training_data = load_dataset(argv[2]);
  test_data = load_dataset(argv[3]);

  int K = atoi(argv[1]);

  int num_image = test_data -> num_items;

  for(int i = 0; i < num_image; i ++){
    int predict = knn_predict(training_data, &test_data -> images[i], K);
    int correct = test_data -> labels[i];
    if(predict == correct){
      total_correct ++;
    }
  }
  printf("%d\n", total_correct);
  return 0;
}