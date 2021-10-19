/* Coded with C Grammar
   This Source uses UTF-8-BOM encoding & CRLF (Windows) line endings
   Alias : calculator.c & Last Modded : 2019.06.02.
   사칙연산이 포함된 수식을 문자열을 입력 받아 그 결과를 계산하여 출력하는 프로그램을 작성하라.
   - 계산식에 공백이 포함될 수 있음
   - 계산식의 수는 양의 정수 (0 포함)만 입력 가능 (음의 정수는 입력 불가)
   - 연산자는 사칙 연산 (+, -, *, /)만 입력 가능
   - 곱셈, 나눗셈 연산은 덧셈, 뺄셈 보다 먼저 계산되어야 함 (실제 계산기 결과와 동일해야 함)
   - 결과는 소수점 2 자리까지 출력할 것
*/

#define _CRT_SECURE_NO_WARNINGS // SDL 검사 오류 경고 무시
#include <stdio.h> // Standard Input/Output Header Library
#define ARR_SIZ 51 // 최초 입력 계산식이 50자리 이상인 경우 오류 발생
double calc_bracket(char *); // ANSI/ISO C 함수 프로토타입
double calc(char *, double *);
int multi_and_divi(double *, char *, int);
int add_and_sub(double *, char *, int);
void rm_space(char *);
void mv_char_array(char *, int, int, int);
void mv_double_array(double *, int, int, int);

/*
이제 여기에서 말해도 될듯?
ㅇㅇ
그 이게 이상한게 아니었고 그냥 구름 계정으로 커밋이 되는건가봄
커밋 로그 확인해봤는데 BrewBan으로 뜨네

그럼 계정 괜히 지운ㄱ너가
ㅇ필요 없는거였지 않나? 그건 맞는데 살짝 아직 미숙한 아이의 짬통이랄까
여튼
튼
이ㅋ이제 뭐하면 되지 니코드보고 ㅋㄴ저ㄴ

일단 내꺼 빌드 되는지부터 봐봐

find: paths must precede expression: `main.c'
find: possible unquoted pattern after predicate `-name'?
/usr/lib/gcc/x86_64-linux-gnu/9/../../../x86_64-linux-gnu/Scrt1.o: In function `_start':
(.text+0x20): undefined reference to `main'
collect2: error: ld returned 1 exit status

*/

int main(void) {
  // 17자리 이상 계산값의 경우 계산 정확도 손실 발생 -_- (ex : 123456789*123456789)
  // double형 정밀도 오류 (ex : 36.5-3+38.7-33.4+1.1*2)
  char str[ARR_SIZ] = {0}; // 스트링 입력 받을 배열 초기화
  // 스트링 입력 받기
  printf("Input : ");
  gets_s(str, sizeof(str)); // or gets(str);
  // 스트링 공백 제거
  rm_space(str);
  // 괄호 선 연산 후 나머지 계산 실행, 계산 값 출력
  printf("Result : %.2f\n", calc_bracket(str));

  int trash; // 함수 반환값 받아 버릴 변수
  trash = getchar(); // or system("pause") with #include <stdlib.h>;
  return 0;
}

double calc_bracket(char * str) { // 괄호 처리 함수
  // 처리 과정
  // 1+(4*(4+(4-2))+(2-1))+(4/(6-8)+1)+(-2) -> 1+(4*(4+@)+(2-1))+(4/(6-8)+1)+(-2), 2 -> 1+(4*@+(2-1))+(4/(6-8)+1)+(-2), 6
  // -> 1+(4*@+@)+(4/(6-8)+1)+(-2), 6, 1 -> 1+@+(4/(6-8)+1)+(-2), 25 -> 1+@+(4/@+1)+(-2), 25, -2 -> 1+@+@+(-2), 25, -1 -> 1+@+@+@, 25, -1, -2 -> 23
  int bracket = 0; // 최외곽 괄호 개수 저장용 변수
  // 최외곽 괄호 개수 세기
  for (int i = 0; i < ARR_SIZ && str[i] != '\0'; i++) {
    if (str[i] == '(') {
      bracket++;
      for (int found_open = 0, found_close = 0; i < ARR_SIZ && str[i] != '\0'; i++) {
        if (str[i] == '(') {
          found_open++;
        } else if (str[i] == ')') {
          found_close++;
          if (found_open == found_close) {
            break;
          }
        }
      }
    }
  }
  // 괄호가 존재하지 않으면 if문, 존재하면 else문
  if (bracket == 0) {
    /// 배열 생성
    double trash[1]; // calc_bracket에 넣을 아무 값 저장용 배열
    /// 전체 연산 실행 & 리턴
    return calc(str, trash);
  } else {
    /// 배열 생성
    double n_bracket[ARR_SIZ/4]; // 괄호 선연산 값 저장용 배열
    char tmpstr[ARR_SIZ/4][ARR_SIZ]; // 괄호 안의 스트링을 저장할 배열
    /// 최외곽 괄호 개수만큼 반복, 곂괄호 존재시 재귀적 연산 실행
    for (int xth_bracket = 0; xth_bracket < bracket; xth_bracket++) {
      int bracket_open = 0, bracket_close = 0, found = 0; // 개-폐괄호 개수 및 인덱스, 곂괄호 존재 여부 저장용 변수
      //// 최외곽 괄호 위치 파악
      for (int i = 0; i < ARR_SIZ && str[i] != '\0' && str[i-1] != ')'; i++) {
        if (str[i] == '(') {
          bracket_open = i;
          for (int found_open = 0, found_close = 0; i < ARR_SIZ && str[i] != '\0'; i++) {
            if (str[i] == '(') {
              found_open++;
            } else if (str[i] == ')') {
              found_close++;
              if (found_open == found_close) {
                bracket_close = i;
                break;
              }
            }
          }
        }
      }
      //// 괄호 안의 스트링을 tmpstr에 복사
      for (int i = 0, j = bracket_open+1; j <= bracket_close; i++, j++) {
        tmpstr[xth_bracket][i] = str[j];
        if (j == bracket_close) {
          tmpstr[xth_bracket][i] = '\0';
        }
      }
      //// 곂괄호 존재 여부 확인
      for (int i = bracket_open+1; i < bracket_close; i++) {
        if (str[i] == '(') {
          found = 1;
          break;
        }
      }
      //// 곂괄호가 있으면 if문, 없으면 esle문 연산 실행
      if (found == 1) {
        n_bracket[xth_bracket] = calc_bracket(tmpstr[xth_bracket]);
      } else {
        double trash[1]; // calc_bracket에 넣을 아무 값 저장용 배열
        n_bracket[xth_bracket] = calc(tmpstr[xth_bracket], trash);
      }
      //// 괄호 계산한 자리에 @ 집어넣기
      mv_char_array(str, ARR_SIZ, bracket_close+1, bracket_open+1);
      str[bracket_open] = '@';
    }
    /// 전체 연산 실행 & 리턴
    return calc(str, n_bracket);
  }
}

double calc(char * str, double * at) { // 스트링을 연산자와 피연산자로 분리하고 계산을 진행하는 함수
  int arrnum = 0; // 연산기호 개수 저장용 변수
  // 연산기호가 몇개인지 검사
  for (int i = 0; i < ARR_SIZ && str[i] != '\0'; i++) {
    if (str[i] == '+' || str[i] == '-' || str[i] == '*' || str[i] == '/') {
      arrnum++;
    }
  }
  // 배열 생성
  char c[ARR_SIZ/2]; // 연산자 저장용 배열
  double n[ARR_SIZ/2]; // 피연산자 저장용 배열
  // 스트링을 배열에 분리
  for (int str_index = 0, n_index = 0, c_index = 0, start_range = 0, dot_point = 0, end_range = 0; str_index < ARR_SIZ; str_index++) {
    int above_point = 0; // 소수점 위의 수(정수) 저장용 변수
    double under_point = 0; // 소수점 아래의 수 저장용 변수
    if (str[str_index] == '+' || str[str_index] == '-' || str[str_index] == '*' || str[str_index] == '/' || str[str_index] == '\0') {
      /// 괄호 선 연산 된 경우
      if (str[str_index-1] == '@') {
        n[n_index++] = *(at++); // at 포인터의 값을 @ 대신 배열에 저장
      }
      /// 괄호 선 연산 되지 않은 경우
      else {
        //// 소수점 존재 여부 확인
        for (; end_range < str_index; end_range++) {
          if (str[end_range] == '.') {
            dot_point = end_range; // 소수점이 존재하면 정수 저장 인덱스 범위를 소수점 발견 인덱스로 설정
            break;
          } else {
            dot_point = str_index; // 소수점이 존재하지 않으면 정수 저장 인덱스 범위를 연산자 위치로 설정
          }
        }
        //// 소수점 위의 수(정수) 저장
        for (; start_range < dot_point; start_range++) {
          above_point *= 10;
          above_point += str[start_range]-'0'; // ASCII코드로 0은 48(D)
        }
        //// 소수점 아래의 수 저장
        if (dot_point == end_range) {
          dot_point = str_index-1; // 연산자는 제외
          for (; dot_point > end_range; dot_point--) {
            under_point += str[dot_point]-'0'; // ASCII코드로 0은 48(D)
            under_point /= 10.0;
          }
        }
        //// 소수점 위의 수(정수)와 소수점 아래의 수를 더하고 배열에 저장
        n[n_index++] = above_point + under_point;
      }
      c[c_index++] = str[str_index];
      end_range = start_range = str_index+1;
    }
    if (str[str_index] == '\0') {
      break;
    }
  }
  // 곱셈과 나눗셈 실시
  arrnum = multi_and_divi(n, c, arrnum);
  // 덧셈과 뺄셈 실시
  arrnum = add_and_sub(n, c, arrnum);
  // 결과값이 저장된 배열 값 리턴
  return n[arrnum]; // or return n[arrnum];
}

int multi_and_divi(double * n, char * c, int arrnum) { // 곱하기 & 나누기 연산 함수
  for (int i = 0; i < arrnum; i++) {
    for (int j = 0; j < arrnum; j++) {
      // 곱하기 기호일 경우
      if (c[j] == '*') {
        n[j] = n[j]*n[j+1];
        mv_double_array(n, arrnum, j+2, j+1);
        mv_char_array(c, arrnum, j+1, j);
        arrnum--;
        j--;
      }
      // 나누기 기호일 경우
      else if (c[j] == '/') {
        n[j] = n[j]/n[j+1];
        mv_double_array(n, arrnum, j+2, j+1);
        mv_char_array(c, arrnum, j+1, j);
        arrnum--;
        j--;
      }
    }
  }
  return arrnum;
}

int add_and_sub(double * n, char * c, int arrnum) { // 더하기 & 빼기 연산 함수
  for (int i = 0; i < arrnum; i++) {
    for (int j = 0; j < arrnum; j++) {
      // 더하기 기호일 경우
      if (c[j] == '+') {
        n[j] = n[j]+n[j+1];
        mv_double_array(n, arrnum, j+2, j+1);
        mv_char_array(c, arrnum, j+1, j);
        arrnum--;
        j--;
      }
      // 빼기 기호일 경우
      else if (c[j] == '-') {
        n[j] = n[j]-n[j+1];
        mv_double_array(n, arrnum, j+2, j+1);
        mv_char_array(c, arrnum, j+1, j);
        arrnum--;
        j--;
      }
    }
  }
  return arrnum;
}

void rm_space(char * str) { // 공백 제거 함수
  int space = 0;
  // 공백 개수 세기
  for (int i = 0; i < ARR_SIZ && str[i] != '\0'; i++) {
    if (str[i] == ' ') {
      space++;
    }
  }
  // 공백 제거
  for (; space > 0; space--) {
    for (int i = 0; i < ARR_SIZ && str[i] != '\0'; i++) {
      if (str[i] == ' ') {
        mv_char_array(str, ARR_SIZ, i+1, i);
        break;
      }
    }
  }
}

void mv_char_array(char * str, int size, int from, int to) { // char 배열[from]부터 배열의 끝까지를 배열[to]부터로 이동시키는 함수
  while (from <= size && str[from-1] != '\0') {
    str[to++] = str[from++];
  }
}

void mv_double_array(double * num, int size, int from, int to) { // double 배열[from]부터 배열의 끝까지를 배열[to]부터로 이동시키는 함수
  while (from <= size) {
    num[to++] = num[from++];
  }
}
