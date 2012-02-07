/**
 *    统计代码的实际字节数, （程序中必不可少的部分）
 *    2012-02-07
 * 
 * 	Coded by Abreto. <m@abreto.net>
 */
#include <stdio.h>
#include <stdlib.h>

/**
 * argv[1]	要统计的文件名
 **/
int main( int argc, char* argv[] )
{
    FILE  *pFile  =  NULL;
    int       nCounter  =  0;
    char    ch;
    char flag[4]={0};	// 标识位数组 0:计数器加, 1:是否处于单行注释中, 2:是否处于多行注释中, 3:是否处于字符串中.

    // 打开文件
    if(  ( pFile  =  fopen(  argv[1] ,  "rb" )  )  ==  NULL  )
    {
        puts(  "打开文件失败！\n"  );
        exit(  -1  );
    }
    
	do
	{
		flag[0] = 0;
		ch = fgetc( pFile);
		switch( ch )
		{
		case '/':	// 处理注释(或除号)
			if( 1 == flag[3] )	// 在字符串中
			{
				flag[0] = 1;
				break;
			}
			ch = fgetc( pFile );
			switch( ch )
			{
			case '/':	// 单行注释
				while( (!feof(pFile)) && (ch != '\n') )
				{
					ch = fgetc(pFile);
				}
				// 感觉有点浪费时间的方法: flag[1] = 1;	// 置'是否处于单行注释中'为1
				break;
			case '*':	// 多行注释
				// 同样感觉有点浪费时间的方法: flag[2] = 1;	// 置‘是否处于多行注释中’为1
				while( !feof(pFile) )	// 忽略掉所有注释
				{
					ch = fgetc( pFile );
					if( ('*' == ch) && (!feof(pFile)) )
					{
h_x:
						ch = fgetc(pFile);
						if( '*' == ch )	goto h_x;	// 邪恶的使用goto..
						if( '/' == ch )
							break;
					}
				}
				break;
			default:	// 可能是除号
				flag[0] = 1;	// 置'是否加一'为1;
				switch( ch )
				{
					case ' ':
					case '\n':
					case '\r':
						break;
					default:
						flag[0] += 1;
				}
			}
			break;
		case '\n':	// 忽略
			break;
		case '\r':	// 不甩
			break;
		case '\\':	// 转义符
			if( !feof(pFile) )
			{
				ch = fgetc(pFile);
				flag[0] = 1;
			}
			flag[0] += 1;
			break;
		case '\'':
			ch = fgetc(pFile);
			if( 1 == flag[3] )	// 字符串结束
				flag[3] = 0;
			else 	// 字符串开始
				flag[3] = 1;
			break;
		case '"':	// 字符串中的字符要鸟
			if( 1 == flag[3] )	// 字符串结束
				flag[3] = 0;
			else 	// 字符串开始
				flag[3] = 1;
			flag[0] = 1;
			break;
		case ' ':	// 空格不鸟
			if( 1 == flag[3] )
				flag[0] = 1;
			break;
		default:
			flag[0] = 1;
			break;
		}
// add to counter:
		nCounter += (int)(flag[0]);
	}while( !feof(pFile) );

    // 关闭文件
    fclose( pFile );

    printf(  "实际代码字数: %d字节\n",  nCounter  );
    return 0;
}

