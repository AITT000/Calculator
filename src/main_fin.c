#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define MAXL 100

void brace_searching(char *, char *, double *, int, int);
void make_sub_op_numbuf(char *, char *, double *, int, int);
double final_calc(char *, char *, double *,int);
double convert_buf(char *, int, int);
void one_pullc(char *, int);
void one_pulln(double *, int);
void calc(double *, char *);
void buf_init(char *);
void rearrange(double *, char *, int, int);

/*
개략적인 설명:
1.  수식을 받아오면 buf에 저장한다
2.  받아온 수식에 괄호가 있는지 찾은 후 괄호가 있는 경우 맨 뒤의 괄호부터 수식의 앞쪽으로 계산을 수행한다.
	이 때 괄호를 계산한 값은 temporary배열에 저장하고 괄호문을 특정문자로 통일한다.
	예를 들어 (1+2)는 AAAAA로 통일한다. 해당 문자가 temporary의 몇 번째 index에 저장되어 있는지 알 수 있게 하기위해 cnt를 이용한다.
	ex) int cnt = 0;으로 선언 후 괄호문의 특정문자를 'A' + cnt로, 연산값은 temporary[cnt]에 저장한다.
3.  위 과정을 통해 괄호를 모두 계산한 후 마지막으로 괄호가 없는 수식에서 마지막 연산을 진행 후 result에 값을 저장하고 출력한다.
*/

double temporary[MAXL] = {};//괄호로 인해 먼저 계산된 값 저장하는 배열
int cnt = 0;

int main()
{
	int c;
	int i = 0;
	int start = 0;
	char buf[MAXL] = {};
	double numbuf[MAXL] = {};
	char opbuf[MAXL] = {};
	
	//수식 받아오기
	while((c = getchar()) != '\n')
	{
		if(!isspace(c))
			buf[i++] = c;
	}
	buf[i] = '\0';
	int buf_max = i;

	brace_searching(buf, opbuf, numbuf, start, buf_max);
	//괄호 서칭 후 괄호 내부 수식 계산

	double result = final_calc(buf, opbuf, numbuf, buf_max);
	//괄호내부 수식을 모두 계산한 후 괄호가 없는 최종식 계산

	printf("%.2lf\n", result);

	getchar();
	return 0;
}

void brace_searching(char * buf, char * opbuf, double * numbuf, int start, int buf_max)//괄호 서칭 후 괄호 내부 수식 계산하는 함수
{
	for(int i = start; i <= buf_max; i++)
	{
		if(buf[i] == '(')
		{
			start = i;
			brace_searching(buf, opbuf, numbuf, start + 1, buf_max);//recursion
		}
	}
	for(int i = start, end = 0; i <= buf_max && buf[start] == '('; i++)
	//recursion에서 함수 인자 start에 start + 1을 넣어주므로 recursion에서 '('를 발견하지 못하고 ')'만 발견될 경우 for문 조건에 의해 for문으로 진입 하지 않고 return하게 된다.
	//따라서 수식의 맨 뒤에있는 괄호부터 계산하게 된다.
	{
		if(buf[i] == ')')
		{
			end = i;
			make_sub_op_numbuf(buf, opbuf, numbuf, start, end);
			//괄호를 제외한 수식들을 subbuf에 저장, subbuf에서 연산자는 opbuf에 피연산자는 numbuf에 저장
			calc(numbuf, opbuf);
			//numbuf와 opbuf를 이용해 연산
			rearrange(numbuf, buf, start, end);
			//기존 buf에서 괄호문을 특정문자로 대체
			cnt++;
			//특정문자 지정을 위한 cnt값 증가
			return ;
		}
	}
	return ;
}

void make_sub_op_numbuf(char * buf, char * opbuf, double * numbuf, int start, int end)//sub,op,numbuf 만드는 함수
{
	char subbuf[MAXL] = {};
	//괄호를 제외한 수식을 담을 배열
	int num = end - start - 1;
	//괄호를 제외한 수식의 갯수
	for(int i = 0; i < num; i++)
	{
		subbuf[i] = buf[start + 1 + i];
	}
	subbuf[num] = '\0';

	int sub_end = 0;//subbuf의 연산자 사이 숫자의 시작 인덱스 ex) +524+ 에서 5의 인덱스
	int sub_start = 0;//subbuf의 연산자 사이 숫자의 끝 인덱스 ex) +524+ 에서 4의 인덱스
	for(int i = 0, j = 0; i <= num; i++)
	{
		if(subbuf[i] == '+' || subbuf[i] == '-' || subbuf[i] == '*' || subbuf[i] == '/' || subbuf[i] == '\0')
		{
			opbuf[j] = subbuf[i];//opbuf에 연산자 저장

			sub_end = i - 1;
			if(subbuf[sub_end] >= 'A')
			{
				numbuf[j++] = temporary[subbuf[sub_end] - 'A'];//특정문자일 경우 특정문자에 해당하는 temporary의 값을 대입
			}
			else
			{
				numbuf[j++] = convert_buf(subbuf, sub_end, sub_start);//subbuf의 숫자를 convert_buf함수를 이용해 double형으로 변환 후 numbuf에 대입
			}
			sub_start = i + 1;
		}
	}
}

double final_calc(char * buf, char * opbuf, double * numbuf, int buf_max)//괄호를 모두 계산한 후 최종 수식의 계산
{
	int end = 0;
	int start = 0;
	for(int i = 0, j = 0; i <= buf_max; i++)
	{
		if(buf[i] == '+' || buf[i] == '-' || buf[i] == '*' || buf[i] == '/' || buf[i] == '\0')
		{
			opbuf[j] = buf[i];//opbuf에 연산자 저장

			end = i - 1;
			if((buf[end]) >= 'A')
			{
				numbuf[j++] = temporary[buf[end] - 'A'];//특정문자일 경우 특정문자에 해당하는 temporary의 값을 대입
			}
			else
			{
				numbuf[j++] = convert_buf(buf, end, start);//buf의 숫자를 convert_buf함수를 이용해 double형으로 변환 후 numbuf에 대입
			}
			start = i + 1;
		}
	}
	calc(numbuf,opbuf);
	return numbuf[0];
}

double convert_buf(char * from, int E, int S)
{
	char convbuf[MAXL] = {};//변환을 위해 임시적으로 담을 converstionbuf 선언
	for(int i = 0; i < E - S + 1; i++)
	{
		convbuf[i] = from[S + i];
	}
	return atof(convbuf);//conversionbuf에 저장된 문자열을 double형으로 변환 후 리턴
}

//opbuf에서 index가 pull_start인 지점부터 한 칸씩 앞으로 땡기기
void one_pullc(char * s, int pull_start)
{
	for(int i = 0; ; i++)
	{
		s[pull_start + i] = s[pull_start + i + 1];
		if(s[pull_start + i] == '\0')
			break;
	}
	return;
}

//numbuf에서 index가 pull_start인 지점부터 한 칸씩 앞으로 땡기기
void one_pulln(double * s, int pull_start)
{
	for(int i = 0; ; i++)
	{
		s[pull_start + i] = s[pull_start + i + 1];
		if(s[pull_start + i] == '\0')
			break;
	}
	return;
}

//사칙연산 수행 함수
void calc(double * numbuf, char * opbuf)
{
	int i = 0;
	while(1)
	{
		if(opbuf[i] == '/')
		{
			numbuf[i] = numbuf[i] / numbuf[i+1];
			one_pulln(numbuf, i+1);
			one_pullc(opbuf, i);
			i = 0;
			continue;
		}
		i++;
		if(opbuf[i] == '\0')
			break;
	}
	i = 0;
	while(1)
	{
		if(opbuf[i] == '*')
		{
			numbuf[i] = numbuf[i] * numbuf[i+1];
			one_pulln(numbuf, i+1);
			one_pullc(opbuf, i);
			i = 0;
			continue;
		}
		i++;
		if(opbuf[i] == '\0')
			break;
	}
	i = 0;
	while(1)
	{
		if(opbuf[i] == '-')
		{
			numbuf[i] = numbuf[i] - numbuf[i+1];
			one_pulln(numbuf, i+1);
			one_pullc(opbuf, i);
			i = 0;
			continue;
		}
		i++;
		if(opbuf[i] == '\0')
			break;
	}
	i = 0;
	while(1)
	{
		if(opbuf[i] == '+')
		{
			numbuf[i] = numbuf[i] + numbuf[i+1];
			one_pulln(numbuf, i+1);
			one_pullc(opbuf, i);
			i = 0;
			continue;
		}
		i++;
		if(opbuf[i] == '\0')
			break;
	}
}

//괄호 내부 수식 계산 후 기존 buf에 특정문자로 괄호문을 통일하는 함수
void rearrange(double * numbuf, char * buf, int start, int end)
{
	temporary[cnt] = numbuf[0];//특정 문자가 어떤 값을 나타내는지 저장하는 배열
	for(int i = start; i <= end; i++)
	{
		buf[i] = 'A' + cnt;//괄호시작(start)부터 괄호끝(end)까지 특정문자로 통일
	}
	return;
}

//배열 초기화
void buf_init(char * s)
{
	int i = 0;
	while(s[i] != '\0')
	{
		s[i++] = '\0';
	}

	return;
}
