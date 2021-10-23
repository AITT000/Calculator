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
중괄호가 바로 옆에 붙는게 K&R

아 그리고 이 구글 계정 복구 이메일을 니 네이버 메일로 등록해놨으니까
혹시 또 로그인 안되거나 그러면 이메일 인증쪽으로 보면 될듯 오 굿
아 mv_char_array 왜 이생각 못했지
one_pull 이러고 있네 ㅋㅋㅋ 사실 one_pull 방금전까지 뭐하는건지 모르고있었다 ㅋㅋㅋㅋㅋㅋ
근데 지금 와서 보니 shift가 mv보다 적절한듯 그런듯 무브는 어떤 빈거에다가 갖다 넣는 느낌이지 않나
그런가? shift가 나은듯 
아 참고로 이거 비주얼 스튜디오 2015에서 컴파일하는 용도로 만들어진 버전이라서 가변길이 배열은 적용 안되어있다. ㅇㅇ

main.c 이상한점 미리 적어둠
STATR는 왜 디파인이 되어있고, 메인 안에 변수로도 있는가?, 그리고 그걸 넌 왜 함수로 전달했고, 함수로 전달하면서 const int가 아니라 그냥 int로 받았는가 -> 이거만 이거 무슨 말임? const int
main_ori 보면 상수 매크로인 START를 함수안에 넣어서 전달하던데, 상수라는건 원래 변하지 않는 값으로 쓰려고 넣는거잖아 start랑 START랑 똑같이 인식함? 내가 똑같이 인식했냐는 말이가? ㄴㄴ 컴퓨터가
C언어는 call by value니까 니가 상수 매크로인 START를 넣어도 그 값만 함수로 전달되기 때문에 함수안의 변수 start는 변하는 값이 되는게 맞긴 한데
변하는 값으로 할거면 왜 함수에다가 상수로 우리가 인식하는 값을 넣냐고
brace_searching(buf, subbuf, opbuf, numbuf, convbuf, START, buf_max); 이부분 말하는거 ㅇㅇ 근데 아 그런가? 완전히 컴퓨터 공학 분야의 커스텀을 부정하고 있잖아 어 맞나? ㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋ
brace_searching(buf, subbuf, opbuf, numbuf, convbuf, 0, buf_max);하기에는 어색해서 그런건데 왜 어색해? 0이 보기 별로임 다 영어인데 나만 그런가 그래서 그냥 START해ㅔㅆ었음 ㅋㅋㅋㅋ
흠 그 숫자를 바로 집어넣는게 좋지 않을 수도 있는 부분이
유지보수 관점에서 보면 저 숫자가 뭔지 모르는 문제가 있거든

근데 그래도 상수를 넣는건 진짜 비 상식적이고
유지 보수적인 관점에서 니가 한것 처럼 하려면
상수를 함수 호출시 집어넣고 함수가 받을때는 const int begin 이렇게 받고
함수 안에서 함수 시작하자 마자
int start = begin; 이렇게 대입해주는게 맞음. 이러면 상수를 함수에 보냈는데 그걸 변형한다 라는 느낌보다는 가독성 측면에서 그렇게 했구나 하는 느낌이다이가. 이해됨? 받을때는 const int begin 이렇게 받고 가
return을 const int begin 형으로 return한다는 거가? 형
ㅡㅎㅁ한숨 쉬지 마요 수명 줄어요ㅁ main_ori로 와봐



그래서 함수로 전달할 때도 int형으로 전달하는게 아니라 const int로 전달하는게 상식적임


temporary는 왜 전역변수인가?, 메인 안에 있는 end 변수는 왜 사용하지 않는가?, brace_searching은 왜 리턴 값 받지도 않으면서 int형을 반환한다고 선언했는가?
음 더 기억나는건 없네

혹시 질문 있나요? 없으면 나 시험공부하게 아 맞네 셤기간이제



*/

int main(void) {
  // 17자리 이상 계산값의 경우 계산 정확도 손실 발생 -_- (ex : 123456789*123456789)
  // double형 정밀도 오류 (ex : 36.5-3+38.7-33.4+1.1*2)
  char str[ARR_SIZ] = {0}; // 스트링 입력 받을 배열 초기화
  // 스트링 입력 받기
  printf("Input : ");
  gets(str);  //gets_s(str, sizeof(str));
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
