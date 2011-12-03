#include "analyze.h"

#define DISTANCE 3

int find_frequency(int data[], int samples){
  int sum = 0;
  int j = 0;
  int last = 0;
  int low = 0;

  for (int i = DISTANCE; i < samples; i++) {
    if (low) {
      if (data[i-DISTANCE] < data[i]) {
        low = 0;
      }
    }
    else {
      if (data[i-DISTANCE] > data[i]) {
        low = 1;
        sum += i-last;
        last = i;
        j++;
      }
    }
  }

  return (1.0f/(((float)sum/(float)j)/44100.0f))/64.0f;
}
