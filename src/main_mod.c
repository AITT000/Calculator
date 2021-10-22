#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// 디스코드 친구 추가는 뒤에 코드 네자리 더 넣어야 되네
// 아이디에다가 1729 붙이면 된다.

#define MAXL 100

typedef struct _result {
  double *queue;
  int front, rear;
  void (*print)(struct _result pobj);
} calc_result;

void f1(struct _result obj) {
  for (int i = obj.front; i < obj.rear; i++) {
    printf("index=%d, value=%lf\n", i, obj.queue[i]);
  }
}

// 함수의 이름은 동사구 형태여야 자연스럽다.
double handle_brackets(char buf[], int start);
double classify_arrays(char buf[], calc_result brackets, int start);
double proceed_calculation(calc_result numbuf, char opbuf[]);
double parse_double(char from[], const int BEGIN, const int UNTIL);
void shift_arrays(calc_result numbuf, char opbuf[], int rm_index);

int main(void) {
  char input[MAXL] = {0};

  printf("Input : ");
  for (int i = 0, c = 0; (c = getchar()) != '\n'; ) {
    if (isspace(c)) {
      continue;
    }
    input[i++] = c;
  }

  double result = handle_brackets(input, (int)NULL);
  long result_long = (long)result;

  if (result == (double)result_long) {
    printf("Result : %ld\n", result_long);
  } else {
    printf("Result : %lf\n", result);
  }

  getchar();
  return 0;
}

double handle_brackets(char buf[], int start) {
  calc_result brackets = {(double[MAXL / 2]){0}, 0, 0, f1};

  for (int i = start; buf[i] != '\0' && buf[i] != ')'; i++) {
    if (buf[i] == '(') { // recursion
      brackets.queue[brackets.rear++] = handle_brackets(buf, i + 1);
      brackets.print(brackets);
      while (buf[i] != '\0') {
        if (buf[i] == ')') {
          buf[i++] = 'S';
          break;
        } else {
          buf[i++] = 'S';
        }
      }
      i--;
    }
  }

  return classify_arrays(buf, brackets, start);
}

double classify_arrays(char buf[], calc_result brackets, int start) {
  calc_result numbuf = {(double[1 + MAXL / 2]){0}, 0, 0, f1};  // 배열 크기는 opbuf보다 하나 커야함.
  char opbuf[MAXL / 2] = {0};

  for (int i = start, sub_start = i, sub_end; i < MAXL/2; i++) {
    if (buf[i] == '+' || buf[i] == '-' || buf[i] == '*' || buf[i] == '/' ||
        buf[i] == '\0' || buf[i] == ')') {
      sub_end = i - 1;

      if (buf[sub_end] == 'S') {
        numbuf.queue[numbuf.rear] = brackets.queue[brackets.front++];
        numbuf.print(numbuf);
      } else {
        numbuf.queue[numbuf.rear] = parse_double(buf, sub_start, sub_end);
      }

      if ((opbuf[numbuf.rear++] = buf[i]) == '\0' || buf[i] == ')') {
        break;
      }

      sub_start = i + 1;
    }
  }

  return proceed_calculation(numbuf, opbuf);
}

double parse_double(char from[], const int BEGIN, const int UNTIL) {
  char to[MAXL / 2] = {0};
  for (int i = 0; i < UNTIL - BEGIN + 1; i++) {
    to[i] = from[BEGIN + i];
  }
  return atof(to);
}

double proceed_calculation(calc_result numbuf, char opbuf[]) {
  for (int i = 0; opbuf[i] != '\0'; i++) {
    if (opbuf[i] == '*') {
      numbuf.queue[i] = numbuf.queue[i] * numbuf.queue[i + 1];
      shift_arrays(numbuf, opbuf, i);
      i--;
    } else if (opbuf[i] == '/') {
      numbuf.queue[i] = numbuf.queue[i] / numbuf.queue[i + 1];
      shift_arrays(numbuf, opbuf, i);
      i--;
    }
  }

  for (int i = 0; opbuf[i] != '\0'; i++) {
    if (opbuf[i] == '+') {
      numbuf.queue[i] = numbuf.queue[i] + numbuf.queue[i + 1];
      shift_arrays(numbuf, opbuf, i);
      i--;
    } else if (opbuf[i] == '-') {
      numbuf.queue[i] = numbuf.queue[i] - numbuf.queue[i + 1];
      shift_arrays(numbuf, opbuf, i);
      i--;
    }
  }

  return numbuf.queue[numbuf.front];
}

void shift_arrays(calc_result numbuf, char opbuf[], int rm_index) {
  for (int i = rm_index; opbuf[i] != '\0'; i++) {
    opbuf[i] = opbuf[i + 1];
    numbuf.queue[i + 1] = numbuf.queue[i + 2];
  }
  numbuf.rear--;
}
