#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

/*입력예시 

./a.out /bin/ls "|" /bin/cd ";"

위와 같이 모든 구분점에는 큰따옴표로 감싸져 있어야한다.

*/

int main(int arc, char **arv, char **env)
{
	/* 파라미터가 없는 경우 체크 */
	if (arc > 1)
	{
		int		i = 1;
		int		in = 0;
		int		p_in = 0;
		int		out = 1;
		int		fd[2];
		pid_t	pid[500];
		int		pp = 0;

		/* 받아온 만큼의 파라미터를 처리할 때까지 */
		while (i < arc)
		{
			char	*args[500];
			int		cc = 0;

			/* NULL, "|", ";" 등 구분점이 나올 때 까지 배열에 옮겨 담기 */
			while (i < arc && strcmp(arv[i], "|") && strcmp(arv[i], ";"))
			{
				args[cc] = arv[i];
				cc++;
				i++;
				args[cc] = 0;
			}

			/* 구분점만 들어올경우 체크 */
			if (cc == 0)
			{
				i++;
				continue ;
			}

			/* 구분점이 파이프라인인 경우 파이프 생성 후 변수에 pipe fd를 저장 */
			if (i < arc && !strcmp(arv[i], "|"))
			{
				if (pipe(fd) < 0)
				{
					write(2, "error : fatal\n", 14);
					exit(0);
				}
				//파이프라인이 있는 경우 파이프의 입출력을 p_in과 out에 저장
				p_in = fd[0];
				out = fd[1];
				i++;
			}

			/* 명령이 cd 인지 체크 후 예외 및 에러 처리 */
			if (!strcmp(args[0], "cd"))
			{
				if (cc != 2)
					write(2, "error : cd : args\n", 18);
				else if (chdir(args[1]) < 0)
					write(2, "error : cd : can't change directory to ", 39), write(2, args[1], strlen(args[1])), write(2, "\n", 1);
			}
			else
			{/* 모든 명령어 공통 실행 부분 */
				pid[pp] = fork();
				if (pid[pp] < 0)
				{
					write(2, "error : fatal\n", 14);
					exit(0);
				}
				/* 자식 프로세스 */
				if (pid[pp] == 0) 
				{ // 정규파일을 표준 입출력으로 사용
					dup2(in, 0);
					dup2(out, 1);
					// 구분점이 파이프인 경우에 해당
					if (p_in != 0)
						close(p_in);
					if (out != 1)
						close(out);

					//파이프로 구분된 이후 들어온 경우에 해당
					if (in != 0)
						close(in);
					
					//명령어 실행
					execve(args[0], args, env);

					//명령어 실행 실패 시 에러출력 후 종료
					write(2, "error : couldn't execute ", 25), write(2, args[0], strlen(args[0])), write(2, "\n", 1);
					exit(0);
				}
				else 
				{/* 부모 프로세스 */
					//파이프인 경우 마찬가지로 부모프로세스에서도 닫아줌
					if (out != 1)
						close(out);
					if (in != 0)
						close(in);
					//다음 명령어 실행 시, 파이프로 입력을 받을 수 있도록 in에 저장
					in = p_in;
					pp++;
				}

				/* NULL 이거나 ;로 프로세스가 종료되었을때 */
				if (i >= arc || (i < arc && !strcmp(arv[i], ";")))
				{
					//자식 프로세스의 종료상태를 회수
					while(pp)
					{
						pp--;
						waitpid(pid[pp], 0, 0);
					}
					//fd 초기화
					in = 0;
					p_in = 0;
					out = 1;
					//다음 파라미터 인덱스로 넘김
					i++;
				}
			}
		}
	}
}
