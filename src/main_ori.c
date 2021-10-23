#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define MAXL 100

int brace_searching(char * buf, char * opbuf, double * numbuf, int start, int buf_max);
void subbuflize(char * buf, int start, int end);
void opnumbuflize_for_buf(char * buf, char * opbuf, double * numbuf,int);
double convert_buflize(char * from, int E, int S);
void one_pullc(char * s, int pull_start);
void one_pulln(double * s, int pull_start);
void calc(double * numbuf, char * opbuf);
void buf_init(char * s);
void rearrange(double * numbuf, char * buf, int start, int end, int cnt);

double temporary[MAXL] = {};
int cnt = 0;

int main()
{
	int c;
	int i = 0;
	int start = 0;
	//int end = 0;
	char buf[MAXL] = {};
	
	double numbuf[MAXL] = {};
	char opbuf[MAXL] = {};
	

	while((c = getchar()) != '\n')
	{
		if(!isspace(c))
			buf[i++] = c;
	}
	buf[i] = '\0';
	int buf_max = i;

	brace_searching(buf, opbuf, numbuf, start, buf_max);
	opnumbuflize_for_buf(buf, opbuf, numbuf, buf_max, cnt);
	calc(numbuf, opbuf);

	printf("%.2lf\n", numbuf[0]);

	getchar();
	return 0;
}

int brace_searching(char * buf, char * opbuf, double * numbuf, int start, int buf_max)
{
	for(int i = start, temp = start; i <= buf_max; i++)
	{
		if(buf[i] == '(')
		{
			start = i;
			brace_searching(buf, opbuf, numbuf, start + 1, buf_max);//recursion
		}
	}
	for(int i = start, end = 0; i <= buf_max && buf[start] == '('; i++)
	{
		if(buf[i] == ')')
		{
			end = i;
			//계산
			subbuflize(buf, start, end);
			cnt++;
			calc(numbuf, opbuf);
			if(start == 0)
				rearrange(numbuf, buf, start, end, cnt);
			else
				rearrange(numbuf, buf, start, end, cnt);
			return 1;
		}
	}
	//if(start == temp)//더 이상 '('가 없다면
	return 0;
}
	//int num = end - start - 1;


void subbuflize(char * buf, int start, int end)
{
	char subbuf[MAXL] = {};
	int num = end - start - 1;
	for(int i = 0; i < num; i++)
	{
		subbuf[i] = buf[start + 1 + i];
	}
	subbuf[num] = '\0';

	int sub_end = 0;
	int sub_start = 0;
	for(int i = 0, j = 0; i <= num; i++)
	{
		if(subbuf[i] == '+' || subbuf[i] == '-' || subbuf[i] == '*' || subbuf[i] == '/' || subbuf[i] == '\0')
		{
			opbuf[j] = subbuf[i];//opbuf

			sub_end = i - 1;
			if(subbuf[sub_end] >= 'A')
			{
				numbuf[j++] = temporary[subbuf[sub_end] - 'A'];
			}
			else
			{
				numbuf[j++] = convert_buflize(subbuf, sub_end, sub_start);
			}
			sub_start = i + 1;
		}
	}
}

void opnumbuflize_for_buf(char * buf, char * opbuf, double * numbuf, int buf_max)
{
	int sub_end = 0;
	int sub_start = 0;
	for(int i = 0, j = 0; i <= buf_max; i++)
	{
		if(buf[i] == '+' || buf[i] == '-' || buf[i] == '*' || buf[i] == '/' || buf[i] == '\0')
		{
			opbuf[j] = buf[i];//opbuf

			sub_end = i - 1;
			if((buf[sub_end]) >= 'A')
			{
				numbuf[j++] = temporary[buf[sub_end] - 'A'];
			}
			else
			{
				numbuf[j++] = convert_buflize(buf, sub_end, sub_start);
			}
			sub_start = i + 1;
		}
	}
}

double convert_buflize(char * from, int E, int S)
{
	char convbuf[MAXL] = {};
	for(int i = 0; i < E - S + 1; i++)
	{
		convbuf[i] = from[S + i];
	}
	return atof(convbuf);
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

//
void rearrange(double * numbuf, char * buf, int start, int end, int cnt)
{
	temporary[cnt] = numbuf[0];
	for(int i = start; i <= end; i++)
	{
		buf[i] = 'A' + cnt;
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
